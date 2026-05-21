#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace entity {

class UnitRegistry {
public:
    void seedDefaults();
    void registerUnit(const std::string& name, double ratioToMeter);
    double getRatioToMeter(const std::string& unit) const;
    bool hasUnit(const std::string& unit) const;
    std::vector<std::string> getAllUnits() const;

private:
    std::unordered_map<std::string, double> units_;
};

} // namespace entity
