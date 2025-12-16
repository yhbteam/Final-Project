// 全局变量
let deviceData = {
    temp: 0.0,
    hum: 0.0,
    led: 0,
    motor_speed: 0,
    buzzer: 0,
    servo_angle: 90,
    infrared: 1, // 1是正常
    zigbee_status: 0
};

let updateInterval = null;

// DOM 元素引用
const elements = {
    'temp-val': document.getElementById('temperature-value'),
    'hum-val': document.getElementById('humidity-value'),
    'led-slider': document.getElementById('led-slider'),
    'led-percent': document.getElementById('led-percent'),
    'led-circle': document.getElementById('led-circle'),
    'motor-slider': document.getElementById('motor-slider'),
    'motor-status': document.getElementById('motor-status-text'),
    'motor-wheel': document.getElementById('motor-wheel'),
    'servo-slider': document.getElementById('servo-slider'),
    'servo-text': document.getElementById('servo-angle-text'),
    'servo-arm': document.getElementById('servo-arm'),
    'buzzer-icon': document.getElementById('buzzer-icon'),
    'buzzer-switch': document.getElementById('buzzer-switch'),
    'infra-state': document.getElementById('infrared-state'),
    'sec-status': document.getElementById('security-status'),
    'conn-text': document.getElementById('zigbee-status'),
    'conn-dot': document.getElementById('connection-dot'),
    'log': document.getElementById('event-log')
};

// 初始化
function init() {
    // === 自动修正视频流地址 ===
    // 获取当前浏览器地址栏中的 IP (例如 10.194.145.175)
    const boardIP = window.location.hostname;
    // 如果是在本地文件打开(file://)，hostname为空，默认用 localhost
    const targetIP = boardIP || '127.0.0.1'; 
    const videoImg = document.getElementById('video-feed');
    
    // 动态设置 src，指向 8080 端口的 mjpg-streamer
    videoImg.src = `http://${targetIP}:8080/?action=stream`;
    console.log('Video stream target:', videoImg.src);
    // =============================

    setupListeners();
    startAutoUpdate();
    updateDateTime();
    addLog('系统初始化...', 'info');
}

// 事件监听
function setupListeners() {
    // LED (change: 松手发送, input: 拖动显示)
    elements['led-slider'].addEventListener('change', function() { updateLED(parseInt(this.value), true); });
    elements['led-slider'].addEventListener('input', function() { updateLED(parseInt(this.value), false); });

    // 电机
    elements['motor-slider'].addEventListener('change', function() { updateMotor(parseInt(this.value), true); });
    elements['motor-slider'].addEventListener('input', function() { updateMotor(parseInt(this.value), false); });

    // 舵机
    elements['servo-slider'].addEventListener('change', function() { updateServo(parseInt(this.value), true); });
    elements['servo-slider'].addEventListener('input', function() { updateServo(parseInt(this.value), false); });

    // 蜂鸣器
    elements['buzzer-switch'].addEventListener('change', function() { updateBuzzer(this.checked ? 1 : 0, true); });

    // 刷新视频
    document.getElementById('refresh-video').addEventListener('click', function() {
        const img = document.getElementById('video-feed');
        // 加个时间戳防止缓存
        const currentSrc = img.src.split('&t=')[0];
        img.src = currentSrc + '&t=' + new Date().getTime();
    });
}

// 发送指令到 device.cgi
async function sendControlCommand() {
    const payload = {
        led: deviceData.led,
        motor_speed: deviceData.motor_speed,
        buzzer: deviceData.buzzer,
        servo_angle: deviceData.servo_angle
    };

    try {
        const res = await fetch('/cgi-bin/device.cgi', {
            method: 'POST',
            body: JSON.stringify(payload)
        });
        // 发送后立即读取一次状态更新界面
        const data = await res.json();
        processData(data);
    } catch (e) {
        console.error('控制失败:', e);
        addLog('控制发送失败', 'error');
    }
}

