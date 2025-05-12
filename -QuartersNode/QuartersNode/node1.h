#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <LoRa.h>
#include <ESP32Time.h>
#include <esp_task_wdt.h> //add watchdog timer
#define WDT_TIMEOUT 600  //timeout every 10 minutes




#define SS 5
#define RST 14
#define DI0 2
#define BAND 433E6
#define SEALEVELPRESSURE_HPA (1013.25)
#define precpin 35

#define DEEP_SLEEP_TIME 10 

Adafruit_BME280 bme;
String LoRaMessage = "";

char device_id[6] = "QTS";

struct Anenometer {
  const uint8_t PIN_Interrupt;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
  uint32_t count;
  float wind_speed;
};

Anenometer signal1 = {12, 0, 100, 0}; 

void isr();

int old_time = 0;
bool startup = 1;

const unsigned long SEND_INTERVAL = 15 * 60 * 1000; // 30 minutes in millisecond
unsigned long int previoussecs = 0;

//ESP32Time rtc;
ESP32Time rtc(3600);  // offset in seconds GMT+1


void sendData();
void goToDeepSleep();