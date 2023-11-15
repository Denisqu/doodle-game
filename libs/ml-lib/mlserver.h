#ifndef MLSERVER_H
#define MLSERVER_H

#include "ml-lib_global.h"
#include <QObject>
#include <QWebSocketServer>

enum class Actions : unsigned int;

class MLLIB_EXPORT MLServer : public QObject {
  Q_OBJECT
public:
  explicit MLServer(quint16 port, QObject *parent = nullptr);
  ~MLServer();

signals:
  // websocket server signals
  void closed();
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
  QWebSocketServer *server_ = nullptr;
  QList<QWebSocket *> clients_;
  bool debug_ = true;
};

#endif // MLSERVER_H
