#include "stm32f10x.h"
#include "USART.h"
#include "DHT11.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "led.h"
#include "servo.h"
#include "buzzer.h"
#include "infrared.h"
#include "motor.h"
#include "Delay.h" 

// ================== MQTT 配置信息 ==================
#define WIFI_SSID       "jk"
#define WIFI_PASS       "woniu@jiankong.com"
#define MQTT_BROKER_IP  "118.196.46.169"
#define MQTT_PORT       1883
#define MQTT_USER       "zg"
#define MQTT_PASS       "12345678"
#define MQTT_SUB_TOPIC  "cmd_topic"
#define MQTT_PUB_TOPIC  "status_topic"

// ================== 全局控制标志 ==================
// 0: 初始化阶段(透传AT回复); 1: 运行阶段(解析JSON)
volatile uint8_t is_mqtt_running = 0; 

// ================== 任务句柄 ==================
TaskHandle_t dhtTaskHandle;
TaskHandle_t jsonTaskHandle;
TaskHandle_t jsonParseTaskHandle;
TaskHandle_t deviceCtrlTaskHandle;

// ================== 全局变量 ==================
float temp = 0.0f;          // 温度
float hum = 0.0f;           // 湿度
int infrared = 1;           // 红外
int led_brightness = 60;    // LED亮度
int motor_dir = 1;          // 电机转向
int motor_speed = 20;       // 电机速度
int buzzer = 0;             // 蜂鸣器
int servo_angle = 90;       // 舵机角度

char jsonBuffer[256];       // 发送缓冲区

// ================== 智能 AT 指令发送函数 ==================
// 发送指令并等待期望的回复（替代死延时）
// 返回值: 1-成功, 0-超时
uint8_t ESP_Send_Check(char* cmd, char* expected_ack, uint32_t timeout_ms) {
    // 1. 清空接收缓冲，防止旧数据干扰
    memset(message, 0, 200); 
    
    // 2. 发送指令
    Send_String(cmd);
    
    // 3. 循环检查回复
    uint32_t time = 0;
    while(time < timeout_ms) {
        // 检查缓冲区是否包含期待的字符串 (如 "OK", "GOT IP")
        if(strstr(message, expected_ack)) {
            return 1; // 成功听到回复，立即返回，无需再等
        }
        delayMs(10); // 短暂延时
        time += 10;
    }
    return 0; // 超时
}

// ================== ESP8266 初始化函数 (极速版) ==================
void ESP8266_Init_Sequence(void) {
    char cmd[128];
    
    // 确保中断处于 AT 模式
    is_mqtt_running = 0;

    Send_String("1. Resetting...\r\n");
    ESP_Send_Check("AT+RST\r\n", "ready", 2000); // 等待 ready 或 2秒超时

    Send_String("2. Disable Echo...\r\n");
    ESP_Send_Check("ATE0\r\n", "OK", 500);

    Send_String("3. Setting Mode...\r\n");
    ESP_Send_Check("AT+CWMODE=1\r\n", "OK", 500);

    Send_String("4. Connecting WiFi...\r\n");
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASS);
    // 这里最关键：连得快就马上过，最长等 10秒
    if(ESP_Send_Check(cmd, "GOT IP", 10000)) {
        Send_String("   WiFi Connected (Fast)!\r\n");
    } else {
        Send_String("   WiFi Timeout (Check Pwd)!\r\n");
    }

    Send_String("5. Config MQTT...\r\n");
    snprintf(cmd, sizeof(cmd), "AT+MQTTUSERCFG=0,1,\"STM32_Edge\",\"%s\",\"%s\",0,0,\"\"\r\n", MQTT_USER, MQTT_PASS);
    ESP_Send_Check(cmd, "OK", 1000);

    Send_String("6. Connecting Broker...\r\n");
    snprintf(cmd, sizeof(cmd), "AT+MQTTCONN=0,\"%s\",%d,1\r\n", MQTT_BROKER_IP, MQTT_PORT);
    // 等待连接成功信号
    if(ESP_Send_Check(cmd, "+MQTTCONNECTED", 5000)) {
        Send_String("   Broker Connected!\r\n");
    }

    Send_String("7. Subscribing...\r\n");
    snprintf(cmd, sizeof(cmd), "AT+MQTTSUB=0,\"%s\",1\r\n", MQTT_SUB_TOPIC);
    ESP_Send_Check(cmd, "OK", 2000);

    Send_String("=== Init Done (Fast Mode) ===\r\n");
    
    // 初始化完成，切换中断逻辑为 MQTT JSON 模式
    is_mqtt_running = 1;
    // 清空一下缓冲，准备接收 FreeRTOS 任务数据
    memset(message, 0, 200);
}

