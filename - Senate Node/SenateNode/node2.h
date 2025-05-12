#include <dummy.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <LoRa.h>
#include <ESP32Time.h>
#include <esp32-sdi12.h>

#define SDI12_DATA_PIN 13
#define SS 5
#define RST 14
#define DI0 2
#define BAND 433E6
#define precpin 35

#define DEEP_SLEEP_TIME 10 //1 

static ESP32_SDI12 sdi12(SDI12_DATA_PIN);
static ESP32_SDI12::Sensors sensors;
static float values[10];

static uint8_t addr;
static uint8_t n_returned_values;

String LoRaMessage = "";

char device_id[6] = "Node2";

int old_time = 0;
bool startup = 1;

const unsigned long SEND_INTERVAL = 15 * 60 * 1000; // 30 minutes in millisecond
unsigned long int previoussecs = 0;

//ESP32Time rtc;
ESP32Time rtc(3600);  // offset in seconds GMT+1


void sendData();
void goToDeepSleep();