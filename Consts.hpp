#pragma once

#include<stdint.h>

const int SPEED_PER_SEC = 50; //% клетки в секунду
const static uint8_t XSIZE = 30;
const static uint8_t YSIZE = 30;

enum class Directions : uint8_t { LEFT, RIGTH, UP, DOWN, UNDEFINED };
enum class Colors : uint8_t { RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, MAX };
enum class ServerTypes { LOCAL, CLIENT, SERVER };
