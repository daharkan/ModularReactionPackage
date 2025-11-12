#ifndef TEMPARC_H
#define TEMPARC_H

#include "Arc.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

using namespace rapidjson;

class TempArc : public Arc
{
public:
    TempArc(float A, float B, float C, unsigned long startTimeMsec, unsigned long durationMSec);

    TempArc(const TempArc& other)
        : Arc(other), m_startTemp(other.m_startTemp), m_finishTemp(other.m_finishTemp)
    {
        // No dynamic memory allocation, so no need for deep copy
    }

    float startTemp() const;
    void setStartTemp(float newStartTemp);

    float finishTemp() const;
    void setFinishTemp(float newFinishTemp);

    Value toJSON(Document::AllocatorType& allocator) const {
        Value tempArc(kObjectType);
        tempArc.AddMember("startTemp", m_startTemp, allocator);
        tempArc.AddMember("finishTemp", m_finishTemp, allocator);
        tempArc.AddMember("coeffA", m_coeffA, allocator);
        tempArc.AddMember("coeffB", m_coeffB, allocator);
        tempArc.AddMember("coeffC", m_coeffC, allocator);
        tempArc.AddMember("durationMSec", m_durationMSec, allocator);
        tempArc.AddMember("startTimeMsec", m_startTimeMsec, allocator);
        tempArc.AddMember("finishTimeMsec", m_finishTimeMsec, allocator);
        return tempArc;
    }

    void fromJSON(const Value& json) {
        m_startTemp = json["startTemp"].GetFloat();
        m_finishTemp = json["finishTemp"].GetFloat();
        m_coeffA = json["coeffA"].GetFloat();
        m_coeffB = json["coeffB"].GetFloat();
        m_coeffC = json["coeffC"].GetFloat();
        m_durationMSec = json["durationMSec"].GetUint64();
        m_startTimeMsec = json["startTimeMsec"].GetUint64();
        m_finishTimeMsec = json["finishTimeMsec"].GetUint64();
    }
    TempArc& operator=(const TempArc& other) {
        if (this != &other) {
            // Call base class's assignment operator
            Arc::operator=(other);

            // Assign member variables specific to TempArc
            m_startTemp = other.m_startTemp;
            m_finishTemp = other.m_finishTemp;
        }
        return *this;
    }

    // Equality operator
    bool operator==(const TempArc& other) const {
        return (m_startTemp == other.m_startTemp &&
                m_finishTemp == other.m_finishTemp &&
                m_coeffA == other.m_coeffA &&
                m_coeffB == other.m_coeffB &&
                m_coeffC == other.m_coeffC &&
                m_durationMSec == other.m_durationMSec &&
                m_startTimeMsec == other.m_startTimeMsec &&
                m_finishTimeMsec == other.m_finishTimeMsec);
    }


private:
    float m_startTemp;
    float m_finishTemp;
};

#endif // TEMPARC_H