// 轮询 data.cgi
async function loadData() {
    try {
        const res = await fetch('/cgi-bin/data.cgi');
        const data = await res.json();
        processData(data);
    } catch (e) {
        console.error('获取数据失败:', e);
        elements['conn-text'].textContent = '连接断开';
        elements['conn-dot'].style.background = 'red';
    }
}

// 处理后端返回的数据
function processData(data) {
    if (!data) return;
    
    // 更新本地数据
    deviceData = { ...deviceData, ...data };
    
    // 1. 连接状态
    const isOnline = deviceData.zigbee_status === 1;
    elements['conn-text'].textContent = isOnline ? '在线' : '等待设备...';
    elements['conn-dot'].style.background = isOnline ? '#4CAF50' : '#ff4444';
    
    // 2. 传感器
    elements['temp-val'].textContent = (deviceData.temp || 0).toFixed(1) + ' °C';
    elements['hum-val'].textContent = (deviceData.hum || 0).toFixed(1) + ' %';
    
    // 3. 红外 (0是遇阻/报警)
    const isBlocked = deviceData.infrared === 0;
    elements['infra-state'].textContent = isBlocked ? '有人/遇阻' : '正常';
    elements['infra-state'].style.color = isBlocked ? 'red' : 'green';
    elements['sec-status'].textContent = isBlocked ? '报警' : '安全';
    
    // 4. 更新控件 (只更新显示，不触发发送)
    updateLED(deviceData.led, false, true); 
    updateMotor(deviceData.motor_speed, false, true);
    updateServo(deviceData.servo_angle, false, true);
    
    // 蜂鸣器
    const buzzerOn = deviceData.buzzer === 1;
    elements['buzzer-switch'].checked = buzzerOn;
    elements['buzzer-icon'].style.color = buzzerOn ? 'red' : '#666';
}

// UI 更新函数
function updateLED(val, shouldSend, isFromBackend = false) {
    deviceData.led = val;
    elements['led-percent'].textContent = val + '%';
    elements['led-circle'].style.background = `rgba(255, 165, 0, ${val/100})`;
    
    if (isFromBackend) elements['led-slider'].value = val;
    
    if (shouldSend) {
        sendControlCommand();
        addLog('设置LED: ' + val, 'control');
    }
}

function updateMotor(val, shouldSend, isFromBackend = false) {
    deviceData.motor_speed = val;
    let status = '停止';
    if (val > 0) status = '正转 ' + val;
    if (val < 0) status = '反转 ' + Math.abs(val);
    
    elements['motor-status'].textContent = status;
    // 简单动画
    elements['motor-wheel'].style.animation = val === 0 ? 'none' : `spin ${Math.abs(1/val)*10}s linear infinite`;
    
    if (isFromBackend) elements['motor-slider'].value = val;

    if (shouldSend) {
        sendControlCommand();
        addLog('设置电机: ' + val, 'control');
    }
}

function updateServo(val, shouldSend, isFromBackend = false) {
    deviceData.servo_angle = val;
    elements['servo-text'].textContent = val + '°';
    elements['servo-arm'].style.transform = `rotate(${val - 90}deg)`; 
    
    if (isFromBackend) elements['servo-slider'].value = val;

    if (shouldSend) {
        sendControlCommand();
        addLog('设置舵机: ' + val, 'control');
    }
}

function updateBuzzer(val, shouldSend) {
    deviceData.buzzer = val;
    if (shouldSend) {
        sendControlCommand();
        addLog('设置蜂鸣器: ' + val, 'control');
    }
}

function addLog(msg, type) {
    const div = document.createElement('div');
    div.textContent = `[${new Date().toLocaleTimeString()}] ${msg}`;
    div.style.color = type === 'error' ? 'red' : (type === 'control' ? 'blue' : '#333');
    elements['log'].prepend(div);
}

function updateDateTime() {
    const now = new Date();
    document.getElementById('current-time').textContent = now.toLocaleTimeString();
    document.getElementById('current-date').textContent = now.toLocaleDateString();
    setTimeout(updateDateTime, 1000);
}

function startAutoUpdate() {
    loadData();
    setInterval(loadData, 4000); // 4秒轮询
}

document.addEventListener('DOMContentLoaded', init);
