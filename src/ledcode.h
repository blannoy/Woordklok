#pragma once
#define MAXLEDS 144
#include "headers.h"
#define colorSaturation 64
#ifdef ESP32
// NeoPixelBus<NeoGrbFeature, DotStarEsp32DmaSpiMethod> strip(config.clockfaceLayout.totalLeds);
NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> strip(MAXLEDS, LEDSTRIPPIN);
#else
NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(MAXLEDS);
//NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> statusStrip(4);
#endif
NeoBuffer<NeoBufferMethod<NeoGrbFeature>> klokImage(MAXLEDS, 1, NULL);
NeoBuffer<NeoBufferMethod<NeoGrbFeature>> targetKlokImage(MAXLEDS, 1, NULL);
NeoPixelAnimator animations(MAXLEDS, NEO_CENTISECONDS);
NeoPixelAnimator rainAnimations(MAXDROPS + 1);

RgbColor orange(colorSaturation, colorSaturation/2, 0);
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

AnimEaseFunction moveEase = NeoEase::Linear;
struct RainDrop
{
  uint8_t column;
  uint8_t lastPixel;
};

struct RainDrop drops[MAXDROPS];
uint8_t nrDrops = 0;

#define LED_RAINBOW_TIME 1000
#define LED_UPDATE_TIME 500

uint8_t ledRandomHue = 0;
long ledRandomTime = 0;
byte **remapLEDS;
byte counter = 1;
uint8_t speed = 1;
bool ledTransit = false;

// define a custom shader object that provides brightness support
// based upon the NeoShaderBase
template <typename T_COLOR_FEATURE>
class BrightnessShader : public NeoShaderBase
{
public:
  BrightnessShader()
      : NeoShaderBase(),
        _brightness(48) // default to full bright
  {
  }

  // required for a shader object, it will be called for
  // every pixel
  void Apply(uint16_t index, uint8_t *pDest, const uint8_t *pSrc)
  {
    // we don't care what the index is so we ignore it
    //
    // to apply our brightness shader,
    // use the source color, modify, and apply to the destination

    // for every byte in the pixel,
    // scale the source value by the brightness and
    // store it in the destination byte
    const uint8_t *pSrcEnd = pSrc + T_COLOR_FEATURE::PixelSize;
    while (pSrc != pSrcEnd)
    {
      *pDest++ = (*pSrc++ * (uint16_t(_brightness) + 1)) >> 8;
    }
  }

  // provide an accessor to set brightness
  void setBrightness(uint8_t brightness)
  {
    _brightness = brightness;
    Dirty(); // must call dirty when a property changes
  }

  // provide an accessor to get brightness
  uint8_t getBrightness()
  {
    return _brightness;
  }

private:
  uint8_t _brightness;
};

// create an instance of our shader object with the same feature as our buffer
BrightnessShader<NeoGrbFeature> shader;

void clockfaceLEDSetup()
{
  remapLEDS = (byte **)malloc(config.clockfaceLayout.wordGridRows * sizeof(byte *));
  for (uint8_t i = 0; i < config.clockfaceLayout.wordGridRows; i++)
    remapLEDS[i] = (byte *)malloc(config.clockfaceLayout.wordGridCols * sizeof(byte));
  for (uint8_t ledNr = 0 + config.clockfaceLayout.extraLEDs; ledNr < config.clockfaceLayout.totalLeds; ledNr++)
  {
    RCCoord coord = calcCoord(ledNr);
    remapLEDS[coord.row][coord.col] = ledNr;
  }
}

void statusLedSetup(){
  strip.Begin();
  statusLed(WIFI, red);
  statusLed(CONFIG, red);
  statusLed(TIME, red);
  strip.Show();
}
void ledSetup()
{
  randomSeed(analogRead(LDRPIN));
  clearLEDS();
  showFace(false);
}

// LEDs are chained from the bottom right -> left, left -> right, right -> left, ...
// calculate row/col based on led number
struct RCCoord calcCoord(uint8_t pos)
{
  RCCoord coord;
  coord.row = int((config.clockfaceLayout.totalLeds - pos - 1) / config.clockfaceLayout.wordGridCols);
  if ((coord.row % 2) == 0)
  {
    coord.col = (config.clockfaceLayout.totalLeds - pos - 1) % config.clockfaceLayout.wordGridCols;
  }
  else
  {
    coord.col = (config.clockfaceLayout.wordGridCols - 1) - (config.clockfaceLayout.totalLeds - pos - 1) % config.clockfaceLayout.wordGridCols;
  }

