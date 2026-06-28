#include <WiFi.h>
#include <WebServer.h>

const int LED_PIN = 2;
const int PWM_FREQ = 5000;
const int PWM_RES = 8;

const char* ssid = "ymt";
const char* password = "20060205ymt666";

WebServer server(80);
int brightness = 0;

String buildHtml() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 网页无极调光</title>
  <style>
    body {text-align:center;margin-top:60px;font-family:Arial;}
    #brightSlider {width:85%;height:32px;margin:30px 0;}
    #showVal {font-size:32px;color:#d02020;font-weight:bold;}
  </style>
</head>
<body>
  <h1>Web端无极PWM调光器</h1>
  <p>拖动滑块调节LED亮度 0 ~ 255</p>
  <input type="range" id="brightSlider" min="0" max="255" value=")rawliteral" + String(brightness) + R"rawliteral(">
  <div>当前亮度数值：<span id="showVal">)rawliteral" + String(brightness) + R"rawliteral(</span></div>

  <script>
    const slider = document.getElementById("brightSlider");
    const valText = document.getElementById("showVal");
    // 防抖：减少重复请求堆积
    let timer = null;
    slider.addEventListener("input", function(){
      let val = this.value;
      valText.innerText = val;
      clearTimeout(timer);
      // 延迟发送，避免大量请求阻塞，保证0参数稳定送达
      timer = setTimeout(()=>{
        fetch("/set?b="+val);
      },80);
    });
  </script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", buildHtml());
}

void handleSetBright() {
  if(server.hasArg("b")){
    brightness = server.arg("b").toInt();
    brightness = constrain(brightness, 0, 255);
    ledcWrite(LED_PIN, brightness);
  }
  // 删除303页面重定向！不再强制刷新页面，解决请求冲突
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);
  ledcWrite(LED_PIN, 0);

  WiFi.begin(ssid, password);
  Serial.print("正在连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！");
  Serial.print("网页访问地址：http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
  Serial.println("Web服务器启动完成");
}

void loop() {
  server.handleClient();
}