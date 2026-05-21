#pragma once
#include "UnitRegistry.h"
#include <map>
#include <string>

namespace entity {

// Domain converter. Uses UnitRegistry for meter-hub conversion.
// Boundary layer injects via ILengthConverter interface.
class LengthConverter {
public:
    explicit LengthConverter(const UnitRegistry& registry);

    // throws std::invalid_argument: unknown unit, negative value
    double convert(const std::string& from, double value,
                   const std::string& to) const;

    // Returns all registered units for a given input
    std::map<std::string, double> convertAll(const std::string& from,
                                              double value) const;

private:
    const UnitRegistry& registry_;
};

} // namespace entity
