/*
 * ESP32 SOS信号 - 使用millis()函数
 * SOS信号：连续三次短闪，三次长闪，再三次短闪
 * 正确实现：每个亮和灭的状态都有独立的持续时间
 */

const int LED_PIN = 2;  // ESP32板载LED在GPIO2

// 时间常量（单位：毫秒）
const unsigned long SHORT_FLASH = 200;    // 短闪持续时间
const unsigned long LONG_FLASH = 600;     // 长闪持续时间  
const unsigned long PAUSE_SHORT = 200;    // 闪烁间熄灭
const unsigned long PAUSE_LETTER = 600;   // 字母间熄灭
const unsigned long PAUSE_WORD = 1400;    // 单词间熄灭

// 使用数组存储每个步骤的状态和持续时间
// 偶数索引：亮的状态，奇数索引：灭的状态
const unsigned long sosTiming[] = {
  // S: 三个短闪 (亮-灭-亮-灭-亮-灭)
  SHORT_FLASH, PAUSE_SHORT,   // 第1个短闪
  SHORT_FLASH, PAUSE_SHORT,   // 第2个短闪
  SHORT_FLASH, PAUSE_LETTER,  // 第3个短闪 + 字母间灭600ms
  
  // O: 三个长闪 (亮-灭-亮-灭-亮-灭)
  LONG_FLASH, PAUSE_SHORT,    // 第1个长闪
  LONG_FLASH, PAUSE_SHORT,    // 第2个长闪
  LONG_FLASH, PAUSE_LETTER,   // 第3个长闪 + 字母间灭600ms
  
  // S: 三个短闪 (亮-灭-亮-灭-亮-灭)
  SHORT_FLASH, PAUSE_SHORT,   // 第1个短闪
  SHORT_FLASH, PAUSE_SHORT,   // 第2个短闪
  SHORT_FLASH, PAUSE_WORD     // 第3个短闪 + 单词间灭1400ms
};

const int TOTAL_STEPS = sizeof(sosTiming) / sizeof(sosTiming[0]);
int currentStep = 0;
unsigned long previousMillis = 0;
bool ledState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(115200);
  Serial.println("=== SOS Signal Started ===");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // 检查当前步骤是否完成
  if (currentMillis - previousMillis >= sosTiming[currentStep]) {
    previousMillis = currentMillis;
    
    // 切换到下一个步骤
    currentStep++;
    
    // 如果完成所有步骤，重新开始
    if (currentStep >= TOTAL_STEPS) {
      currentStep = 0;
      Serial.println("=== SOS Cycle Complete ===");
    }
    
    // 切换LED状态（亮/灭交替）
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    
    // 打印调试信息
    if (ledState) {
      if (sosTiming[currentStep] == SHORT_FLASH) {
        Serial.print(".");
      } else if (sosTiming[currentStep] == LONG_FLASH) {
        Serial.print("-");
      }
    } else {
      if (sosTiming[currentStep] == PAUSE_LETTER) {
        Serial.print(" / ");
      } else if (sosTiming[currentStep] == PAUSE_WORD) {
        Serial.println("  [SOS]");
      }
      // PAUSE_SHORT 不打印，保持简洁
    }
  }
}