#include "node1.h"

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   Wire.begin();  

   while (!Serial);
  Serial.println(F("LoRa Sender"));
 
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND))
  {
    Serial.println(F("Starting LoRa failed!"));
    while (1);
  }

  pinMode(signal1.PIN_Interrupt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(signal1.PIN_Interrupt), isr, FALLING ); 
  
  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
  sendData();
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(rtc.getMinute());        //  (int)     38    (0-59)

  int time = rtc.getMinute();
  // unsigned long elapsed = time - previoustime;

  //for (int i = 0; i <=1; i++)
  //if (startup == 1)

    if (time == 2)
    {

     //if (elapsed >= SEND_INTERVAL) {
    // Send data from the current device
      sendData(); 
      
      //startup = 0;
      old_time = 2;
      delay(100000);

    }

  else
    {
      //Serial.print(F("out of the first if block "));
      //delay(20000);

      if (time == old_time + 2)
      {
        sendData();
        old_time = time;
        delay(100000);
      }
    }
    

    // Update the previous send time
  //   previousMillis = currentMillis;
  // }
  //     sendData();
   esp_task_wdt_reset();
}


// #define DEEP_SLEEP_TIME 4 
// void goToDeepSleep(){
//   Serial.println("going to sleep");
//   esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 60*1000000);
//   esp_deep_sleep_start();
// }
