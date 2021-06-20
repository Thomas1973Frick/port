#include <SPI.h>
#include <WiFiNINA.h>
#include "CytronMotorDriver.h"

IPAddress ip(192, 168, 1, 254);           // bestämmer fast IP nummer
char ssid[] = "R16";                      // your network SSID (name)
char pass[] = "sommaramiga500";           // your network password

int buttonPortPin = 8;                  // sätter gränslägen port stängd till pin nr
int buttonPort;                           // definierar gränsläge port stängd
int buttonLockPin = 9;                  // sätter gränslägen port låst till pin nr
int buttonLock;                           // definierar gränslägen port låst

String port;
String lock;

int runTime = 5000;                       // körtid för motor
int dt(1000);

CytronMD motor(PWM_DIR, 3, 4);            // motordrivare PWM = Pin 3, DIR = Pin 4.

int status = WL_IDLE_STATUS;
WiFiServer server(80);

String readString;

// The setup routine runs once when you press reset.
void setup() {

  pinMode(buttonPortPin, INPUT);        // definierar buttonClosedPin som ingång
  pinMode(buttonLockPin, INPUT);        // definierar buttonLockedPin som ingång

  Serial.begin(9600);                     // initierar serial kommunikation
  WiFi.config(ip);                        // Initierar wifi konfiguration

  while (status != WL_CONNECTED) {        // kopplar upp mot wifi
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                 // print the network name (SSID);
    status = WiFi.begin(ssid, pass);      // kopplar upp mot wifi
    delay(10000);                         // wait 10 seconds for connection:
  }
  server.begin();                         // start the web server on port 80

  Serial.print("SSID: ");                 // skriver ut nätverksnamn
  Serial.println(WiFi.SSID());            // skriver ut nätverksnamn
  IPAddress ip = WiFi.localIP();          //
  Serial.print("IP Address: ");           // skriver ut ip nummer
  Serial.println(ip);                     // skriver ut ip nummer
}

// The loop routine runs over and over again forever.
void loop() {
  buttonPort = digitalRead(buttonPortPin); // läser gränslägesbrytare på port
  if (buttonPort == HIGH) {                // om port är stängd
    port = "stängd";                       // sätt variabel port till stängd
  }
  if (buttonPort == LOW) {                 // om port är öppen
    port = "öppen";                        // sätt variabel port till öppen
  }

  buttonLock = digitalRead(buttonLockPin); // läser gränslägesbrytare på port
  if (buttonLock == HIGH) {                // om port är stängd
    lock = "låst";                       // sätt variabel port till stängd
  }
  if (buttonLock == LOW) {                 // om port är öppen
    lock = "olåst";                        // sätt variabel port till öppen
  }

  Serial.print("porten är ");
  Serial.println(port);                    // skriv ut variabel port
  Serial.print(" och ");
  Serial.println(lock);                    // skriv ut variabel port
  delay(dt);                               // väntetid


  WiFiClient client = server.available();           // listen for incoming clients
  if (client)                                       // if you get a client,
  {
    Serial.println("new client");                   // print a message out the serial port

    while (client.connected())                      // loop while the client's connected
    {
      if (client.available())                       // if there's bytes to read from the client,
      {
        char c = client.read();                     // read a byte, then
        if (readString.length() < 100)
        {
          readString += c;
          Serial.write(c);                          // print it out the serial monitor

          if (c == '\n') {                          // if the byte is a newline character

            // start av webbsida
            client.println("<head><meta http-equiv=\"refresh\" content=\"5\"></head>"); //5 sek refresh
            //client.println("<head></head>");
            client.println("<center>");
            client.print("<h1 style=\"font-size:100px;\">");
            client.println("<a href=\"/?lock\"\">L&arings</a>");
            client.println("<br />");
            client.println("<a href=\"/?unlock\"\">L&arings upp</a><br />");
            client.println("<br />");
            client.print("porten är ");
            client.println(port);
            client.println("<br />");
            client.print("och ");
            client.println(lock);
            client.println("</center>");
            client.println("</h1>");
            // slut på webbsida

            delay(1);

            if (readString.indexOf("?lock") > 0 && port == stängd && lock == olåst)        // väntar på lock från webbsida
            {
              motor.setSpeed(255);                      // motor kör fram (låser)
              delay(runTime);
              motor.setSpeed(0);                        // Stannar motor.
            }
            else {
              if (readString.indexOf("?unlock") > 0 && lock == låst)    // väntar på unlock från webbsida
              {
                motor.setSpeed(-255);                   // motor kör bak (låser upp)
                delay(runTime);
                motor.setSpeed(0);                      // Stannar motor.
              }
            }
            readString = "";

            delay(1);
            client.stop();
            Serial.println("client disonnected");
          }
        }
      }
    }
  }
}
