#include <SoftwareSerial.h>
#include "HUSKYLENS.h"

// Download HUSKYLENS library from https://github.com/HuskyLens/HUSKYLENSArduino/tree/master/HUSKYLENS
// and put it into your Arduino/libraries folder

SoftwareSerial huskySerial(2, 3);
HUSKYLENS huskylens;

void setup() {
  Serial.begin(9600);
  huskySerial.begin(9600);

  while (!huskylens.begin(huskySerial)) {
    Serial.println("HuskyLens not found, retrying...");
    delay(1000);
  }
  Serial.println("HuskyLens connected!");
}

void loop() {
  if (!huskylens.request()) {
    Serial.println("No data from HuskyLens");
    return;
  }

  if (!huskylens.isLearned()) {
    Serial.println("No colors learned yet");
    return;
  }

  if (!huskylens.available()) {
    Serial.println("No color detected");
    return;
  }

  while (huskylens.available()) {
    HUSKYLENSResult result = huskylens.read();


  if (result.ID == 1) Serial.print("Object1:");
  if (result.ID == 2) Serial.print("Object2:");
  if (result.ID == 1 || result.ID == 2) Serial.println(result.xCenter);


    continue;

    Serial.print("Color ID: ");
    Serial.print(result.ID);
    Serial.print("  X: ");
    Serial.print(result.xCenter);
    Serial.print("  Y: ");
    Serial.print(result.yCenter);
    Serial.print("  Width: ");
    Serial.print(result.width);
    Serial.print("  Height: ");
    Serial.println(result.height);
  }

  delay(100);
}
