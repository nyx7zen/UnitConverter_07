#include "ConversionService.h"
#include "InputParser.h"
#include <iomanip>
#include <sstream>

namespace boundary {

ConversionService::ConversionService(entity::ILengthConverter& converter)
    : converter_(converter) {}

std::string ConversionService::parseAndConvert(const std::string& input) const {
    auto parsed = parse(input);
    auto conversions = converter_.convertAll(parsed.unit, parsed.value);

    std::ostringstream oss;
    bool first = true;
    for (const auto& [toUnit, toValue] : conversions) {
        if (toUnit == parsed.unit) continue;
        if (!first) oss << '\n';
        oss << parsed.value << ' ' << parsed.unit << " = "
            << std::fixed << std::setprecision(6) << toValue << ' ' << toUnit;
        first = false;
    }
    return oss.str();
}

std::string ConversionService::parseAndConvertJson(const std::string& /*input*/) const {
    return "";
}

} // namespace boundary
