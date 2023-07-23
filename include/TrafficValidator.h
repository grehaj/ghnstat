#pragma once

#include "TrafficData.h"
#include "Alarm.h"

#include <memory>
#include <string>
#include <vector>

namespace scan_detector
{

namespace validation
{

constexpr counter_t max_port_threshold{100};
constexpr counter_t distributed_port_scan_threshold{5};

class Validator
{
public:
    virtual TrafficStatus validate(const PortTraffic& td) const = 0;
    virtual void add_validation(std::unique_ptr<Validator>);
};

class ScanValidator : public Validator
{
public:
    TrafficStatus validate(const PortTraffic& td) const override;
};

class TrafficValidator : public Validator
{
private:
    std::vector<std::unique_ptr<Validator>> validators;
    mutable Alarm alarm;

public:
    TrafficStatus validate(const PortTraffic& td) const override;
    void add_validation(std::unique_ptr<Validator>) override;
};

std::unique_ptr<Validator> create_traffic_validaor();
}
}
