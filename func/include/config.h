#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <windows.h>
#include <vector>

/**
 * @brief Screen resolution width.
 */
extern int resx;

/**
 * @brief Screen resolution height.
 */
extern int resy;

/**
 * @brief Screen width offset.
 */
extern int resosux;

/**
 * @brief Screen height offset.
 */
extern int resosuy;

/**
 * @brief Zero point X coordinate.
 */
extern int zeropointx;

/**
 * @brief Zero point Y coordinate.
 */
extern int zeropointy;

/**
 * @brief Stack hits value.
 */
extern double stackHits;

/**
 * @brief Beats per minute.
 */
extern int bpm;

/**
 * @brief Slider velocity.
 */
extern double SV;

/**
 * @brief Pixel modification value.
 */
extern double pixelmod;

/**
 * @brief Base signature for memory search.
 */
extern std::vector<BYTE> baseSignature;

/**
 * @brief Time signature for memory search.
 */
extern std::vector<BYTE> timeSignature;

#endif // CONFIG_H
