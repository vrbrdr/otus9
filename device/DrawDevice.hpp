#pragma once

#include "../GameState.hpp"

class DrawDevice {
  public:
    virtual void draw(GameState& state) = 0;
    virtual ~DrawDevice() {}
};