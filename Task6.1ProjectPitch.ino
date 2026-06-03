#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

#define WIFI_SSID      "Felix"
#define WIFI_PASS      "11112222"
#define CHANNEL_ID     3308856
#define WRITE_API_KEY  "U4X7IN0M6EZ05LWC"

const int LED1_PIN   = 2;
const int LED2_PIN   = 3;
const int PIR_PIN    = 4;
const int SWITCH_PIN = 5;

const float DARK_THRESHOLD          = 50.0;
const unsigned long LIGHT_ON_MS     = 20000;
const unsigned long INACTIVITY_LIMIT_MS = 60000;
const unsigned long THINGSPEAK_MIN_MS   = 20000;
const unsigned long DEBOUNCE_MS     = 200;

BH1750 lightMeter;
WiFiClient client;

volatile bool motionFlag = false;
volatile bool switchFlag = false;

bool lightsOn = false;
bool alertActive = false;
unsigned long lightsOnSince  = 0;
unsigned long lastMotionTime = 0;
unsigned long lastSwitchTime = 0;
unsigned long lastUploadTime = 0;
float lastLux = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  Wire.begin();
  lightMeter.begin();

  connectWiFi();
  ThingSpeak.begin(client);

  attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchISR, CHANGE);

  lastMotionTime = millis();
  Serial.println("Node ready. PIR needs ~30-60s to warm up.");
}

void loop() {
  handleMotion();
  handleSwitch();
  handleLightTimer();
  handleInactivity();
}

void motionISR() { motionFlag = true; }
void switchISR() { switchFlag = true; }

void handleMotion() {
  if (!motionFlag) return;
  motionFlag = false;

  lastMotionTime = millis();
  if (alertActive) {
    alertActive = false;
    Serial.println("Movement returned -> alert cleared");
  }

  lastLux = lightMeter.readLightLevel();
  Serial.print("Motion. Light = "); Serial.print(lastLux); Serial.println(" lux");

  if (lastLux < DARK_THRESHOLD) {
    turnLightsOn();
    Serial.println("Dark -> lights ON");
  } else {
    Serial.println("Bright enough -> lights stay off");
  }
  uploadToCloud(1);
}

void handleSwitch() {
  if (!switchFlag) return;
  switchFlag = false;
  if (millis() - lastSwitchTime < DEBOUNCE_MS) return;
  lastSwitchTime = millis();
  toggleLights();
  Serial.print("Manual switch -> lights "); Serial.println(lightsOn ? "ON" : "OFF");
}

void handleLightTimer() {
  if (lightsOn && (millis() - lightsOnSince > LIGHT_ON_MS)) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    lightsOn = false;
    Serial.println("Light timer expired -> lights OFF");
  }
}

void handleInactivity() {
  if (!alertActive && (millis() - lastMotionTime > INACTIVITY_LIMIT_MS)) {
    alertActive = true;
    Serial.println("No movement for too long -> ALERT carer");
    uploadToCloud(0);
  }
}

void turnLightsOn() {
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  lightsOn = true;
  lightsOnSince = millis();
}

void toggleLights() {
  lightsOn = !lightsOn;
  digitalWrite(LED1_PIN, lightsOn ? HIGH : LOW);
  digitalWrite(LED2_PIN, lightsOn ? HIGH : LOW);
  if (lightsOn) lightsOnSince = millis();
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500); Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "\nConnected" : "\nNo WiFi (running offline)");
}

void uploadToCloud(int motionEvent) {
  if (WiFi.status() != WL_CONNECTED) return;
  if (millis() - lastUploadTime < THINGSPEAK_MIN_MS) return;
  lastUploadTime = millis();

  int inactiveMin = (millis() - lastMotionTime) / 60000;

  ThingSpeak.setField(1, motionEvent);
  ThingSpeak.setField(2, lastLux);
  ThingSpeak.setField(3, inactiveMin);
  ThingSpeak.setField(4, alertActive ? 1 : 0);

  int result = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
  Serial.println(result == 200 ? "Cloud update sent" : "Cloud error: " + String(result));
}
