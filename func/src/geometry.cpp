#include "geometry.h"
#include "utils.h"

#include <sstream>
#include <bitset>
#include <iostream>
#include "config.h"
#include <chrono>
#include <fstream>
#include <windows.h>
#include <regex>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>

using namespace std::chrono;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool parseLine(const std::string& line, int& x, int& y, int& time, int& type, int& sliderType, std::vector<std::vector<int>>& sliderCords, int& sliderDistance, int& spinnerTime, int& sliderIterations) {
  std::stringstream ss(line);
  std::string item;
  std::vector<std::string> parsedItems;
  std::string sliderTypeAndCords;

  while (getline(ss, item, ',')) {
    parsedItems.push_back(item);
  }

  if (parsedItems.size() >= 4) {
    try {
      x = std::stoi(parsedItems[0]);
      y = std::stoi(parsedItems[1]);
      time = std::stoi(parsedItems[2]);
      type = std::stoi(parsedItems[3]);
      if (std::bitset<8>(type)[1] == 1) {
        std::string sliderChar;
        std::string token;
        sliderTypeAndCords = parsedItems[5];
        std::stringstream sliderString(sliderTypeAndCords);
        char delimiter = '|';
        char subDelimiter = ':';
        getline(sliderString, sliderChar, delimiter);
        if (sliderChar == "L") {
          sliderType = 0;
        } else if (sliderChar == "B") {
          sliderType = 1;
        } else if (sliderChar == "P") {
          sliderType = 2;
        }
        while (std::getline(sliderString, token, delimiter)) {
          std::vector<int> coordinatePair;
          std::stringstream subss(token);
          std::string subToken;
          while (std::getline(subss, subToken, subDelimiter)) {
            coordinatePair.push_back(std::stoi(subToken));
          }
          sliderCords.push_back(coordinatePair);
        }
        sliderDistance = std::stod(parsedItems[7]);
        sliderIterations = std::stoi(parsedItems[6]);
      }
      if (std::bitset<8>(type)[3] == 1) {
        spinnerTime = std::stoi(parsedItems[5]);
      }
      return true;
    } catch (const std::exception& e) {
      std::cerr << "������ ��� ����������� ����� � �����: " << e.what() << std::endl;
      return false;
    }
  }
  return false;
}

std::vector<int> calculatePixel(int x, int y) {
  int modX = x * pixelmod;
  int modY = y * pixelmod;
  return {modX + zeropointx, modY + zeropointy};
}

std::vector<std::string> readFileToVector(const std::string filename) {
  std::ifstream file(filename);
  std::vector<std::string> data;
  bool flagh = false;

  if (file.is_open()) {
    std::string line;
    while (getline(file, line)) {
      if (flagh == true || line.find("[HitObjects]") != std::string::npos) {
        data.push_back(line);
        flagh = true;
      }
    }
    file.close();
  } else {
    std::cerr << "�� ������� ������� ����" << std::endl;
  }
  return data;
}

std::vector<std::string> readFileToVectorTimings(const std::string filename, double& sliderMultiplier) {
  std::regex sliderMultiplierRegex("SliderMultiplier:([0-9]+(\\.[0-9]+)?)");
  std::ifstream file(filename);
  std::vector<std::vector<double>> finalTimings;
  std::vector<std::string> data;
  bool flagh = false;
  bool flagm = false;
  SIZE_T bytes = 100;

  if (file.is_open()) {
    std::string line;
    while (getline(file, line)) {
      if (flagh && (line.size() < 6)) {
        break;
      }
      if (!flagm && line.find("SliderMultiplier") != std::string::npos) {
        std::smatch match;
        std::regex_search(line, match, sliderMultiplierRegex);
        std::cout << "debug regex: " << match[1] << " " << std::stod(match[1]) << std::endl;
        sliderMultiplier = std::stod(match[1]);
        flagm = true;
      }
      if (flagh || line.find("[TimingPoints]") != std::string::npos) {
        data.push_back(line);
        flagh = true;
      }
    }
    file.close();
  } else {
    std::cerr << "�� ������� ������� ����" << std::endl;
  }
  return data;
}

