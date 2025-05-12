#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <ESP32Time.h>

#include <esp_task_wdt.h>
#include <esp_system.h>    // Add for reset reason
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h" //add to configure brownout circuitry
////////////////////////////////////////////////////////////////
#define WDT_TIMEOUT 1200000    // Timeout every 20 minutes
//if 1 core doesn't work, try with 2
#define CONFIG_FREERTOS_NUMBER_OF_CORES 1
/////////////////////////////////////////////////////////////////////
// Define RX and TX pins (adjust for your board)

#define RX_PIN 23
#define TX_PIN 22

// Create a SoftwareSerial object
SoftwareSerial uart(RX_PIN, TX_PIN);
SoftwareSerial myserial(16,17); // RX: 16, TX:17

#define SEALEVELPRESSURE_HPA (1013.25)
#define precpin 35
#define INTERVAL 10000 
Adafruit_BME280 bme;
float temperature;
float humidity;
float rainVolume;
float wind_speed;
float light;
float bar_pressure;
float wnddir;
String serverName = "https://weatherwan.oauife.edu.ng/update.php";
//String queryString = " ";
String queryString1 =" ";
String device_id = "OPA";
byte mybyte [21];

//ESP32Time rtc;
ESP32Time rtc(3600);  // offset in seconds GMT+1
///////////////////////
esp_task_wdt_config_t twdt_config = {
        .timeout_ms = WDT_TIMEOUT,
        .idle_core_mask = (1 << CONFIG_FREERTOS_NUMBER_OF_CORES) - 1,    // Bitmask of all cores
        .trigger_panic = true,
    };
//////////////////////////////
int old_time = 0;
void setup()
{

WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
////////////////////////////////////////////
delay(1000); //wait for 1 seconds for things to normalise before running the code
WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1); //enable brownout detector 
   // Configure and start the Watchdog Timer
//esp_task_wdt_init(WDT_TIMEOUT, true);
//esp_task_wdt_add(NULL); 
myserial.begin(9600);
uart.begin(9600);
Serial.begin(9600);
rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
//set_sim800l_9600_baud_rate ();
// configure watchdog timer
esp_task_wdt_deinit(); //wdt is enabled by default, so we need to deinit it first
esp_task_wdt_init(&twdt_config); //enable panic so ESP32 restarts
esp_task_wdt_add(NULL); //add current thread to WDT watch
//////////////////////////////////////////////////////
set_sim800l_9600_baud_rate (); //change the baud rate of the SIM module to 9600
 // Wire.begin();  
  

  // if (!bme.begin(0x76))
  // {
  //   Serial.println("Could not find a valid BME280 sensor, check wiring!");
  //   while (1);
  // }
 
}
 

