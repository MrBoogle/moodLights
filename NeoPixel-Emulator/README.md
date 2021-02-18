# NeoPixel-Emulator
NeoPixel Emulator in Python with Pyglet

This emulator has two main ways to emulate. One is a simple NeoPixel strip with the basic setPixelColor and some simple effects, and the other is a matrix emulator. You can create large matrices that dynamically resize the window based on the size. There is also an almost one-to-one port of the Adafruit GFX library, which allows for the creation of many graphics primitives and even the drawing of monochrome bitmaps from an array. The only thing not implemented is tiling and printing of text.

## Dependancies:
```
Python3
Git
Pip3
```

## Installation
```
git clone https://github.com/MrBoogle/moodLights.git
pip3 install pyglet
```

## Testing
python3 neopixel_viewer.py
