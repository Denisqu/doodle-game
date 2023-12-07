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
  void stepCallback(DoodleEnv::StepDataPtr data); // unit-tests+
  void makeCallback();                            // unit-tests+
  void resetCallback(QString state);              // unit-tests+

signals:
  // websocket server signals
  void closed();
  void invalidTextInputReceived(QString str);
  // logical signals
  void make();               // unit-tests+
  void reset();              // unit-tests+
  void step(Actions action); // unit-tests+

private slots:
  void onNewConnection();               // unit-tests+
  void processTextMessage(QString msg); // unit-tests+
  void processBinaryMessage(QByteArray msg);
  void socketDisconnected(); // unit-tests+

private:
  void processReceivedJson(const QJsonObject &json);       // unit-tests+
  void sendJsonMessage(const QJsonObject &json);           // unit-tests+
  std::optional<QJsonObject> stringToJson(QString string); // unit-tests+
  std::optional<Actions> stringToAction(QString string);   // unit-tests+

  QWebSocketServer *server_ = nullptr;
  QList<QWebSocket *> clients_;
  bool debug_ = true;

  friend struct MLServerTests;
  friend struct MLServerWebsocketClientTests;
};

#endif // MLSERVER_H
