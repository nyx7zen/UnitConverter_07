#include "JsonUnitDefinitionReader.h"

namespace data {

void loadConfig(const std::string& /*path*/, entity::UnitRegistry& registry) {
    registry.seedDefaults();
}

} // namespace data