void loop()
{//Serial.println("looping");
  esp_task_wdt_reset();
///////////////////////////////////////
  int time = rtc.getMinute();
  // unsigned long elapsed = time - previoustime;

  //for (int i = 0; i <=1; i++)
  //if (startup == 1)

    if (time == 10)
    {

     //if (elapsed >= SEND_INTERVAL) {
    // Send data from the current device
      sendData(); 
      
      //startup = 0;
      old_time = 10;
      delay(100000);

    }

  else
    {
      //Serial.print(F("out of the first if block "));
      //delay(20000);

      if (time == old_time + 10)
      {
        sendData();
        old_time = time;
        delay(100000);
      }
    }





  ///////////////////////////////
  if (uart.available()) {
    // Read bytes from UART and construct a string
   // String receivedString = "";
    Serial.println("no of available bytes is");
    Serial.println (uart.available());
    if (uart.available() > 21)
    {
      uart.flush();
    }

    if (uart.available() == 21)
    { int i = 0 ;
      while (uart.available())
       {
        mybyte[i] = uart.read();
      // String stringOne =  String(mybyte[i], HEX);
       // Serial.println(myChar[i]);
      // Serial.println(stringOne);
        i++ ;
     // char receivedChar = uart.read();
     // receivedString += receivedChar;     
       }
        i = 0;
    // Print the received string to the Serial monitor
   // Serial.println("Received: " + receivedString);
    temperature = get_temperature ();
    Serial.print("temperature = ");
    Serial.println(temperature );
    humidity = get_humidity ();
    Serial.print("humidity = ");
    Serial.println(humidity );
    wind_speed = get_windspeed ();
    Serial.print("wind_speed = ");
    Serial.println(wind_speed);
    bar_pressure = get_barometric_pressure ();
    Serial.print("bar_pressure = ");
    Serial.println(bar_pressure );
    wnddir = wind_direction ();
    Serial.print("wind_direction = ");
    Serial.println(wnddir);
    light = get_light (); 
    Serial.print("light = ");
    Serial.println(light );
    queryString1 = serverName + "?pass=xYzAbC&tmp="+String(temperature)+"&hum="+String(humidity)+"&wndspd="+String(wind_speed)+"&vapor_pressure="+String(0)+"&bar_pressure="+String(bar_pressure)+"&wnddir="+String(wnddir)+"&lghtint="+String(light)+"&vap="+String(0)+"&bar="+String(bar_pressure)+"&loc="+String(device_id);   
    //Serial.println(queryString1);
 /*   sendGPRSMessage (queryString1);
    if (get_battery_status ())
     {
        sendErrorMessage();
     } */
    }
   // Serial.println("no of available bytes is");
   // Serial.println (uart.available());
   // float temperature = uart.parseFloat();
   // Serial.print("data is");
   // Serial.println(uart.readString());
   // float humidity = uart.parseFloat();

   // float rainVolume = uart.parseFloat();

   // float wind_speed = uart.parseFloat();

   // float light = uart.parseFloat();

   // float bar_pressure = uart.parseFloat();
   // Serial.println("Available sensor");
    //delay(1000); 

} 
 //String queryString = serverName + "?pass=xYzAbc&tmp="+String(temperature)+"&hum="+String(humidity)+"&wndspd="+String(wind_speed)+"&vapor_pressure="+String(vap_pressure)+"&bar_pressure="+String(bar_pressure)+"&wnddir="+String(0)+"&lghtint="String(0)+"&vap="+String(vap_pressure)+"&bar="+String(bar_pressure)+"&loc="+String(NIM);
//used//  String queryString = serverName + "?pass=xYzAbC&tmp="+String(temperature)+"&hum="+String(humidity)+"&wndspd="+String(wind_speed)+"&vapor_pressure="+String(0)+"&bar_pressure="+String(bar_pressure)+"&wnddir="+String(wnddir)+"&lghtint="+String(light)+"&vap="+String(0)+"&bar="+String(bar_pressure)+"&loc="+String(device_id);
 /* 
  if (myserial.available())
  //Serial.print("GSM STARTED");
  Serial.write(myserial.read());  //Serial.print("GSM STARTED")
  myserial.println("AT");
  delay(3000);
  ShowSerialData();

  myserial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=3,1,\"APN\",\"web.gprs.mtnnigeria.net\"");  //APN
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=1,1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=2,1");
  delay(6000);
  ShowSerialData();
 
 
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
  
  delay(60000); */
  /********************GSM Communication Stops********************/
 

 }

void sendGPRSMessage (String queryString)
{
 if (myserial.available())
  //Serial.print("GSM STARTED");
  Serial.write(myserial.read());  //Serial.print("GSM STARTED")
  myserial.println("AT");
  delay(3000);
  ShowSerialData();

  myserial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=3,1,\"APN\",\"web.gprs.mtnnigeria.net\"");  //APN
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=1,1");
  delay(6000);
  ShowSerialData();
 
  myserial.println("AT+SAPBR=2,1");
  delay(6000);
  ShowSerialData();
 
 
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
  
  delay(60000); 
}

void ShowSerialData()
{
  while (myserial.available() != 0)
    Serial.write(myserial.read());
  delay(1000);
 
}

byte Get_High_Nibble (byte in_data)
{
  byte temp_data = in_data & 0xF0 ;
  temp_data = temp_data >> 4;
  return temp_data;
}

byte Get_Low_Nibble (byte in_data)
{
  byte temp_data = in_data & 0x0F ;
  return temp_data;
}

