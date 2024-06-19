#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <gtest/gtest.h>
#include "geometry.h"
#include "myfunctions.h"
#include "utils.h"
#include "config.h"
#include <chrono>


// Test for the function calculatePixel
TEST(GeometryTestSuite, CalculatePixelPositive) {
  int x = 100;
  int y = 200;
  std::vector<int> result = calculatePixel(x, y);

  EXPECT_EQ(result[0], static_cast<int>(x * pixelmod + zeropointx));
  EXPECT_EQ(result[1], static_cast<int>(y * pixelmod + zeropointy));
}

TEST(GeometryTestSuite, CalculatePixelNegative) {
  int x = 100;
  int y = 200;
  std::vector<int> result = calculatePixel(x, y);

  EXPECT_NE(result[0], static_cast<int>(x + zeropointx));
  EXPECT_NE(result[1], static_cast<int>(y + zeropointy));
}

// Test for the function parseLine
TEST(GeometryTestSuite, ParseLinePositive) {
  std::string line = "256,192,3000,1,0,0:0:0:0:0:";
  int x, y, time, type, sliderType, sliderDistance, spinnerTime, sliderIterations;
  std::vector<std::vector<int>> sliderCords;
  bool success = parseLine(line, x, y, time, type, sliderType, sliderCords, sliderDistance, spinnerTime, sliderIterations);

  EXPECT_TRUE(success);
  EXPECT_EQ(x, 256);
  EXPECT_EQ(y, 192);
  EXPECT_EQ(time, 3000);
  EXPECT_EQ(type, 1);
  EXPECT_EQ(sliderCords.size(), 0);
}

TEST(GeometryTestSuite, ParseLineNegative) {
  std::string line = "invalid,line,for,testing";
  int x, y, time, type, sliderType, sliderDistance, spinnerTime, sliderIterations;
  std::vector<std::vector<int>> sliderCords;
  bool success = parseLine(line, x, y, time, type, sliderType, sliderCords, sliderDistance, spinnerTime, sliderIterations);

  EXPECT_FALSE(success);
}

// Test for the function convertMap
TEST(GeometryTestSuite, ConvertMapPositive) {
  std::vector<std::string> map = { "256,192,3000,1,0,0:0:0:0:0:", "128,96,6000,1,0,0:0:0:0:0:" };
  auto result = convertMap(map);

  ASSERT_EQ(result.size(), 2);
  EXPECT_EQ(result[0].first[0], 256);
  EXPECT_EQ(result[0].first[1], 192);
  EXPECT_EQ(result[0].first[2], 3000);
  EXPECT_EQ(result[1].first[0], 128);
  EXPECT_EQ(result[1].first[1], 96);
  EXPECT_EQ(result[1].first[2], 6000);
}

TEST(GeometryTestSuite, ConvertMapNegative) {
  std::vector<std::string> map = { "invalid,map,line" };
  auto result = convertMap(map);

  EXPECT_EQ(result.size(), 0);
}

// Test for the function lSliderCords
TEST(GeometryTestSuite, LSliderCordsPositive) {
  int x1 = 0, y1 = 0, x2 = 100, y2 = 100, time = 1000;
  double distance = 50.0;
  auto result = lSliderCords(x1, y1, x2, y2, time, distance);

  EXPECT_NEAR(result[0], 35, 1);
  EXPECT_NEAR(result[1], 35, 1);
}

TEST(GeometryTestSuite, LSliderCordsNegative) {
  int x1 = 0, y1 = 0, x2 = 100, y2 = 0, time = 1000;
  double distance = 500.0;
  auto result = lSliderCords(x1, y1, x2, y2, time, distance);

  EXPECT_NE(result[0], 35);
  EXPECT_NE(result[1], 35);
}

// Test for the function findPointOnCircle
TEST(GeometryTestSuite, FindPointOnCirclePositive) {
  float centerX = 0.0, centerY = 0.0, radius = 100.0, startAngle = 0.0, arcLength = M_PI / 2;
  bool clockwise = true;
  auto result = findPointOnCircle(centerX, centerY, radius, startAngle, arcLength, clockwise);

  EXPECT_NEAR(result[0], 100, 1);
  EXPECT_NEAR(result[1], 0, 1);
}

TEST(GeometryTestSuite, FindPointOnCircleNegative) {
  float centerX = 0.0, centerY = 0.0, radius = 100.0, startAngle = 0.0, arcLength = M_PI / 2;
  bool clockwise = false;
  auto result = findPointOnCircle(centerX, centerY, radius, startAngle, arcLength, clockwise);

  EXPECT_NE(result[0], 100);
  EXPECT_NE(result[1], 0);
}

// Tests for the function getProcessHandleByName
TEST(MyFunctionsTestSuite, GetProcessHandleByNamePositive) {
  std::string processName = "explorer.exe";
  HANDLE handle = getProcessHandleByName(processName);

  EXPECT_NE(handle, nullptr);
  if (handle) CloseHandle(handle);
}

TEST(MyFunctionsTestSuite, GetProcessHandleByNameNegative) {
  std::string processName = "non_existent_process.exe";
  HANDLE handle = getProcessHandleByName(processName);

  EXPECT_EQ(handle, nullptr);
  if (handle) CloseHandle(handle);
}

// Tests for the function convertBytesToString
TEST(MyFunctionsTestSuite, ConvertBytesToStringPositive) {
  std::vector<BYTE> bytes = { 72, 101, 108, 108, 111 };
  std::string result = convertBytesToString(bytes);

  EXPECT_EQ(result, "0x6F6C6C6548");
}

TEST(MyFunctionsTestSuite, ConvertBytesToStringNegative) {
  std::vector<BYTE> bytes = { 72, 101, 108, 108, 111, 112 };
  std::string result = convertBytesToString(bytes);

  EXPECT_NE(result, "0x6F6C6C6548");
}

// Tests for the function delay
TEST(UtilsTestSuite, DelayPositive) {
  auto start = std::chrono::high_resolution_clock::now();
  delay(100);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;

  EXPECT_NEAR(elapsed.count(), 100, 10);
}

TEST(UtilsTestSuite, DelayNegative) {
  auto start = std::chrono::high_resolution_clock::now();
  delay(0);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;

  EXPECT_NE(elapsed.count(), 0);
}

// Тесты для других функций могут быть добавлены здесь

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