std::vector<std::pair<std::vector<int>, std::vector<std::vector<int>>>> convertMap(std::vector<std::string> map) {
  std::vector<std::string> convertedMap;
  std::vector<std::pair<std::vector<int>, std::vector<std::vector<int>>>> finalMap;
  for (int i = map.size() - 1; i >= 0; --i) {
    std::cout << map[i] << std::endl;
    convertedMap.push_back(map[i]);
  }
  int x1 = -1, y1 = -1;
  int x2 = -1, y2 = -1;
  int time1 = -1;
  int sliderType = -1;
  int spinnerTime = -1;
  int sliderIterations = -1;
  int sliderDistance = -1.0;
  int x, y, time, type;

  for (const std::string& line : convertedMap) {
    std::vector<std::vector<int>> sliderCords;
    if (parseLine(line, x, y, time, type, sliderType, sliderCords, sliderDistance, spinnerTime, sliderIterations)) {
      if (x == x1 && y == y1 && time1 - time <= bpm * 3) {
        std::cout << "EQUALS" << std::endl;
        x2 -= 3.2;
        y2 -= 3.2;
      } else {
        std::cout << x << " " << y << " " << x1 << " " << y1 << std::endl;
        x2 = x;
        y2 = y;
      }
      finalMap.push_back({{x2, y2, time, type, sliderType, sliderDistance, spinnerTime, sliderIterations}, sliderCords});
      x1 = x;
      y1 = y;
      time1 = time;
    } else {
      std::cout << "������ �������� " << line << std::endl;
    }
  }

  std::vector<std::pair<std::vector<int>, std::vector<std::vector<int>>>> result;
  for (int i = finalMap.size() - 1; i >= 0; --i) {
    result.push_back(finalMap[i]);
  }
  return result;
}

std::vector<int> lSliderCords(int x1, int y1, int x2, int y2, int time, double distance) {
  double fullLength = pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
  double modifier = distance / fullLength;
  int dx = x2 - x1;
  int dy = y2 - y1;
  int resultX = dx * modifier + x1;
  int resultY = dy * modifier + y1;
  return {resultX, resultY};
}

void smoothMove(int startX, int startY, int endX, int endY, int duration) {
  LARGE_INTEGER frequency;
  LARGE_INTEGER start, end, now;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);
  end.QuadPart = start.QuadPart + (duration * frequency.QuadPart / 1000);
  int steps = duration / 10;
  float deltaX = (endX - startX) / static_cast<float>(steps);
  float deltaY = (endY - startY) / static_cast<float>(steps);

  for (int i = 0; i <= steps; i++) {
    QueryPerformanceCounter(&now);
    if (now.QuadPart > end.QuadPart) {
      break;
    }
    int newX = round(startX + i * deltaX);
    int newY = round(startY + i * deltaY);
    SetCursorPos(newX, newY);
    while (true) {
      QueryPerformanceCounter(&now);
      long long remaining = (start.QuadPart + (i * duration / steps) * frequency.QuadPart / 1000) - now.QuadPart;
      if (remaining <= 0) break;
    }
  }
}

int sliderTime(double length, double sliderMultiplier, double sv, int beatLength) {
  return length / (sliderMultiplier * 100 * sv) * beatLength;
}

