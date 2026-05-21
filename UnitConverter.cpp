#include "entity/UnitRegistry.h"
#include "entity/LengthConverter.h"
#include "boundary/ConversionService.h"
#include "boundary/CliApp.h"
#include "data/JsonUnitDefinitionReader.h"
#include <iostream>

int main() {
    entity::UnitRegistry reg;
    reg.seedDefaults();

    try {
        data::loadConfig("config/units.json", reg);
    } catch (const std::exception& e) {
        std::cerr << "Config error: " << e.what() << '\n';
        return 1;
    }

    entity::LengthConverter conv(reg);
    boundary::ConversionService svc(conv);
    return boundary::CliApp(svc).run();
}
