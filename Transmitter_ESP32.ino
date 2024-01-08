#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

int FlameSensor = 22;
int GasSensor = 34;
int TemperatureSensor = 21;

const char *ssid = "ESP32-Flame";
const char *password = "12345678";

WiFiServer server(80);
WiFiClient client;

// Data wire is connected to the Arduino digital pin 4
#define ONE_WIRE_BUS 21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(FlameSensor, INPUT);
  pinMode(TemperatureSensor, INPUT);

  // Connect to Wi-Fi
  WiFi.softAP(ssid, password);
  delay(100);

  server.begin();
}

void loop() {
  client = server.available();

  if (client) {
    while (client.connected()) {
      int flameValue = digitalRead(FlameSensor);
      sensors.requestTemperatures();
      float temperatureValue = sensors.getTempCByIndex(0);
      int gasValue = analogRead(GasSensor);

      // Send data to the receiver
      client.print(flameValue);
      client.print(",");
      client.print(temperatureValue);
      client.print(",");
      client.println(gasValue);
      delay(10);
      //delay(10); // small delay for stability
    }

    client.stop();
  }

  delay(1000);
}
