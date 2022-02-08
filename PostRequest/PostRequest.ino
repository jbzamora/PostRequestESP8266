/*****************************************************************************

  This is an example for doing a simple Post Request in JSON format with
  a microcontroller [WiFi] ESP8266 of Espressif Systems.

  In this example we send a region or timezone similar to 'America/Guayaquil'
  for received a string on JSON format with the local time and other dates
  provided for a micro service of https://binaryec.com

  Developer: Jhonny Zamora A.
  https://github.com/jbzamora
  Email: jzamora@binaryec.com

*****************************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

//fingerprint of binaryec.com valid until 04/05/2022
const uint8_t fingerprint[20] = {0x9F, 0xEE, 0x15, 0xFD, 0x50, 0x46, 0xF0, 0x45, 0xBD, 0xCB, 0x5C, 0x60, 0xDE, 0x0F, 0xE3, 0x21, 0x0B, 0x8C, 0x8A, 0x76};

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASS";

String timezone = "America/Guayaquil";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting to WiFi");
  int cont = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(". ");
    cont++;
    if (cont >= 10) {
      ESP.restart();
    }
  }

  Serial.println();
  Serial.print("Conectado a: ");
  Serial.print(ssid);
  Serial.print(" Con IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  String json = "{\"timezone\":\"" + timezone + "\"}";

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setFingerprint(fingerprint);
  HTTPClient https;
  https.begin(*client, "https://binaryec.com/timezone/index.php");
  https.addHeader("Content-Type", "application/json");         //Specify content-type header

  int httpResponseCode = https.POST(json);   //Send actual POST request

  if (httpResponseCode > 0) {
    String response = https.getString();   //Get the response to the request
    Serial.print("Code: "); Serial.println(httpResponseCode);     //Print return code
    Serial.print("Server Response: "); Serial.println(response);  //Print request answer
  }
  else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  https.end();  //Free resources

  delay(5000);
}
