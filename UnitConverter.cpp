#include "entity/UnitRegistry.h"
#include "entity/LengthConverter.h"
#include "boundary/ConversionService.h"
#include "boundary/CliApp.h"
#include "data/JsonUnitDefinitionReader.h"
#include <iostream>

static constexpr const char* kDefaultConfigPath = "config/units.json";

int main() {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    try {
        data::loadConfig(kDefaultConfigPath, reg);
    } catch (const std::exception& e) {
        std::cerr << "Config error: " << e.what() << '\n';
        return 1;
    }

    entity::LengthConverter conv(reg);
    boundary::ConversionService svc(conv);
    return boundary::CliApp(svc).run();
}