  return coord;
}

void clearLEDS()
{
  // Serial.println("ClearLED");
  clearLEDTo(klokImage, black);
  clearLEDTo(targetKlokImage, black);
  strip.ClearTo(black);
  animations.StopAll();
  strip.Show();
  // ledTransit=true;
  // showFace(false);
}

void FadeAll(uint8_t darkenBy)
{
  RgbColor color;
  for (uint8_t indexPixel = config.clockfaceLayout.extraLEDs; indexPixel < strip.PixelCount(); indexPixel++)
  {
    color = strip.GetPixelColor(indexPixel);
    color.Darken(darkenBy);
    setColor(indexPixel, color);
  }
}

void showSplash()
{
  clearLEDS();
  for (uint8_t pos = 0; pos < sizeof(splashScreen); pos++)
  {
    dropLetter(splashScreen[pos]);
    /*    klokImage.SetPixelColor(splashScreen[pos], blue);
        showFace();
        delay(200);*/
  }
  delay(2000);
  clearLEDS();
}

void FadeAnimUpdate(const AnimationParam &param)
{
  if (param.state == AnimationState_Completed)
  {
    FadeAll(5);
    rainAnimations.RestartAnimation(param.index);
  }
}

void MoveAnimUpdate(const AnimationParam &param)
{
  // apply the movement animation curve
  float progress = moveEase(param.progress);
  byte dropNr = param.index - 1;
  uint8_t lastPixel = drops[dropNr].lastPixel;
  // use the curved progress to calculate the pixel to effect
  uint8_t nextPixel;
  nextPixel = int(progress * config.clockfaceLayout.wordGridRows);
  if (nextPixel < config.clockfaceLayout.wordGridRows)
  {
    // if progress moves fast enough, we may move more than
    // one pixel, so we update all between the calculated and
    // the last
    if (lastPixel != nextPixel)
    {
      for (uint8_t i = lastPixel + 1; i != nextPixel; i++)
      {
        setColor(remapLEDS[i][drops[dropNr].column], green);
      }
    }
    setColor(remapLEDS[nextPixel][drops[dropNr].column], green);

    drops[dropNr].lastPixel = nextPixel;
  }
  if (param.state == AnimationState_Completed)
  {
    drops[dropNr].column = random(config.clockfaceLayout.wordGridCols);
    // Serial.print("new drop ");
    // Serial.println(drops[dropNr].column);
    // drops[dropNr].column=1;
    drops[dropNr].lastPixel = 0;
    delay(random(200));
    rainAnimations.RestartAnimation(param.index);
  }
}

void stopRain()
{
  if (rainAnimations.IsAnimating())
  {
    rainAnimations.StopAll();
    clearLEDS();
    setStatusLeds();
  }
}

void dropLetter(uint8_t targetPosition)
{
  RCCoord coord = calcCoord(targetPosition);
  uint8_t previousLED = 0;
  RgbColor previousColor = black;
  for (uint8_t row = 0; row < coord.row + 1; row++)
  {
    uint8_t LEDpos = remapLEDS[row][coord.col];
    if (row > 0)
    {
      setColor(previousLED, previousColor);
    }
    previousColor = getColor(klokImage, LEDpos);
    setColor(LEDpos, blue);
    showFace(false);
    delay(100);
    previousLED = LEDpos;
  }
}

void setupRain()
{

  rainAnimations.StartAnimation(0, 20, FadeAnimUpdate);
  drops[0].column = random(config.clockfaceLayout.wordGridCols);
  drops[1].column = random(config.clockfaceLayout.wordGridCols);
  drops[2].column = random(config.clockfaceLayout.wordGridCols);
  nrDrops = 3;
  rainAnimations.StartAnimation(1, 1000, MoveAnimUpdate);
}

void rainLoop()
{
  if (nrDrops < MAXDROPS)
  {
    // Serial.println("Generate drop");
    if (random(40) == 2)
    {
      drops[nrDrops].column = random(config.clockfaceLayout.wordGridCols);
      rainAnimations.StartAnimation(nrDrops + 1, 1000, MoveAnimUpdate);
      drops[nrDrops].lastPixel = 0;
      nrDrops++;
    }
  }
  rainAnimations.UpdateAnimations();

  targetKlokImage.Blt(strip, 0);
  strip.Show();
}

