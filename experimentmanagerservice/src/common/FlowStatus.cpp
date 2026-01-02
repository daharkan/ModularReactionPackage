#include "FlowStatus.h"

FlowStatus::FlowStatus() {}

float FlowStatus::flowRateLpm() const
{
    return m_flowRateLpm;
}

void FlowStatus::setFlowRateLpm(float newFlowRateLpm)
{
    m_flowRateLpm = newFlowRateLpm;
}

float FlowStatus::flowTemp() const
{
    return m_flowTemp;
}

void FlowStatus::setFlowTemp(float newFlowTemp)
{
    m_flowTemp = newFlowTemp;
}

unsigned long FlowStatus::timestamp() const
{
    return m_timestamp;
}

void FlowStatus::setTimestamp(unsigned long newTimestamp)
{
    m_timestamp = newTimestamp;
}

Value FlowStatus::toJSON(Document::AllocatorType& allocator) const
{
    Value flowObject(kObjectType);
    flowObject.AddMember("flowRateLpm", m_flowRateLpm, allocator);
    flowObject.AddMember("flowTemp", m_flowTemp, allocator);
    flowObject.AddMember("timestamp", m_timestamp, allocator);
    return flowObject;
}

void FlowStatus::fromJSON(const Value& json)
{
    m_flowRateLpm = json["flowRateLpm"].GetFloat();
    m_flowTemp = json["flowTemp"].GetFloat();
    m_timestamp = json["timestamp"].GetUint64();
}