bool Get_bit (byte in_data, unsigned int indx)
// we are expected to pass the index which must lie between 0 and 7
{ byte shft_byte = 0x01 ; byte result;

  if (indx <=7)
  {
    result = (shft_byte<<indx)& in_data ; 
    if (result)
    {
      return 1;
    }else
    {
      return 0;
    }
  }

}

float wind_direction ()
{ unsigned int result;
   if (Get_bit(mybyte[3],7)) //get the 9th bit of the wind direction
   {
      result = mybyte[2] * 2 ; 
   }else
   {
      result = mybyte[2];
   }
  return float (result);
}

float get_temperature ()
{ //mybyte[4]  mybyte[3] 
 byte temp_data = Get_Low_Nibble(mybyte[3]) ;
 temp_data = temp_data & 0x07 ; //remove the 4th bit
 unsigned int dta = (256*temp_data)+mybyte[4] ;
 //let's calculate the actual temperature
 float temp_1 = float((dta -400))/10.0 ;
 return temp_1;
}

float get_humidity ()
{
 float humidity =  float(mybyte[5]);
 return humidity;
}

float get_windspeed ()
{  unsigned int tmp_wspd; float wndspd;
  if (Get_bit(mybyte[3],4)) //8th bit of windspeed
  {
    tmp_wspd = mybyte[6] * 2;
  }else
  {
    tmp_wspd = mybyte[6];
  }
  wndspd = float((tmp_wspd))*(1.12)/8;
  return wndspd ;
}

float get_gustspeed ()
{//mybyte[7]//
  float gust_speed = float(mybyte[7])*1.12 ;
  return gust_speed;
}

float get_accummulation_rainfall ()
{//mybyte[8],mybyte[9]
 float acc_rainfall = (256*mybyte[8] + mybyte[9])*0.3;
 return acc_rainfall ;
}

unsigned int get_ultraviolet_light ()
{
  unsigned int ultraviolet_lght = (256*mybyte[10]+mybyte[11]);
  return ultraviolet_lght;
}
float get_light ()
{  //mybyte[12], mybyte[13],mybyte[14],mybyte[15]
  unsigned long light = (16777216*mybyte[12])+(65536*mybyte[13])+(256*mybyte[14])+mybyte[15] ; //2^24 *mybyte[12] + 2^16*mybyte[13] + 2^8 *mybyte[14] + mybyte[15] ;
  float light_2 = float (light)/10.0 ;
  return light_2 ;
}

float get_barometric_pressure ()
{ //mybyte[17], mybyte[18],mybyte[19]
 float temp_baro_press =float ((65536*mybyte[17])+(256*mybyte[18])+mybyte[19] )/100.0 ;
 return temp_baro_press;
}

bool get_battery_status ()
{//mybyte [3]
 //return a '1' if battery level is low
 byte battery_byte = mybyte [3] & 0x08 ;
 if (battery_byte)
 {
  return 1;
 }else
 {
  return 0;
 }
 
}

void sendErrorMessage() {
  String queryString2 = "https://weatherwan.oauife.edu.ng/status.php?pass=xYzAbC&status=Low_Battery&loc=OPA";

  sendGPRSMessage(queryString2); // Send error message to the server
}

void sendData()
{ //String queryString = serverName + "?pass=xYzAbC&tmp="+String(temperature)+"&hum="+String(humidity)+"&wndspd="+String(wind_speed)+"&vapor_pressure="+String(0)+"&bar_pressure="+String(bar_pressure)+"&wnddir="+String(wnddir)+"&lghtint="+String(light)+"&vap="+String(0)+"&bar="+String(bar_pressure)+"&loc="+String(device_id);   
 sendGPRSMessage (queryString1);
    if (get_battery_status ())
     {
        sendErrorMessage();
     }
}

void set_sim800l_9600_baud_rate ()
{
 if (myserial.available())
  //Serial.print("GSM STARTED");
  Serial.write(myserial.read());  //Serial.print("GSM STARTED")
  delay(1000);
  myserial.println("AT");
  delay(2000);
  ShowSerialData();

  myserial.println("AT+IPR=9600");
  delay(2000);
  ShowSerialData();
 
  myserial.println("AT&W");  //APN
  delay(2000);
  ShowSerialData();

  myserial.println("AT+IPR?");  //APN
  delay(2000);
  ShowSerialData();
}

