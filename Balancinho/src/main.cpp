#include <Arduino.h>
#include <BalancinhoAux.h>

namespace {

ServoSweep servo({D2, 0, 50, 14, 1493, 11, 15, 12});
DistanceSensor sensor({220, 500, 4, 4, 60, 50000, 50});

}

void setup() {
  Serial.begin(115200);
  delay(1000);

  servo.begin();

  if (!sensor.begin()) {
    Serial.println("Sensor nao encontrado!");
    while (true) {
      delay(100);
    }
  }
}

void loop() {
  servo.update(millis());

  DistanceMeasurement measurement{};
  if (sensor.update(measurement)) {
    Serial.print("Distancia:");
    Serial.print(measurement.distance);
    Serial.print("\tPosicao:");
    Serial.print(measurement.position);
    Serial.print("\tPulso:");
    Serial.println(servo.currentPulse());
  }
}