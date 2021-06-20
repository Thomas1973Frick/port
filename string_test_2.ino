int buttonPortPin = 8;                     // sätter gränslägesbrytare på port till pin nr
int buttonPort;                            // definierar gränsläge port
int dt(1000);                              // definierar väntetid
String port;                               // definierar en variabel port

void setup() {
  pinMode(buttonPortPin, INPUT);           // definierar buttonClosedPin som ingång

  Serial.begin(9600);                      // initierar serial kommunikation
}


void loop() {
  buttonPort = digitalRead(buttonPortPin); // läser gränslägesbrytare på port
  if (buttonPort == HIGH) {                // om port är stängd
    port = "stängd";                       // sätt variabel port till stängd
  }
  if (buttonPort == LOW) {                 // om port är öppen
    port = "öppen";                        // sätt variabel port till öppen
  }
  Serial.print("porten är ");
  Serial.println(port);                    // skriv ut variabel port
  delay(dt);                               // väntetid

}
