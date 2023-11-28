#ifndef MLSERVER_H
#define MLSERVER_H

#include "doodleenv.h"
#include "ml-lib_global.h"
#include <QObject>
#include <QWebSocketServer>
#include <optional>

enum class Actions : unsigned int;

class MLLIB_EXPORT MLServer : public QObject {
  Q_OBJECT
public:
  explicit MLServer(quint16 port, QObject *parent = nullptr);
  ~MLServer();

public slots:
  void stepCallback(DoodleEnv::StepDataPtr data);
  void makeCallback();
  void resetCallback(QString state);

signals:
  // websocket server signals
  void closed();
  void invalidTextInputReceived(QString str);
  // logical signals
  void make();
  void reset();
  void step(Actions action);

private slots:
  void onNewConnection();
  void processTextMessage(QString msg);
  void processBinaryMessage(QByteArray msg);
  void socketDisconnected();

private:
  void processReceivedJson(const QJsonObject &json); // unit-tests+
  void sendJsonMessage(const QJsonObject &json);
  std::optional<QJsonObject> stringToJson(QString string);
  std::optional<Actions> stringToAction(QString string);

  QWebSocketServer *server_ = nullptr;
  QList<QWebSocket *> clients_;
  bool debug_ = true;

  friend struct MLServerTests;
};

#endif // MLSERVER_H
