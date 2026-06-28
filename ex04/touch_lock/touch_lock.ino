#define TOUCH_PIN 4    // 触摸引脚GPIO4（开发板丝印D2）
#define LED_PIN_BOARD 2// 板载蓝灯（开发板丝印D4）
#define LED_PIN_EXT 13 // 外接LED引脚

// 全局常量变量，全部放在最顶部，只定义一次
const int touchThreshold = 400;
const unsigned long debounceDelay = 150;

// 作业要求布尔状态变量
bool ledState = false;
bool lastTouchStatus = false;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN_BOARD, OUTPUT);
  pinMode(LED_PIN_EXT, OUTPUT);
  digitalWrite(LED_PIN_BOARD, LOW);
  digitalWrite(LED_PIN_EXT, LOW);
}

void loop() {
  int touchVal = touchRead(TOUCH_PIN);
  bool currentTouchStatus = (touchVal < touchThreshold);
  unsigned long currentTime = millis();

  if (currentTime - lastDebounceTime > debounceDelay) {
    // 上升沿检测：未触摸→触摸瞬间翻转
    if (currentTouchStatus == true && lastTouchStatus == false) {
      ledState = !ledState; // 翻转自锁状态
      // 同步控制板载+外接LED
      digitalWrite(LED_PIN_BOARD, ledState);
      digitalWrite(LED_PIN_EXT, ledState);
      Serial.print("状态切换：");
      Serial.println(ledState ? "亮" : "灭");
    }
    lastTouchStatus = currentTouchStatus;
    lastDebounceTime = currentTime;
  }

  Serial.print("触摸数值：");
  Serial.println(touchVal);
  delay(50);
}