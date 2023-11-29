#ifndef WEBSOCKETTESTCLIENT_H
#define WEBSOCKETTESTCLIENT_H

#include <QtWebSockets/QWebSocket>

class WebSocketTestClient : public QObject {
  Q_OBJECT
public:
  explicit WebSocketTestClient(const QUrl &url, bool debug = false,
                               QObject *parent = nullptr) {
    if (m_debug)
      qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this,
            &WebSocketTestClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this,
            &WebSocketTestClient::closed);
    m_webSocket.open(QUrl(url));
  }

Q_SIGNALS:
  void closed();

public Q_SLOTS:
  void onConnected() {
    if (m_debug)
      qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this,
            &WebSocketTestClient::onTextMessageReceived);
    // m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
  }
  void onTextMessageReceived(QString message) {
    if (m_debug)
      qDebug() << "Message received:" << message;
    m_webSocket.close();
  }

  void sendTextMessage(QString msg) { m_webSocket.sendTextMessage(msg); }

  void close() { m_webSocket.close(); }

private:
  QWebSocket m_webSocket;
  QUrl m_url;
  bool m_debug;
};

#endif // WEBSOCKETTESTCLIENT_H
