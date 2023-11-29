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

protected:
  virtual void SetUp() override {
    int argc = 1;
    char *argv = "test";
    application = new QCoreApplication(argc, &argv);
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

TEST_F(MLServerWebsocketClientTests, OnNewConnection) {
  createTestClient();

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }
  auto result = getServerClients().count();
  EXPECT_EQ(1, result);
}

TEST_F(MLServerWebsocketClientTests, NoMoreThanOneSimultaneousConnection) {
  createTestClient();
  createTestClient();

  for (int i = 0; i < 50000; ++i) {
    QCoreApplication::processEvents();
  }
  auto result = getServerClients().count();
  EXPECT_EQ(1, result);
}

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

TEST_F(MLServerWebsocketClientTests, StepCallback) {
  // todo
}

TEST_F(MLServerWebsocketClientTests, MakeCallback) {
  // todo
}

TEST_F(MLServerWebsocketClientTests, ResetCallback) {
  // todo
}
