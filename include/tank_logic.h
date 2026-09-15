#pragma once

#include <array>
#include <cstddef>

#include "tank_config.h"

namespace blackwater {

struct TankReading {
  float level;
  bool pattern_valid;
};

// A physically plausible pattern is wet from the bottom up, followed only by
// dry sensors. For safety, an isolated upper wet sensor still raises the level
// to that sensor's threshold while pattern_valid is set false.
inline TankReading calculateTankReading(
    const std::array<bool, kSensorCount>& wet) {
  float level = 0.0F;
  bool dry_seen = false;
  bool pattern_valid = true;

  for (std::size_t i = 0; i < kSensorCount; ++i) {
    if (wet[i]) {
      level = kLevelAtSensor[i];
      if (dry_seen) {
        pattern_valid = false;
      }
    } else {
      dry_seen = true;
    }
  }

  return {level, pattern_valid};
}

}  // namespace blackwater
