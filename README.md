# Torch Timer (for use with ShadowDark RPG)
## Why
I'm a big fan of the Shadowdark RPG, an Ennie award winning game published by [The Arcane Library](https://www.thearcanelibrary.com/pages/shadowdark). One of Shadowdark's standout features is the Torch Timer. When the adventurers light a torch in game, the gamemaster starts a real-world 60 minute timer. As the players play, the timer ticks down. Sometimes they pay attention and rememebr to light another torch before time is up - sometimes they don't! Since only monsters can see in the dark, this can become a major isuee.

While you could use any time you like, I have been wanting a more visual timer that could add to the atmosphere of the game. Flickering firelight would be ideal, but not practical. I decided on the next best thing - LEDs.

## What
**A Gaming Table with an Integrated Torch Timer:**

![image](https://github.com/user-attachments/assets/76a455a4-ae8d-4026-9a2a-5f31d72829fc)

Here's a YouTube of the timer in action.

[![Video of Gaming Table](https://img.youtube.com/vi/pY_wV5WMrg4/0.jpg)](https://youtube.com/shorts/pY_wV5WMrg4)

## How Does this Work?

I added a pair of WS2812 addressable RBG LED strips to the recessed area of my gaming table. The strips are wired to an ESP32 module (specifically, an Ideaspark ESP32 Dev Board with an integrated 0.96in OLED display - link below) and an appropriately sized power supply. The ESP32 is configured to attach to a Wi-Fi network and operate a web server. When it starts up, the ESP32 connects to the Wi-Fi and presents its IP address on the OLED display. Connect a web browser to the IP, and you are presented with the following rudimentary yet servicable interface:

![image](https://github.com/user-attachments/assets/94787ed8-23fc-405c-8f91-1c4e7adae08f)

To start the torch, you select one of the four Duration options and click 'Light Torch'. The "torch" lights and the countdown begins. The LED light strips will display a fire effect pattern until the timer expires or the "Extinguish Torch" button is clicked.

The Duration options are:
- The standard Shadowdark 1 hour timer.
- The Sly Hour, named for [Sly Flourish](https://slyflourish.com/delving_into_shadowdark.html), who came up with the idea. When this option is selected, the timer is set for 60 minutes minus 1d12 minutes. 
- Shadowdark's "Blitz Mode" half hour timer.
- A Duration of the Game Master's choosing.


## To set this up for yourself, you'll need some hardware and the software in this repo.

### Hardware (With links to Amazon)

- One or more WS2812B addressable RBG LED Strips (I used two [5m strips](https://amzn.to/3XaA1tH). [1m strips](https://amzn.to/3yF1kmp) are also available.)
- A microcontroller ([ESP32 Dev Board](https://amzn.to/3WKjwD4)) I used one with an integrated OLED Display. If you prefer to use an external display or no display at all (be sure to alter to the code to account for that), then somehting like [this](https://amzn.to/3SSrPLY) would work just fine.
- An appropriate 5V [power supply]( https://amzn.to/3MbQs2p). I am using 5V because the WS2812B strips use that and I can also power the microcontroller with it. Be sure you know what type of power supply you need and purchase accordingly.
- A Gaming Table (or other suitable mounting point for the LED Strips.
- Something to access the web page. A tablet, laptop, or smart phone works nicely. If you look at the source code, you'll find the URLs for the light and extinguish commands. You can trigger the effect with anything that can get those URLs. I also have a Zigbee button configured with Home Assistant to trigger the effect.

Before you can use it, you have to connect all this stuff, like so:

![image](https://github.com/user-attachments/assets/17c558ea-239e-4ea8-b597-bcf0837018a3)

Your wiring may differ. For example, you may want to use a USB adapter to power the ESP32. (**Caution:** Be sure you do not hook up the 5V power and USB at the same time. For example, you should disconnect the 5V from the ESP32 when connecting the USB to upload the code.)

How you connect the LED strip(s) may vary. I have two strips connected in parallel (both data lines connect to GPIO5 of my ESP32, so they mirror each other. If you want to connect strips to multiple GPIO pins, you will need to modify the code to account for that.

### Software

The software is in this repo. One way to load it is to:

- Clone the repo to your local machine
- Use VScode (with [PlatformIO](https://platformio.org/) to configure the code.
  - Edit the secrets.h file to put in your Wi-Fi SSID and Password
  - Change the values for NUM_LEDS (the total number of LEDs in your strip(s)) and LED_PIN (the GPIO pin, to which you connected the strip(s) data line(s)).
  - If you use a different board than I did, you will also need to change the OLED settings.
- Compile and load the code
- Test your setup





The flame effect is from https://github.com/davepl/DavesGarageLEDSeries (Episdoe 10)

This project uses the FastLED Library (http://fastled.io/)
