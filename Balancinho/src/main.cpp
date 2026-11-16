#include <Arduino.h>
#include <BalancinhoAux.h>

namespace {

ServoSweep servo({D2, 0, 50, 14, 1493, 11, 15, 12});
DistanceSensor sensor({216, 500, 4, 4, 60, 33000, 33});

}

float filterPosition(float input) {
  constexpr size_t taps = 5;
  constexpr float coefficient = 1.0f / taps;

  static float history[taps] = {};
  static bool initialized = false;

  // Evita que o filtro comece com zeros e gere um pico inicial.
  if (!initialized) {
    for (size_t i = 0; i < taps; ++i) {
      history[i] = input;
    }
    initialized = true;
  }

  for (size_t i = taps - 1; i > 0; --i) {
    history[i] = history[i - 1];
  }
  history[0] = input;

  float output = 0.0f;
  for (size_t i = 0; i < taps; ++i) {
    output += coefficient * history[i];
  }

  return output;
}

void setup() {
  Serial.begin(115200);

  servo.begin();

  if (!sensor.begin()) {
    Serial.println("Sensor nao encontrado!");
  }
}

void loop() {

  DistanceMeasurement measurement{};
  if (sensor.update(measurement)) {
    Serial.println(filterPosition(measurement.position) / 1.043f);
  }
}