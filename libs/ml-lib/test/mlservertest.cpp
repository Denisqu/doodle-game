#include "../mlserver.h"
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QVariant>
#include <QWebSocket>
#include <gtest/gtest.h>

// https://doc.qt.io/qt-5/qttest-index.html
struct MLServerTests : public testing::Test {
  // Test interface
  MLServer *server;

  void processTextMessage(QString msg) { server->processTextMessage(msg); }

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

TEST_F(MLServerTests, StringToJson_Valid) {
  auto json = QJsonObject();
  json["f"] = "step";
  json["status"] = true;
  auto string = QJsonDocument(json).toJson();
  auto result = stringToJson(string);

  EXPECT_EQ(result.has_value(), result.has_value());
}

TEST_F(MLServerTests, StringToJson_Invalid) {
  auto invalidInput = QString("123abcda'");
  auto result = stringToJson(invalidInput);

  EXPECT_EQ(std::optional<QJsonObject>().has_value(), result.has_value());
}

TEST_F(MLServerTests, StringToAction_Valid) {

  QList<QPair<QString, Actions>> validInputsToResultMap;
  validInputsToResultMap.push_back({"R", Actions::Right});
  validInputsToResultMap.push_back({"L", Actions::Left});

  for (const auto &validInputToResult : validInputsToResultMap) {
    const auto &string = validInputToResult.first;
    const auto &expectedResult = validInputToResult.second;
    EXPECT_EQ(expectedResult, stringToAction(string));
  }
}

TEST_F(MLServerTests, StringToAction_Invalid) {
  QList<QPair<QString, Actions>> invalidInputsToResultMap;
  invalidInputsToResultMap.push_back({"RRR", Actions::Invalid});
  invalidInputsToResultMap.push_back({"LLL", Actions::Invalid});
  invalidInputsToResultMap.push_back({"r", Actions::Invalid});
  invalidInputsToResultMap.push_back({"l", Actions::Invalid});
  invalidInputsToResultMap.push_back({"RL", Actions::Invalid});
  invalidInputsToResultMap.push_back({"LR", Actions::Invalid});

  for (const auto &validInputToResult : invalidInputsToResultMap) {
    const auto &string = validInputToResult.first;
    EXPECT_EQ(std::optional<Actions>(), stringToAction(string));
  }
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