void lSlider(double length, double sliderMultiplier, double sv, double beatLength, int x1, int y1, int x2, int y2, int iterations) {
  int sT = sliderTime(length, sliderMultiplier, sv, beatLength);
  std::vector<int> cords = lSliderCords(x1, y1, x2, y2, sT, length);
  std::vector<int> pointer1 = calculatePixel(x1, y1);
  int ppx1 = pointer1[0];
  int ppy1 = pointer1[1];
  std::vector<int> pointer2 = calculatePixel(cords[0], cords[1]);
  int ppx2 = pointer2[0];
  int ppy2 = pointer2[1];
  for (int i = 0; i < iterations; i++) {
    if (i % 2 == 0) smoothMove(ppx1, ppy1, ppx2, ppy2, sT);
    else smoothMove(ppx2, ppy2, ppx1, ppy1, sT);
    delay(1);
  }
}

void moveCursorInCircle(int timeNow, int timeEnd) {
  const int radius = 100;
  const int centerX = 1280;
  const int centerY = 735;
  const int steps = 120;
  int totalTime = timeEnd - timeNow;
  int delay = totalTime / steps;
  auto start = system_clock::now();
  auto startTime = duration_cast<milliseconds>(start.time_since_epoch()).count();

  while (true) {
    auto now = system_clock::now();
    auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count();
    if (millis - startTime > totalTime) {
      break;
    }

    for (int i = 0; i <= steps; ++i) {
      now = system_clock::now();
      millis = duration_cast<milliseconds>(now.time_since_epoch()).count();
      if (millis - startTime > totalTime) break;

      double angle = i * (2.0 * M_PI / steps);
      int x = static_cast<int>(centerX + radius * cos(angle));
      int y = static_cast<int>(centerY + radius * sin(angle));
      SetCursorPos(x, y);
      auto endWait = system_clock::now() + milliseconds(1);
      while (system_clock::now() < endWait) {
      }
    }
  }
}

struct Point {
    int x; ///< The X coordinate of the point.
    int y; ///< The Y coordinate of the point.
};

Point bezier(float t, const std::vector<std::vector<int>>& controlPoints) {
  int n = controlPoints.size() - 1;
  Point p = {0, 0};
  for (int i = 0; i <= n; i++) {
    float binomialCoeff = tgamma(n + 1) / (tgamma(i + 1) * tgamma(n - i + 1));
    float bernstein = binomialCoeff * pow(t, i) * pow(1 - t, n - i);
    p.x += static_cast<int>(bernstein * controlPoints[i][0]);
    p.y += static_cast<int>(bernstein * controlPoints[i][1]);
  }
  return p;
}

float bezierLength(const std::vector<std::vector<int>>& controlPoints, int numSegments) {
  float length = 0.0;
  Point prevPoint = bezier(0.0, controlPoints);
  for (int i = 1; i <= numSegments; i++) {
    float t = static_cast<float>(i) / numSegments;
    Point currPoint = bezier(t, controlPoints);
    float segmentLength = std::sqrt(std::pow(currPoint.x - prevPoint.x, 2) + std::pow(currPoint.y - prevPoint.y, 2));
    length += segmentLength;
    prevPoint = currPoint;
  }
  return length;
}

void smoothMoveBezier(const std::vector<std::vector<int>>& controlPoints, int duration, double length, int iterations) {
  LARGE_INTEGER frequency;
  LARGE_INTEGER startTime, endTime, now;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&startTime);
  endTime.QuadPart = startTime.QuadPart + (duration * frequency.QuadPart / 1000);
  int steps = duration / 10;
  int spx = controlPoints[0][0];
  int spy = controlPoints[0][1];
  for (int iter = 0; iter < iterations; iter++) {
    if (iter % 2 == 0) {
      QueryPerformanceCounter(&startTime);
      for (int i = 0; i <= steps; i++) {
        QueryPerformanceCounter(&now);
        float t = i / static_cast<float>(steps);
        Point newPos = bezier(t, controlPoints);
        std::vector<int> xy = calculatePixel(spx + (newPos.x - spx) * 1, spy + (newPos.y - spy) * 1);
        SetCursorPos(xy[0], xy[1]);
        while (true) {
          QueryPerformanceCounter(&now);
          long long remaining = (startTime.QuadPart + (i * duration / steps) * frequency.QuadPart / 1000) - now.QuadPart;
          if (remaining <= 0) break;
        }
      }
    } else {
      QueryPerformanceCounter(&startTime);
      for (int i = steps; i >= 0; i--) {
        QueryPerformanceCounter(&now);
        float t = i / static_cast<float>(steps);
        Point newPos = bezier(t, controlPoints);
        std::vector<int> xy = calculatePixel(spx + (newPos.x - spx) * 1, spy + (newPos.y - spy) * 1);
        SetCursorPos(xy[0], xy[1]);
        while (true) {
          QueryPerformanceCounter(&now);
          long long remaining = (startTime.QuadPart + ((steps - i) * duration / steps) * frequency.QuadPart / 1000) - now.QuadPart;
          if (remaining <= 0) break;
        }
      }
    }
    delay(1);
  }
}

