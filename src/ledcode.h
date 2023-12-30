#pragma once

#include <headers.h>
#include <clockface.h>

NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(NUM_LEDS);
NeoBuffer<NeoBufferMethod<NeoGrbFeature>> klokImage(NUM_LEDS, 1, NULL);
NeoBuffer<NeoBufferMethod<NeoGrbFeature>> targetKlokImage(NUM_LEDS, 1, NULL);
NeoPixelAnimator animations(NUM_LEDS, NEO_CENTISECONDS);
NeoPixelAnimator rainAnimations(MAXDROPS + 1);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

AnimEaseFunction moveEase = NeoEase::Linear;
struct RainDrop
{
  int column;
  int lastPixel;
};

struct RainDrop drops[MAXDROPS];
int nrDrops = 0;

#define LED_RAINBOW_TIME 1000
#define LED_UPDATE_TIME 500

int ledRandomHue = 0;
long ledRandomTime = 0;
byte remapLEDS[NUM_ROWS][NUM_COLS];
byte counter = 1;
int speed = 1;
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

void ledSetup()
{
  randomSeed(analogRead(0));
  strip.Begin();
  clearLEDS();

  for (int ledNr = 0 + OFFSET; ledNr < NUM_LEDS; ledNr++)
  {
    RCCoord coord = calcCoord(ledNr);
    remapLEDS[coord.row][coord.col] = ledNr;
  }

  statusLed(WIFI, red);
  statusLed(Time, red);
}

// LEDs are chained from the bottom right -> left, left -> right, right -> left, ...
// calculate row/col based on led number
struct RCCoord calcCoord(int pos)
{
  RCCoord coord;
  coord.row = int((NUM_LEDS - pos - 1) / NUM_COLS);
  if ((coord.row % 2) == 0)
  {
    coord.col = (NUM_LEDS - pos - 1) % NUM_COLS;
  }
  else
  {
    coord.col = (NUM_COLS - 1) - (NUM_LEDS - pos - 1) % NUM_COLS;
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
  for (int indexPixel = OFFSET; indexPixel < strip.PixelCount(); indexPixel++)
  {
    color = strip.GetPixelColor(indexPixel);
    color.Darken(darkenBy);
    setColor(indexPixel, color);
  }
}

void showSplash()
{
  clearLEDS();
  for (int pos = 0; pos < sizeof(splashScreen) / sizeof(int); pos++)
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
  int lastPixel = drops[dropNr].lastPixel;
  // use the curved progress to calculate the pixel to effect
  int nextPixel;
  nextPixel = int(progress * NUM_ROWS);
  if (nextPixel < NUM_ROWS)
  {
    // if progress moves fast enough, we may move more than
    // one pixel, so we update all between the calculated and
    // the last
    if (lastPixel != nextPixel)
    {
      for (int i = lastPixel + 1; i != nextPixel; i++)
      {
        setColor(remapLEDS[i][drops[dropNr].column], green);
      }
    }
    setColor(remapLEDS[nextPixel][drops[dropNr].column], green);

    drops[dropNr].lastPixel = nextPixel;
  }
  if (param.state == AnimationState_Completed)
  {
    drops[dropNr].column = random(NUM_COLS);
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

void dropLetter(int targetPosition)
{
  RCCoord coord = calcCoord(targetPosition);
  int previousLED = 0;
  bool keepPrevious = false;
  RgbColor previousColor = black;
  for (int row = 0; row < coord.row + 1; row++)
  {
    int LEDpos = remapLEDS[row][coord.col];
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
  drops[0].column = random(NUM_COLS);
  drops[1].column = random(NUM_COLS);
  drops[2].column = random(NUM_COLS);
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
      drops[nrDrops].column = random(NUM_COLS);
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
    color = hexToRgb(config.singleColor.color);
    backgroundColor = hexToRgb(config.singleColor.backgroundColor, color);
    break;
  case hourlyColor:
    color = hexToRgb(config.hourlyColor.color[dateHours]);
    backgroundColor = hexToRgb(config.hourlyColor.backgroundColor, color);
    break;
  case rainbowColor:
    color = HueToRgbColor(ledRandomHue);
    backgroundColor = hexToRgb(config.hourlyColor.backgroundColor, color);
    break;
  case wordColor:
    backgroundColor = hexToRgb(config.wordColor.backgroundColor);
    break;
  }

  for (ClockfaceWord word : clockface)
  {
    // last word is background
    if (word.colorCodeInTable == NUMWORDS)
    {
      for (int pos : word.leds)
        {
          setColor(pos, backgroundColor);
        }
    }
    else
    {
      if (word.isActive(dateHours % 12, dateMinutes))
      {
        for (int pos : word.leds)
        {
          if (config.ledMode != wordColor)
          {
            setColor(pos, color);
          }
          else
          {
            color = hexToRgb(config.wordColor.color[word.colorCodeInTable]);
            setColor(pos, color);
          }
        }
        // debug_printf(word.label);
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

void statusLed(statusLedList status, NeoBufferMethod<NeoGrbFeature>::ColorObject color)
{
  // Serial.println("StatusLED ");
  switch (status)
  {
  case WIFI:
    setColor(3, color);
    break;
  case Time:
    setColor(2, color);
    break;
  case AP:
    setColor(0, color);
    break;
  }
}

byte calculateTimeBrightness()
{
  if (dateHours > config.timeBrightness.timeSlot[0] && dateHours < config.timeBrightness.timeSlot[1])
  {
    return config.timeBrightness.brightness[1];
  }
  else if (dateHours < config.timeBrightness.timeSlot[0] || dateHours > config.timeBrightness.timeSlot[1])
  {
    return config.timeBrightness.brightness[0];
  }
  else if (dateHours == config.timeBrightness.timeSlot[0])
  {
    return constrain(
        map(dateMinutes, 0, 29, config.timeBrightness.brightness[0], config.timeBrightness.brightness[1]),
        config.timeBrightness.brightness[0], config.timeBrightness.brightness[1]);
  }
  else if (dateHours == config.timeBrightness.timeSlot[1])
  {
    return constrain(
        map(dateMinutes, 0, 29, config.timeBrightness.brightness[1], config.timeBrightness.brightness[0]),
        config.timeBrightness.brightness[0], config.timeBrightness.brightness[1]);
  }
  return config.timeBrightness.brightness[0];
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
        map(LDRvalue, config.ldrBrightness.ldrRange[0], config.ldrBrightness.ldrRange[1], config.ldrBrightness.brightness[0], config.ldrBrightness.brightness[1]),
        config.ldrBrightness.brightness[0], config.ldrBrightness.brightness[1]);
    break;
  case timeBrightness:
    brightness = calculateTimeBrightness();
    break;
  }
  return brightness;
}

void setColor(int ledNr, NeoBufferMethod<NeoGrbFeature>::ColorObject color)
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

RgbColor getColor(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &image, int ledNr)
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
  for (int pixel = 0; pixel < NUM_LEDS; pixel++)
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
    strip.Show();
  }
}

void setStatusLeds()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    statusLed(WIFI, green);
  }
  else
  {
    statusLed(WIFI, red);
  }
  if (dateYear > 70)
  {
    statusLed(Time, green);
  }
  else
  {
    statusLed(Time, red);
  }
  if ((WiFi.getMode() != WIFI_STA))
  {
    statusLed(AP, red);
  }
  else
  {
    statusLed(AP, black);
  }
  showFace(false);
}

RgbColor HueToRgbColor(int colorValue)
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