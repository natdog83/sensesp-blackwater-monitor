#include <Arduino.h>

#include <array>
#include <cstddef>

#include "tank_config.h"
#include "tank_logic.h"

namespace {

std::array<bool, blackwater::kSensorCount> stable_wet{};
std::array<bool, blackwater::kSensorCount> candidate_wet{};
std::array<unsigned long, blackwater::kSensorCount> candidate_since{};

void printReading() {
  const auto reading = blackwater::calculateTankReading(stable_wet);

  Serial.println();
  Serial.println("Sensor     GPIO     State");
  Serial.println("-------------------------");
  for (std::size_t i = 0; i < blackwater::kSensorCount; ++i) {
    Serial.printf("%3u%%       %2u       %s\n",
                  static_cast<unsigned int>((i + 1) * 20),
                  blackwater::kSensorPins[i],
                  stable_wet[i] ? "WET" : "DRY");
  }
  Serial.printf("Tank level: %.0f%%\n", reading.level * 100.0F);
  Serial.printf("Pattern:    %s\n",
                reading.pattern_valid ? "VALID" : "INVALID - check sensors");
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Black Water Tank Sensor Hardware Test");
  Serial.println("LOW at a GPIO means WET after the transistor interface.");
  Serial.println("For a dry-contact test, briefly jumper one GPIO to GND.");

  const unsigned long now = millis();
  for (std::size_t i = 0; i < blackwater::kSensorCount; ++i) {
    pinMode(blackwater::kSensorPins[i], INPUT_PULLUP);
    const bool wet = digitalRead(blackwater::kSensorPins[i]) == LOW;
    stable_wet[i] = wet;
    candidate_wet[i] = wet;
    candidate_since[i] = now;
  }
  printReading();
}

void loop() {
  bool changed = false;
  const unsigned long now = millis();

  for (std::size_t i = 0; i < blackwater::kSensorCount; ++i) {
    const bool wet = digitalRead(blackwater::kSensorPins[i]) == LOW;

    if (wet != candidate_wet[i]) {
      candidate_wet[i] = wet;
      candidate_since[i] = now;
    }

    if (candidate_wet[i] != stable_wet[i] &&
        now - candidate_since[i] >= blackwater::kDebounceMs) {
      stable_wet[i] = candidate_wet[i];
      changed = true;
    }
  }

  if (changed) {
    printReading();
  }
  delay(blackwater::kReadIntervalMs);
}
