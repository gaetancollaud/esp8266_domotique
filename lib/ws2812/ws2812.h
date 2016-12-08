#ifndef _WS2812_H
#define _WS2812_H

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "Arduino.h"

/**
 * Abstract class to help debug class
 */
class WS2812 {
public:
  WS2812(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, NeoPixelAnimator* animations);

	void init();

  void loop();

protected:

   void setupAnimationSet();


   NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip;
   NeoPixelAnimator* animations;
};

#endif
