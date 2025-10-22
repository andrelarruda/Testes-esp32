#include <Wire.h>                 // Must include Wire library for I2C
#include <HardwareSerial.h>
#include <Adafruit_NeoPixel.h>


volatile bool causedByMotion = false;

// forward declaration
void print_wakeup_reason();

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  15       /* Time ESP32 will go to sleep (in seconds) */
#define LED_PIN 48
#define NUMPIXELS 1  // Apenas 1 LED onboard

// Cria o objeto NeoPixel
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  pixels.begin();
  Serial.println("***************-Hello!-***************");
  pixels.setPixelColor(0, pixels.Color(28, 153, 61));
  pixels.show();

  Wire.begin();

  print_wakeup_reason();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  delay(100);

  Serial.println("Configura a ESP32 para acordar a cada " + String(TIME_TO_SLEEP) +
  " segundos");

  delay(2000);
  Serial.println("Dormindo agora...");

  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  delay(1000);
  pixels.clear();
  pixels.show();
  
  esp_deep_sleep_start();


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Executando o loop.");
  delay(100);
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("***Wakeup caused by EXTERNAL SIGNAL using RTC_IO***");
      causedByMotion = true;
      break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER: 
      Serial.println("***Wakeup caused by TIMER***");
      break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("***Wakeup was not caused by deep sleep: %d***\n",wakeup_reason); break;
  }
}

