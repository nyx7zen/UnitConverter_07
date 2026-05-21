#pragma once
#include <map>
#include <string>

namespace entity {

class ILengthConverter {
public:
    virtual ~ILengthConverter() = default;
    virtual double convert(const std::string& from, double value,
                           const std::string& to) const = 0;
    virtual std::map<std::string, double> convertAll(const std::string& from,
                                                      double value) const = 0;
};

} // namespace entity
