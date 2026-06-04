#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

const char WIFI_SSID[] = "YOUR_WIFI_NAME";
const char WIFI_PASS[] = "YOUR_WIFI_PASSWORD";

const char BROKER[]     = "broker.emqx.io";
const int  PORT         = 1883;
const char TOPIC_WAVE[] = "ES/Wave";
const char TOPIC_PAT[]  = "ES/Pat";
const char MY_NAME[]    = "Felix";   

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const int LED_BATHROOM = 2;  
const int LED_HALLWAY  = 3;  
const int TRIG_PIN     = 9;   
const int ECHO_PIN     = 8;  

const long PAT_CM  = 8;    
const long WAVE_CM = 25;   

unsigned long lastGesture = 0;
const unsigned long COOLDOWN_MS = 2000;

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 3000); 
  
  pinMode(LED_BATHROOM, OUTPUT);
  pinMode(LED_HALLWAY, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(LED_BATHROOM, LOW);
  digitalWrite(LED_HALLWAY, LOW);

  connectWiFi();
  connectMQTT();
}

void loop() {
  mqttClient.poll();             
  if (!mqttClient.connected()) {
    connectMQTT();
  }

  checkGesture();
}

void connectWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    delay(3000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

void connectMQTT() {
  mqttClient.setId("felix-nano33iot-3c");   

  Serial.print("Connecting to broker ");
  Serial.println(BROKER);
  while (!mqttClient.connect(BROKER, PORT)) {
    Serial.print("MQTT failed, code ");
    Serial.println(mqttClient.connectError());
    delay(2000);
  }
  Serial.println("MQTT connected.");

  mqttClient.onMessage(onMqttMessage);
  mqttClient.subscribe(TOPIC_WAVE);
  mqttClient.subscribe(TOPIC_PAT);
  Serial.println("Subscribed to ES/Wave and ES/Pat.");
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);   
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  
  if (duration == 0) return -1;                  
  return duration / 58;                            
}

void checkGesture() {
  if (millis() - lastGesture < COOLDOWN_MS) return; 

  long d = readDistanceCM();
  if (d < 0) return;

  if (d <= PAT_CM) {              
    publishGesture(TOPIC_PAT);
    lastGesture = millis();
  } else if (d <= WAVE_CM) {        
    publishGesture(TOPIC_WAVE);
    lastGesture = millis();
  }
}

void publishGesture(const char* topic) {
  Serial.print("Publishing to ");
  Serial.println(topic);
  mqttClient.beginMessage(topic);
  mqttClient.print(MY_NAME);      
  mqttClient.endMessage();
}

void onMqttMessage(int messageSize) {
  String topic = mqttClient.messageTopic();
  String payload = "";
  while (mqttClient.available()) {
    payload += (char)mqttClient.read();
  }

  Serial.print("Message on ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);

  if (topic == TOPIC_WAVE) {
    digitalWrite(LED_BATHROOM, HIGH);
    digitalWrite(LED_HALLWAY, HIGH);
    Serial.println("-> Lights ON");
  } else if (topic == TOPIC_PAT) {
    digitalWrite(LED_BATHROOM, LOW);
    digitalWrite(LED_HALLWAY, LOW);
    Serial.println("-> Lights OFF");
  }
}
