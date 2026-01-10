#include "CellTarget.h"
#include "rapidjson/rapidjson.h"


Value CellTarget::toJSON(Document::AllocatorType& allocator) const {
    Value targetObject(kObjectType);
    targetObject.AddMember("cellID", Value(m_cellID.c_str(), allocator).Move(), allocator);
    targetObject.AddMember("targetTemp", m_targetTemp, allocator);
    targetObject.AddMember("targetRPM", m_targetRPM, allocator);
    targetObject.AddMember("motorSelect", m_motorSelect, allocator);
    targetObject.AddMember("timestamp", m_timestamp, allocator);
    return targetObject;
}

// Method to deserialize JSON to object
void CellTarget::fromJSON(const Value& json) {
    m_cellID = json["cellID"].GetString();
    m_targetTemp = json["targetTemp"].GetFloat();
    m_targetRPM = json["targetRPM"].GetUint();
    if (json.HasMember("motorSelect")) {
        m_motorSelect = json["motorSelect"].GetUint();
    } else {
        m_motorSelect = 0;
    }
    m_timestamp = json["timestamp"].GetUint64();
}

std::string CellTarget::cellID() const
{
    return m_cellID;
}

void CellTarget::setCellID(const std::string &newCellID)
{
    m_cellID = newCellID;
}

float CellTarget::targetTemp() const
{
    return m_targetTemp;
}

void CellTarget::setTargetTemp(float newTargetTemp)
{
    m_targetTemp = newTargetTemp;
}

unsigned int CellTarget::targetRPM() const
{
    return m_targetRPM;
}

void CellTarget::setTargetRPM(unsigned int newTargetRPM)
{
    m_targetRPM = newTargetRPM;
}

unsigned int CellTarget::motorSelect() const
{
    return m_motorSelect;
}

void CellTarget::setMotorSelect(unsigned int newMotorSelect)
{
    m_motorSelect = newMotorSelect;
}

unsigned long CellTarget::timestamp() const
{
    return m_timestamp;
}

void CellTarget::setTimestamp(unsigned long newTimestamp)
{
    m_timestamp = newTimestamp;
}
