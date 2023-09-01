#include <WiFiNINA.h> // provides necessary functions to connect to Wi-Fi network

#include <BH1750.h> // used to interface with the BH1750 light sensor
#include <Wire.h> // used for I2C communication

// defining Wi-Fi credentials
char ssid[] = "DUA";
char pass[] = "06122003";

WiFiClient client; // object used to create a client to communicate 
BH1750 lightMeter; // object used to interact with BH1750 sensor

char   HOST_NAME[] = "maker.ifttt.com"; // hostname of the IFTTT server
String PATH_NAME  = "/trigger/Light Sensed/with/key/foQ-XYu5torLNTRaXDz6lQ3FFOSyfM6oNsOpFLBzTlZ"; // path to the IFTTT webhook URL
String queryString = "?value1=57&value2=25"; // additional query data to be sent to the IFTTT webhook

void setup() {

  Serial.begin(9600);
  Serial.print("START");
  while (!Serial);
     WiFi.begin(ssid, pass);
     Wire.begin();
  while(true)
  {
  if (client.connect(HOST_NAME, 80)) {
    Serial.println("Connected to the server.");
    break;
  }
  else {
    Serial.println("Connection failed.");
  }
  delay(500);
}
lightMeter.begin();
Serial.println("Connected to server");
}
void loop() {
  Serial.print("START");
  float intensity = lightMeter.readLightLevel(); // reads the intensity of light
  queryString +="?value1=" ;
  queryString += intensity; // updates the data of the intensity of light
  Serial.println(queryString);

  if (intensity > 500) {
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1"); // sends an HTTP GET request to the IFTTT webhook
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();


    while (client.connected()) { // continuously reads the response
      if (client.available()) {
        char response = client.read();
        Serial.print(response);
      }
    }
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  }
  queryString = "";
  delay(3000);
}