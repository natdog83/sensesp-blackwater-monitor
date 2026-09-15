#pragma once

#include <array>
#include <cstdint>

namespace blackwater {

constexpr std::size_t kSensorCount = 5;

// Sensors are ordered from lowest to highest on the tank.
// These GPIOs avoid the ESP32 boot-strapping pins and SPI flash pins.
constexpr std::array<std::uint8_t, kSensorCount> kSensorPins = {
    32, 33, 25, 26, 27};

// Fraction-full reported when each sensor is the highest wet sensor.
// Adjust these values if the tank geometry is not uniform by height.
constexpr std::array<float, kSensorCount> kLevelAtSensor = {
    0.20F, 0.40F, 0.60F, 0.80F, 1.00F};

constexpr const char* kHostname = "blackwater-monitor";
constexpr const char* kLevelPath = "tanks.blackWater.0.currentLevel";
constexpr const char* kPatternValidPath =
    "tanks.blackWater.0.sensorPatternValid";

constexpr std::array<const char*, kSensorCount> kSensorPaths = {
    "tanks.blackWater.0.sensors.level20",
    "tanks.blackWater.0.sensors.level40",
    "tanks.blackWater.0.sensors.level60",
    "tanks.blackWater.0.sensors.level80",
    "tanks.blackWater.0.sensors.level100"};

constexpr unsigned int kReadIntervalMs = 250;
constexpr unsigned int kDebounceMs = 1500;
constexpr unsigned int kPublishIntervalMs = 1000;

}  // namespace blackwater
