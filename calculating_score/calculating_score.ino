#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Your-SSID";
const char* password = "Your-Password";

int targetPin = 1;       // GPIO pin connected to the target sensor
int laserPin = 2;        // GPIO pin connected to the laser module

int hits = 0;             // Variable to store the number of hits
int misses = 0;           // Variable to store the number of misses
int score = 0;            // Variable to store the score

ESP8266WebServer server(80);

void handleRootRequest() {
  String response = "<html><body>";
  response += "<h1>Laser Gun Game</h1>";
  response += "<p>Score: " + String(score) + "</p>";
  response += "<p>Hits: " + String(hits) + "</p>";
  response += "<p>Misses: " + String(misses) + "</p>";
  response += "</body></html>";
  server.send(200, "text/html", response);
}

void handleHitRequest() {
  hits++;
  score++;
  server.send(200, "text/plain", "Hit recorded!");
}

void handleMissRequest() {
  misses++;
  score--;
  server.send(200, "text/plain", "Miss recorded!");
}

void setup() {
  pinMode(targetPin, INPUT);
  pinMode(laserPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRootRequest);
  server.on("/hit", handleHitRequest);
  server.on("/miss", handleMissRequest);

  server.begin();
}

void loop() {
  int targetState = digitalRead(targetPin);

  if (targetState == HIGH) {
    digitalWrite(laserPin, HIGH);  // Activate laser module
  } else {
    digitalWrite(laserPin, LOW);   // Deactivate laser module
  }

  server.handleClient();
  delay(100);  // Adjust the delay as per your game requirements
}