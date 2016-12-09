#include "ws2812.h"


#define colorSaturation 20

WS2812::WS2812(NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>* strip, NeoPixelAnimator* animations) :
  strip(strip), animations(animations), lastColor(RgbColor(0)){
}

void WS2812::init(){
  this->strip->Begin();
}

void WS2812::loop(){

  if (animations->IsAnimating()) {
    animations->UpdateAnimations();
    strip->Show();
  }
}


void WS2812::setColor(std::string color){
  if(color.compare("ON")==0){
    this->setupAnimationSet(this->lastColor);
  }else if(color.compare("OFF")==0){
    this->setupAnimationSet(RgbColor(0));
  }else{
    int pos1 = color.find(",");
    int pos2 = color.find(",", pos1+1);
    float h = atof(color.substr(0, pos1).c_str());
    float s = atof(color.substr(pos1+1, pos2-pos1-1).c_str());
    float v = atof(color.substr(pos2+1, color.npos).c_str());

    RgbColor rgb = this->getRgb(h,s,v);

    Serial.print("RGB\t");

    this->setupAnimationSet(rgb);
    this->lastColor = rgb;
    Serial.print(rgb.R);
    Serial.print("\t");
    Serial.print(rgb.G);
    Serial.print("\t");
    Serial.println(rgb.B);
  }

}

RgbColor WS2812::getRgb(float h, float s, float v){
  double r = 0, g = 0, b = 0;
	s /= 100;
	v /= 100;

	float t = (float) (((int) (h / 60 * 1000)) % 2000) / 1000;
	float c = v*s;
	float x = c * (1 - abs(t - 1));
	float m = v - c;

	if (h < 60) {
		r = c;
		g = x;
	} else if (h < 120) {
		r = x;
		g = c;
	} else if (h < 180) {
		g = c;
		b = x;
	} else if (h < 240) {
		g = x;
		b = c;
	} else if (h < 300) {
		r = x;
		b = c;
	} else {
		r = c;
		b = x;
	}

  return RgbColor((r+m)*255, (g+m)*255, (b+m)*255);
}

void WS2812::setupAnimationSet(RgbColor targetColor)
{
  RgbColor originalColor = strip->GetPixelColor(0);
  uint16_t time = 300;
  AnimUpdateCallback animUpdate = [=](const AnimationParam& param)  {
    float progress = NeoEase::QuadraticInOut(param.progress);
    RgbColor updatedColor = RgbColor::LinearBlend(originalColor, targetColor, progress);
    strip->ClearTo(updatedColor);
  };

  animations->StartAnimation(0, time, animUpdate);
}
