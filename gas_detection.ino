#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

//#define BLYNK_PRINT Serial
//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>
//BlynkTimer timer;
//
//char auth[] = "2w9Ogu_8iYXR8xB8Qu5S5JIQGxom7A_-";
//
//char ssid[] = "DIGI_0afd08";
//char pass[] = "aa21f502";

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

const int led1 = 16;
const int led2 = 17;
const int MQ2 = 34;

//int sensorValue = 0;
//boolean state = false;

void Task1code( void * pvParameters );
void Task2code( void * pvParameters );
void Task3code( void * pvParameters );

void setup() {
  Serial.begin(115200); 
//  Blynk.begin(auth, ssid, pass);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(MQ2, INPUT);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    ARDUINO_RUNNING_CORE);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    ARDUINO_RUNNING_CORE);          /* pin task to core 1 */
  delay(500); 
    
  xTaskCreatePinnedToCore(
                    Task3code,
                    "Task3",
                    1024,  
                    NULL,
                    1,  
                    &Task3,
                    ARDUINO_RUNNING_CORE);

  delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){

  for(;;){
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    delay(1000);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){

  for(;;){
    digitalWrite(led2, HIGH);
    delay(700);
    digitalWrite(led2, LOW);
    delay(700);
  }
}
  void Task3code(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

//  int sensorValueA3 = analogRead(MQ2);
//  Blynk.virtualWrite(V1, sensorValueA3);
//  Serial.println(sensorValueA3);
  
  for (;;)
  { 
    int sensorValueA3 = analogRead(MQ2);
    Serial.println(sensorValueA3);
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
}
void loop() 
{ 
}
