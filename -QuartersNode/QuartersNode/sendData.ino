double vapor_pressure(double T, double humidity)
{
  double a = 0.611;
  double b = 17.502;
  double c = 240.97;
  double sat_vap_press = a * exp((b * T)/(T + c));
  double vap_pre = sat_vap_press * humidity;
  return vap_pre;
}

void sendData()
{
  if ((millis() - signal1.lastDebounceTime) > signal1.debounceDelay)
{
       signal1.lastDebounceTime = millis();
       signal1.wind_speed= signal1.count * 8.75;
       signal1.count = 0;
  }
  delay(1);
  int sensorValue = analogRead(precpin);
  float precipitation = (100./1023.)*sensorValue;
  Serial.println(sensorValue);
  Serial.println(precipitation);
  
    if (sensorValue<900)
  {
    Serial.println("clear"); 
    }
   else
   {
    Serial.println("rainy"); 
   }

  float temperature = bme.readTemperature();
  float bar_pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.readHumidity();
  double vap_pressure = vapor_pressure(temperature, humidity);

  Serial.print(F("Device_id = "));
  Serial.print(device_id);

  Serial.print(F("Temperature = "));
  Serial.print(temperature);
  Serial.println(F("*C"));
 
  Serial.print(F("Humidity = "));
  Serial.print(humidity);
  Serial.println(F("%"));
  
  Serial.print(F("barometric_Pressure = "));
  Serial.print(bar_pressure);
  Serial.println(F("hPa"));
 
  Serial.print(F("Vapor Pressure = "));
  Serial.print(vap_pressure);
  Serial.println(F("kPa"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(altitude);
  Serial.println(F("m"));

  Serial.print(F("Wind Speed = "));
  Serial.print(signal1.wind_speed);
  Serial.println(F("cm/s"));

  Serial.print(F("rainfall = "));
  Serial.print(precipitation);
  Serial.println(F("%"));

 
  Serial.println();

  LoRaMessage = String(device_id) + "/" + String(temperature) + "&" + String(bar_pressure)
                + "#" + String(altitude) + "@" + String(humidity) + "*" + String(precipitation)+ "$" + String(vap_pressure);
 
  
  // send packet
  Serial.print("lora packet"); 
   
  LoRa.beginPacket();
  Serial.print("working");
  
  LoRa.print(LoRaMessage);
  Serial.println(LoRaMessage);
  LoRa.endPacket();

 
}

void IRAM_ATTR isr() 
{
    if (digitalRead(signal1.PIN_Interrupt) == LOW)
        signal1.count++;  
}