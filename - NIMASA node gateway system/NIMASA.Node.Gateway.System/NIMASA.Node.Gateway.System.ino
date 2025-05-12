#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>

SoftwareSerial myserial(16,17); // RX: 16, TX:17

#define SEALEVELPRESSURE_HPA (1013.25)
#define precpin 35
#define INTERVAL 10000 
Adafruit_BME280 bme;
     

String serverName = "https://weatherwan.oauife.edu.ng/update.php";
//String serverName = "https://weatherwan.oauife.edu.ng/update.php?pass=xYzAbC&tmp=temperature&hum=humidity&wndspd=wind_speed&vapor_pressure=vap_pressure&bar_pressure=bar_pressure&wnddir=0&lghtint=0&vap=vap_pressure&bar=bar_pressure&loc=NIM";   //url for Node 4
//String serverName = "https://webhook.site/40a0d6c8-ae60-41e5-ba59-35224b99d684";
String device_id = "NIM";


//String queryString2 = "&tmp=0&hum=0&wndspd=0&vapor_pressure=0&bar_pressure=0&wnddir=0&lghtint=0&vap=0&bar=0&loc=0" ;

void setup()
{
  myserial.begin(9600);
  Serial.begin(9600);
  Wire.begin();  
  

  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
 
}
 

void loop()
{
  float sensorValue = analogRead(27);
 // Serial.print("Analog Value =");
  //Serial.println(sensorValue);
 
  float voltage = (sensorValue / 4095) * 5;
 // Serial.print("Voltage =");
 //Serial.print(voltage);
  //Serial.println(" V");
 
  float wind_speed = mapfloat(voltage, 0.415, 2, 0, 32.4);
  float speed_mph = ((wind_speed *3600)/1609.344);
 // Serial.print("Wind Speed =");
  //Serial.print(wind_speed);
 // Serial.println("m/s");
 // Serial.print(speed_mph);
 // Serial.println("mph");
 // Serial.println(" ");

  float temperature = bme.readTemperature();
  float bar_pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.readHumidity();
  float wnddir = 0;
  float lghtint = 0; 
  double vap_pressure = vapor_pressure(temperature, humidity);

  int sensorValuet = analogRead(precpin);  // Read the analog value from sensor
  int rain = map(sensorValuet, 0, 4095, 100, 1);
  
  
 
  //String queryString = serverName + "?pass=xYzAbc&tmp="+String(temperature)+"&hum="+String(humidity)+"&wndspd="+String(wind_speed)+"&vapor_pressure="+String(vap_pressure)+"&bar_pressure="+String(bar_pressure)+"&wnddir="+String(0)+"&lghtint="String(0)+"&vap="+String(vap_pressure)+"&bar="+String(bar_pressure)+"&loc="+String(NIM);
  String queryString = serverName + "?pass=xYzAbC&tmp="+String(temperature)+"&hum="+String(humidity)+"&wndspd="+String(wind_speed)+"&vapor_pressure="+String(vap_pressure)+"&bar_pressure="+String(bar_pressure)+"&wnddir="+String(0)+"&lghtint="+String(0)+"&vap="+String(vap_pressure)+"&bar="+String(bar_pressure)+"&loc="+String(device_id);
  //queryString = serverName;
  //queryString += "&tmp=";
  //queryString += String(temperature);
  //queryString += "&hum=";
  //queryString += String(humidity);
  //queryString += "&wndspd=";
  //queryString += String(speed_mph);
  //queryString += "&vapor_pressure=";
  //queryString += String(vap_pressure);
  //queryString += "&bar_pressure=";
  //queryString += String(bar_pressure);
  //queryString += "&wnddir=";
  //queryString += String(wnddir);
  //queryString += "&lghtint=";
  //queryString += String(lghtint);
  //queryString += "&vap=";
  //queryString += String(vap_pressure);
  //queryString += "&bar=";
  //queryString += String(bar_pressure);
  //queryString += "&loc=";
  //queryString += String(device_id);


   
 
 // Serial.println(sensorValuet);
 // Serial.println(rain);

  
  //Serial.print(device_id);

 // Serial.print(F("Temperature = "));
 // Serial.print(temperature);
// Serial.println(F("*C"));
 
//  Serial.print(F("Humidity = "));
// Serial.print(humidity);
//  Serial.println(F("%"));
  
//  Serial.print(F("barometric_Pressure = "));
//  Serial.print(bar_pressure);
//  Serial.println(F("hPa"));
 
//  Serial.print(F("Vapor Pressure = "));
//  Serial.print(vap_pressure);
//  Serial.println(F("kPa"));

//  Serial.print(F("Approx. Altitude = "));
//  Serial.print(altitude);
//  Serial.println(F("m"));

//  Serial.print(F("Wind Speed = "));
//  Serial.print(speed_mph);
//  Serial.println(F("mph"));
 
//  Serial.print(F("rainfall = "));
//  Serial.print(rain);
//  Serial.println(F("%"));

//  Serial.print(F("Humidity = "));
//  Serial.print(humidity);
//  Serial.println(F("%"));

//  Serial.println();
//  delay(1000);

   
 /********************GSM Communication Starts********************/
 
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
  /********************GSM Communication Stops********************/
 }


double vapor_pressure(double T, double humidity)
{
  double a = 0.611;
  double b = 17.502;
  double c = 240.97;
  double sat_vap_press = a * exp((b * T)/(T + c));
  double vap_pre = sat_vap_press * humidity;
  return vap_pre;
}

void ShowSerialData()
{
  while (myserial.available() != 0)
    Serial.write(myserial.read());
  delay(1000);
 
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


