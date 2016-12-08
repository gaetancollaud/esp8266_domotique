#include "ws2812.h"


#define colorSaturation 20

WS2812::WS2812(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, NeoPixelAnimator* animations) : strip(strip), animations(animations){
}

void WS2812::init(){
  this->strip->Begin();

  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);

  for (int shifts = 3; shifts < 31; shifts += 3){
    seed ^= analogRead(0) << shifts;
    delay(1);
  }

Serial.print("seed: ");
  Serial.println(seed);
  randomSeed(seed);

}

void WS2812::loop(){

  if (animations->IsAnimating()) {
    animations->UpdateAnimations();
    strip->Show();
  } else {
    setupAnimationSet();
  }

  //     const uint8_t peak = 50;
  // RgbColor targetColor = RgbColor(random(peak), random(peak), random(peak));
  // strip->ClearTo(targetColor);
  // strip->Show();
  // delay(1000);

}


void WS2812::setupAnimationSet()
{
    const uint8_t peak = 50;
RgbColor targetColor = RgbColor(random(peak), random(peak), random(peak));
RgbColor originalColor = strip->GetPixelColor(0);
    // setup some animations
    // for (uint16_t pixel = 0; pixel < strip->PixelCount(); pixel++)
    // {

        // pick a random duration of the animation for this pixel
        // since values are centiseconds, the range is 1 - 4 seconds
        uint16_t time = 300;

        // each animation starts with the color that was present
        // and ends with a random color

        AnimUpdateCallback animUpdate = [=](const AnimationParam& param)
        {
            // progress will start at 0.0 and end at 1.0
            // we convert to the curve we want
            float progress = NeoEase::QuadraticInOut(param.progress);

            // use the curve value to apply to the animation
            RgbColor updatedColor = RgbColor::LinearBlend(originalColor, targetColor, progress);
            strip->ClearTo(updatedColor);
            // strip->SetPixelColor(pixel, updatedColor);
        };

        // now use the animation properties we just calculated and start the animation
        // which will continue to run and call the update function until it completes
        animations->StartAnimation(0, time, animUpdate);
    // }
}
