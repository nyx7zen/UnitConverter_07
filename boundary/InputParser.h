#pragma once
#include <string>

namespace boundary {

struct ParseResult {
    std::string unit;
    double value;
};

// parse("unit:value") → ParseResult
// throws std::invalid_argument on: missing ':', non-numeric value, negative value
ParseResult parse(const std::string& input);

} // namespace boundary
