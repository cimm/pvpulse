// Turns on LED and pushes an MQTT message when interruptPin
// changes voltage. This is the start of an IoT pulse meter for
// my photovoltaic system.
// Based on https://archive.is/XO36j

#include "publisher.h"

const byte interruptPin = 2;
const byte ledPin = 9; // LED_BUILTIN pin on Arduino UNO, pin 9 on Arduino Ethernet
Publisher pub;
bool connected = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP); // https://archive.is/qp8g1
  attachInterrupt(digitalPinToInterrupt(interruptPin), pulseDetected, RISING);
}

void loop() {
  if (!connected) {
    error("No internet connection");
    delay(10000); // retry evey 10 sec untill connected
    connected = pub.connect();
  }
}

void pulseDetected() {
  digitalWrite(ledPin, digitalRead(interruptPin));
  if (!pub.publish("pvpulse", "1")) {
    error("Message not sent");
  };
  delay(500); // prevent false hits
}

void error(String message) {
  Serial.print("! ");
  Serial.println(message);
}
