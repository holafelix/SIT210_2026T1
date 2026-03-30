#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>

BH1750 lightMeter;
WiFiClient client;

// WiFi
char ssid[] = "Felix";
char pass[] = "11112222";

// IFTTT
char host[] = "maker.ifttt.com";
String key = "cVArivCKMm74ZjCw5gVRHL";
String eventOn = "sunlight_on";
String eventOff = "sunlight_off";

float threshold = 300.0;  
bool isBright = false;    

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Wire.begin();

  if (lightMeter.begin()) {
    Serial.println("BH1750 started");
  } else {
    Serial.println("BH1750 failed to start");
    while (1);
  }

  connectWiFi();
  Serial.println("System ready");
}

void loop() {
  float lux = lightMeter.readLightLevel();

  Serial.print("Light level: ");
  Serial.println(lux);

  if (lux > threshold && isBright == false) {
    Serial.println("Sunlight started");
    sendIFTTT(eventOn, lux);
    isBright = true;
  }
  else if (lux <= threshold && isBright == true) {
    Serial.println("Sunlight stopped");
    sendIFTTT(eventOff, lux);
    isBright = false;
  }

  delay(5000);
}

void connectWiFi() {
  Serial.print("Connecting to WiFi");

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(3000);
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void sendIFTTT(String eventName, float luxValue) {
  Serial.print("Sending event: ");
  Serial.println(eventName);

  if (client.connect(host, 80)) {
    String url = "/trigger/" + eventName + "/json/with/key/" + key + "?value1=" + String(luxValue);

    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();

    Serial.println("Webhook sent");
  } else {
    Serial.println("Connection failed");
    return;
  }

  delay(1000);

  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  client.stop();
  Serial.println();
  Serial.println("Disconnected");
}