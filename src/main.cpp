#include <Arduino.h>
#include <string>
#include <Metro.h>

//#define DEBUG

#define LED_PIN   13

#define COMPUTER  Serial
#define DUT       Serial7

//Metro dutTimeout(100);
//Metro compTimeout(100);
Metro ledTimer(250);


bool ledEnable = false;
bool ledStatus = true;

#if defined(DEBUG)
void debugPrintout();
Metro debugTimer(100);
#endif

void setup() {
  COMPUTER.begin(115200);
  DUT.begin(115200);

  delay(200);
  COMPUTER.println(" --- DEBUG START --- ");

  pinMode(LED_PIN, OUTPUT);
  delay(200);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  //COMPUTER.println("IN MAIN LOOP.");
  // Transfer DUT msg to COMPUTER
  if(DUT.available() > 0){
    // enable LED Blink
    ledEnable = true;

    // read the string
    char msg = DUT.read();

    // pass on the msg
    COMPUTER.print((char)F(msg));
  }

  if(COMPUTER.available() > 0){
    // enable LED Blink
    ledEnable = true;

    // read the string
    char msg = COMPUTER.read();

    // pass on the msg
    DUT.print((char)F(msg));
  }

  ledStatus = true;
  if(ledTimer.check() && ledEnable){
    //COMPUTER.println("BLINKING LED");
    ledStatus = digitalRead(LED_PIN);
    digitalWrite(LED_PIN, !ledStatus);
    ledEnable = false;
  }

  ledEnable = ledEnable || !ledStatus;

  #if defined(DEBUG)
  if(debugTimer.check()) debugPrintout();
  #endif
}

#if defined(DEBUG)
void debugPrintout() {
  COMPUTER.println("\n\n=== DEBUG ===\n");
  COMPUTER.print("LED Blink Enable: ");
  COMPUTER.println(ledEnable);
  COMPUTER.print("LED STATUS: ");
  COMPUTER.println(ledStatus);
  COMPUTER.println("\n === END DEBUG === \n");
}
#endif