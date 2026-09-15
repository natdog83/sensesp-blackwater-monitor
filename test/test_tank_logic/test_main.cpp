#include <array>

#include <unity.h>

#include "tank_logic.h"

using blackwater::calculateTankReading;

void test_empty_tank() {
  constexpr std::array<bool, 5> wet = {false, false, false, false, false};
  const auto reading = calculateTankReading(wet);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.0F, reading.level);
  TEST_ASSERT_TRUE(reading.pattern_valid);
}

void test_each_contiguous_level() {
  for (std::size_t count = 1; count <= 5; ++count) {
    std::array<bool, 5> wet{};
    for (std::size_t i = 0; i < count; ++i) {
      wet[i] = true;
    }
    const auto reading = calculateTankReading(wet);
    TEST_ASSERT_FLOAT_WITHIN(0.001F, count * 0.2F, reading.level);
    TEST_ASSERT_TRUE(reading.pattern_valid);
  }
}

void test_impossible_pattern_is_flagged_and_conservative() {
  constexpr std::array<bool, 5> wet = {true, false, false, true, false};
  const auto reading = calculateTankReading(wet);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 0.8F, reading.level);
  TEST_ASSERT_FALSE(reading.pattern_valid);
}

int main(int, char**) {
  UNITY_BEGIN();
  RUN_TEST(test_empty_tank);
  RUN_TEST(test_each_contiguous_level);
  RUN_TEST(test_impossible_pattern_is_flagged_and_conservative);
  return UNITY_END();
}