double distanceXtoY(std::vector<int> v1, std::vector<int> v2) {
  return std::sqrt(std::pow(v1[0] - v2[0], 2) + std::pow(v1[1] - v2[1], 2));
}

std::vector<int> findPointOnCircle(float centerX, float centerY, float radius, float startAngle, float arcLength, bool clockwise) {
  float angleChange = (arcLength / radius);
  float angle;
  if (!clockwise) {
    angle = startAngle - angleChange;
  } else {
    angle = startAngle + angleChange;
  }
  int resultX = centerX + radius * std::cos(angle);
  int resultY = centerY + radius * std::sin(angle);
  return {resultX, resultY};
}

int orientation(std::vector<int> p, std::vector<int> q, std::vector<int> r) {
  int val = (q[0] - p[0]) * (r[1] - p[1]) - (q[1] - p[1]) * (r[0] - p[0]);
  if (val == 0) return 0;
  return (val > 0) ? 1 : 2;
}

bool checkOrientation(std::vector<std::vector<int>> points) {
  std::vector<int> p1 = points[0];
  std::vector<int> p2 = points[1];
  std::vector<int> p3 = points[2];
  int result = orientation(p1, p2, p3);
  if (result == 0) {
    std::cout << "����� �����������\n";
  } else if (result == 1) {
    std::cout << "�� ������� �������\n";
    return true;
  } else {
    std::cout << "������ ������� �������\n";
    return false;
  }
}