void ledShowClockface()
{

  clearLEDTo(targetKlokImage, black);

  RgbColor color;
  RgbColor backgroundColor;

  switch (config.ledMode)
  {
  case singleColor:
    color = colorDefToRgb(config.singleColor.color);
    backgroundColor = colorDefToRgb(config.singleColor.backgroundColor, color).Dim(config.backgroundDimFactor);
    break;
  case hourlyColor:
    color = colorDefToRgb(config.hourlyColor.color[dateHours]);
    backgroundColor = colorDefToRgb(config.hourlyColor.backgroundColor, color).Dim(config.backgroundDimFactor);
    break;
  case rainbowColor:
    color = HueToRgbColor(ledRandomHue);
    backgroundColor = colorDefToRgb(config.rainbowColor.backgroundColor, color).Dim(config.backgroundDimFactor);
    break;
  case wordColor:
    backgroundColor = colorDefToRgb(config.wordColor.backgroundColor).Dim(config.backgroundDimFactor);
    break;
  }
  for (uint8_t iWord = 0; iWord < config.clockfaceLayout.totalWords + 1; iWord++)
  {
    ClockfaceWord word = config.clockface[iWord];
    if (iWord == config.clockfaceLayout.totalWords)
    {
      for (uint8_t pos = 0; pos < strlen(word.label); pos++)
      {
        setColor(word.leds[pos], backgroundColor);
      }
    }
    else
    {
      if (isActiveCheck(word.isActive, dateHours % 12, dateMinutes))
      {
        for (uint8_t pos = 0; pos < strlen(word.label); pos++)
        {
          {
            if (config.ledMode != wordColor)
            {
              setColor(word.leds[pos], color);
            }
            else
            {
              color = colorDefToRgb(config.wordColor.color[word.colorCodeInTable]);
              setColor(word.leds[pos], color);
            }
          }
        }
      }
      else
      {
        for (uint8_t pos = 0; pos < strlen(word.label); pos++)
        {
          {
            if (getColor(targetKlokImage, word.leds[pos]) == black)
            {
              setColor(word.leds[pos], backgroundColor);
            }
          }
        }
      }
    }
  }
  showFace(true);
}

void clockLoop()
{

  // TODO check if correct time, otherwise show blinking minute (also for WIFI connection? LED1 = WIFI (red/green), LED2 = time (red/green))
  long current = millis();
  if ((current - ledRandomTime) > LED_RAINBOW_TIME)
  {
    ledRandomTime = current;
    ledRandomHue = (ledRandomHue + 1) % 255;
  }

  if (!doAnimation())
  {
    if ((millis() - lastLedUpdate) > LED_UPDATE_TIME)
    {
      lastLedUpdate = millis();
      ledShowClockface();
    }
  }
}



byte calculateTimeBrightness()
{
  if (dateHours > config.timeBrightness.timeSlot.startHour && dateHours < config.timeBrightness.timeSlot.endHour)
  {
    return config.timeBrightness.brightness.max;
  }
  else if (dateHours < config.timeBrightness.timeSlot.startHour || dateHours > config.timeBrightness.timeSlot.endHour)
  {
    return config.timeBrightness.brightness.min;
  }
  else if (dateHours == config.timeBrightness.timeSlot.startHour)
  {
    return constrain(
        map(dateMinutes, 0, 29, config.timeBrightness.brightness.min, config.timeBrightness.brightness.max),
        config.timeBrightness.brightness.min, config.timeBrightness.brightness.max);
  }
  else if (dateHours == config.timeBrightness.timeSlot.endHour)
  {
    return constrain(
        map(dateMinutes, 0, 29, config.timeBrightness.brightness.max, config.timeBrightness.brightness.min),
        config.timeBrightness.brightness.min, config.timeBrightness.brightness.max);
  }
  return config.timeBrightness.brightness.min;
}

byte calcBrightness()
{
  byte brightness = 0;
  switch (config.brightnessMode)
  {
  case fixedBrightness:
    brightness = config.fixedBrightness.brightness;
    break;
  case ldrBrightness:
    brightness = constrain(
        map(LDRvalue, config.ldrBrightness.ldrRange.dark, config.ldrBrightness.ldrRange.bright, config.ldrBrightness.brightness.min, config.ldrBrightness.brightness.max),
        config.ldrBrightness.brightness.min, config.ldrBrightness.brightness.max);
    break;
  case timeBrightness:
    brightness = calculateTimeBrightness();
    break;
  }
  return brightness;
}

