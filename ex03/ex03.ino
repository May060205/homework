/*
 * 简化版 - 更清晰的SOS信号
 * 使用标志位更直观地控制
 */

const int LED_PIN = 2;

// 时间常量
const unsigned long SHORT = 200;
const unsigned long LONG = 600;
const unsigned long GAP = 200;      // 点/横之间
const unsigned long LETTER_GAP = 600; // 字母之间
const unsigned long WORD_GAP = 1400;  // 单词之间

// 状态定义
enum SOSState {
  S1_ON, S1_OFF,  // S第一个点
  S2_ON, S2_OFF,  // S第二个点
  S3_ON, S3_OFF,  // S第三个点
  LETTER1_OFF,    // S和O之间
  O1_ON, O1_OFF,  // O第一个横
  O2_ON, O2_OFF,  // O第二个横
  O3_ON, O3_OFF,  // O第三个横
  LETTER2_OFF,    // O和S之间
  S4_ON, S4_OFF,  // S第一个点
  S5_ON, S5_OFF,  // S第二个点
  S6_ON, S6_OFF,  // S第三个点
  WORD_OFF        // 单词之间
};

SOSState currentState = S1_ON;
unsigned long lastChange = 0;

// 获取当前状态的持续时间
unsigned long getDuration(SOSState state) {
  switch(state) {
    case S1_ON: case S2_ON: case S3_ON: case S4_ON: case S5_ON: case S6_ON:
      return SHORT;
    case O1_ON: case O2_ON: case O3_ON:
      return LONG;
    case S1_OFF: case S2_OFF: case S3_OFF: case S4_OFF: case S5_OFF: case S6_OFF:
    case O1_OFF: case O2_OFF: case O3_OFF:
      return GAP;
    case LETTER1_OFF: case LETTER2_OFF:
      return LETTER_GAP;
    case WORD_OFF:
      return WORD_GAP;
    default:
      return GAP;
  }
}

// 获取下一个状态
SOSState getNextState(SOSState state) {
  int next = (int)state + 1;
  if (next > WORD_OFF) next = S1_ON;
  return (SOSState)next;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("SOS Signal Started...");
}

void loop() {
  unsigned long now = millis();
  
  if (now - lastChange >= getDuration(currentState)) {
    lastChange = now;
    
    // 设置LED状态
    bool ledOn = (currentState == S1_ON || currentState == S2_ON || currentState == S3_ON ||
                  currentState == O1_ON || currentState == O2_ON || currentState == O3_ON ||
                  currentState == S4_ON || currentState == S5_ON || currentState == S6_ON);
    
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    
    // 调试输出
    if (ledOn) {
      if (getDuration(currentState) == SHORT) Serial.print(".");
      else Serial.print("-");
    } else {
      if (getDuration(currentState) == LETTER_GAP) Serial.print(" / ");
      else if (getDuration(currentState) == WORD_GAP) Serial.println("  [SOS]\n");
    }
    
    // 切换到下一个状态
    currentState = getNextState(currentState);
  }
}