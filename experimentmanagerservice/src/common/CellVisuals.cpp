#include "CellVisuals.h"

CellVisuals::CellVisuals() {}

int CellVisuals::rpm() const
{
    return m_rpm;
}

void CellVisuals::setRpm(int newRpm)
{
    m_rpm = newRpm;
}

float CellVisuals::temperature() const
{
    return m_temperature;
}

void CellVisuals::setTemperature(float newTemperature)
{
    m_temperature = newTemperature;
}

float CellVisuals::stirrerAmp() const
{
    return m_stirrerAmp;
}

void CellVisuals::setStirrerAmp(float newStirrerAmp)
{
    m_stirrerAmp = newStirrerAmp;
}

unsigned long CellVisuals::timestamp() const
{
    return m_timestamp;
}

void CellVisuals::setTimestamp(unsigned long newTimestamp)
{
    m_timestamp = newTimestamp;
}

Value CellVisuals::toJSON(Document::AllocatorType& allocator) const {
    Value visualsObject(kObjectType);
    visualsObject.AddMember("rpm", m_rpm, allocator);
    visualsObject.AddMember("temperature", m_temperature, allocator);
    visualsObject.AddMember("stirrerAmp", m_stirrerAmp, allocator);
    visualsObject.AddMember("timestamp", m_timestamp, allocator);
    return visualsObject;
}

// Method to deserialize JSON to object
void CellVisuals::fromJSON(const Value& json) {
    m_rpm = json["rpm"].GetInt();
    m_temperature = json["temperature"].GetFloat();
    m_stirrerAmp = json["stirrerAmp"].GetFloat();
    m_timestamp = json["timestamp"].GetUint64();
}
