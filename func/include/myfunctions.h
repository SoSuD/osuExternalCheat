#pragma once

#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

#include <string>
#include <vector>
#include <windows.h> // Для использования HANDLE

/**
 * @brief Retrieves the handle of a process by its name.
 *
 * @param processName The name of the process.
 * @return The handle to the process.
 */
HANDLE getProcessHandleByName(const std::string& processName);

/**
 * @brief Gets the final address in the process memory by following a list of offsets.
 *
 * @param hProcess The handle to the process.
 * @param baseAddress The base address.
 * @param offsets A vector of offsets.
 * @return The final address.
 */
DWORD getFinalAddress(HANDLE hProcess, DWORD baseAddress, const std::vector<DWORD>& offsets);

/**
 * @brief Reads a sequence of bytes from the process memory.
 *
 * @param hProcess The handle to the process.
 * @param address The memory address.
 * @param size The size of the byte sequence.
 * @return A vector of bytes read from memory.
 */
std::vector<BYTE> readBytesFromMemory(HANDLE hProcess, DWORD address, SIZE_T size);

/**
 * @brief Converts a vector of bytes to a UTF-16 string.
 *
 * @param data The vector of bytes.
 * @return The UTF-16 string.
 */
std::wstring bytesToUtf16String(const std::vector<BYTE>& data);

/**
 * @brief Finds the address of a signature in the process memory.
 *
 * @param hProcess The handle to the process.
 * @param signature The byte signature to search for.
 * @return The address of the signature.
 */
LPVOID findSignatureAddress(HANDLE hProcess, const std::vector<BYTE>& signature);

/**
 * @brief Reads memory from the process.
 *
 * @param hProcess The handle to the process.
 * @param lpBaseAddress The base address.
 * @param lpBuffer The buffer to store the read data.
 * @param nSize The size of the buffer.
 * @return True if the read was successful, false otherwise.
 */
bool readMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize);

/**
 * @brief Converts a vector of bytes to a string.
 *
 * @param bytes The vector of bytes.
 * @return The string representation of the bytes.
 */
std::string convertBytesToString(const std::vector<BYTE>& bytes);

/**
 * @brief Prints the hexadecimal representation of a vector of bytes.
 *
 * @param data The vector of bytes.
 */
void printHex(const std::vector<BYTE>& data);

/**
 * @brief Extracts a subarray from a vector of bytes.
 *
 * @param data The vector of bytes.
 * @return The subarray.
 */
std::vector<BYTE> extractSubArray(const std::vector<BYTE>& data);

/**
 * @brief Converts a wide string to a string.
 *
 * @param wstr The wide string.
 * @return The string.
 */
std::string wstringToString(const std::wstring& wstr);

#endif // MYFUNCTIONS_H
