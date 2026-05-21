#include "JsonUnitDefinitionReader.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace data {

void loadConfig(const std::string& path, entity::UnitRegistry& registry) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return;  // silent fallback — keep seedDefaults()
    }

    nlohmann::json j;
    try {
        file >> j;
    } catch (const std::exception&) {
        return;  // malformed JSON — silent fallback
    }

    if (!j.contains("units") || !j["units"].is_object()) {
        return;  // empty or missing "units" key — silent fallback
    }

    for (const auto& [name, ratio] : j["units"].items()) {
        registry.registerUnit(name, ratio.get<double>());
    }
}

} // namespace data
