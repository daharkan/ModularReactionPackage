#ifndef RPMARC_H
#define RPMARC_H

#include "Arc.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

using namespace rapidjson;

class RPMArc : public Arc
{
public:
    RPMArc(float A, float B, float C, unsigned long startTimeMsec, unsigned long durationMSec);
    RPMArc(const RPMArc& other)
        : Arc(other), m_startRPM(other.m_startRPM), m_finishRPM(other.m_finishRPM)
    {
        // No dynamic memory allocation, so no need for deep copy
    }

    int startRPM() const;
    void setStartRPM(int newStartRPM);

    int finishRPM() const;
    void setFinishRPM(int newFinishRPM);

    Value toJSON(Document::AllocatorType& allocator) const {
        Value rpmArc(kObjectType);
        rpmArc.AddMember("startRPM", m_startRPM, allocator);
        rpmArc.AddMember("finishRPM", m_finishRPM, allocator);
        rpmArc.AddMember("coeffA", m_coeffA, allocator);
        rpmArc.AddMember("coeffB", m_coeffB, allocator);
        rpmArc.AddMember("coeffC", m_coeffC, allocator);
        rpmArc.AddMember("durationMSec", m_durationMSec, allocator);
        rpmArc.AddMember("finishTimeMsec", m_finishTimeMsec, allocator);
        return rpmArc;
    }

    void fromJSON(const Value& json) {
        m_startRPM = json["startRPM"].GetInt();
        m_finishRPM = json["finishRPM"].GetInt();
        m_coeffA = json["coeffA"].GetFloat();
        m_coeffB = json["coeffB"].GetFloat();
        m_coeffC = json["coeffC"].GetFloat();
        m_durationMSec = json["durationMSec"].GetUint64();
        m_finishTimeMsec = json["finishTimeMsec"].GetUint64();
    }


    // Copy assignment operator
    RPMArc& operator=(const RPMArc& other) {
        if (this != &other) {
            m_startRPM = other.m_startRPM;
            m_finishRPM = other.m_finishRPM;
            m_coeffA = other.m_coeffA;
            m_coeffB = other.m_coeffB;
            m_coeffC = other.m_coeffC;
            m_durationMSec = other.m_durationMSec;
            m_finishTimeMsec = other.m_finishTimeMsec;
        }
        return *this;
    }

    // Equality operator
    bool operator==(const RPMArc& other) const {
        return (m_startRPM == other.m_startRPM &&
                m_finishRPM == other.m_finishRPM &&
                m_coeffA == other.m_coeffA &&
                m_coeffB == other.m_coeffB &&
                m_coeffC == other.m_coeffC &&
                m_durationMSec == other.m_durationMSec &&
                m_finishTimeMsec == other.m_finishTimeMsec);
    }


private:
    int m_startRPM;
    int m_finishRPM;
};

#endif // RPMARC_H
