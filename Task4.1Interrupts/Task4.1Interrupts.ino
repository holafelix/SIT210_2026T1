#include <Wire.h>
#include <BH1750.h>

const int LED1_PIN   = 2;
const int LED2_PIN   = 3;
const int PIR_PIN    = 4;
const int SWITCH_PIN = 5;

BH1750 lightMeter;
const float DARK_THRESHOLD = 50.0;

volatile bool motionFlag = false;
volatile bool switchFlag = false;

bool lightsOn = false;
unsigned long lastSwitchTime = 0;
const unsigned long DEBOUNCE_MS = 200;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  Wire.begin();
  lightMeter.begin();

  attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchISR, CHANGE);

  Serial.println("System ready. Waiting for motion or the slider switch...");
}

void loop() {
  if (motionFlag) {
    motionFlag = false;

    float lux = lightMeter.readLightLevel();
    Serial.print("Motion detected. Light level = ");
    Serial.print(lux);
    Serial.println(" lux");

    if (lux < DARK_THRESHOLD) {
      turnLightsOn();
      Serial.println("It is dark -> porch and hallway lights ON (motion)");
    } else {
      Serial.println("Room is bright enough -> no lights needed");
    }
  }

  if (switchFlag) {
    switchFlag = false;
    if (millis() - lastSwitchTime > DEBOUNCE_MS) {
      lastSwitchTime = millis();
      toggleLights();
      Serial.print("Slider switch used (backup) -> lights ");
      Serial.println(lightsOn ? "ON" : "OFF");
    }
  }
}

void motionISR() {
  motionFlag = true;
}

void switchISR() {
  switchFlag = true;
}

void turnLightsOn() {
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  lightsOn = true;
}

void toggleLights() {
  lightsOn = !lightsOn;
  digitalWrite(LED1_PIN, lightsOn ? HIGH : LOW);
  digitalWrite(LED2_PIN, lightsOn ? HIGH : LOW);
}
