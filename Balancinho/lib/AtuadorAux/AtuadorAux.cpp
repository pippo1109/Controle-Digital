#include "AtuadorAux.h"
#include <Wire.h>

// ===============================================================
// Servo
// ===============================================================
static int sv_channel;
static int sv_frequency;
static int sv_resolution;
static float sv_centerPulse;
static float sv_usPerDegree;
static float sv_limitDegrees;
static float sv_currentPulse;

void servoBegin(int pin, int channel, int frequency, int resolution,
                float centerPulse, float microsecondsPerDegree,
                float limitDegrees) {
  sv_channel = channel;
  sv_frequency = frequency;
  sv_resolution = resolution;
  sv_centerPulse = centerPulse;
  sv_usPerDegree = microsecondsPerDegree;
  sv_limitDegrees = limitDegrees;
  sv_currentPulse = centerPulse;

  ledcSetup(channel, frequency, resolution);
  ledcAttachPin(pin, channel);
  servoWrite(0.0f);
}

float servoWrite(float degrees) {
  degrees = constrain(degrees, -sv_limitDegrees, sv_limitDegrees);
  sv_currentPulse = sv_centerPulse + degrees * sv_usPerDegree;

  const float periodUs = 1000000.0f / sv_frequency;
  const uint32_t maxDuty = (1UL << sv_resolution) - 1;
  const uint32_t duty = lroundf(sv_currentPulse / periodUs * maxDuty);
  ledcWrite(sv_channel, duty);
  return degrees;
}

