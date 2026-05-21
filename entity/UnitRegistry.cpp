#include "UnitRegistry.h"
#include <stdexcept>

namespace entity {

static constexpr double kFeetPerMeter = 3.28084;
static constexpr double kYardPerMeter = 1.09361;

void UnitRegistry::seedDefaults() {
    units_["meter"] = 1.0;
    units_["feet"]  = 1.0 / kFeetPerMeter;
    units_["yard"]  = 1.0 / kYardPerMeter;
}

void UnitRegistry::registerUnit(const std::string& name, double ratioToMeter) {
    if (ratioToMeter <= 0.0)
        throw std::invalid_argument("ratio must be positive");
    units_[name] = ratioToMeter;
}

double UnitRegistry::getRatioToMeter(const std::string& unit) const {
    auto it = units_.find(unit);
    if (it == units_.end())
        throw std::invalid_argument("unknown unit: " + unit);
    return it->second;
}

bool UnitRegistry::hasUnit(const std::string& unit) const {
    return units_.find(unit) != units_.end();
}

std::vector<std::string> UnitRegistry::getAllUnits() const {
    std::vector<std::string> result;
    result.reserve(units_.size());
    for (const auto& [name, ratio] : units_)
        result.push_back(name);
    return result;
}

} // namespace entity
