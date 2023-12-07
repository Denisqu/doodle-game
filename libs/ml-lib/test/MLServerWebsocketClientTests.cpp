#include "../mlserver.h"
#include "websockettestclient.h"
#include <QAbstractEventDispatcher>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QThread>
#include <QtWebSockets/QWebSocket>
#include <gtest/gtest.h>

///
/// \brief The MLServerWebsocketClientTests struct
/// Струткута тестов, использующих клиент WebSocket
/// Для обращения к MLServer
struct MLServerWebsocketClientTests : public testing::Test {
  MLServer *server = nullptr;
  QList<WebSocketTestClient *> testclients;
  QCoreApplication *application;

  const QList<QWebSocket *> &getServerClients() { return server->clients_; }

  WebSocketTestClient *createTestClient() {
    auto client = new WebSocketTestClient(
        QUrl(QStringLiteral("ws://localhost:80")), true);
    testclients.push_back(client);
    return client;
  }

  void resetCallback(QString state) { server->resetCallback(state); }
  void makeCallback() { server->makeCallback(); }
  void stepCallback(std::shared_ptr<std::tuple<QString, double, bool>> data) {
    server->stepCallback(data);
  }

protected:
  virtual void SetUp() override {
    int argc = 1;
    const char *argv = "test";
    application = new QCoreApplication(argc, const_cast<char **>(&argv));
    server = new MLServer(80);
  };
  virtual void TearDown() override {
    if (testclients.count() > 0)
      qDeleteAll(testclients);
    delete server;
    QCoreApplication::instance()->exit();
    delete application;
  };
};

// Проверяем, что клиент успешно подключился к серверу
TEST_F(MLServerWebsocketClientTests, OnNewConnection) {
  createTestClient();

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }
  auto result = getServerClients().count();
  EXPECT_EQ(1, result);
}

// Проверяем, что одновременно к серверу может быть подключен только один клиент
TEST_F(MLServerWebsocketClientTests, NoMoreThanOneSimultaneousConnection) {
  createTestClient();
  createTestClient();

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }
  auto result = getServerClients().count();
  EXPECT_EQ(1, result);
}

// Проверяем, что MLServer::processTextMessage правильно обрабатывает
// валидные входные данные от клиента WebSocket
TEST_F(MLServerWebsocketClientTests, ProcessTextMessage_Valid) {
  auto client = createTestClient();
  auto spy = QSignalSpy(server, &MLServer::make);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  auto json = QJsonObject();
  json["f"] = "make";
  auto validMessage = QJsonDocument(json).toJson();
  client->sendTextMessage(validMessage);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  EXPECT_EQ(1, spy.count());
}

// Проверяем, что MLServer::processTextMessage правильно обрабатывает
// невалидные входные данные от клиента WebSocket
TEST_F(MLServerWebsocketClientTests, ProcessTextMessage_Invalid) {
  auto client = createTestClient();
  const QString invalidMessage = "123abcd";
  client->sendTextMessage(invalidMessage);

  auto spy = QSignalSpy(server, &MLServer::invalidTextInputReceived);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  client->sendTextMessage(invalidMessage);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  EXPECT_EQ(invalidMessage, spy.takeFirst().at(0).value<QString>());
}

// Проверяем обработку отключение клиента от сервера
TEST_F(MLServerWebsocketClientTests, SocketDisconnected) {
  auto client = createTestClient();

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  client->close();

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  auto result = getServerClients().count();
  EXPECT_EQ(0, result);
}

// Проверяем, приходят ли данные от сервера без искажений
TEST_F(MLServerWebsocketClientTests, StepCallback) {
  auto client = createTestClient();
  auto spy = QSignalSpy(client, &WebSocketTestClient::messageReceived);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  auto data = std::make_shared<std::tuple<QString, double, bool>>(
      "base64-encoded-frame", 100, true);
  stepCallback(data);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  auto json = QJsonObject();
  json["f"] = "step";
  json["status"] = true;
  json["base64-frame"] = std::get<0>(*data);
  json["reward"] = std::get<1>(*data);
  json["is-terminal"] = std::get<2>(*data);

  auto receivedString = spy.takeFirst().at(0).value<QString>();
  QByteArray jsonBytes = receivedString.toLocal8Bit();
  auto jsonDoc = QJsonDocument::fromJson(jsonBytes);
  auto receivedJson = jsonDoc.object();

  EXPECT_EQ(json, receivedJson);
}

// Проверяем, приходят ли данные от сервера без искажений
TEST_F(MLServerWebsocketClientTests, MakeCallback) {
  auto client = createTestClient();
  auto spy = QSignalSpy(client, &WebSocketTestClient::messageReceived);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  makeCallback();

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  auto json = QJsonObject();
  json["f"] = "make";
  json["status"] = true;

  auto receivedString = spy.takeFirst().at(0).value<QString>();
  QByteArray jsonBytes = receivedString.toLocal8Bit();
  auto jsonDoc = QJsonDocument::fromJson(jsonBytes);
  auto receivedJson = jsonDoc.object();

  EXPECT_EQ(json, receivedJson);
}

// Проверяем, приходят ли данные от сервера без искажений
TEST_F(MLServerWebsocketClientTests, ResetCallback) {
  auto client = createTestClient();
  auto spy = QSignalSpy(client, &WebSocketTestClient::messageReceived);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  QString state = "base64-encoded-string";
  resetCallback(state);

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }

  auto json = QJsonObject();
  json["f"] = "reset";
  json["status"] = true;
  json["base64-frame"] = state;

  auto receivedString = spy.takeFirst().at(0).value<QString>();
  QByteArray jsonBytes = receivedString.toLocal8Bit();
  auto jsonDoc = QJsonDocument::fromJson(jsonBytes);
  auto receivedJson = jsonDoc.object();

  EXPECT_EQ(json, receivedJson);
}
