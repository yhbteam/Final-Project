#include "DHT11.h"

GPIO_InitTypeDef GPIOStruct;

// DHT11引脚初始化（PA5）
void DHTInit(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIOStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOStruct.GPIO_Pin = GPIO_Pin_5;
    GPIOStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIOStruct);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_5); // 初始拉高
}

// 设置为输出模式
static void SetoutDHT(void) {
    GPIOStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIOStruct);
}

// 设置为输入模式
static void SetIptDHT(void) {
    GPIOStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIOStruct);
}

// 发送起始信号
static void SendRequest(void) {
    SetoutDHT();
    GPIO_ResetBits(GPIOA, GPIO_Pin_5); // 拉低总线
    delayMs(20);                       // 保持20ms
    GPIO_SetBits(GPIOA, GPIO_Pin_5);   // 释放总线
    delayUs(40);                       // 等待响应
}

// 检测响应信号
static uint8_t getRespone(void) {
    SetIptDHT();
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) {
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0); // 等待低电平结束
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1); // 等待高电平结束
        return SUCCESS;
    }
    return ERROR;
}

// 读取1字节数据
static uint8_t getByte(void) {
    uint8_t data = 0x00;
    for (int i = 0; i < 8; i++) {
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1); // 等待总线拉低
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0); // 等待低电平结束
        delayUs(35);                                           // 延时判断电平
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1) {
            data |= (0x80 >> i); // 高位先传
        }
    }
    return data;
}

// 读取DHT11完整数据（5字节）
uint8_t DHT_ReadData(uint8_t data[5]) {
    SendRequest();
    if (getRespone() != SUCCESS) {
        return ERROR;
    }
    
    // 读取5字节数据
    for (int i = 0; i < 5; i++) {
        data[i] = getByte();
    }
    
    // 校验和验证
    if (data[4] == (data[0] + data[1] + data[2] + data[3])) {
        return SUCCESS;
    }
    return ERROR;
}

// 解析温度值
float DHT_GetTemperature(uint8_t data[5]) {
    return (float)data[2] + (float)data[3] * 0.1f;
}

// 解析湿度值
float DHT_GetHumidity(uint8_t data[5]) {
    return (float)data[0] + (float)data[1] * 0.1f;
}