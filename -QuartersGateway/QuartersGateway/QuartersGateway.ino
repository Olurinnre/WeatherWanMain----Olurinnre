#include <LoRa.h>
#include <SoftwareSerial.h>

#include <esp_task_wdt.h>
#include <esp_system.h>    // Add for reset reason
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h" //add to configure brownout circuitry

#define WDT_TIMEOUT 600    // Timeout every 10 minutes

// Define the pins for SoftwareSerial
SoftwareSerial myserial(16, 17); // RX: 16, TX:17

// Define the pins for LoRa
#define SS 5
#define RST 14
#define DI0 2
#define BAND 433E6

String device_id;
String temperature;
String bar_pressure;
String altitude;
String humidity;
String vap_pressure;
String precipitation;

// Define the server for wakeup messages
String serverName = "https://weatherwan.oauife.edu.ng/update.php";

int counter = 0;
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 3600000 ;//1 hour period
void setup() {
WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
////////////////////////////////////////////
delay(1000); //wait for 1 seconds for things to normalise before running the code
WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1); //enable brownout detector 
   // Configure and start the Watchdog Timer
  esp_task_wdt_init(WDT_TIMEOUT, true);
  esp_task_wdt_add(NULL); 
  ///////////////////////////////////
  // Start the serial communication

///////////////////////////////////////////////////////

//////////////////////////////////////////
  Serial.begin(9600);
  Serial.println("Initializing..........");

   Serial.println("Initializing GPRS MODULE");
   myserial.begin(9600);
    // Set up the GPRS connection
  setupGPRS();
   Serial.println("GPRS MODULE INITIALISED");
 
  // Send a wakeup message via GPRS
  sendGPRSMessage("Device starting up...");

  // Set up LoRa communication
  Serial.println("Initialising LoRa radio");
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
Serial.println("LORA MODULE INITIALISED");
  // Send a wakeup message post LoRa setup
  sendGPRSMessage("Gatewway setup complete.");
  startMillis = millis();
}

void loop() {
 int pos1, pos2, pos3, pos4, pos5, pos6;
Serial.println("RUNNING MAIN LOOP");
//bool flag = 0;
currentMillis = millis ();
if ((currentMillis - startMillis) > period) 
{
// 1 hour has elapsed
  peripheral_Setup_within_code ();
  startMillis = currentMillis;

}
int packetSize = LoRa.parsePacket();

if (packetSize) {
 // flag = 1;
  Serial.print("Received packet:  ");
  String LoRaData = LoRa.readString();
  Serial.print(LoRaData);

  while (LoRa.available())
   { // Read packet
  Serial.print((char)LoRa.read());
  }

  Serial.print("' with RSSI ");    // Print RSSI of packet
  Serial.println(LoRa.packetRssi());

  pos1 = LoRaData.indexOf('/');
  pos2 = LoRaData.indexOf('&');
  pos3 = LoRaData.indexOf('#');
  pos4 = LoRaData.indexOf('@');
  pos5 = LoRaData.indexOf('*');
  pos6 = LoRaData.indexOf('$');

  device_id = LoRaData.substring(0, pos1);
  temperature = LoRaData.substring(pos1 + 1, pos2);
  bar_pressure = LoRaData.substring(pos2 + 1, pos3);
  altitude = LoRaData.substring(pos3 + 1, pos4);
  humidity = LoRaData.substring(pos4 + 1, pos5);
  humidity = LoRaData.substring(pos5 + 1, pos6);
  vap_pressure = LoRaData.substring(pos6 + 1, LoRaData.length());
  String QTS;

  String queryString = serverName + "?pass=xYzAbC&tmp=" + String(temperature) + "&hum=" + String(humidity) + "&wndspd=" + String(0) + "&vapor_pressure=" + String(vap_pressure) + "&bar_pressure=" + String(bar_pressure) + "&wnddir=" + String(0) + "&lghtint=" + String(0) + "&vap=" + String(vap_pressure) + "&bar=" + String(bar_pressure) + "&loc=" + "QTS";

  sendGPRSMessage(queryString); // Send data to the server

 }
  else
   {
    counter++;
    if (counter == 10000000)
    {
      sendErrorMessage(); // Send error message to the backend 
      counter = 0;
    }
   
  }

  // Reset the watchdog timer
  esp_task_wdt_reset();
}

// Function to set up GPRS connection
void setupGPRS() {
  if (myserial.available())
    Serial.write(myserial.read());

  myserial.println("AT");
  delay(3000);
  myserial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=3,1,\"APN\",\"web.gprs.mtnnigeria.net\"");  //APN    internet.ng.airtel.com     web.gprs.mtnnigeria.net
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=1,1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=2,1");
  delay(6000);
  ShowSerialData();

}

// Function to send a message via GPRS
void sendGPRSMessage(String queryString) {
  //myserial.begin(9600);
  myserial.println("AT+HTTPINIT");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPPARA=\"CID\",1");
  delay(6000);
  ShowSerialData();
  
  myserial.println("AT+HTTPPARA=\"URL\",\""+queryString+"\""); //Server address
  delay(4000);
  ShowSerialData();

  myserial.println("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"");
  delay(4000);
  ShowSerialData();
 
  myserial.println("AT+HTTPDATA=" + String(queryString.length()) + ",100000");
  Serial.println(queryString);
  delay(6000);
  ShowSerialData();
 
  myserial.println(queryString);
  delay(6000);
  ShowSerialData();

  myserial.println("AT+HTTPSSL=1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPACTION=1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPREAD");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPTERM");
  delay(10000);
  ShowSerialData();
}

void sendErrorMessage() {
  String queryString = serverName + "?pass=xYzAbC&tmp=" + String(-1) + "&hum=" + String(-1) + "&wndspd=" + String(-1) + "&vapor_pressure=" + String(-1) + "&bar_pressure=" + String(-1) + "&wnddir=" + String(-1) + "&lghtint=" + String(-1) + "&vap=" + String(-1) + "&bar=" + String(-1) + "&loc=" + "QTS";

  sendGPRSMessage(queryString); // Send error message to the server
}

// Function to show serial data
void ShowSerialData() {
  while (myserial.available() != 0)
  Serial.write(myserial.read());
  delay(1000);
}

void peripheral_Setup_within_code ()
{
  Serial.begin(9600);
   Serial.println("Initializing GPRS MODULE");
   myserial.begin(9600);
    // Set up the GPRS connection
  setupGPRS();
   Serial.println("GPRS MODULE INITIALISED");
 
  // Send a wakeup message via GPRS
  //sendGPRSMessage("Device starting up...");

  // Set up LoRa communication
  Serial.println("Initialising LoRa radio");
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
 Serial.println("LORA MODULE INITIALISED");
  // Send a wakeup message post LoRa setup
 Serial.println("Gatewway setup complete."); 
}