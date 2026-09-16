#include "BalancinhoAux.h"

ServoSweep::ServoSweep(const ServoSweepConfig& config)
    : config_(config),
      currentPulse_(config.centerPulse),
      targetPulse_(config.centerPulse + config.limitDegrees * config.microsecondsPerDegree),
      state_(0),
      lastStep_(0) {}

void ServoSweep::begin() {
  ledcSetup(config_.channel, config_.frequency, config_.resolution);
  ledcAttachPin(config_.pin, config_.channel);
  writePulse(currentPulse_);
}

void ServoSweep::update(unsigned long now) {
  if (now - lastStep_ < config_.stepInterval) {
    return;
  }

  lastStep_ = now;

  if (currentPulse_ < targetPulse_) {
    currentPulse_ += config_.microsecondsPerDegree;
    if (currentPulse_ > targetPulse_) {
      currentPulse_ = targetPulse_;
    }
  } else if (currentPulse_ > targetPulse_) {
    currentPulse_ -= config_.microsecondsPerDegree;
    if (currentPulse_ < targetPulse_) {
      currentPulse_ = targetPulse_;
    }
  }

  writePulse(currentPulse_);

  if (currentPulse_ == targetPulse_) {
    selectNextTarget();
  }
}

int ServoSweep::currentPulse() const {
  return currentPulse_;
}

void ServoSweep::writePulse(int pulse) {
  const uint32_t maxDuty = (1UL << config_.resolution) - 1;
  const uint32_t duty = (static_cast<uint64_t>(pulse) * maxDuty) / 20000;
  ledcWrite(config_.channel, duty);
}

void ServoSweep::selectNextTarget() {
  state_ = (state_ + 1) % 4;

  if (state_ == 0) {
    targetPulse_ = config_.centerPulse + config_.limitDegrees * config_.microsecondsPerDegree;
  } else if (state_ == 1 || state_ == 3) {
    targetPulse_ = config_.centerPulse;
  } else {
    targetPulse_ = config_.centerPulse - config_.limitDegrees * config_.microsecondsPerDegree;
  }
}

DistanceSensor::DistanceSensor(const DistanceSensorConfig& config)
    : config_(config) {}

bool DistanceSensor::begin() {
  Wire.begin();
  sensor_.setTimeout(config_.timeout);

  if (!sensor_.init()) {
    return false;
  }

  sensor_.setDistanceMode(VL53L1X::Short);
  sensor_.setROISize(config_.roiWidth, config_.roiHeight);
  sensor_.setROICenter(config_.roiCenter);
  sensor_.setMeasurementTimingBudget(config_.timingBudget);
  sensor_.startContinuous(config_.continuousPeriod);
  return true;
}

bool DistanceSensor::update(DistanceMeasurement& measurement) {
  if (!sensor_.dataReady()) {
    return false;
  }

  measurement.distance = sensor_.read(false);
  measurement.position = static_cast<int>(measurement.distance) - config_.centerPosition;
  return true;
}