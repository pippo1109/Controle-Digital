#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <VL53L1X.h>

struct ServoSweepConfig {
  int pin;
  int channel;
  int frequency;
  int resolution;
  int centerPulse;
  int microsecondsPerDegree;
  int limitDegrees;
  unsigned long stepInterval;
};

class ServoSweep {
 public:
  explicit ServoSweep(const ServoSweepConfig& config);

  void begin();
  void update(unsigned long now);
  int currentPulse() const;

 private:
  void writePulse(int pulse);
  void selectNextTarget();

  ServoSweepConfig config_;
  int currentPulse_;
  int targetPulse_;
  int state_;
  unsigned long lastStep_;
};

struct DistanceMeasurement {
  uint16_t distance;
  int position;
};

struct DistanceSensorConfig {
  int centerPosition;
  uint16_t timeout;
  uint8_t roiWidth;
  uint8_t roiHeight;
  uint8_t roiCenter;
  uint32_t timingBudget;
  uint32_t continuousPeriod;
};

class DistanceSensor {
 public:
  explicit DistanceSensor(const DistanceSensorConfig& config);

  bool begin();
  bool update(DistanceMeasurement& measurement);

 private:
  DistanceSensorConfig config_;
  VL53L1X sensor_;
};