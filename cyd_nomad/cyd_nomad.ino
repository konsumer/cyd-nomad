// This is a CYD-based Reticulum client


#include "../pins.h"
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include <SD.h>

SPIClass touchspi = SPIClass(VSPI);
XPT2046_Touchscreen ts(TP_CS, TP_IRQ);
TFT_eSPI tft = TFT_eSPI();
I2SClass i2s;

#include "service_ui.h"
#include "service_lora.h"

void setup() {
	Serial.begin(115200);

	touchspi.begin(TP_CLK, TP_DIN, TP_OUT, TP_CS);
	ts.begin(touchspi);
	ts.setRotation(1);
	tft.init();
	tft.setRotation(1);
	tft.fillScreen(TFT_BLACK);

	pinMode(BTN_BOOT, INPUT);
	pinMode(LDR_PIN, INPUT);
	
	pinMode(LED_R, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_B, OUTPUT);

	
	// register the services you want to use
	xTaskCreate(service_ui, "UI", 2048, NULL, 1, NULL);
}

void loop() {}