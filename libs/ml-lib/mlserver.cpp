#include "mlserver.h"
#include "doodleenv.h"
#include "exampleservice.h"
#include <Optional>
#include <QDebug>
#include <QtWebSockets/QtWebSockets>

namespace {

std::optional<Actions> stringToAction(QString string) {
  if (string == "L")
    return Actions::Left;
  else if (string == "R")
    return Actions::Right;
  else
    return {};
}

std::optional<QJsonObject> stringToJson(QString string) {
  QByteArray jsonBytes = string.toLocal8Bit();
  auto jsonDoc = QJsonDocument::fromJson(jsonBytes);
  if (jsonDoc.isNull()) {
    qDebug() << "Failed to create JSON doc.";
    return {};
  }
  if (!jsonDoc.isObject()) {
    qDebug() << "JSON is not an object.";
    return {};
  }
  QJsonObject json = jsonDoc.object();
  return json;
}

} // namespace

MLServer::MLServer(quint16 port, QObject *parent)
    : QObject{parent},
      server_(new QWebSocketServer(QStringLiteral("Echo Server"),
                                   QWebSocketServer::NonSecureMode, this)) {
  if (server_->listen(QHostAddress::Any, port)) {
    server_->setMaxPendingConnections(1);
    if (debug_)
      qDebug() << "MLServer listening on port" << port;
    connect(server_, &QWebSocketServer::newConnection, this,
            &MLServer::onNewConnection);
    connect(server_, &QWebSocketServer::closed, this, &MLServer::closed);
  }
}

MLServer::~MLServer() {
  server_->close();
  qDeleteAll(clients_.begin(), clients_.end());
}

void MLServer::stepCallback(
    std::shared_ptr<std::tuple<QString, double, bool>> data) {
  auto json = QJsonObject();
  json["f"] = "step";
  json["status"] = true;
  json["base64-frame"] = std::get<0>(*data);
  json["reward"] = std::get<1>(*data);
  json["is-terminal"] = std::get<2>(*data);
  sendJsonMessage(json);
}

void MLServer::makeCallback() {
  auto json = QJsonObject();
  json["f"] = "make";
  json["status"] = true;
  sendJsonMessage(json);
}

void MLServer::resetCallback(QString state) {
  auto json = QJsonObject();
  json["f"] = "reset";
  json["status"] = true;
  json["base64-frame"] = state;
  sendJsonMessage(json);
}

void MLServer::onNewConnection() {
  QWebSocket *socket = server_->nextPendingConnection();

  if (clients_.size() > 0) {
    socket->close(QWebSocketProtocol::CloseCodeAbnormalDisconnection);
    delete socket;
    return;
  }

  connect(socket, &QWebSocket::textMessageReceived, this,
          &MLServer::processTextMessage);
  connect(socket, &QWebSocket::binaryMessageReceived, this,
          &MLServer::processBinaryMessage);
  connect(socket, &QWebSocket::disconnected, this,
          &MLServer::socketDisconnected);

  clients_ << socket;
}

void MLServer::processTextMessage(QString msg) {
  QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
  if (debug_)
    qDebug() << "Message received:" << msg;
  if (!pClient)
    return;

  auto optionalJson = stringToJson(msg);
  if (!optionalJson.has_value()) {
    qDebug() << "invalid json";
    return;
  }
  auto json = optionalJson.value();
  processReceivedJson(json);
}

void MLServer::processBinaryMessage(QByteArray msg) {
  QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
  if (debug_)
    qDebug() << "Binary Message received:" << msg;
  if (pClient) {
    // pClient->sendBinaryMessage(msg);
  }
}

void MLServer::socketDisconnected() {
  QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
  if (debug_)
    qDebug() << "socketDisconnected:" << pClient;
  if (pClient) {
    clients_.removeAll(pClient);
    pClient->deleteLater();
  }
}

void MLServer::processReceivedJson(const QJsonObject &json) {
  if (json.contains("f") && json["f"] == "make") {
    qDebug() << "make received";
    emit make();
  } else if (json.contains("f") && json["f"] == "reset") {
    qDebug() << "reset received";
    emit reset();
  } else if (json.contains("f") && json["f"] == "step") {
    auto params = json["params"];
    qDebug() << "step received with params = " << params.toString();

    auto action = stringToAction(params.toString());
    if (action.has_value()) {
      emit step(action.value());
    }
  }
}

void MLServer::sendJsonMessage(const QJsonObject &json) {
  auto pClient = clients_.count() > 0 ? clients_[0] : nullptr;
  if (!pClient)
    return;

  auto doc = QJsonDocument(json);
  QByteArray jsonMessage(doc.toJson(QJsonDocument::Compact));
  QString stringMessage = QString::fromUtf8(jsonMessage);

  pClient->sendTextMessage(stringMessage);
}
