#ifndef _WS2812_H
#define _WS2812_H

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "Arduino.h"

class WS2812 {
public:
  WS2812(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, NeoPixelAnimator* animations);

  void init();

  void loop();

  void setColor(std::string color);

protected:

  RgbColor lastColor;

  void setupAnimationSet(RgbColor targetColor);

  RgbColor getRgb(float h, float s, float v);


  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip;
  NeoPixelAnimator* animations;
};

#endif
