#include "ConversionService.h"
#include "InputParser.h"

namespace boundary {

ConversionService::ConversionService(entity::ILengthConverter& converter)
    : converter_(converter) {}

std::string ConversionService::parseAndConvert(const std::string& input) const {
    parse(input);
    return "";
}

std::string ConversionService::parseAndConvertJson(const std::string& /*input*/) const {
    return "";
}

} // namespace boundary
