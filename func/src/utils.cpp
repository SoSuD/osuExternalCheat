#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <Windows.h>

std::vector<std::vector<double>> convertToDouble(const std::vector<std::string> input) {
  std::vector<std::vector<double>> result;

  for (const std::string& str : input) {
    try {
      std::vector<double> temp;
      std::string token;
      std::istringstream tokenStream(str);
      while (std::getline(tokenStream, token, ',')) {
        temp.push_back(std::stod(token));
      }
      result.push_back(temp);
      std::cout << "dobavilTiming" << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "Ошибка при конвертации строк в числа: " << e.what() << std::endl;
    }
  }
  return result;
}

void spaceCheck() {
  while (true) {
    if (GetAsyncKeyState(VK_F9) & 0x8000) {
      std::cout << "Пробел нажат. Запуск основного кода..." << std::endl;
      break;
    }
  }
}

void delay(int milliseconds) {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  LARGE_INTEGER now, endTime;
  QueryPerformanceCounter(&now);
  endTime.QuadPart = now.QuadPart + (milliseconds * frequency.QuadPart / 1000);
  while (true) {
    QueryPerformanceCounter(&now);
    if (now.QuadPart >= endTime.QuadPart) {
      break;
    }
  }
}
