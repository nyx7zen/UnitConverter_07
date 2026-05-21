#include "ConversionService.h"

namespace boundary {

ConversionService::ConversionService(entity::ILengthConverter& converter)
    : converter_(converter) {}

std::string ConversionService::parseAndConvert(const std::string& /*input*/) const {
    return "";
}

std::string ConversionService::parseAndConvertJson(const std::string& /*input*/) const {
    return "";
}

} // namespace boundary
