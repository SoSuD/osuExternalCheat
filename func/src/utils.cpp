#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <Windows.h>

void PressKey(char key) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;

  if (key == 'Z') {
      input.ki.wVk = 0x5A; 
  } else {
      input.ki.wVk = 0x58; 
  }
  input.ki.dwFlags = 0; 

  SendInput(1, &input, sizeof(INPUT));
}

void ReleaseKey(char key) {
  INPUT input = {0};
  input.type = INPUT_KEYBOARD;

  if (key == 'Z') {
      input.ki.wVk = 0x5A; 
  } else {
      input.ki.wVk = 0x58; 
  }
  input.ki.dwFlags = KEYEVENTF_KEYUP; 

  SendInput(1, &input, sizeof(INPUT));
}

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
      std::cerr << "Error in ConvertToDouble: " << e.what() << std::endl;
    }
  }
  return result;
}

void spaceCheck() {
  while (true) {
    if (GetAsyncKeyState(VK_F9) & 0x8000) {
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
