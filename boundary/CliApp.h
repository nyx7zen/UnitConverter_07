#pragma once
#include "ConversionService.h"

namespace boundary {

class CliApp {
public:
    explicit CliApp(ConversionService& svc);
    int run();

private:
    ConversionService& svc_;
};

} // namespace boundary
