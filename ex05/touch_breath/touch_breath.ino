#define TOUCH_PIN 4    // 触摸T0 GPIO4(开发板丝印D2)
#define LED_PIN 2      // 板载PWM LED
const int touchThreshold = 600;
const unsigned long debounceDelay = 80;

// PWM基础参数
const int freq = 5000;
const int resolution = 8; // 0~255亮度

// 档位变量：1/2/3档，控制呼吸快慢
int speedGear = 1;
// 触摸状态变量（边沿检测+防抖）
bool lastTouchStatus = false;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  // 绑定PWM通道到LED引脚
  ledcAttach(LED_PIN, freq, resolution);
}

void loop() {
  // ========== 触摸档位切换逻辑 ==========
  int touchVal = touchRead(TOUCH_PIN);
  bool currentTouchStatus = (touchVal < touchThreshold);
  unsigned long currentTime = millis();

  if (currentTime - lastDebounceTime > debounceDelay) {
    // 仅触摸上升沿切换档位 1→2→3→1循环
    if (currentTouchStatus == true && lastTouchStatus == false) {
      speedGear += 1;
      if (speedGear > 3) speedGear = 1;
      Serial.print("切换至档位：");
      Serial.println(speedGear);
    }
    lastTouchStatus = currentTouchStatus;
    lastDebounceTime = currentTime;
  }

  // ========== 三档呼吸延时（差距明显，极易分辨） ==========
  int breathDelay;
  switch(speedGear) {
    case 1: breathDelay = 25; break; // 1档：慢速平缓呼吸
    case 2: breathDelay = 10; break; // 2档：中等速度
    case 3: breathDelay = 2; break;  // 3档：急促快速呼吸
    default: breathDelay = 25;
  }

  // PWM渐亮
  for(int duty = 0; duty <= 255; duty++) {
    ledcWrite(LED_PIN, duty);
    delay(breathDelay);
  }
  // PWM渐暗
  for(int duty = 255; duty >= 0; duty--) {
    ledcWrite(LED_PIN, duty);
    delay(breathDelay);
  }

  Serial.print("触摸数值：");
  Serial.print(touchVal);
  Serial.print(" | 当前档位：");
  Serial.println(speedGear);
}