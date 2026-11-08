#include "SensorAux.h"
#include <Wire.h>
#include <VL53L1X.h>

// ===============================================================
// Sensor de distancia
// ===============================================================
static VL53L1X distSensor;
static int dist_centerPosition;

bool distanceBegin(int centerPosition, uint16_t timeout,
                   uint8_t roiWidth, uint8_t roiHeight, uint8_t roiCenter,
                   uint32_t timingBudget, uint32_t continuousPeriod) {
  dist_centerPosition = centerPosition;

  Wire.begin();
  Wire.setClock(400000);
  distSensor.setTimeout(timeout);

  if (!distSensor.init()) {
    return false;
  }

  distSensor.setDistanceMode(VL53L1X::Short);
  distSensor.setROISize(roiWidth, roiHeight);
  distSensor.setROICenter(roiCenter);
  distSensor.setMeasurementTimingBudget(timingBudget);
  distSensor.startContinuous(continuousPeriod);
  return true;
}

bool distanceUpdate(uint16_t& distance, int& position, bool& valid,
                    uint32_t& timestampUs) {
  if (!distSensor.dataReady()) {
    return false;
  }

  distSensor.read(false);
  timestampUs = micros();
  distance = distSensor.ranging_data.range_mm;
  valid = (distSensor.ranging_data.range_status == VL53L1X::RangeValid);
  position = static_cast<int>(distance) - dist_centerPosition;
  return true;
}