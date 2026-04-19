#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

// ================= CAMERA =================
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// ================= WIFI =================
const char* ssid = "MotorolaEdge50Fusion";
const char* password = "mattaravi";

// ================= SERVER =================
WebServer server(80);
String message = "READY";
String lastState = "";

// ================= WEB PAGE =================
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    body {
      margin: 0;
      height: 100vh;
      background: black;
      color: white;
      display: flex;
      justify-content: center;
      align-items: center;
      font-size: 22px;
      text-align: center;
    }
  </style>
</head>

<body>

<div id="status">Tap anywhere to start</div>

<script>
let last = "";
let started = false;

// 🔥 Works anywhere on screen
document.addEventListener("touchstart", startAudio, {once:true});
document.addEventListener("click", startAudio, {once:true});

function startAudio() {
  started = true;

  // 🔊 confirmation voice
  let confirm = new SpeechSynthesisUtterance("System started");
  confirm.rate = 1.2;
  speechSynthesis.speak(confirm);

  document.getElementById("status").innerText = "Running...";
}

setInterval(async () => {
  if (!started) return;

  let res = await fetch('/data');
  let text = await res.text();

  document.getElementById("status").innerText = text;

  if (text !== last) {
    speechSynthesis.cancel();

    let speech = new SpeechSynthesisUtterance(text);
    speech.rate = 1.3;

    speechSynthesis.speak(speech);
    last = text;
  }
}, 200);
</script>

</body>
</html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleData() {
  server.send(200, "text/plain", message);
}

// ================= SETUP =================
void setup() {
  delay(3000); // avoid boot garbage

  Serial.begin(115200);
  Serial.setDebugOutput(false);

  Serial.println("\nSYSTEM STARTED CLEANLY\n");

  // -------- Camera --------
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.frame_size = FRAMESIZE_QQVGA;
  config.pixel_format = PIXFORMAT_RGB565;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_gain_ctrl(s, 0);
  s->set_exposure_ctrl(s, 0);
  s->set_brightness(s, 2);
  s->set_contrast(s, 1);

  // -------- WiFi --------
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");

  // -------- SERVER --------
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();

  // -------- SHOW LINK --------
  Serial.println("\n==============================");
  Serial.println(" OPEN THIS LINK IN YOUR PHONE ");
  Serial.print(" http://");
  Serial.println(WiFi.localIP());
  Serial.println("==============================\n");

  Serial.println("Tap anywhere on screen to start voice\n");
}

// ================= LOOP =================
void loop() {
  server.handleClient();

  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) return;

  int darkPixels = 0;

  for (int i = 0; i < fb->len; i += 2) {
    uint16_t pixel = fb->buf[i] | (fb->buf[i+1] << 8);

    uint8_t r = (pixel >> 11) & 0x1F;
    uint8_t g = (pixel >> 5) & 0x3F;
    uint8_t b = pixel & 0x1F;

    uint8_t brightness = (r * 76 + g * 150 + b * 29) >> 8;

    if (brightness < 30) darkPixels++;
  }

  esp_camera_fb_return(fb);

  // ===== DECISION =====
  if (darkPixels > 14000) {
    if (lastState != "OBSTACLE") {
      Serial.println("OBSTACLE AHEAD");
      message = "Obstacle Ahead";
      lastState = "OBSTACLE";
    }
  } else {
    if (lastState != "CLEAR") {
      Serial.println("CLEAR PATH");
      message = "Path Clear";
      lastState = "CLEAR";
    }
  }

  delay(200);
}