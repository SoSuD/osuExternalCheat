#include <iostream>
#include <windows.h>
#include <sstream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath>
#include <thread>
#include <string>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <bitset>
#include <regex>

#include "myfunctions.h"
#include "geometry.h"
#include "utils.h"
#include "config.h"

using namespace std::chrono;

int main() {
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
  //std::wcin.imbue(std::locale("ru_RU.UTF-8"));
  //std::wcout.imbue(std::locale("ru_RU.UTF-8"));

  std::wcout << L"Start osu! and press F9..." << std::endl;
  spaceCheck();
  HANDLE phandler = getProcessHandleByName("osu!.exe");
  DWORD baseAddress = (uintptr_t)findSignatureAddress(phandler, baseSignature) - 0xC;
  DWORD timeAddress = (uintptr_t)findSignatureAddress(phandler, timeSignature);

  DWORD mapNameAddr = getFinalAddress(phandler, baseAddress, {0x0, 0x90, 0x0});
  DWORD mapTimeAddr = getFinalAddress(phandler, timeAddress + 0x5, {0x0});

  std::wcout << L"Select map and press F9..." << std::endl;
  spaceCheck();

  SIZE_T size = 256;
  std::vector<BYTE> mapName = readBytesFromMemory(phandler, mapNameAddr, size);
  std::vector<BYTE> mapHex = extractSubArray(mapName);
  printHex(mapHex);
  std::wstring mapFileName = bytesToUtf16String(mapHex);
  std::wcout << mapFileName << std::endl;

  DWORD mapFolderAddr = getFinalAddress(phandler, baseAddress, {0x0, 0x78, 0x0});
  std::vector<BYTE> mapFolder = readBytesFromMemory(phandler, mapFolderAddr, size);
  std::vector<BYTE> mapFolderHex = extractSubArray(mapFolder);
  std::wstring mapFolderName = bytesToUtf16String(mapFolderHex);

  std::string songsPath = "C:/Users/SSD1337/AppData/Local/osu!/Songs/";
  std::string filename = songsPath + wstringToString(mapFolderName) + "/" + wstringToString(mapFileName);
  std::cout << filename << std::endl;

  double sliderMultiplier;
  std::vector<std::string> timingData = readFileToVectorTimings(filename, sliderMultiplier);
  for (const std::string& line : timingData) {
    std::cout << line << std::endl;
  }

  std::cout << "multiplier: " << sliderMultiplier << std::endl;
  std::vector<std::vector<double>> timingPoints = convertToDouble(timingData);
  for (const std::vector<double>& point : timingPoints) {
    std::cout << point[0] << std::endl;
  }

  std::vector<std::string> data = readFileToVector(filename);
  std::vector<std::pair<std::vector<int>, std::vector<std::vector<int>>>> map = convertMap(data);
  for (const auto& pair : map) {
    std::cout << "{ ";
    for (const auto& value : pair.first) {
      std::cout << value << " ";
    }
    std::cout << "} , { ";
    for (const auto& coordPair : pair.second) {
      std::cout << "{ ";
      for (const auto& coord : coordPair) {
        std::cout << coord << " ";
      }
      std::cout << "} ";
    }
    std::cout << "}" << std::endl;
  }
  Sleep(1000);
  std::wcout << L"Run map and press F9..." << std::endl;
  spaceCheck();
  int firstNoteTime = map[0].first[2];
  int mapTime = -1;
  while (mapTime < firstNoteTime) {
    SIZE_T size = 4;
    ReadProcessMemory(phandler, (LPCVOID)mapTimeAddr, &mapTime, sizeof(int), &size);
    std::cout << std::dec << mapTime << std::endl;
  }

  int millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
  std::cout << millis << std::endl;

  std::cout << "start time: " << firstNoteTime << std::endl;
  int startTime = millis - firstNoteTime;
  char key = 'Z';
  double beatLength;
  for (const auto& line : map) {
    //std::cout << "{ ";
    //for (const auto& value : line.first) {
    //  std::cout << value << " ";
    //}
    //std::cout << "} , { ";
    //for (const auto& coordPair : line.second) {
    //  std::cout << "{ ";
    //  for (const auto& coord : coordPair) {
    //    std::cout << coord << " ";
    //  }
    //  std::cout << "} ";
    //}
    //std::cout << "}" << std::endl;

    if (key == 'Z') {
      key = 'X';
    } else {
      key = 'Z';
    }

    int x = line.first[0];
    int y = line.first[1];
    int time = line.first[2];
    int type = line.first[3];
    std::vector<int> xy = calculatePixel(x, y);
    SetCursorPos(xy[0], xy[1]);

    while (true) {
      if (!timingPoints.empty() && timingPoints[0][0] <= time) {
        if (timingPoints[0][6] == 1) {
          beatLength = timingPoints[0][1];
          bpm = 1 / beatLength * 1000 * 60;
          std::cout << "BPM CHANGED" << std::endl;
          timingPoints.erase(timingPoints.begin());
        } else {
          SV = 1.0 / (abs(timingPoints[0][1]) / 100.0);
          std::cout << std::dec << "SV_CHANGED " << SV << " data" << abs(timingPoints[0][1]) << " " << timingPoints[0][1] << std::endl;
          timingPoints.erase(timingPoints.begin());
        }
      } else {
        break;
      }
    }

    if (std::bitset<8>(type)[0] == 1) {
      std::vector<int> xy = calculatePixel(line.first[0], line.first[1]);
      int x = xy[0];
      int y = xy[1];

      while (true) {
        int millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if (millis - startTime >= time) {
          std::cout << "values: x = " << x << ", y = " << y << ", time = " << time << std::endl;
          SetCursorPos(x, y);
          PressKey(key);
          delay(15);
          ReleaseKey(key);
          delay(3);
          break;
        }
      }
    }

    int iterations = line.first[7];
    if (std::bitset<8>(type)[1] == 1) {
      if (line.first[4] == 0) {
        int distance = line.first[5];
        int x2 = line.second[0][0];
        int y2 = line.second[0][1];

        while (true) {
          int millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
          if (millis - startTime >= time) {
            std::cout << "values: x = " << x << ", y = " << y << ", time = " << time << std::endl;
            std::vector<int> xy = calculatePixel(x, y);
            SetCursorPos(xy[0], xy[1]);
            PressKey(key);
            delay(12);
            lSlider(distance, sliderMultiplier, SV, beatLength, x, y, x2, y2, iterations);
            delay(2);
            keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
            break;
          }
        }
      }

      if (line.first[4] == 1) {
        int distance = line.first[5];
        while (true) {
          int millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
          if (millis - startTime >= time) {
            std::cout << "values: x = " << x << ", y = " << y << ", time = " << time << std::endl;
            std::vector<int> xy = calculatePixel(x, y);
            SetCursorPos(xy[0], xy[1]);
            PressKey(key);
            std::vector<std::vector<int>> m = line.second;
            m.insert(m.begin(), {x, y});
            delay(12);
            smoothMoveBezier(m, sliderTime(distance, sliderMultiplier, SV, beatLength), distance, iterations);
            delay(1);
            ReleaseKey(key);
            break;
          }
        }
      }

      if (line.first[4] == 2) {
        int distance = line.first[5];
        std::vector<int> xy = calculatePixel(x, y);
        std::vector<std::vector<int>> m = line.second;
        m.insert(m.begin(), {x, y});
        while (true) {
          int millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
          if (millis - startTime >= time) {
            PressKey(key);
            delay(12);
            circularSlider(m, sliderTime(distance, sliderMultiplier, SV, beatLength), distance, 1, iterations);
            delay(3);
            ReleaseKey(key);
            break;
          }
        }
      }
    }

    if (std::bitset<8>(type)[3] == 1) {
      int spinnerTimeEnd = line.first[6];
      while (true) {
        int millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        if (millis - startTime >= time) {
          PressKey(key);
          moveCursorInCircle(time, spinnerTimeEnd);
          ReleaseKey(key);
          break;
        }
      }
    }
  }
}