void setColor(uint8_t ledNr, NeoBufferMethod<NeoGrbFeature>::ColorObject color)
{
  // check if color changed
  RgbColor currentColor = getColor(klokImage, ledNr);
  RgbColor targetColor = color;
  targetKlokImage.SetPixelColor(ledNr, 0, color);
  if (currentColor != targetColor)
  {
    // Serial.println("Change in clockface");
    ledTransit = true;
  }
}

RgbColor getColor(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &image, uint8_t ledNr)
{
  return image.GetPixelColor(ledNr, 0);
}

void clearLEDTo(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &image, NeoBufferMethod<NeoGrbFeature>::ColorObject color)
{
  image.ClearTo(color);
}

void SetupAnimationSet()
{
  uint16_t time = 200;
  for (uint8_t pixel = 0; pixel < config.clockfaceLayout.totalLeds; pixel++)
  {
    RgbColor originalColor = getColor(klokImage, pixel);
    RgbColor targetColor = getColor(targetKlokImage, pixel);
    AnimEaseFunction easing = NeoEase::CubicCenter;
    AnimUpdateCallback animUpdate = [=](const AnimationParam &param)
    {
      float progress = easing(param.progress);
      RgbColor updatedColor = RgbColor::LinearBlend(originalColor, targetColor, progress);
      klokImage.SetPixelColor(pixel, 0, updatedColor);
      ledTransit = true;
    };
    animations.StartAnimation(pixel, time, animUpdate);
  }
}

bool doAnimation()
{
  if (animations.IsAnimating())
  {
    animations.UpdateAnimations();
    klokImage.Render<BrightnessShader<NeoGrbFeature>>(strip, shader);
    strip.Show();
  }
  return animations.IsAnimating();
}
void showFace(bool doTransit)
{
  shader.setBrightness(calcBrightness());

  // Serial.println("ShowFace");

  if (doTransit && ledTransit)
  {
    ledTransit = false;
    SetupAnimationSet();
  }
  if (animations.IsAnimating())
  {
    animations.UpdateAnimations();
  }
  else
  {
    targetKlokImage.Blt(klokImage, 0);
  }
  if (ledTransit)
  {
    klokImage.Render<BrightnessShader<NeoGrbFeature>>(strip, shader);
    //   for (uint8_t i=0;i<config.clockfaceLayout.totalLeds;i++){
    //     RgbColor color = strip.GetPixelColor(i);
    //     color=color.Dim(random(120));
    //     strip.SetPixelColor(i, color);
    // }
    strip.Show();
  }
}

void showFaceSimple()
{
  targetKlokImage.Blt(klokImage, 0);
  klokImage.Render<BrightnessShader<NeoGrbFeature>>(strip, shader);
}
void statusLed(statusLedList status, NeoBufferMethod<NeoGrbFeature>::ColorObject color)
{
  // Serial.println("StatusLED ");
  switch (status)
  {
  case WIFI:
    strip.SetPixelColor(3, color);
    break;
  case CONFIG:
    strip.SetPixelColor(2, color);
    break;
  case TIME:
    strip.SetPixelColor(1, color);
    break;
  }
  strip.Show();
}
void setStatusLeds()
{
  if ((WiFi.getMode() != WIFI_STA))
  {
    statusLed(WIFI, blue);
  }
  else
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      statusLed(WIFI, green);
    }
    else
    {
      statusLed(WIFI, red);
    }
  }
  if (configLoaded)
  {
    statusLed(CONFIG, green);
  }
  else
  {
    statusLed(CONFIG, red);
  }

  if (dateYear > 70)
  {
    statusLed(TIME, green);
  }
  else
  {
    if (millis()/1000 % 2 == 0)
    {
      statusLed(TIME, orange);
    }
    else
    {
      statusLed(TIME, red);
    }
  }

  //strip.Show();
}

RgbColor HueToRgbColor(uint8_t colorValue)
{
  if (colorValue == -1)
  {
    return RgbColor(white);
  }
  else
  {
    return RgbColor(HslColor(colorValue / 255.0, 1.0f, 0.5f));
  }
}
