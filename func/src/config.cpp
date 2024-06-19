// config.cpp
#include "config.h"
#include <vector>
#include <windows.h>

int resx = 1920;
int resy = 1080;

int resosux = 512;
int resosuy = 384;

int zeropointx = 385;
int zeropointy = 124;
double stackHits = 3.2;
int bpm = 260;
double SV = 1.0;

double pixelmod = 2.242;

std::vector<BYTE> baseSignature = { 0xF8, 0x01, 0x74, 0x04, 0x83, 0x65 };
std::vector<BYTE> timeSignature = { 0x5E, 0x5F, 0x5D, 0xC3, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x89, 0x00, 0x04 };
