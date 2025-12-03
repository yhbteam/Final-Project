#include "stm32f10x.h"
#include "USART.h"
#include "DHT11.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include "OLED.h"

// 任务句柄
TaskHandle_t dhtTaskHandle;
TaskHandle_t jsonTaskHandle;

// 全局数据
float temperature = 0.0f;
float humidity = 0.0f;
char jsonBuffer[128];

// DHT11数据采集任务
void dhtTask(void* pvParameters) {
    uint8_t dhtData[5];
    
    while (1) {
        if (DHT_ReadData(dhtData) == SUCCESS) {
            temperature = DHT_GetTemperature(dhtData);
            humidity = DHT_GetHumidity(dhtData);
					
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS); // 2秒采集一次
    }
}

// JSON数据上报任务
void jsonTask(void* pvParameters) {
    while (1) {
        // 构建JSON格式
				snprintf(jsonBuffer, sizeof(jsonBuffer),
        "{\"temperature\":%.1f,\"humidity\":%.1f}\r\n",  // 新增\r\n
					temperature, humidity);
				Send_String(jsonBuffer);
        
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 1秒上报一次
    }
}

// 任务创建总任务
void createTasks(void* pvParameters) {
    taskENTER_CRITICAL(); // 临界区保护
    
    xTaskCreate(dhtTask, "DHT_Task", 128, NULL, 2, &dhtTaskHandle);
    xTaskCreate(jsonTask, "JSON_Task", 256, NULL, 1, &jsonTaskHandle);
    
    vTaskDelete(NULL); // 删除自身任务
    taskEXIT_CRITICAL();
}

int main(void) {
    // 外设初始化
    DHTInit();       // DHT11初始化
    USARTInit();     // 串口初始化（波特率9600）
	
    
    // 创建任务
    xTaskCreate(createTasks, "Create_Tasks", 128, NULL, 1, NULL);
    
    // 启动FreeRTOS调度器
    vTaskStartScheduler();
    
    // 防止程序跑飞
    while (1);
}
