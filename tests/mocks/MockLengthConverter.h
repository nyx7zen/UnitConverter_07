#pragma once
#include "../../entity/ILengthConverter.h"
#include <map>
#include <stdexcept>
#include <string>

namespace mocks {

// Fixed-snapshot mock — for boundary contract tests only.
// Does NOT implement real conversion logic.
class MockLengthConverter : public entity::ILengthConverter {
public:
    double convert(const std::string& from, double value,
                   const std::string& to) const override {
        if (value < 0.0)
            throw std::invalid_argument("negative value not allowed");
        if (from != "meter" && from != "feet" && from != "yard")
            throw std::invalid_argument("unknown unit: " + from);
        if (to != "meter" && to != "feet" && to != "yard")
            throw std::invalid_argument("unknown unit: " + to);

        // Snapshots keyed on (from, to) — meter hub calculation
        if (from == "meter" && to == "feet")  return value * 3.28084;
        if (from == "meter" && to == "yard")  return value * 1.09361;
        if (from == "meter" && to == "meter") return value;
        if (from == "feet"  && to == "meter") return value / 3.28084;
        if (from == "yard"  && to == "meter") return value / 1.09361;
        if (from == "feet"  && to == "yard")  return (value / 3.28084) * 1.09361;
        if (from == "yard"  && to == "feet")  return (value / 1.09361) * 3.28084;
        return 0.0;
    }

    std::map<std::string, double> convertAll(const std::string& from,
                                              double value) const override {
        double meters = convert(from, value, "meter");
        return {
            {"meter", meters},
            {"feet",  meters * 3.28084},
            {"yard",  meters * 1.09361},
        };
    }
};

} // namespace mocks
