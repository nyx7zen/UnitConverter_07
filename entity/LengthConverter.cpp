#include "LengthConverter.h"
#include <stdexcept>

namespace entity {

LengthConverter::LengthConverter(const UnitRegistry& registry)
    : registry_(registry) {}

double LengthConverter::convert(const std::string& from, double value,
                                const std::string& to) const {
    if (value < 0.0)
        throw std::invalid_argument("negative value not allowed");
    double metersPerFrom = registry_.getRatioToMeter(from);
    double metersPerTo   = registry_.getRatioToMeter(to);
    return (value * metersPerFrom) / metersPerTo;
}

std::map<std::string, double> LengthConverter::convertAll(const std::string& from,
                                                           double value) const {
    double metersPerFrom = registry_.getRatioToMeter(from);
    double meters = value * metersPerFrom;
    std::map<std::string, double> result;
    for (const auto& unit : registry_.getAllUnits())
        result[unit] = meters / registry_.getRatioToMeter(unit);
    return result;
}

} // namespace entity
