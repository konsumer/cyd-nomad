// this is the named-pins for me, but yours might be different

// this should also go in TFT_eSPI/User_Setup.h
#define TFT_BL   27
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST  -1

// Touch Panel (TP)
#define TP_CLK   14
#define TP_CS    33
#define TP_DIN   13
#define TP_OUT   12
#define TP_IRQ   36

// same, for https://randomnerdtutorials.com/lvgl-cheap-yellow-display-esp32-2432s028r/
#define XPT2046_CLK TP_CLK
#define XPT2046_CS TP_CS
#define XPT2046_IRQ TP_IRQ
#define XPT2046_MISO TP_DIN
#define XPT2046_MOSI TP_OUT

// TF Card (SD Card)
#define SD_CS    5
#define SD_MOSI  23
#define SD_MISO  19
#define SD_SCK   18

// Audio I2S pins
#define I2S_DOUT 26

// LED-RGB
#define LED_R    4
#define LED_G    17
#define LED_B    16

// Light-dependant resistor for "brightness" needs to be isolated from screen backlight & RGB LED
#define LDR_PIN 34

// Boot Button
#define BTN_BOOT 0
