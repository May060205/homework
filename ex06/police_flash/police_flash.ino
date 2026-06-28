
#define LED_A 15    
#define LED_B 16    

// PWM基础参数
const int PWM_FREQ = 5000;
const int PWM_RES = 8; // 亮度范围 0 ~ 255

void setup() {
  Serial.begin(115200);
  // 双通道独立PWM绑定
  ledcAttach(LED_A, PWM_FREQ, PWM_RES);
  ledcAttach(LED_B, PWM_FREQ, PWM_RES);
}

void loop() {
  // A渐亮 同步 B渐暗：dutyB = 255 - dutyA 反相逻辑
  for(int dutyA = 0; dutyA <= 255; dutyA++){
    int dutyB = 255 - dutyA;
    ledcWrite(LED_A, dutyA);
    ledcWrite(LED_B, dutyB);
    delay(8);
  }

  // A渐暗 同步 B渐亮
  for(int dutyA = 255; dutyA >= 0; dutyA--){
    int dutyB = 255 - dutyA;
    ledcWrite(LED_A, dutyA);
    ledcWrite(LED_B, dutyB);
    delay(8);
  }

  Serial.println("一轮反相双闪渐变完成");
}