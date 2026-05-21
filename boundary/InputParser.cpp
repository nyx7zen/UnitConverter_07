#include "InputParser.h"
#include <stdexcept>
#include <string>

namespace boundary {

ParseResult parse(const std::string& input) {
    auto colonPos = input.find(':');
    if (colonPos == std::string::npos) {
        throw std::invalid_argument("invalid format: missing ':'");
    }
    std::string unit = input.substr(0, colonPos);
    std::string valueStr = input.substr(colonPos + 1);
    double value = 0.0;
    try {
        value = std::stod(valueStr);
    } catch (const std::exception&) {
        throw std::invalid_argument("invalid number: '" + valueStr + "'");
    }
    if (value < 0.0) {
        throw std::invalid_argument("negative value not allowed");
    }
    return {unit, value};
}

} // namespace boundary
