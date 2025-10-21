> [!WARNING]
> There is nothing here, yet, other than ideas. I am currently too busy to work on this, but I will get to it, eventually, and add some actual code.

I have been thinking about how to make a full standalone lora reticulum node/client as cheap as possible, and easy to assemble, so it becomes semi-disposable (cheap & easy to make more.)

The goal with this is to make it as easy as possible to create a kind of hybrid pocket-device, that does similar to RNode (like on a heltec v3) but also allows you to free yourself from using a phone or more expensive hardware (like those [fancier blackberry-keyboard things](https://lilygo.cc/products/t-deck?srsltid=AfmBOooNlbN6kFLsLGA_LThQp4CTwV2MoVRcYSb0au0VrHBD6YNL0vFe).)

### Features:

- should cost under $30, with shipping
- (resistive) 320 x 240 LCD touchscreen. not great, but usable.
- ESP32 (arduino, micropython, circuitpython, good arduino support for reticulum)
- bluetooth & wifi
- sound: it has a speaker-jack (and a i2s amp-chip on-board.) This would be cool for notifications, and maybe eventually voice!
- SD card for storing notifcation sounds, downloaded files & messages, identity, etc. Could be encrypted-at-rest, for better physical security (turn it off, and no one can get your private key without a password.)
- no battery, but could run off a standard old phone-battery. I really like [these cute, lightweight, long-life things](https://www.amazon.com/DCHK-20000mAh-Charging-Portable-Motorola/dp/B0DPWVYMN5)
- does not require any assembly, other than connecting the radio (5 wires, including power)

### Hardware

- [CYDs](https://www.aliexpress.us/item/3256808128499162.html) are cool & cheap, self-contained, and easy to source. They have [pretty good support](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display). I am specifically using [this USB-C variant](https://www.tztstore.com/goods/show-7983.html) (no micro-usb, only single USB-C) so things might be a lil different than your setup, but pretty similar. One example is I use `TFT_INVERSION_ON` and `TFT_BL` set to pin 27 (not 21, like some CYDs) in my [User_Setup.h](User_setup.h). With this CYD stuff, there are just tons of variations, and it you might find it cheaper to just go with the flow on that (these were $14, shipped to US!) and experiment a little. I carefully examined [the downloads](https://www.tztstore.com/goods/show-7983.html) that they made available to figure out the [pins](pins.h). This project is very much about supporting whatever devices you find, with a little work.
- SPI SX1276 (or similar) radio-board, like [this](https://www.aliexpress.us/item/3256805989899200.html) Get the right freq for your region (in US: 915)

### Software

I am using arduino-cli, and I included a [Makefile](Makefile) to make things easier. Run `make` for help.

I abstracted differences in CYD-like hardware by modifying [pins.h](pins.h), which you can use in your own sketch, as well as [User_Setup.h](User_setup.h), which you should put in Arduino/libraries/TFT_eSPI/

### Notes

- look into LVGL. There is already a [nice onscreen keyboard demo](https://docs.lvgl.io/8.3/widgets/extra/keyboard.html#keyboard-with-text-area)
- [this article](https://randomnerdtutorials.com/cheap-yellow-display-esp32-2432s028r/) has some pretty complete directions for interfacinf all the peripherals
- [micropython](https://randomnerdtutorials.com/micropython-cheap-yellow-display-board-cyd-esp32-2432s028r/) would be nice, but crypto was very tricky
- [thumbprint sensor](https://www.aliexpress.us/item/3256808453331144.html) seems nice for physical-security (decrypt private keys with finger.) I have seen them for around $3. It could be used with finger combos (middle, ring, etc) and "duress pattern" to wipe
- [microreticulum](https://github.com/attermann/microReticulum) is a good place to start looking at client-code, but it should probly be simplified/abstracted for general use
- it would be cool if it was [rnode-compatable](https://unsigned.io/rnode_firmware/) so you can use it like that (with phone/computer) or press a button to go into "client mode"
- Good in-depth [pinout info](https://randomnerdtutorials.com/esp32-cheap-yellow-display-cyd-pinout-esp32-2432s028r/)
- [this](https://github.com/SzymonPriv/CydExample) is a great example for arduino that uses all CYD peripherals
- I will probably need to reuse the SD SPI pins (all except CS) for radio. There are these [extender things](https://www.sparkfun.com/sparkfun-microsd-sniffer.html) but soldering to the board should be fine.
