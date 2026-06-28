#include <WiFi.h>
#include <WebServer.h>

// 硬件引脚定义
#define TOUCH_PIN 4

// WiFi配置，替换为你2.4G WiFi名称、密码
const char* ssid = "ymt";
const char* password = "20060205ymt666";

WebServer server(80);

// 生成仪表盘网页（AJAX定时拉取传感器数据）
String buildDashboardPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>实时触摸传感器仪表盘</title>
<style>
body {text-align:center;margin-top:80px;font-family:Arial;}
.title {font-size:28px;}
.val-box {font-size:60px;color:#d62828;font-weight:bold;margin:40px 0;}
.tip {font-size:20px;color:#555;}
</style>
</head>
<body>
  <div class="title">ESP32 触摸电容实时监测仪表盘</div>
  <div class="val-box">触摸原始数值：<span id="sensorVal">0</span></div>
  <div class="tip">手指靠近引脚 → 数值变小；手指离开 → 数值回升</div>

  <script>
    // 定时AJAX拉取传感器数据，100ms刷新一次
    function updateData(){
      fetch("/data")
      .then(res=>res.text())
      .then(val=>{
        document.getElementById("sensorVal").innerText = val;
      });
    }
    // 循环刷新
    setInterval(updateData, 100);
  </script>
</body>
</html>
)rawliteral";
  return html;
}

// 首页路由：返回仪表盘页面
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", buildDashboardPage());
}

// 数据接口：返回当前触摸原始数值
void handleGetData() {
  int touchRaw = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchRaw));
}

void setup() {
  Serial.begin(115200);

  // WiFi连接逻辑
  WiFi.begin(ssid, password);
  Serial.print("正在连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi联网成功");
  Serial.print("仪表盘访问地址：http://");
  Serial.println(WiFi.localIP());

  // 注册网页、数据接口路由
  server.on("/", handleRoot);
  server.on("/data", handleGetData);
  server.begin();
  Serial.println("Web仪表盘服务已启动");
}

void loop() {
  server.handleClient();
}