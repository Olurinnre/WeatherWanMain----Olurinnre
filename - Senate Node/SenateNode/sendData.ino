void sendData()
{
  float temperature;
  float bar_pressure;
  float lintensity;
  float humidity;
  float precipitation;
  float windSpeedMPH = 0;
  float altitude;

  for(uint8_t i = 0; i < sensors.count; i++){
        // Get the sensors address
        addr = sensors.sensor[i].address;

        // Take a measurement
        ESP32_SDI12::Status res = sdi12.measure(addr, values,
                                                sizeof(values),
                                                &n_returned_values);

        // Handle response status
        if(res  == ESP32_SDI12::SDI12_OK){
            // Measure was successful, print values
            Serial.printf("Measure complete.\n");
            for(uint8_t v = 0; v < n_returned_values; v++){
                Serial.printf("Value %d: %f\n", v + 1, values[v]);
                if (v <= 4) {
                  if (v == 1) {
                    temperature = values[0];
                } else if (v == 2) {
                    humidity = values[1];
                } else if (v == 3) {
                    bar_pressure = values[2]/100;
               } else {
                  lintensity = values[3];
              }
            }
        }
        } 
        else {
            // Measure was unsuccessful check res against Status enum
            Serial.printf("Error: %d\n", res);
        }
    }


  float sensorValue = analogRead(27);
 // Serial.print("Analog Value =");
  //Serial.println(sensorValue);
 
  float voltage = (sensorValue / 4095) * 5;
  float vap_pressure = 293.40;
  Serial.print("Voltage =");
  Serial.print(voltage);
  Serial.println(" V");

  int sensorValuet = analogRead(precpin);  // Read the analog value from sensor
  precipitation = map(sensorValuet, 0, 4095, 100, 1);
  Serial.println(sensorValuet);
  Serial.println(precipitation);

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
 
  Serial.print(F("Light Intensity = "));
  Serial.print(lintensity);
  Serial.println(F("lux"));

  Serial.print(F("windspeed = "));
  Serial.print(windSpeedMPH);
  Serial.println(F("mph"));

  Serial.print(F("precipitation = "));
  Serial.print(precipitation);
  Serial.println(F("%"));
 
  Serial.println();

 LoRaMessage = String(device_id) + "/" + String(temperature) + "&" + String(bar_pressure)
                + "#" + String(altitude) + "@" + String(humidity) + "%" + String(precipitation) + "*" + String(windSpeedMPH)+ "$" + String(vap_pressure);
 
  // send packet
  Serial.print("lora packet"); 
   
  LoRa.beginPacket();
  Serial.print("working");
  
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
 
 //delay(10000);
 
}
