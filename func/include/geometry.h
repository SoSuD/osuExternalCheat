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

/**
 * @brief Calculates the slider coordinates.
 *
 * @param x1 Starting X coordinate.
 * @param y1 Starting Y coordinate.
 * @param x2 Ending X coordinate.
 * @param y2 Ending Y coordinate.
 * @param time Time value.
 * @param distance Distance value.
 * @return A vector containing the slider coordinates.
 */
std::vector<int> lSliderCords(int x1, int y1, int x2, int y2, int time, double distance);

/**
 * @brief Smoothly moves the cursor from start to end coordinates over a duration.
 *
 * @param startX Starting X coordinate.
 * @param startY Starting Y coordinate.
 * @param endX Ending X coordinate.
 * @param endY Ending Y coordinate.
 * @param duration Duration of the move in milliseconds.
 */
void smoothMove(int startX, int startY, int endX, int endY, int duration);

/**
 * @brief Calculates the slider time.
 *
 * @param length The length of the slider.
 * @param sliderMultiplier The slider multiplier.
 * @param sv The SV value.
 * @param beatLength The beat length.
 * @return The slider time.
 */
int sliderTime(double length, double sliderMultiplier, double sv, int beatLength);

/**
 * @brief Performs a linear slider movement.
 *
 * @param length The length of the slider.
 * @param sliderMultiplier The slider multiplier.
 * @param sv The SV value.
 * @param beatLength The beat length.
 * @param x1 Starting X coordinate.
 * @param y1 Starting Y coordinate.
 * @param x2 Ending X coordinate.
 * @param y2 Ending Y coordinate.
 * @param iterations The number of iterations.
 */
void lSlider(double length, double sliderMultiplier, double sv, double beatLength, int x1, int y1, int x2, int y2, int iterations);

/**
 * @brief Moves the cursor in a circular motion.
 *
 * @param timeNow The current time.
 * @param timeEnd The end time.
 */
void moveCursorInCircle(int timeNow, int timeEnd);


/**
 * @struct Point
 * @brief A structure to represent a point.
 *
 * The `Point` structure is used to store the coordinates of a point.
 */
struct Point;

/**
 * @brief Calculates the Bezier point for a given t value.
 *
 * @param t The t value.
 * @param controlPoints The control points.
 * @return The Bezier point.
 */
Point bezier(float t, const std::vector<std::vector<int>>& controlPoints);

/**
 * @brief Calculates the length of a Bezier curve.
 *
 * @param controlPoints The control points.
 * @param numSegments The number of segments.
 * @return The length of the Bezier curve.
 */
float bezierLength(const std::vector<std::vector<int>>& controlPoints, int numSegments);

/**
 * @brief Smoothly moves the cursor along a Bezier curve.
 *
 * @param controlPoints The control points.
 * @param duration The duration of the move.
 * @param length The length of the curve.
 * @param iterations The number of iterations.
 */
void smoothMoveBezier(const std::vector<std::vector<int>>& controlPoints, int duration, double length, int iterations);

/**
 * @brief Finds a point on a circle given certain parameters.
 *
 * @param centerX The X coordinate of the center.
 * @param centerY The Y coordinate of the center.
 * @param radius The radius of the circle.
 * @param startAngle The starting angle.
 * @param arcLength The arc length.
 * @param clockwise Whether the movement is clockwise.
 * @return The point on the circle.
 */
std::vector<int> findPointOnCircle(float centerX, float centerY, float radius, float startAngle, float arcLength, bool clockwise);

/**
 * @brief Determines the orientation of three points.
 *
 * @param p The first point.
 * @param q The second point.
 * @param r The third point.
 * @return The orientation.
 */
int orientation(std::vector<int> p, std::vector<int> q, std::vector<int> r);

/**
 * @brief Checks the orientation of a set of points.
 *
 * @param points The points to check.
 * @return True if the points are clockwise, false otherwise.
 */
bool checkOrientation(std::vector<std::vector<int>> points);

/**
 * @brief Performs a circular slider movement.
 *
 * @param cords The coordinates.
 * @param duration The duration of the move.
 * @param length The length of the move.
 * @param haltDist The halt distance.
 * @param interpolates The number of interpolates.
 */
void circularSlider(std::vector<std::vector<int>> cords, double duration, double length, int haltDist, int interpolates);

#endif // GEOMETRY_H
