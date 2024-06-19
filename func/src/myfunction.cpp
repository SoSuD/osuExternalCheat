// myfunctions.cpp
#include "myfunctions.h"
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <iomanip>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <sstream>

HANDLE getProcessHandleByName(const std::string& processName) {
  HANDLE hProcess = nullptr;
  PROCESSENTRY32 pe32;
  pe32.dwSize = sizeof(PROCESSENTRY32);

  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot == INVALID_HANDLE_VALUE) {
    std::cerr << "Failed to create snapshot" << std::endl;
    return nullptr;
  }

  if (Process32First(hSnapshot, &pe32)) {
    do {
      if (processName == pe32.szExeFile) {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        break;
      }
    } while (Process32Next(hSnapshot, &pe32));
  }

  CloseHandle(hSnapshot);
  return hProcess;
}

DWORD getFinalAddress(HANDLE hProcess, DWORD baseAddress, const std::vector<DWORD>& offsets) {
  DWORD address = baseAddress;
  std::cout << "Base address: " << std::hex << address << std::endl;

  for (DWORD offset : offsets) {
    DWORD tempAddress = 0;
    if (!ReadProcessMemory(hProcess, (BYTE*)address, &tempAddress, sizeof(tempAddress), nullptr)) {
      std::cerr << "Failed to read memory at address: " << std::hex << address << std::endl;
      return 0;
    }
    address = tempAddress + offset;
    std::cout << "Read address: " << std::hex << tempAddress << ", with offset: " << std::hex << offset << ", resulting address: " << std::hex << address << std::endl;
  }

  return address;
}

std::vector<BYTE> readBytesFromMemory(HANDLE hProcess, DWORD address, SIZE_T size) {
  std::vector<BYTE> buffer(size);
  if (!ReadProcessMemory(hProcess, (LPCVOID)address, buffer.data(), size, nullptr)) {
    std::cerr << "Failed to read memory at address: " << std::hex << address << std::endl;
    return {};
  }

  return buffer;
}

std::wstring bytesToUtf16String(const std::vector<BYTE>& bytes) {
  if (bytes.size() % 2 != 0) {
    std::cerr << "Byte array size is not even" << std::endl;
    return L"";
  }
  std::wstring result;
  for (size_t i = 0; i < bytes.size(); i += 2) {
    wchar_t wchar = (bytes[i + 1] << 8) | bytes[i];
    result.push_back(wchar);
  }
  return result;
}

bool readMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize) {
  SIZE_T bytesRead;
  return ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, &bytesRead) && bytesRead == nSize;
}

LPVOID findSignatureAddress(HANDLE hProcess, const std::vector<BYTE>& signature) {
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  LPVOID lpStartAddress = si.lpMinimumApplicationAddress;
  LPVOID lpEndAddress = si.lpMaximumApplicationAddress;

  MEMORY_BASIC_INFORMATION mbi;
  const SIZE_T bufferSize = 6553600;
  std::vector<BYTE> buffer(bufferSize);

  while (lpStartAddress < lpEndAddress) {
    if (VirtualQueryEx(hProcess, lpStartAddress, &mbi, sizeof(mbi))) {
      if (mbi.State == MEM_COMMIT && (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_EXECUTE_READWRITE)) {
        SIZE_T regionSize = mbi.RegionSize;
        LPBYTE regionBase = static_cast<LPBYTE>(mbi.BaseAddress);

        for (SIZE_T offset = 0; offset < regionSize; offset += bufferSize) {
          SIZE_T bytesToRead = std::min(bufferSize, regionSize - offset);
          if (readMemory(hProcess, regionBase + offset, buffer.data(), bytesToRead)) {
            for (SIZE_T i = 0; i < bytesToRead - signature.size(); i++) {
              bool found = true;
              for (SIZE_T j = 0; j < signature.size(); j++) {
                if (signature[j] != 0x00 && signature[j] != buffer[i + j]) {
                  found = false;
                  break;
                }
              }
              if (found) {
                return regionBase + offset + i;
              }
            }
          }
        }
      }
      lpStartAddress = static_cast<BYTE*>(mbi.BaseAddress) + mbi.RegionSize;
    }
  }

  return nullptr;
}

std::string convertBytesToString(const std::vector<BYTE>& bytes) {
  std::ostringstream oss;
  oss << "0x";
  for (auto it = bytes.rbegin(); it != bytes.rend(); ++it) {
    oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(*it);
  }
  return oss.str();
}

void printHex(const std::vector<BYTE>& data) {
  for (BYTE byte : data) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
  }
  std::cout << std::dec << std::endl;
}

std::vector<BYTE> extractSubArray(const std::vector<BYTE>& data) {
  std::vector<BYTE> result;
  bool foundStart = false;
  for (size_t i = 0; i < data.size() - 3; ++i) {
    if (!foundStart) {
      if (data[i] == 0x00 && data[i + 1] == 0x00 && data[i + 2] == 0x00) {
        foundStart = true;
        i += 1;
      }
    } else {
      if (data[i] == 0x00 && data[i + 1] == 0x00 && data[i + 2] == 0x00) {
        break;
      }
      result.push_back(data[i + 1]);
    }
  }
  return result;
}

std::string wstringToString(const std::wstring& wstr) {
  std::string str(wstr.begin(), wstr.end());
  return str;
}
