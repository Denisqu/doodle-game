#include "../mlserver.h"
#include <QApplication>
#include <QJsonObject>
#include <QSignalSpy>
#include <QVariant>
#include <gtest/gtest.h>

// https://doc.qt.io/qt-5/qttest-index.html
struct MLServerTests : public testing::Test {
  // Test interface
  MLServer *server;

  void processTextMessage(QString msg) { server->processTextMessage(msg); }

  void processBinaryMessage(QByteArray msg) {
    server->processBinaryMessage(msg);
  }

  void processReceivedJson(const QJsonObject &json) {
    server->processReceivedJson(json);
  }

  std::optional<QJsonObject> stringToJson(QString string) {
    return server->stringToJson(string);
  }

  std::optional<Actions> stringToAction(QString string) {
    return server->stringToAction(string);
  }

protected:
  virtual void SetUp() override { server = new MLServer(80); }
  virtual void TearDown() override { delete server; }
};

TEST_F(MLServerTests, ProcessTextMessage_Invalid) {
  // todo
}

TEST_F(MLServerTests, ProcessBinaryMessage_Arbitrary) {
  // todo
}

TEST_F(MLServerTests, StringToJson_Valid) {
  // todo
}

TEST_F(MLServerTests, StringToJson_Invalid) {
  // todo
}

TEST_F(MLServerTests, StringToAction_Valid) {
  // todo
}

TEST_F(MLServerTests, StringToAction_Invalid) {
  // todo
}

TEST_F(MLServerTests, ProcessReceivedJson_Make_CorrectInput) {
  auto spy = QSignalSpy(server, &MLServer::make);
  auto json = QJsonObject();
  json["f"] = "make";
  processReceivedJson(json);

  EXPECT_EQ(1, spy.count());
}

TEST_F(MLServerTests, ProcessReceivedJson_Reset_CorrectInput) {
  auto spy = QSignalSpy(server, &MLServer::reset);
  auto json = QJsonObject();
  json["f"] = "reset";
  processReceivedJson(json);

  EXPECT_EQ(1, spy.count());
}

TEST_F(MLServerTests, ProcessReceivedJson_Step_CorrectInput) {
  QHash<QJsonObject, QList<QVariant>> expectedArguments;
  expectedArguments[{{"f", "step"}, {"params", "R"}}] = {
      QVariant::fromValue(Actions::Right)};
  expectedArguments[{{"f", "step"}, {"params", "L"}}] = {
      QVariant::fromValue(Actions::Left)};

  for (auto i = expectedArguments.cbegin(), end = expectedArguments.cend();
       i != end; ++i) {
    const auto &json = i.key();
    const auto &expectedArgument = i.value();
    auto spy = QSignalSpy(server, &MLServer::step);
    processReceivedJson(json);

    EXPECT_EQ(1, spy.count());
    EXPECT_EQ(expectedArgument.at(0).value<Actions>(),
              spy.takeFirst().at(0).value<Actions>());
  }
}

TEST_F(MLServerTests, ProcessReceivedJson_Step_IncorrectInput) {
  QList<QJsonObject> invalidJsons = {{{"f", "step"}, {"params", "RRR"}},
                                     {{"f", "step"}, {"params", "LLL"}},
                                     {{"f", "step"}, {"paramss", "LLL"}}};

  for (const auto &invalidJson : invalidJsons) {
    auto spy = QSignalSpy(server, &MLServer::step);
    processReceivedJson(invalidJson);
    EXPECT_EQ(0, spy.count());
  }
}
