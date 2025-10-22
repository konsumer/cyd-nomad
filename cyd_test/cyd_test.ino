/*

This will test all the peripherals on the CYD

- RTOS
- Serial
- RGB LEDs
- touch
- TFT
- LDR
- BOOT button
- sound
- SD card
*/

// shared pin-definition
// you may need to edit this
#include "../pins.h"

#include <XPT2046_Touchscreen.h>

// make sure to copy User_Setup.h!
#include <TFT_eSPI.h>

// use for task_audio_dac
#include <driver/dac.h>

// used for task_audio_i2s
#include <ESP_I2S.h>

// used for task_sd_test
#include <SD.h>

SPIClass touchspi = SPIClass(VSPI);
XPT2046_Touchscreen ts(TP_CS, TP_IRQ);
TFT_eSPI tft = TFT_eSPI();
I2SClass i2s;

// I make a few seperate RTOS tasks that each manage their own setup & loop

// Show various CYD peripherals & print it to serial
void task_show_cyd(void *parameter) {
  touchspi.begin(TP_CLK, TP_DIN, TP_OUT, TP_CS);
  ts.begin(touchspi);
  ts.setRotation(1);
  tft.init();
  tft.setRotation(1);

  pinMode(BTN_BOOT, INPUT);
  pinMode(LDR_PIN, INPUT);

  TS_Point p = {};
  char line1[100] = {};
  char line2[100] = {};
  char line1p[100] = {};
  char line2p[100] = {};

  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);


  while (true) {
    if (ts.tirqTouched() && ts.touched()) {
      p = ts.getPoint();
    }else {
      p.z = 0;
    }

    sprintf(line1, "X: %d. Y: %d, Pressure: %d", p.x, p.y, p.z);
    sprintf(line2, "BOOT: %d, LDR: %d", digitalRead(BTN_BOOT), analogRead(LDR_PIN));

    Serial.print(line1);
    Serial.print(" ");
    Serial.println(line2);

    // pad with space, so it blocks out previous without flicker
    sprintf(line1p, "          %s          ", line1);
    sprintf(line2p, "          %s          ", line2);
    tft.drawCentreString(line1p, TFT_HEIGHT/2, (TFT_WIDTH/2) - 16, 2);
    tft.drawCentreString(line2p, TFT_HEIGHT/2, TFT_WIDTH/2, 2);

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Blink 1 LED (param is GPIO pin-number)
void task_blink_rgb(void *parameter) {
  int pin = (int)parameter;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  
  while (true) {
    digitalWrite(pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


// bad quality audio, but uses DAC instead of i2s
void task_audio_dac(void *parameter) {
  dac_output_enable(DAC_CHANNEL_2); // GPIO 26 or I2S_DOUT
  const int tableSize = 100;
  uint8_t waveTable[tableSize];
  for (int i = 0; i < tableSize; i++) {
    waveTable[i] = (uint8_t)(128 + 100 * sin(2.0 * 3.14159 * i / tableSize));
  }
  int index = 0;
  while (true) {
    dac_output_voltage(DAC_CHANNEL_2, waveTable[index]);
    index = (index + 1) % tableSize;
    delayMicroseconds(57); // ~440Hz tone
  }
  vTaskDelete(NULL);
}

// "better" quality audio, uses built-in i2s
void task_audio_i2s(void *parameter) {
  i2s.setPins(-1, -1, I2S_DOUT, -1, -1);
  
  if (!i2s.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
    Serial.println("Failed to initialize I2S!");
    vTaskDelete(NULL);
    return;
  }
  
  Serial.println("I2S initialized on GPIO 26");
  
  // Generate 440Hz sine wave
  const int sampleCount = 100;
  int16_t samples[sampleCount];
  
  for (int i = 0; i < sampleCount; i++) {
    samples[i] = (int16_t)(sin(2.0 * 3.14159 * 440.0 * i / 16000.0) * 10000);
  }
  
  while (true) {
    i2s.write((uint8_t*)samples, sizeof(samples));
  }
  
  vTaskDelete(NULL);
}

void task_sd_test(void *parameter) {
  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card initialization failed!");
    vTaskDelete(NULL);
    return;
  }
  
  Serial.println("SD Card initialized successfully");
  Serial.printf("Card Size: %lluMB\n", SD.cardSize() / (1024 * 1024));
  Serial.printf("Card Type: %d\n", SD.cardType());
  
  // Read run counter
  int runCount = 0;
  File counterFile = SD.open("/run_counter.txt", FILE_READ);
  if (counterFile) {
    String countStr = counterFile.readStringUntil('\n');
    runCount = countStr.toInt();
    counterFile.close();
    Serial.printf("Previous run count: %d\n", runCount);
  } else {
    Serial.println("No previous run counter found");
  }
  
  // Increment and write new count
  runCount++;
  counterFile = SD.open("/run_counter.txt", FILE_WRITE);
  if (counterFile) {
    counterFile.println(runCount);
    counterFile.close();
    Serial.printf("New run count: %d\n", runCount);
  } else {
    Serial.println("Failed to write counter");
  }
  
  // Write a test file with timestamp
  File testFile = SD.open("/test_log.txt", FILE_APPEND);
  if (testFile) {
    testFile.printf("Run #%d - Boot time: %lu ms\n", runCount, millis());
    testFile.close();
    Serial.println("Test log updated");
  }
  
  // List all files in root
  Serial.println("\nFiles on SD card:");
  File root = SD.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.printf("  %s (%d bytes)\n", file.name(), file.size());
    file = root.openNextFile();
  }
  
  // Read and display the log file
  Serial.println("\nLog file contents:");
  testFile = SD.open("/test_log.txt", FILE_READ);
  if (testFile) {
    while (testFile.available()) {
      Serial.write(testFile.read());
    }
    testFile.close();
  }
  
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);

  // setup some RTOS tasks
  // you might have to hit RESET to see serial-output
  
  xTaskCreate(task_blink_rgb, "BlinkGreen", 2048, (void*)LED_G, 1, NULL);
  xTaskCreate(task_show_cyd, "ShowCyd", 2048, NULL, 1, NULL);
  
  // this seems to sometimes conflict with touch/tft
  // so you might want to pause touch while you use it
  // xTaskCreate(task_sd_test, "SDTest", 8192, NULL, 1, NULL);
  
  // these are annoying!
  // xTaskCreate(task_audio_dac, "AudioDacTest", 2048, NULL, 1, NULL);
  // xTaskCreate(task_audio_i2s, "Audioi2sTest", 4096, NULL, 1, NULL);
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}