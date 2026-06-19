const int ledPin = 2;
unsigned long prevTime = 0;
const unsigned long interval = 1000;
bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long now = millis();
  if(now - prevTime >= interval){
    prevTime = now;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    Serial.print("Time:");
    Serial.print(now);
    Serial.println(ledState ? " LED ON" : " LED OFF");
  }
}