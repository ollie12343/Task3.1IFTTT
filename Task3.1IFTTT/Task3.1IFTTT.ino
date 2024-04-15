#include <WiFiNINA.h>
#include "secrets.h"
#include <BH1750FVI.h>


//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "/trigger/sunlight/with/key/caH-RfyisLUV2vGsfTJfLw"; // change your EVENT-NAME and YOUR-KEY
String queryString = "?value1=";

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);

bool flag = false;

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);
  LightSensor.begin();

  Serial.begin(9600);
  while (!Serial);
  while (WiFi.status() != WL_CONNECTED)
  { 
      // delay is first because we already call it above.
      delay(3000);
      WiFi.begin(ssid, pass);
      
  }

  Serial.println("Ready");
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
  delay(500);

  Serial.println("Sending Email...");

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
    Serial.print("\nTerrarium is now in "); Serial.println(value);
    
    Serial.println("disconnected\n\n");
}





void loop() {

  uint16_t lux = LightSensor.GetLightIntensity();
  //Serial.println(lux);

  if (flag && lux < 300 ) 
  {
    flag = false;
    sendEmail("shade");
  }
  else if (!flag && lux > 300)
  {
      flag = true;
      sendEmail("sunlight");
  }
  //delay(5000);
}