# Woordklok

Word clock originally based on [Bitlair Wordclock](https://github.com/bitlair/wordclock) but rebuilt from scratch.

## Construction

The clock was built following the Bitlair instructions. I've made some additional material but this could still be considered a work in progress.

There are photos of the different parts in the pictures-folder.

### Components:

* Ikea RIBBA Photo Frame 23x23. Unfortunately this frame seems to be out of production. You will need to adapt the hardware design to the frame you use.
* Front plate with lettering.

  * There are a few templates in the[ template](templates/) folder. There are several parts: the clockface itself with the letters, a grid with the positioning of the LEDs and spacers to keep the LED light from radiating to other letters.
  * If you want to design your own letterplate, I've made an Excel file to check the letters/words with the different needed time-phrases.
* An arduino chip: I've used a NodeMCU and a ESP32 S3 (Lolin Wemos S3 mini). The last one has my preference because it's small and has touch pins.
* A LED strip that has enough leds for the entire plate (114 in my case). I've used a WS2812B strip.
* Optional:

  * Light sensitive sensor / LDR for adaptive brightness.
  * You can make the top surface touch sensitive using tin foil with a wire going to a touch pin. If you glue a thin piece of paper (silk paper) in the right color to the tin foil, this makes an 'invisible' button.

  ### Assembly

## Main code for the clock works with touch sensor and light sensor

TODO

* web interface mini
