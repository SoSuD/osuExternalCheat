#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

/**
 * @brief Converts a vector of comma-separated strings to a 2D vector of doubles.
 *
 * @param input The vector of strings to be converted.
 * @return A 2D vector of doubles.
 */
std::vector<std::vector<double>> convertToDouble(const std::vector<std::string> input);

/**
 * @brief Checks if the space key is pressed.
 */
void spaceCheck();

/**
 * @brief Smoothly moves the cursor from start position to end position.
 *
 * @param startX The starting X coordinate.
 * @param startY The starting Y coordinate.
 * @param endX The ending X coordinate.
 * @param endY The ending Y coordinate.
 * @param duration The duration of the move in milliseconds.
 */
void smoothMove(int startX, int startY, int endX, int endY, int duration);

/**
 * @brief Delays the execution for a specified number of milliseconds.
 *
 * @param milliseconds The duration of the delay in milliseconds.
 */
void delay(int milliseconds);

#endif // UTILS_H
