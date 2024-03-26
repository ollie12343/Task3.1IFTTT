#include <WiFiNINA.h>
#include "secrets.h"
#include <BH1750FVI.h>


//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";

String PATH_NAME   = "/trigger/sunlight/with/key/caH-RfyisLUV2vGsfTJfLw"; // change your EVENT-NAME and YOUR-KEY
String queryString = "?";

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

bool flag = false;

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);
  LightSensor.begin();

  Serial.begin(9600);
  while (!Serial);
}


void connect()
{
    // connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
  }
  else {// if not connected:
    Serial.println("connection failed");
  }
}

void sendEmail(String value)
{

  connect();

  // make a HTTP request:
    // send HTTP header
    client.println("GET " + PATH_NAME + queryString + value + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header


    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

        // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
}





void loop() {

  uint16_t lux = LightSensor.GetLightIntensity();

  if (flag && lux < 500 ) 
  {
    flag = false;
    sendEmail("value1=&value2=out of");
  }
  else if (!flag && lux > 500)
  {
      flag = true;
      sendEmail("in");
  }
}