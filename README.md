I have been thinking about how to make a full standalone lora reticulum node/client as cheap as possible, and easy to assemble, so it becomes semi-disposable.

The goal with this is to build this (once I have a lil more free-time) and make it as easy as possible to create a kind of hybrid pocket-device, that does similar to RNode (like on a heltec v3) but also allows you to free yourself from using a phone or more expensive hardware (like those [fancier blackberry-keyboard things](https://lilygo.cc/products/t-deck?srsltid=AfmBOooNlbN6kFLsLGA_LThQp4CTwV2MoVRcYSb0au0VrHBD6YNL0vFe).)

I am currently too busy to work on this, but I will get to it, eventually.

### Hardware

- [CYDs](https://www.aliexpress.us/item/3256808128499162.html) are cool & cheap, self-contained, and easy to source. They have [pretty good support](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)
- SPI radio-board, like [this](https://www.aliexpress.us/item/3256805989899200.html) Get the right freq for your region (in US: 915)


### Features:

- (resistive) 320 x 240 LCD touchscreen. not great, but usable.
- ESP32 (arduino, miicropython, circuitpython, good arduino support for reticulum)
- bluetooth & wifi
- sound: it has a speaker-jack (and a i2s chip on-board.) This would be cool for notifications, and maybe eventuially voice!
- no battery, but could run off a standard old phone-battery
- should cost about $20, with shipping
- does  not require any assembly, other than connecting the radio (5 wires, including power)


### Notes

- look into LVGL. There is already a [nice onscreen keyboard demo](https://docs.lvgl.io/8.3/widgets/extra/keyboard.html#keyboard-with-text-area)
