#include "DHT11.h"
#include "FreeRTOS.h" // 包含FreeRTOS头文件，用于vTaskDelayUntil（可选）
#include "USART.h"

GPIO_InitTypeDef GPIOStruct;

// 超时计数最大值（根据实际需求调整，这里设为10万次，约100ms级别）
#define DHT_TIMEOUT_MAX  100000UL

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
    GPIOStruct.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入（DHT11默认高电平）
    GPIO_Init(GPIOA, &GPIOStruct);
}

// 微秒延时（注意：STM32F103的delayUs需要精准实现，这里用简单循环，可替换为定时器延时）
static void DHT_delayUs(uint32_t us) {
    us *= (SystemCoreClock / 1000000) / 5; // 粗略计算循环次数（根据CPU频率调整）
    while (us--) {
        __NOP();
    }
}

// 毫秒延时（FreeRTOS中建议用vTaskDelay，但这里是底层驱动，用简单循环）
static void DHT_delayMs(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        DHT_delayUs(1000);
    }
}

// 发送起始信号
static void SendRequest(void) {
    SetoutDHT();
    GPIO_ResetBits(GPIOA, GPIO_Pin_5); // 拉低总线
    DHT_delayMs(20);                   // 保持20ms（DHT11要求至少18ms）
    GPIO_SetBits(GPIOA, GPIO_Pin_5);   // 释放总线
    DHT_delayUs(40);                   // 等待DHT11响应（20~40us）
}

// 检测响应信号（添加超时）
static uint8_t getRespone(void) {
    SetIptDHT();
    uint32_t timeout = DHT_TIMEOUT_MAX;
    
    // 等待DHT11拉低总线（响应起始信号）
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == SET) {
        if (--timeout == 0) return ERROR; // 超时返回错误
    }
    
    // 等待低电平结束（DHT11响应低电平约80us）
    timeout = DHT_TIMEOUT_MAX;
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == RESET) {
        if (--timeout == 0) return ERROR;
    }
    
    // 等待高电平结束（DHT11响应高电平约80us）
    timeout = DHT_TIMEOUT_MAX;
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == SET) {
        if (--timeout == 0) return ERROR;
    }
    
    return SUCCESS;
}

// 读取1字节数据（添加超时）
static uint8_t getByte(void) {
    uint8_t data = 0x00;
    uint32_t timeout;
    
    for (int i = 0; i < 8; i++) {
        // 等待总线拉低（每个bit起始低电平约50us）
        timeout = DHT_TIMEOUT_MAX;
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == SET) {
            if (--timeout == 0) return 0xFF; // 超时返回无效值
        }
        
        // 等待低电平结束
        timeout = DHT_TIMEOUT_MAX;
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == RESET) {
            if (--timeout == 0) return 0xFF;
        }
        
        // 延时35us：高电平持续>26us为1，否则为0（DHT11时序要求）
        DHT_delayUs(35);
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == SET) {
            data |= (0x80 >> i); // 高位先传
        }
        
        // 等待当前bit的高电平结束（避免影响下一个bit）
        timeout = DHT_TIMEOUT_MAX;
        while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == SET) {
            if (--timeout == 0) return 0xFF;
        }
    }
    return data;
}

// 读取DHT11完整数据（5字节）
uint8_t DHT_ReadData(uint8_t data[5]) {
    // 清空接收缓存
    memset(data, 0, 5);
    
    // 发送起始信号
    SendRequest();
    
    // 检测响应
    if (getRespone() != SUCCESS) {
        // 响应失败，恢复引脚为高电平
        SetoutDHT();
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        return ERROR;
    }
    
    // 读取5字节数据
    for (int i = 0; i < 5; i++) {
        data[i] = getByte();
        // 若读取到无效值（超时），直接返回错误
        if (data[i] == 0xFF && i > 0) {
            SetoutDHT();
            GPIO_SetBits(GPIOA, GPIO_Pin_5);
            return ERROR;
        }
    }
    
    // 校验和验证
    if (data[4] == (data[0] + data[1] + data[2] + data[3])) {
        return SUCCESS;
    }
    
    // 校验失败，恢复引脚状态
    SetoutDHT();
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
    return ERROR;
}

// 解析温度值（DHT11温度范围0~50℃，精度0.1℃）
float DHT_GetTemperature(uint8_t data[5]) {
    return (float)data[2] + (float)data[3] * 0.1f;
}

// 解析湿度值（DHT11湿度范围20%~90%RH，精度0.1%RH）
float DHT_GetHumidity(uint8_t data[5]) {
    return (float)data[0] + (float)data[1] * 0.1f;
}