// ... (JSON 解析逻辑 保持不变) ...
int jsonExtractValue(const char* json, const char* key, char* value, int valLen) {
    char keyBuf[32];
    snprintf(keyBuf, sizeof(keyBuf), "\"%s\":", key);
    const char* keyPos = strstr(json, keyBuf);
    if (keyPos == NULL) return -1;
    keyPos += strlen(keyBuf);
    const char* valStart = keyPos;
    const char* valEnd = valStart;
    while (*valStart == ' ' || *valStart == '\t') valStart++;
    valEnd = valStart;
    while (*valEnd != ',' && *valEnd != '}' && *valEnd != '\0') valEnd++;
    int len = valEnd - valStart;
    if (len > valLen - 1) len = valLen - 1;
    strncpy(value, valStart, len);
    value[len] = '\0';
    return 0;
}

void parseRecvJson(const char* json) {
    char valBuf[32];
    if (jsonExtractValue(json, "led", valBuf, sizeof(valBuf)) == 0) led_brightness = atoi(valBuf);
    if (jsonExtractValue(json, "motor_dir", valBuf, sizeof(valBuf)) == 0) motor_dir = atoi(valBuf);
    if (jsonExtractValue(json, "motor_speed", valBuf, sizeof(valBuf)) == 0) motor_speed = atoi(valBuf);
    if (jsonExtractValue(json, "buzzer", valBuf, sizeof(valBuf)) == 0) buzzer = atoi(valBuf);
		//if (jsonExtractValue(json, "infrared", valBuf, sizeof(valBuf)) == 0) infrared = atoi(valBuf);
    if (jsonExtractValue(json, "servo_angle", valBuf, sizeof(valBuf)) == 0) servo_angle = atoi(valBuf);
}

