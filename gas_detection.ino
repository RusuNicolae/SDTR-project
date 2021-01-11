#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

//char auth[] = "2w9Ogu_8iYXR8xB8Qu5S5JIQGxom7A_-";
char auth[] = "HmjJ7MVLfcpCdEl9-TwEiFVn0qsWpKE7";


char ssid[] = "DIGI_0afd08";
char pass[] = "aa21f502";

const int led1 = 16;
const int led2 = 17;
const int MQ2 = 34;

void Task1code( void * pvParameters );
void Task2code( void * pvParameters );
void Task3code( void * pvParameters );

SemaphoreHandle_t xSerialSemaphore;

void setup() {
  
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(MQ2, INPUT);
//  timer.setInterval(1000L, sendUptime);
  while (!Serial) {
    ; // wait for serial port to connect
  } 
   if (xSerialSemaphore == NULL){
    xSerialSemaphore = xSemaphoreCreateMutex();
    if ((xSerialSemaphore) != NULL)
    xSemaphoreGive((xSerialSemaphore));
    }
    
 xTaskCreatePinnedToCore(
                    Task1code,  
                    "Task1",    
                    1024,       
                    NULL,       
                    1,          
                    NULL,     
                    ARDUINO_RUNNING_CORE);                           
  vTaskDelay(500); 

  xTaskCreatePinnedToCore(
                    Task2code,   
                    "Task2",     
                    1024,       
                    NULL,        
                    1,           
                    NULL,      
                    ARDUINO_RUNNING_CORE);          
  vTaskDelay(500); 
    
  xTaskCreatePinnedToCore(
                    Task3code,
                    "Task3",
                    10000,  
                    NULL,
                    2,  
                    NULL,
                    ARDUINO_RUNNING_CORE);

  vTaskDelay(500); 
}

void loop()
{  
}

void Task1code(void *pvParameters)  
{
  (void) pvParameters;
  pinMode(led1, OUTPUT);
  
  while(1)
  {
  if (xSemaphoreTake (xSerialSemaphore, (TickType_t) 5) == pdTRUE){
     Serial.println("LED verde aprins ");
    }
    digitalWrite(led1, HIGH);   
    vTaskDelay( 1000 / portTICK_PERIOD_MS );    
    digitalWrite(led1, LOW);    
    vTaskDelay( 1000 / portTICK_PERIOD_MS );  
    xSemaphoreGive(xSerialSemaphore);
 
    vTaskDelay(10);
}
}

void Task2code(void *pvParameters)  
{
  (void) pvParameters;
  pinMode(led2, OUTPUT);
  
  while(1)
  {
    if (xSemaphoreTake (xSerialSemaphore, (TickType_t) 5) == pdTRUE){
     Serial.println("LED rosu aprins ");
    }
    digitalWrite(led2, HIGH);   
    vTaskDelay( 1000 / portTICK_PERIOD_MS );  
    digitalWrite(led2, LOW);    
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
    xSemaphoreGive(xSerialSemaphore);
    vTaskDelay(10); 
  }
}

void Task3code(void *pvParameters)  
{
  (void) pvParameters;

while(1){
  int sensorValueA3 = analogRead(MQ2);
  Blynk.virtualWrite(V1, sensorValueA3);
  Serial.println(sensorValueA3);
  if (sensorValueA3 > 1750)
  {
//  if (xSemaphoreTake (xSerialSemaphore, (TickType_t) 10) == pdTRUE){
  Blynk.notify("Gas Detected!");
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }

  else
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }
  }
//        xSemaphoreGive(xSerialSemaphore);
  
   vTaskDelay(10);
}