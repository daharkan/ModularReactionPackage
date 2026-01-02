#ifndef FLOWSTATUS_H
#define FLOWSTATUS_H

#include "rapidjson/document.h"

using namespace rapidjson;

class FlowStatus
{
public:
    FlowStatus();
    FlowStatus(float flowRateLpm, float flowTemp, unsigned long timestamp)
        : m_flowRateLpm(flowRateLpm), m_flowTemp(flowTemp), m_timestamp(timestamp) {}

    Value toJSON(Document::AllocatorType& allocator) const;
    void fromJSON(const Value& json);

    float flowRateLpm() const;
    void setFlowRateLpm(float newFlowRateLpm);

    float flowTemp() const;
    void setFlowTemp(float newFlowTemp);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long newTimestamp);

private:
    float m_flowRateLpm = 0.0f;
    float m_flowTemp = 0.0f;
    unsigned long m_timestamp = 0;
};

#endif // FLOWSTATUS_H
