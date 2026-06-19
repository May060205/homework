// ESP32板载LED硬件引脚，固定为GPIO2
const int ledPin = 2;

// 记录上一次切换LED状态的系统时间，必须用unsigned long匹配millis返回值
unsigned long prevTime = 0;
// LED状态切换间隔：1000毫秒 = 1秒，实现1Hz闪烁
const unsigned long interval = 1000;
// 标记LED当前电平状态，LOW=熄灭，HIGH=点亮
bool ledState = LOW;

// setup函数：上电仅执行1次，用于硬件、串口初始化
void setup() {
  // 初始化串口通信，波特率设置115200，串口监视器必须匹配此数值才能正常显示
  Serial.begin(115200);
  // 将GPIO2引脚配置为输出模式，用来控制LED高低电平
  pinMode(ledPin, OUTPUT);
  // 上电初始状态：熄灭LED，避免开机乱闪
  digitalWrite(ledPin, LOW);
  // 串口打印程序启动提示
  Serial.println("=== ESP32 millis无阻塞LED闪烁程序启动 ===");
}

// loop函数：上电循环持续执行，程序主逻辑
void loop() {
  // 获取开发板从上电到当前的总运行毫秒数
  unsigned long now = millis();

  // 判断：当前时间 - 上次切换时间 ≥ 设定1秒间隔，满足条件就翻转LED
  if(now - prevTime >= interval){
    // 更新时间戳，把本次切换的时间记录为新基准
    prevTime = now;
    // 翻转LED状态：LOW变HIGH、HIGH变LOW
    ledState = !ledState;
    // 输出电平到GPIO2，控制LED亮灭
    digitalWrite(ledPin, ledState);

    // 串口打印当前运行毫秒数
    Serial.print("运行毫秒Time: ");
    Serial.print(now);
    // 三元运算符：根据ledState打印对应LED状态
    Serial.print("  LED状态: ");
    Serial.println(ledState ? "LED ON 点亮" : "LED OFF 熄灭");
  }
  // 无阻塞优势：if外可添加其他任务代码，不会被延时卡住并行运行
}