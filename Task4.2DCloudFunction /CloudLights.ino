#include <WiFiNINA.h>

char ssid[] = "Felix";
char pass[] = "11112222";

const int livingRoomLED = 2;
const int bathroomLED   = 3;
const int closetLED     = 4;

bool livingRoomOn = false;
bool bathroomOn   = false;
bool closetOn     = false;

int status = WL_IDLE_STATUS;
WiFiServer server(80);            

void setup() {
  Serial.begin(9600);

  pinMode(livingRoomLED, OUTPUT);
  pinMode(bathroomLED, OUTPUT);
  pinMode(closetLED, OUTPUT);

  digitalWrite(livingRoomLED, LOW);
  digitalWrite(bathroomLED, LOW);
  digitalWrite(closetLED, LOW);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module not found");
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  server.begin();

  Serial.print("Open this in your browser: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();  
  if (!client) return;

  String requestLine = "";  
  String currentLine = "";
  bool gotRequestLine = false;

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      if (c == '\n') {
        if (currentLine.length() == 0) {
          handleRequest(requestLine);  
          sendPage(client);           
          break;
        } else {
          if (!gotRequestLine) {     
            requestLine = currentLine;
            gotRequestLine = true;
          }
          currentLine = "";
        }
      } else if (c != '\r') {
        currentLine += c;
      }
    }
  }

  delay(1);
  client.stop();
}

void handleRequest(String requestLine) {
  if (requestLine.indexOf("room=living") >= 0) {
    toggleLight("living room");
  } else if (requestLine.indexOf("room=bathroom") >= 0) {
    toggleLight("bathroom");
  } else if (requestLine.indexOf("room=closet") >= 0) {
    toggleLight("closet");
  }
}

void toggleLight(String room) {
  if (room == "living room") {
    livingRoomOn = !livingRoomOn;
    digitalWrite(livingRoomLED, livingRoomOn ? HIGH : LOW);
  } else if (room == "bathroom") {
    bathroomOn = !bathroomOn;
    digitalWrite(bathroomLED, bathroomOn ? HIGH : LOW);
  } else if (room == "closet") {
    closetOn = !closetOn;
    digitalWrite(closetLED, closetOn ? HIGH : LOW);
  }
}

void sendPage(WiFiClient &client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html><html><head>");
  client.println("<meta charset='utf-8'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("<title>Linda's Lights</title>");
  client.println("<style>");
  client.println("body{font-family:Arial;text-align:center;background:#f2f2f2;}");
  client.println("h1{color:#333;}");
  client.println(".light{background:#fff;margin:12px auto;padding:16px;width:240px;border-radius:10px;}");
  client.println(".on{color:green;font-weight:bold;}");
  client.println(".off{color:gray;font-weight:bold;}");
  client.println("a.btn{display:inline-block;margin-top:8px;padding:8px 18px;background:#0a7;color:#fff;text-decoration:none;border-radius:6px;}");
  client.println("</style></head><body>");
  client.println("<h1>Linda's Home Lights</h1>");

  printLight(client, "Living Room", "living", livingRoomOn);
  printLight(client, "Bathroom", "bathroom", bathroomOn);
  printLight(client, "Closet", "closet", closetOn);

  client.println("</body></html>");
}

void printLight(WiFiClient &client, String name, String token, bool isOn) {
  client.print("<div class='light'><h2>");
  client.print(name);
  client.print("</h2><p>Status: <span class='");
  client.print(isOn ? "on'>ON" : "off'>OFF");
  client.print("</span></p>");
  client.print("<a class='btn' href='/toggle?room=");
  client.print(token);
  client.print("'>Toggle</a></div>");
}
