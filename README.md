# 密码与mqtt配置

qt登录：用户名：admin
        	密码：123456
上位机mqtt发布主题：cmd_topic
上位机mqtt订阅主题：status_topic
mqtt服务端ip为成都服务器






# RK3568 开发环境配置与问题解决

**适用硬件：** Rockchip RK3568 开发板  
**构建系统：** Buildroot  
**日期：** 2025-12-17

---

## 第一部分：环境配置文档

本部分详细记录了在 Buildroot `menuconfig` 中的关键配置路径，用于复现项目所需的软件环境。

### 1. Boa Web 服务器

用于部署前端控制页面。

- **功能：** 提供轻量级 HTTP 服务。
- **配置路径：**
  `Target packages` -> `Networking applications` -> `boa`
- **操作：** 选中 `boa`。

### 2. mjpg-streamer 流媒体服务

用于摄像头视频流的采集与网页端显示。

- **功能：** 处理 OV5695 摄像头数据并输出 HTTP 视频流。
- **配置路径：**
  `Target packages` -> `Audio and video applications` -> `mjpg-streamer`
- **关键插件：** 确保勾选 `input_uvc.so` 和 `output_http.so`。

### 3. JSON-C 库

用于处理 STM32 与 RK3568 之间的串口通信数据。

- **功能：** C 语言版本的 JSON 解析与生成库。
- **配置路径：**
  `Target packages` -> `Libraries` -> `JSON/XML` -> `json-c`
- **操作：** 选中 `json-c`。

### 4. Mosquitto Client

用于 MQTT 协议通信支持。

- **功能：** 提供 `mosquitto_pub` 和 `mosquitto_sub` 等客户端工具。
- **配置路径：**
  `Target packages` -> `Networking applications` -> `mosquitto`
- **操作：** 勾选 `Install the mosquitto client`。

### 5. Qt5 图形界面库

用于运行板载 GUI 应用程序。

- **配置路径：**
  `Target packages` -> `Graphic libraries and applications` -> `Qt5`
- **关键模块配置：**
  - **MQTT 支持：** 勾选 `qt5mqtt` (或 Connectivity 模块下的相关项)。
  - **数据库支持：** 在 `SQL module` 选项中，勾选 `SQLite 3 driver`。

---

## 第二部分：问题解决

本部分记录了在开发过程中遇到的关键技术难点、原因分析及最终解决方案。

### 问题 1：Buildroot 配置修改不生效

**问题描述：**
在使用 `make menuconfig` 增加软件包配置后，执行编译。但配置不生效。

**原因分析：**
`make menuconfig` 仅修改当前临时的 `.config` 文件。Buildroot 在配置时，会一直加载默认的 `defconfig` 文件，导致未固化的修改丢失。

**解决方案：**

1. 在 `make menuconfig` 中配置好所需环境并保存。
2. 使用生成的 `.config` 文件覆盖 SDK 默认的配置文件。
3. **操作路径：**
   用配置后的 `.config` 覆盖 `configs/rockchip_rk3568_defconfig`。

### 问题 2：mjpg-streamer 编译失败

**问题描述：**
构建系统时，mjpg-streamer 包编译报错，导致镜像生成中断。

**原因分析：**
SDK 中的 Glibc 版本更新，导致旧版 mjpg-streamer 源码与新版 C 运行库不兼容。

**解决方案：**
采用“手动替换源码 + 修改编译脚本”的方法：

1. **替换源码包：**
   - 路径：`source/rk356x_linux/buildroot/dl`
   - 操作：下载兼容新版 Glibc 的源码包，并重命名为：
     `mjpg-streamer-310b29f4a94c46652b20c4b7b6e5cf24e532af39`
2. **修改编译脚本：**
   - 路径：`source/rk356x_linux/buildroot/package/mjpg-streamer`
   - 操作：修改该目录下的 `.mk` 文件（更改 Hash 校验值、更新编译环境配置及编译器路径）。
3. **清理冲突文件：**
   - 删除同目录下的 `.hash` 文件（跳过哈希校验）。
   - 删除同目录下的补丁文件（`patch`），防止旧补丁应用错误。

### 问题 3：Menuconfig 与 Kconfig 命名符号差异

**问题描述：**
在手动编辑 `.config` 文件或在脚本中查找变量时，无法直接找到如 `json-c` 这样带连字符的名称。

**技术细节：**
Buildroot (Kconfig) 的语法规范规定，配置项名称中的连字符 `-` 在生成宏定义时会被自动转换为下划线 `_`。

**示例：**

- **Menuconfig 显示名称：** `json-c`
- **配置文件保存名称：** `json_c` (例如 `BR2_PACKAGE_JSON_C=y`)



## 第三部分：stm32部署难点：           

1. **FreeRTOS 任务与资源管理**

   - 任务优先级与调度冲突：多任务（如设备控制、传感器读取、通信处理）需合理分配优先级，防止高优先级任务阻塞低优先级任务，或因资源竞争导致死锁（如队列操作、事件组同步）。

   - 内存管理：任务动态内存分配

     

