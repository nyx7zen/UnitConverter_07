#pragma once
#include "../entity/ILengthConverter.h"
#include <string>

namespace boundary {

// Parses "unit:value", validates, converts, formats result.
// Uses ILengthConverter so tests can inject a mock.
class ConversionService {
public:
    explicit ConversionService(entity::ILengthConverter& converter);

    // Returns "2.5 meter = 8.202100 feet\n2.5 meter = 2.734025 yard"
    std::string parseAndConvert(const std::string& input) const;

    // Returns JSON with input + all unit conversions
    std::string parseAndConvertJson(const std::string& input) const;

private:
    entity::ILengthConverter& converter_;
};

} // namespace boundary
