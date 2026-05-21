#include "entity/UnitRegistry.h"
#include "entity/LengthConverter.h"
#include "boundary/ConversionService.h"
#include "data/JsonUnitDefinitionReader.h"
#include <iostream>

int main() {
    entity::UnitRegistry reg;
    reg.seedDefaults();
    entity::LengthConverter conv(reg);
    boundary::ConversionService svc(conv);

    std::cout << "Insert value for converting (ex: meter:2.5): ";
    std::string input;
    std::getline(std::cin, input);

    try {
        data::loadConfig("config/units.json", reg);
        std::cout << svc.parseAndConvert(input) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
