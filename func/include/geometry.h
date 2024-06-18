#pragma once

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>

/**
 * @brief Parses a line of map data into individual elements.
 *
 * @param line The line to parse.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param time Time value.
 * @param type Type of the object.
 * @param sliderType Slider type.
 * @param sliderCords Slider coordinates.
 * @param sliderDistance Slider distance.
 * @param spinnerTime Spinner time.
 * @param sliderIterations Slider iterations.
 * @return True if parsing was successful, false otherwise.
 */
bool parseLine(const std::string& line, int& x, int& y, int& time, int& type, int& sliderType, std::vector<std::vector<int>>& sliderCords, int& sliderDistance, int& spinnerTime, int& sliderIterations);

/**
 * @brief Calculates the pixel coordinates based on input coordinates.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return A vector containing the pixel coordinates.
 */
std::vector<int> calculatePixel(int x, int y);

/**
 * @brief Reads a file and returns its contents as a vector of strings.
 *
 * @param filename The name of the file to read.
 * @return A vector containing the lines of the file.
 */
std::vector<std::string> readFileToVector(const std::string filename);

/**
 * @brief Reads timing data from a file and returns it as a vector of strings.
 *
 * @param filename The name of the file to read.
 * @param sliderMultiplier The slider multiplier.
 * @return A vector containing the timing data.
 */
std::vector<std::string> readFileToVectorTimings(const std::string filename, double& sliderMultiplier);

/**
 * @brief Converts a map to a specific format.
 *
 * @param map The map data to convert.
 * @return The converted map.
 */
std::vector<std::pair<std::vector<int>, std::vector<std::vector<int>>>> convertMap(std::vector<std::string> map);
