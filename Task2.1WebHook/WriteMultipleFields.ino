#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define WIFI_SSID "Felix"
#define WIFI_PASS "11112222"

#define CHANNEL_ID 3308856   // <-- replace with your channel ID
#define WRITE_API_KEY "J66UCZ1H5VTVQYPQ"

#define LIGHT_PIN A0
#define DHTPIN 2
#define DHTTYPE DHT11

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  int lightLevel = analogRead(LIGHT_PIN);

  Serial.print("Temp: ");
  Serial.println(temperature);
  Serial.print("Light: ");
  Serial.println(lightLevel);

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, lightLevel);

  int result = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);

  if (result == 200) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Error sending data: " + String(result));
  }

  delay(30000);
}