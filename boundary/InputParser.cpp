#include "InputParser.h"
#include <stdexcept>
#include <string>

namespace boundary {

ParseResult parse(const std::string& input) {
    if (input.find(':') == std::string::npos) {
        throw std::invalid_argument("invalid format: missing ':'");
    }
    return {"", 0.0};
}

} // namespace boundary