2. **时间敏感型操作**

   - 传感器数据读取（如 DHT11 的`getRespone`函数）存在严格的超时检测（`DHT_TIMEOUT_MAX`），需确保在 FreeRTOS 的任务延迟（`vTaskDelay`）或阻塞机制下不超出硬件响应时间。
   - 延时函数（如`delays`）基于`delayMs`实现，需与系统时钟（如`SetSysClockTo24`配置的 24MHz 时钟）同步，避免时间累积误差影响设备控制精度。

   

3. **中断与临界区处理**

   - 中断服务程序（ISR）中调用 FreeRTOS API（如`xQueueSendFromISR`）需严格遵循中断安全规范，确保临界区（`taskENTER_CRITICAL`/`taskEXIT_CRITICAL`）的正确使用，防止数据竞争。

   - 中断优先级配置需符合 FreeRTOS 要求（如`configMAX_SYSCALL_INTERRUPT_PRIORITY`），避免高优先级中断抢占内核操作导致系统不稳定。

     

4. **硬件兼容性与可靠性**

   - 不同硬件模块的引脚复用（如 GPIOA 同时连接蜂鸣器、红外传感器、DHT11
   - 外设驱动（如 USART 的 DMA 配置）需处理异常情况（如传输超时、缓冲区溢出），确保在极端条件下系统的容错能力。



## 第四部分：stm32问题



1.硬件与实时系统协同

- 需协调多个硬件模块（蜂鸣器、红外传感器、DHT11、USART 等）的初始化和操作时序，确保在 FreeRTOS 任务调度下硬件响应的实时性（如`deviceCtrlTask`中需在 50ms 周期内完成多个设备控制逻辑）。
- 中断处理与任务通信的同步问题，例如 USART 的 DMA 配置需与 FreeRTOS 队列（`uartRecvQueue`）配合，避免数据传输冲突或丢失。

2.stm32接收控制json时  串口接收数据不完整  在中途就触发中断  导致json格式错误  不能解析json控制外设

串口使用DMA+空闲中断，使用zigbee 时  zigbee 可能会分段发送数据 导致触发空闲中断 ，使json错误

解决方法：在串口定义缓存区接收数据  ，检测到{就把数据存到缓存区   检测到}结束  把缓存复制到freertos队列    处理json时从队列里取出数据降低代码的耦合性

3.stm32通过zigbee发送数据时，zigbee会发送空包   

在cgi文件里增加处理逻辑   丢弃空包和不完整的json

## 第五部分：交叉编译

在搭建 mjpg-streamer 视频流服务环境时，发现 Buildroot 内的编译环境与当前 RK3568 开发板存在兼容性问题。为了解决这个问题，我们尝试了两种不同的技术路径：

**交叉编译方案**：

我们从 GitHub 下载了 mjpg-streamer 的最新源码包，但在交叉编译过程中发现该软件包缺少对 libjpeg 库的必要依赖。经过分析，这是因为交叉编译工具链中的 libjpeg 库版本与 mjpg-streamer 所需的不匹配。为了解决这个问题，我们需要先单独下载并交叉编译 libjpeg 库，确保其与目标平台架构兼容，然后重新配置 mjpg-streamer 的编译选项，使其能够正确链接到我们新编译的 libjpeg 库。

**重新编译 Buildroot 方案**：

我们实现了交叉编译，但是我们也尝试了另一种方案：更换 Buildroot 的软件源，重新构建整个根文件系统。我们更新了 Buildroot 配置，启用了对 mjpg-streamer 及其所有依赖（包括 libjpeg、libjpeg-turbo 等）的支持，然后重新编译整个系统镜像。虽然这种方法耗时较长，但它确保了所有软件包之间的版本兼容性，最终得到了一个完全集成的解决方案。

经过对比测试，我们最终选择了重新编译 Buildroot 的方案，因为它提供了更稳定、更易于维护的视频流服务环境。

5. MQTT 集成与库链接问题

在将系统升级为基于 MQTT 的物联网架构时，我们的 CGI 控制程序需要使用 Eclipse Paho MQTT C 客户端库。在编译过程中，我们遇到了以下技术挑战和解决方案：

**静态库链接问题**：

最初我们尝试使用静态链接的方式编译 CGI 程序，但遇到了链接错误。具体问题是，MQTTClient 静态库与系统其他库（如 OpenSSL、pthread 等）存在符号冲突和依赖关系问题。即使使用 `-static`标志强制静态链接，仍然出现未定义符号错误，这是因为某些系统库没有提供静态版本，或者静态库版本不兼容。

**动态库解决方案**：

为了解决这个问题，我们改为使用动态链接方式。具体步骤如下：重新配置开发板环境，确保动态链接库路径正确设置；在目标系统上安装 MQTT C 客户端库的动态版本；修改 CGI 程序的编译脚本，移除 `-static`标志，使用动态链接；在 Makefile 中添加正确的库路径和依赖项。

​    6.Web 端视频流自适应网络环境优化

在部署过程中，我们发现 Web 端视频流显示存在一个严重问题：当开发板的本地网络 IP 地址发生变化时（例如从 DHCP 获取新地址），前端页面中的硬编码视频流 URL 将失效，导致视频监控功能无法使用。
