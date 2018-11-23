#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[6] = "11111";
const int motor1 = 30;
const int motor2 = 31;
int order = 0;

void setup() {
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, addresses);
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  radio.startListening();
  
  while (!radio.available());
  radio.read(&order, sizeof(order));
  Serial.print(order);
  switch (order) {
  case 0:
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
    break;
  case 1:
    digitalWrite(motor1, HIGH);
    digitalWrite(motor2, HIGH);
    break;
  case 2:
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, HIGH);
    break;
  case 3:
    digitalWrite(motor1, HIGH);
    digitalWrite(motor2, LOW);
    break;
  }
}