// ... (DHT, DeviceCtrl, JSON 任务保持不变) ...
void dhtTask(void* pvParameters) {
    uint8_t dhtData[5];
    while (1) {
        if (DHT_ReadData(dhtData) == SUCCESS) {
            temp = DHT_GetTemperature(dhtData);
            hum = DHT_GetHumidity(dhtData);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
}

void deviceCtrlTask(void* pvParameters) {
    while (1) {
        setBrightness(led_brightness);
        setAngle(servo_angle);
        switch(motor_dir) {
            case 1: forward(); break;
            case 0: back(); break;
            case -1: stop(); break;
            default: stop(); break;
        }
        setSpeed(motor_speed);
       // if(buzzer == 1) BuzzerOn(); else BuzzerOff();
        
//        if(InfraredRead() == 1) { infrared = 1; } 
//        else { infrared = 0; }
				
				       if(InfraredRead() == 1) { BuzzerOff();   buzzer=0;infrared=1;} 
               else { BuzzerOn(); buzzer=1;infrared=0;}
        vTaskDelay(50 / portTICK_PERIOD_MS); 
    }
}

void jsonParseTask(void* pvParameters) {
    char recvBuf[200]; 
    while (1) {
        if (xQueueReceive(uartRecvQueue, recvBuf, 100 / portTICK_PERIOD_MS) == pdPASS) {
            if (recvBuf[0] == '{' && recvBuf[strlen(recvBuf)-1] == '}') {
                parseRecvJson(recvBuf);
            }
            memset(recvBuf, 0, sizeof(recvBuf));
        }
    }
}

void jsonTask(void* pvParameters) {
    char mqttHeader[64];
    while (1) {
        memset(jsonBuffer, 0, sizeof(jsonBuffer));
        snprintf(jsonBuffer, sizeof(jsonBuffer),
            "{\"temp\":%.1f,\"hum\":%.1f,\"infrared\":%d,\"led\":%d,\"motor_dir\":%d,\"motor_speed\":%d,\"buzzer\":%d,\"servo_angle\":%d}",
            temp, hum, infrared, led_brightness, motor_dir, motor_speed, buzzer, servo_angle);

        int len = strlen(jsonBuffer);
        xQueueReset(uartRecvQueue); 

        snprintf(mqttHeader, sizeof(mqttHeader), "AT+MQTTPUBRAW=0,\"%s\",%d,0,0\r\n", MQTT_PUB_TOPIC, len);
        Send_String(mqttHeader);
        vTaskDelay(500 / portTICK_PERIOD_MS); 
        Send_String(jsonBuffer);
        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
}

void createTasks(void* pvParameters) {
    taskENTER_CRITICAL();
    xTaskCreate(deviceCtrlTask, "DEV_CTRL",   512,  NULL, 3, &deviceCtrlTaskHandle);
    xTaskCreate(dhtTask,        "DHT_Task",   512,  NULL, 2, &dhtTaskHandle);
    xTaskCreate(jsonTask,       "MQTT_PUB",   1024, NULL, 2, &jsonTaskHandle); 
    xTaskCreate(jsonParseTask,  "MQTT_SUB",   512,  NULL, 4, &jsonParseTaskHandle); 
    taskEXIT_CRITICAL();
    vTaskDelete(NULL);
}

// ================== 修改后的中断服务函数 ==================
void USART1_IRQHandler(void) {
    static uint8_t recvLen = 0;
    static uint8_t jsonCache[200] = {0}; // 假设 USART_REC_LEN 是 200
    static uint16_t jsonCacheIdx = 0;
    static uint8_t isJsonStarted = 0;
    
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(USART1); 
        // 外部定义在 USART.c 中的 DMA 通道
        recvLen = 200 - DMA_GetCurrDataCounter(DMA1_Channel5); 
        
        if (recvLen > 0) {
            // ============ 模式 A: 初始化阶段 (普通 AT 模式) ============
            if (is_mqtt_running == 0) {
                // 直接保留 message 中的数据供 ESP_Send_Check 检查
                // 仅需确保字符串结尾，不做任何清空
                message[recvLen] = '\0'; 
            }
            // ============ 模式 B: 运行阶段 (JSON 解析模式) ============
            else {
                for (uint16_t i = 0; i < recvLen; i++) {
                    uint8_t ch = message[i];
                    if (ch == '{') {
                        isJsonStarted = 1; jsonCacheIdx = 0;
                        memset(jsonCache, 0, 200);
                        jsonCache[jsonCacheIdx++] = ch;
                        continue;
                    }
                    if (!isJsonStarted) continue;
                    if (jsonCacheIdx < 199) {
                        jsonCache[jsonCacheIdx++] = ch;
                        if (ch == '}') {
                            jsonCache[jsonCacheIdx] = '\0';
                            isJsonStarted = 0;
                            // 投递队列
                            memset(message, 0, 200);
                            memcpy(message, jsonCache, jsonCacheIdx + 1);
                            xQueueOverwriteFromISR(uartRecvQueue, message, NULL);
                            jsonCacheIdx = 0;
                            memset(jsonCache, 0, 200);
                            break; 
                        }
                    } else {
                        isJsonStarted = 0; jsonCacheIdx = 0; // 溢出重置
                    }
                }
            }
        }
        
        // 重置 DMA
        DMA_Cmd(DMA1_Channel5, DISABLE);
        DMA_SetCurrDataCounter(DMA1_Channel5, 200);
        DMA_Cmd(DMA1_Channel5, ENABLE);
        
        // 只有在 MQTT 运行模式下才清空 message，否则保留给初始化函数查阅
        if (is_mqtt_running == 1) {
            recvLen = 0;
            memset(message, 0, 200);
        }
    }
}

int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    uartRecvQueue = xQueueCreate(4, sizeof(message));
    if(uartRecvQueue == NULL) while(1);

    USARTInit();     
    DHTInit();
    servoInit();
    ledInit();
    motorInit(); 
    BuzzerInit();
    InfraredInit();
    
    Send_String("Init...\r\n");

    // 运行极速初始化
    ESP8266_Init_Sequence();
    
    Send_String("Scheduler Start!\r\n");
    xTaskCreate(createTasks, "Create_Tasks", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    
    while (1);
}