void circularSlider(std::vector<std::vector<int>> cords, double duration, double length, int haltDist, int interpolates) {
  LARGE_INTEGER frequency;
  LARGE_INTEGER startTime, endTime, now;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&startTime);
  endTime.QuadPart = startTime.QuadPart + static_cast<LONGLONG>(duration * interpolates * frequency.QuadPart / 1000);
  duration = duration;
  int totalSteps = duration / 10;
  float a = cords[0][0] - cords[1][0];
  float b = cords[0][1] - cords[1][1];
  float c = cords[2][0] - cords[1][0];
  float d = cords[2][1] - cords[1][1];
  float e = a * (cords[1][0] + cords[0][0]) + b * (cords[1][1] + cords[0][1]);
  float f = c * (cords[1][0] + cords[2][0]) + d * (cords[1][1] + cords[2][1]);
  float g = 2 * (a * (cords[2][1] - cords[1][1]) - b * (cords[2][0] - cords[1][0]));
  float centerX = (d * e - b * f) / g;
  float centerY = (a * f - c * e) / g;
  float radius = std::sqrt(std::pow(cords[1][0] - centerX, 2) + std::pow(cords[1][1] - centerY, 2));
  float startAngle = std::atan2(cords[0][1] - centerY, cords[0][0] - centerX);
  float midAngle = std::atan2(cords[1][1] - centerY, cords[1][0] - centerX);
  float endAngle = std::atan2(cords[2][1] - centerY, cords[2][0] - centerX);
  float sweepAngle1 = midAngle - startAngle;
  float sweepAngle2 = endAngle - midAngle;
  if (sweepAngle1 < 0) {
    sweepAngle1 += 2 * M_PI;
  }
  if (sweepAngle2 < 0) {
    sweepAngle2 += 2 * M_PI;
  }
  if (sweepAngle1 + sweepAngle2 > 2 * M_PI) {
    sweepAngle1 -= 2 * M_PI;
    sweepAngle2 -= 2 * M_PI;
  }
  float totalSweepAngle = std::abs(sweepAngle1) + std::abs(sweepAngle2);
  int numSteps1 = static_cast<int>(totalSteps * (std::abs(sweepAngle1) / totalSweepAngle));
  int numSteps2 = totalSteps - numSteps1;
  float stepAngle1 = sweepAngle1 / numSteps1;
  float stepAngle2 = sweepAngle2 / numSteps2;
  std::vector<int> currentPos = cords[0];
  for (int inters = 0; inters < interpolates; inters++) {
    if (inters % 2 == 0) {
      QueryPerformanceCounter(&startTime);
      for (int i = 0; i < numSteps1; i++) {
        QueryPerformanceCounter(&now);
        float nextAngle = startAngle + stepAngle1 * (i + 1);
        int nextPosX = centerX + radius * std::cos(nextAngle);
        int nextPosY = centerY + radius * std::sin(nextAngle);
        currentPos = calculatePixel(nextPosX, nextPosY);
        SetCursorPos(currentPos[0], currentPos[1]);
        while (true) {
          QueryPerformanceCounter(&now);
          long long remaining = (startTime.QuadPart + (i * duration / totalSteps) * frequency.QuadPart / 1000) - now.QuadPart;
          if (remaining <= 0) break;
        }
      }
      for (int i = 0; i < numSteps2; i++) {
        QueryPerformanceCounter(&now);
        float nextAngle = midAngle + stepAngle2 * (i + 1);
        int nextPosX = centerX + radius * std::cos(nextAngle);
        int nextPosY = centerY + radius * std::sin(nextAngle);
        currentPos = calculatePixel(nextPosX, nextPosY);
        SetCursorPos(currentPos[0], currentPos[1]);
        while (true) {
          QueryPerformanceCounter(&now);
          long long remaining = (startTime.QuadPart + ((numSteps1 + i) * duration / totalSteps) * frequency.QuadPart / 1000) - now.QuadPart;
          if (remaining <= 0) break;
        }
      }
    } else {
      QueryPerformanceCounter(&startTime);
      for (int i = numSteps2; i >= 0; i--) {
        QueryPerformanceCounter(&now);
        float nextAngle = midAngle + stepAngle2 * i;
        int nextPosX = centerX + radius * std::cos(nextAngle);
        int nextPosY = centerY + radius * std::sin(nextAngle);
        currentPos = calculatePixel(nextPosX, nextPosY);
        SetCursorPos(currentPos[0], currentPos[1]);
        while (true) {
          QueryPerformanceCounter(&now);
          long long remaining = (startTime.QuadPart + ((numSteps2 - i) * duration / totalSteps) * frequency.QuadPart / 1000) - now.QuadPart;
          if (remaining <= 0) break;
        }
      }
      for (int i = numSteps1; i >= 0; i--) {
        QueryPerformanceCounter(&now);
        float nextAngle = startAngle + stepAngle1 * i;
        int nextPosX = centerX + radius * std::cos(nextAngle);
        int nextPosY = centerY + radius * std::sin(nextAngle);
        currentPos = calculatePixel(nextPosX, nextPosY);
        SetCursorPos(currentPos[0], currentPos[1]);
        while (true) {
          QueryPerformanceCounter(&now);
          long long remaining = (startTime.QuadPart + ((totalSteps - i) * duration / totalSteps) * frequency.QuadPart / 1000) - now.QuadPart;
          if (remaining <= 0) break;
        }
      }
    }
    delay(2);
  }
}
