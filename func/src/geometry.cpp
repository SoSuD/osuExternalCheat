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


