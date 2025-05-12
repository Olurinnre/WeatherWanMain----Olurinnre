#include "node2.h"
#include <esp_task_wdt.h> //add watchdog timer
#define WDT_TIMEOUT  1800 //timeout every 30 minutes     //600  //timeout every 10 minutes

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   Wire.begin();  
  
   
    // Initialise SDI-12 pin definition
  sdi12.begin();

    // Get a list of sensors on the bus on startup
    ESP32_SDI12::Status res = sdi12.sensorsOnBus(&sensors);

    if(res != ESP32_SDI12::SDI12_OK){
        // No sensors found, stop execution
        Serial.printf("No SDI-12 sensors found. Stopping.\n");
        while(true){ delay(1); }
    } else {
        // Devices found
        Serial.printf("%d SDI-12 sensors found.\n", sensors.count);
    }

   while (!Serial);
  Serial.println(F("LoRa Sender"));
 
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND))
  {
    Serial.println(F("Starting LoRa failed!"));
    while (1);
  }

 // rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
// rtc.setTime(30, 0, 15, 17, 1, 2021);  // 17th Jan 2021 15:24:30
 // sendData();
 ///watchdog timer
esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
esp_task_wdt_add(NULL); //add current thread to WDT watch
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(rtc.getMinute());        //  (int)     38    (0-59)

 // int time = rtc.getMinute();
  // unsigned long elapsed = time - previoustime;
  //Serial.println(time);
  
  //for (int i = 0; i <=1; i++)
  //if (startup == 1)

/*    if (time == 1)
    {

     //if (elapsed >= SEND_INTERVAL) {
    // Send data from the current device
      sendData(); 
      
      //startup = 0;
      old_time = 1;
     // delay(100000);
    goToDeepSleep_one_minute();
    }

  else
    {
      //Serial.print(F("out of the first if block "));
      //delay(20000);

      if (time == old_time + 5)
      {
        sendData();
        old_time = time;
        goToDeepSleep();
      }
    }
 */

//   if (time == 5)
//      {
//        sendData();
//        //old_time = time;
//        goToDeepSleep();
//     } 
esp_task_wdt_reset(); //reset watchdog timer
sendData();
goToDeepSleep();  


    // Update the previous send time
  //   previousMillis = currentMillis;
  // }
  //     sendData();
  


  
}


// #define DEEP_SLEEP_TIME 4 
// void goToDeepSleep(){
//   Serial.println("going to sleep");
//   esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 60*1000000);
//   esp_deep_sleep_start();
// }
