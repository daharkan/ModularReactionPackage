#ifndef CELLTARGET_H
#define CELLTARGET_H
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <string>

using namespace rapidjson;

class CellTarget
{
public:
    CellTarget(const std::string& cellID = "", float targetTemp = 0.0f, int targetRPM = 0, int motorSelect = 0, int timestamp = 0)
        : m_cellID(cellID), m_targetTemp(targetTemp), m_targetRPM(targetRPM), m_motorSelect(motorSelect) , m_timestamp(timestamp){}

    // Copy constructor
    CellTarget(const CellTarget& other)
        : m_cellID(other.m_cellID), m_targetTemp(other.m_targetTemp), m_targetRPM(other.m_targetRPM), m_motorSelect(other.m_motorSelect), m_timestamp(other.m_timestamp) {}

    // Assignment operator
    CellTarget& operator=(const CellTarget& other) {
        if (this != &other) {
            m_cellID = other.m_cellID;
            m_targetTemp = other.m_targetTemp;
            m_targetRPM = other.m_targetRPM;
            m_motorSelect = other.m_motorSelect;
            m_timestamp = other.m_timestamp;
        }
        return *this;
    }

    Value toJSON(Document::AllocatorType& allocator) const;
    void fromJSON(const Value& json);


    std::string cellID() const;
    void setCellID(const std::string &newCellID);

    float targetTemp() const;
    void setTargetTemp(float newTargetTemp);

    unsigned int targetRPM() const;
    void setTargetRPM(unsigned int newTargetRPM);

    unsigned int motorSelect() const;
    void setMotorSelect(unsigned int newMotorSelect);

    unsigned long timestamp() const;
    void setTimestamp(unsigned long newTimestamp);

private:
    std::string m_cellID;
    float m_targetTemp;
    unsigned int m_targetRPM;
    unsigned int m_motorSelect;
    unsigned long m_timestamp;

};

#endif // CELLTARGET_H
