#include <array>
#include <cstddef>

#include "sensesp/sensors/digital_input.h"
#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_metadata.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/system/lambda_consumer.h"
#include "sensesp/transforms/debounce.h"
#include "sensesp/transforms/lambda_transform.h"
#include "sensesp_app_builder.h"
#include "tank_config.h"
#include "tank_logic.h"

using namespace sensesp;

namespace {

std::array<bool, blackwater::kSensorCount> sensor_wet{};

}  // namespace

void setup() {
  SetupLogging();

  SensESPAppBuilder builder;
  sensesp_app = builder.set_hostname(blackwater::kHostname)->get_app();

  for (std::size_t i = 0; i < blackwater::kSensorCount; ++i) {
    auto* raw_input = new DigitalInputState(
        blackwater::kSensorPins[i], INPUT_PULLUP,
        blackwater::kReadIntervalMs);

    // The documented transistor interface is active-low at the ESP32:
    // LOW = liquid present, HIGH = dry.
    auto* active_low_to_wet = new LambdaTransform<bool, bool>(
        [](bool raw_state) { return !raw_state; });
    auto* debounce = new DebounceBool(blackwater::kDebounceMs);

    raw_input->connect_to(active_low_to_wet)->connect_to(debounce);

    debounce->connect_to(new LambdaConsumer<bool>([i](bool wet) {
      sensor_wet[i] = wet;
    }));
    debounce->connect_to(
        new SKOutputBool(blackwater::kSensorPaths[i]));
  }

  auto* level = new RepeatSensor<float>(
      blackwater::kPublishIntervalMs,
      []() { return blackwater::calculateTankReading(sensor_wet).level; });
  level->connect_to(new SKOutputFloat(
      blackwater::kLevelPath, "",
      new SKMetadata("ratio", "Black water tank level")));

  auto* pattern_valid = new RepeatSensor<bool>(
      blackwater::kPublishIntervalMs, []() {
        return blackwater::calculateTankReading(sensor_wet).pattern_valid;
      });
  pattern_valid->connect_to(
      new SKOutputBool(blackwater::kPatternValidPath));
}

void loop() { event_loop()->tick(); }
