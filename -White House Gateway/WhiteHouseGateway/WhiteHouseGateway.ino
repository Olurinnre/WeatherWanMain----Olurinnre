


/** POST REQUEST **/

#include <LoRa.h>
#include <SoftwareSerial.h>
#include <esp_task_wdt.h>  // Add watchdog timer
#include <esp_system.h>    // Add for reset reason
#define WDT_TIMEOUT 600    
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


SoftwareSerial myserial(16,17); // RX: 16, TX:17


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
String windSpeedMPH;
String jsonData;

String ussdcode = "*323*4#";

int counter = 0;

String serverName = "https://weatherwan.oauife.edu.ng/update.php";
//String serverName1 = "https://weatherwan.oauife.edu.ng/update.php?pass=xYzAbC&tmp=12&hum=34&wndspd=123&vapor_pressure=34&bar_pressure=0.1&wnddir=1111&lghtint=12434&vap=56&bar=78&loc=WHT ";      //url for Node 3     https://weatherwan.oauife.edu.ng/api/node/history/64032f9f284f88d9c7748f99"
//String serverName2 = "https://weatherwan.oauife.edu.ng/update.php?pass=xYzAbC&tmp=12&hum=34&wndspd=123&vapor_pressure=34&bar_pressure=0.1&wnddir=1111&lghtint=12434&vap=56&bar=78&loc=USB ";

//String serverName1 = "https://webhook.site/40a0d6c8-ae60-41e5-ba59-35224b99d684";      //url for Node 3     https://weatherwan.oauife.edu.ng/api/node/history/64032f9f284f88d9c7748f99"
//String serverName2 = "https://webhook.site/40a0d6c8-ae60-41e5-ba59-35224b99d684";

void setup() 
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  delay(1000);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1); 

  esp_task_wdt_init(WDT_TIMEOUT, true); // Enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); 

  // Start the serial communication
 
  Serial.begin(9600);
  Serial.println("Initializing..........");

   Serial.println("Initializing GPRS MODULE");
   myserial.begin(9600);
    // Set up the GPRS connection
  setupGPRS();
   Serial.println("GPRS MODULE INITIALISED");

  // Send a wakeup message via GPRS
  sendGPRSMessage("https://weatherwan.oauife.edu.ng/status.php?pass=xYzAbC&status=Device_starting_up&loc=WHT");

  // Set up LoRa communication
  Serial.println("Initialising LoRa radio");
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
Serial.println("LORA MODULE INITIALISED");
  // Send a wakeup message post LoRa setup
  sendGPRSMessage("https://weatherwan.oauife.edu.ng/status.php?pass=xYzAbC&status=Gateway_setup_complete&loc=WHT");
}

unsigned long previousMillis = 0;  
const long interval = 3600000;  

void loop() 
{
  int pos1, pos2, pos3, pos4, pos5, pos6, pos7;
 
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet:  ");
    String LoRaData = LoRa.readString();
    Serial.print(LoRaData);

    while (LoRa.available())          // readpacket
    {
      Serial.print((char)LoRa.read());
    }
    
    Serial.print("' with RSSI ");       // print RSSI of packet
    Serial.println(LoRa.packetRssi());
 
    pos1 = LoRaData.indexOf('/');
    pos2 = LoRaData.indexOf('&');
    pos3 = LoRaData.indexOf('#');
    pos4 = LoRaData.indexOf('@');
    pos5 = LoRaData.indexOf('%');
    pos6 = LoRaData.indexOf('*');
    pos7 = LoRaData.indexOf('$');

  
 
    device_id = LoRaData.substring(0, pos1);
    temperature = LoRaData.substring(pos1 + 1, pos2);
    bar_pressure = LoRaData.substring(pos2 + 1, pos3);
    altitude = LoRaData.substring(pos3 + 1, pos4);
    humidity = LoRaData.substring(pos4 + 1, pos5);
    precipitation = LoRaData.substring(pos5 + 1, pos6);
    windSpeedMPH = LoRaData.substring(pos6 + 1, pos7);
    vap_pressure = LoRaData.substring(pos7 + 1, LoRaData.length());
    String WHT;
    String USB;

    
    String queryString = serverName + "?pass=xYzAbC&tmp="+String(temperature)+"&hum="+String(humidity)+"&wndspd="+String(windSpeedMPH)+"&vapor_pressure="+String(vap_pressure)+"&bar_pressure="+String(bar_pressure)+"&wnddir="+String(0)+"&lghtint="+String(0)+"&vap="+String(vap_pressure)+"&bar="+String(bar_pressure)+"&loc="+"WHT";
    String queryString2 = serverName + "?pass=xYzAbC&tmp="+String(temperature)+"&hum="+String(humidity)+"&wndspd="+String(windSpeedMPH)+"&vapor_pressure="+String(vap_pressure)+"&bar_pressure="+String(bar_pressure)+"&wnddir="+String(0)+"&lghtint="+String(0)+"&vap="+String(vap_pressure)+"&bar="+String(bar_pressure)+"&loc="+"USB";
    
//sendGPRSMessage(queryString);
   
 
 
 /********************GSM Communication Starts********************/
 
  
  // /********************GSM Communication Stops********************/
  
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
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    checkdata();
  }

esp_task_wdt_reset();
}

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

void sendGPRSMessage(String queryString) {
  myserial.println("AT+HTTPINIT");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+HTTPPARA=\"CID\",1");
  delay(6000);
  ShowSerialData();

  if (device_id == "Node1")
  {
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

    else if (device_id == "Node2")
    {
      myserial.println("AT+HTTPPARA=\"URL\",\""+queryString2+"\""); //Server address
      delay(4000);
      ShowSerialData();

      myserial.println("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"");
      delay(4000);
      ShowSerialData();
        
        
     myserial.println("AT+HTTPDATA=" + String(queryString2.length()) + ",100000");
        Serial.println(queryString2);
        delay(6000);
        ShowSerialData();
        
        myserial.println(queryString2);
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
}

void checkdata() {
  myserial.println("AT+CUSD=1,\"ussdcode\",15"); 

  while (myserial.available()) {
    String message = myserial.readString();
    Serial.println(message);
    sendGPRSMessage(status+"?pass=xYzAbC&status="+String(message)+"&loc="+"WHT");
  }
}

void sendErrorMessage() {
  String queryStringstatus = "https://weatherwan.oauife.edu.ng/status.php?pass=xYzAbC&status=LoRA_is_down&loc=WHT";

  sendGPRSMessage(queryStringstatus); // Send error message to the server
}

void sendErrorMessage() {
  String queryStringstatus = "https://weatherwan.oauife.edu.ng/status.php?pass=xYzAbC&status=LoRA_is_down&loc=WHT";

  sendGPRSMessage(queryStringstatus); // Send error message to the server
}

void ShowSerialData()
{
  while (myserial.available() != 0)
  Serial.write(myserial.read());
  delay(1000);
 
}
