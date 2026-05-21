#include "CliApp.h"
#include <iostream>
#include <string>

namespace boundary {

CliApp::CliApp(ConversionService& svc) : svc_(svc) {}

int CliApp::run() {
    std::cout << "Insert value for converting (ex: meter:2.5): ";
    std::string input;
    if (!std::getline(std::cin, input)) return 0;
    try {
        std::cout << svc_.parseAndConvert(input) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

} // namespace boundary
