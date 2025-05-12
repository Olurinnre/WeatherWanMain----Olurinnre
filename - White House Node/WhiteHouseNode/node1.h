#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <LoRa.h>
#include <ESP32Time.h>

#define SS 5
#define RST 14
#define DI0 2
#define BAND 433E6
#define SEALEVELPRESSURE_HPA (1013.25)
#define precpin 35


#define DEEP_SLEEP_TIME 5 

const int anemometerPin = 27;
float M = 0.8;
float B = 0.447;
volatile unsigned long pulseCount = 0;
unsigned long lastWindTime = 0;
float windSpeedMPH = 0.0;
float windSpeedFactor;

Adafruit_BME280 bme;
String LoRaMessage = "";

char device_id[6] = "Node1";

void isr();

int old_time = 0;
bool startup = 1;

const unsigned long SEND_INTERVAL = 15 * 60 * 1000; // 30 minutes in millisecond
unsigned long int previoussecs = 0;

//ESP32Time rtc;
ESP32Time rtc(3600);  // offset in seconds GMT+1


void sendData();
void goToDeepSleep();