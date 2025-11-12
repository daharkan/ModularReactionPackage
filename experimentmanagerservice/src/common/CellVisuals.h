#ifndef CELLVISUALS_H
#define CELLVISUALS_H
#include "rapidjson/document.h"

using namespace rapidjson;


class CellVisuals
{
public:
    CellVisuals();

    CellVisuals(int rpm, float temperature, float stirrerAmp, unsigned long timestamp)
        : m_rpm(rpm), m_temperature(temperature), m_stirrerAmp(stirrerAmp), m_timestamp(timestamp) {}

    // Copy constructor
    CellVisuals(const CellVisuals& other)
        : m_rpm(other.m_rpm), m_temperature(other.m_temperature), m_stirrerAmp(other.m_stirrerAmp), m_timestamp(other.m_timestamp) {}

    // Assignment operator
    CellVisuals& operator=(const CellVisuals& other) {
        if (this != &other) {
            m_rpm = other.m_rpm;
            m_temperature = other.m_temperature;
            m_stirrerAmp = other.m_stirrerAmp;
            m_timestamp = other.m_timestamp;
        }
        return *this;
    }

    Value toJSON(Document::AllocatorType& allocator) const;
    void fromJSON(const Value& json);

    int rpm() const;
    void setRpm(int newRpm);

    float temperature() const;
    void setTemperature(float newTemperature);

    float stirrerAmp() const;
    void setStirrerAmp(float newStirrerAmp);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long newTimestamp);

private:
    int m_rpm;
    float m_temperature;
    float m_stirrerAmp;
    unsigned long m_timestamp;
};

#endif // CELLVISUALS_H
