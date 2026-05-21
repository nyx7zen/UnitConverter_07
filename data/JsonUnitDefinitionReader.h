#pragma once
#include "../entity/UnitRegistry.h"
#include <string>

namespace data {

// Loads {"units": {"feet": 3.28084, ...}} into registry.
// Falls back to seedDefaults() if file not found.
// throws std::invalid_argument if a ratio is <= 0.
void loadConfig(const std::string& path, entity::UnitRegistry& registry);

} // namespace data
