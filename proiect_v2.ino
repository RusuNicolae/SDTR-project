#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
//#include<semphr.h>
//#include<SoftwareSerial.h>
#include<HardwareSerial.h>
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#ifndef INC_FREERTOS_H
  #error "include FreeRTOS.h" must appear in source files before "include semphr.h"
#endif

#include "queue.h"
//aici cred ca mai trebuie ceva
#endif

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;

const int led1 = 16;
const int led2 = 17;
const int MQ2 = 34;

void Task_ReadFromSensor(void *pvParameters);
void Led(void *pvParameters);
void Taskprint( void *pvParameters );

SemaphoreHandle_t xSerialSemaphore;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(MQ2, INPUT);

while (!Serial) {
    ; // wait for serial port to connect
  }


   if (xSerialSemaphore == NULL){
    xSerialSemaphore = xSemaphoreCreateMutex();
    if ((xSerialSemaphore) != NULL)
    xSemaphoreGive((xSerialSemaphore));
    }
   
 xTaskCreate(
    Task_ReadFromSensor
    , "Sensor"
    , 100
    , NULL
    , 1 //prioritate
    , NULL
    );

  xTaskCreate(
    Led
    , "Led"
    , 128
    , NULL
    , 1
    , NULL
    );
vTaskStartScheduler();
}
void loop() 
{ 
}
void Task_ReadFromSensor(void *pvParameters)
{
  while(1){
   
  int sensorReading = analogRead(MQ2); 

  if (sensorReading > 2000)
  {
    if (xSemaphoreTake (xSerialSemaphore, (TickType_t) 10) == pdTRUE){
     
 digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  }

  else
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
  }

    }     

      xSemaphoreGive(xSerialSemaphore);
  
   vTaskDelay(1);
  }
}
void Led(void *pvParameters)  
{
  pinMode(17, OUTPUT);
  while(1)
  {
    if (xSemaphoreTake (xSerialSemaphore, (TickType_t) 5) == pdTRUE){
     Serial.println("LED aprins-stins (blink)");
    }
     digitalWrite(17, HIGH);   
     vTaskDelay( 300 / portTICK_PERIOD_MS ); 
     digitalWrite(17, LOW);   
     vTaskDelay( 300 / portTICK_PERIOD_MS ); 
     xSemaphoreGive(xSerialSemaphore);
 
    vTaskDelay(1);
}
}

void Taskprint(void *pvParameters)  {
  int counter = 0;
  while(1)
  {
    if (xSemaphoreTake (xSerialSemaphore, (TickType_t) 5) == pdTRUE){
  counter++;
  Serial.println(counter); 
    }
  xSemaphoreGive(xSerialSemaphore);
  vTaskDelay(500 / portTICK_PERIOD_MS);    
}
  vTaskDelay(1);}