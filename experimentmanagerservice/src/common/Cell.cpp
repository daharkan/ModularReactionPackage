#include "Cell.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <chrono>

Cell::Cell() {
    std::setlocale(LC_ALL, "C");
}

Cell::Cell(const Cell& other)
    : m_positionIdx(other.m_positionIdx),
    m_isPlugged(other.m_isPlugged),
    m_isExtTempPlugged(other.m_isExtTempPlugged),
    m_cellID(other.m_cellID),
    m_currentRPM(other.m_currentRPM),
    m_assignedRPM(other.m_assignedRPM),
    m_currentTempInner(other.m_currentTempInner),
    m_currentTempExt(other.m_currentTempExt),
    m_assignedTemp(other.m_assignedTemp),
    m_stirrerMotorAmp(other.m_stirrerMotorAmp),
    m_flowRateLpm(other.m_flowRateLpm),
    m_flowTemp(other.m_flowTemp),
    m_assignedExperiment(other.m_assignedExperiment),
    m_lastUpdatedTimestamp(other.m_lastUpdatedTimestamp),
    m_peltierUseMinute(other.m_peltierUseMinute),
    m_heaterUseMinute(other.m_heaterUseMinute),
    m_motorUseMinute(other.m_motorUseMinute),
    m_heaterDutyPercent(other.m_heaterDutyPercent),
    m_peltierDutyPercent(other.m_peltierDutyPercent),
    m_targetSyncState(other.m_targetSyncState) {}

// Assignment operator
Cell& Cell::operator=(const Cell& other) {
    if (this != &other) {
        m_positionIdx = other.m_positionIdx;
        m_isPlugged = other.m_isPlugged;
        m_isExtTempPlugged = other.m_isExtTempPlugged;
        m_cellID = other.m_cellID;
        m_assignedRPM = other.m_assignedRPM;
        m_assignedTemp = other.m_assignedTemp;
        m_currentRPM = other.m_currentRPM;
        m_currentTempInner = other.m_currentTempInner;
        m_currentTempExt = other.m_currentTempExt;
        m_stirrerMotorAmp = other.m_stirrerMotorAmp;
        m_flowRateLpm = other.m_flowRateLpm;
        m_flowTemp = other.m_flowTemp;
        m_assignedExperiment = other.m_assignedExperiment;
        m_peltierUseMinute = other.m_peltierUseMinute;
        m_heaterUseMinute = other.m_heaterUseMinute;
        m_motorUseMinute = other.m_motorUseMinute;
        m_heaterDutyPercent = other.m_heaterDutyPercent;
        m_peltierDutyPercent = other.m_peltierDutyPercent;
        m_lastUpdatedTimestamp = other.m_lastUpdatedTimestamp;
        m_targetSyncState = other.m_targetSyncState;
    }
    return *this;
}

int Cell::positionIdx() const
{
    return m_positionIdx;
}

void Cell::setPositionIdx(int newPositionIdx)
{
    m_positionIdx = newPositionIdx;
}

bool Cell::isPlugged() const
{
    return m_isPlugged;
}

void Cell::setIsPlugged(bool newIsPlugged)
{
    m_isPlugged = newIsPlugged;
}

std::string Cell::cellID() const
{
    return m_cellID;
}

void Cell::setCellID(const std::string &newCellID)
{
    m_cellID = newCellID;
}

int Cell::currentRPM() const
{
    return m_currentRPM;
}

void Cell::setCurrentRPM(int newCurrentRPM)
{
    m_currentRPM = newCurrentRPM;
}

Experiment Cell::asignedExperiment() const
{
    return m_assignedExperiment;
}

void Cell::setAsignedExperiment(const Experiment &newAsignedExperiment)
{
    m_assignedExperiment = newAsignedExperiment;
}

Value Cell::toJSON(Document::AllocatorType& allocator) const {
    Value cellObject(kObjectType);
    cellObject.AddMember("positionIdx", m_positionIdx, allocator);
    cellObject.AddMember("isPlugged", m_isPlugged, allocator);
    cellObject.AddMember("isExtTempPlugged", m_isExtTempPlugged, allocator);
    cellObject.AddMember("cellID", Value(m_cellID.c_str(), allocator).Move(), allocator);
    cellObject.AddMember("currentRPM", m_currentRPM, allocator);
    cellObject.AddMember("assignedRPM", m_assignedRPM, allocator);
    cellObject.AddMember("assignedTemp", m_assignedTemp, allocator);
    cellObject.AddMember("currentTempInner", m_currentTempInner, allocator);
    cellObject.AddMember("currentTempExt", m_currentTempExt, allocator);
    cellObject.AddMember("stirrerMotorAmp", m_stirrerMotorAmp, allocator);
    cellObject.AddMember("flowRateLpm", m_flowRateLpm, allocator);
    cellObject.AddMember("flowTemp", m_flowTemp, allocator);
    cellObject.AddMember("assignedExperiment", m_assignedExperiment.toJSON(allocator), allocator);
    cellObject.AddMember("peltierUseMinute", m_peltierUseMinute, allocator);
    cellObject.AddMember("heaterUseMinute", m_heaterUseMinute, allocator);
    cellObject.AddMember("motorUseMinute", m_motorUseMinute, allocator);
    cellObject.AddMember("heaterDutyPercent", m_heaterDutyPercent, allocator);
    cellObject.AddMember("peltierDutyPercent", m_peltierDutyPercent, allocator);
    cellObject.AddMember("targetSyncState", m_targetSyncState, allocator);
    cellObject.AddMember("lastUpdatedTimestamp", m_lastUpdatedTimestamp, allocator);
    return cellObject;
}

void Cell::fromJSON(const Value& json) {
    m_positionIdx = json["positionIdx"].GetInt();
    m_isPlugged = json["isPlugged"].GetBool();
    m_isExtTempPlugged = json["isExtTempPlugged"].GetBool();
    m_cellID = json["cellID"].GetString();
    m_currentRPM = json["currentRPM"].GetInt();
    m_assignedRPM = json["assignedRPM"].GetInt();
    m_assignedTemp = json["assignedTemp"].GetFloat();
    m_currentTempInner = json["currentTempInner"].GetFloat();
    m_currentTempExt = json["currentTempExt"].GetFloat();
    m_stirrerMotorAmp = json["stirrerMotorAmp"].GetFloat();
    m_flowRateLpm = json.HasMember("flowRateLpm") ? json["flowRateLpm"].GetFloat() : 0.0f;
    m_flowTemp = json.HasMember("flowTemp") ? json["flowTemp"].GetFloat() : 0.0f;
    m_assignedExperiment.fromJSON(json["assignedExperiment"]);
    m_peltierUseMinute = json["peltierUseMinute"].GetUint64();
    m_heaterUseMinute = json["heaterUseMinute"].GetUint64();
    m_motorUseMinute = json["motorUseMinute"].GetUint64();
    m_heaterDutyPercent = json.HasMember("heaterDutyPercent") ? json["heaterDutyPercent"].GetInt() : -1;
    m_peltierDutyPercent = json.HasMember("peltierDutyPercent") ? json["peltierDutyPercent"].GetInt() : -1;
    m_targetSyncState = json.HasMember("targetSyncState") ? json["targetSyncState"].GetInt() : TargetSyncUnknown;
    m_lastUpdatedTimestamp = json["lastUpdatedTimestamp"].GetUint64();
}

void Cell::updateStatusFromBoard(std::string statusDataStringFromBoard)
{
    /// string::  cellID # posIdx # blockTemp(inner) # currentTempExt # currentRPM # stirrerAmp
    statusDataStringFromBoard.erase(std::remove_if(statusDataStringFromBoard.begin(),
                                              statusDataStringFromBoard.end(),
                                              ::isspace),
                                    statusDataStringFromBoard.end());

    // Use stringstream to parse the input string
    std::stringstream ss(statusDataStringFromBoard);
    std::string token;

    ///bb_000#s25_444#4#13.0#16.0#195.0#26.0#0.0#0

    // Read tokens separated by '#' delimiter
    std::getline(ss, token, '#'); // bus Serial number, ignoring

    std::getline(ss, token, '#'); // Serial number, ignoring
    m_cellID = token;

    std::getline(ss, token, '#'); // Position index
    m_positionIdx = std::stoi(token);

    std::getline(ss, token, '#'); // Current  temperature inner
    m_currentTempInner = std::stof(token);

    std::getline(ss, token, '#'); // Current  temperature external
    m_currentTempExt = std::stof(token);
    std::cout << "cell curr temp: " << m_currentTempExt << std::endl;

    std::getline(ss, token, '#'); // Current  RPM
    m_currentRPM = std::stoi(token);

    std::getline(ss, token, '#'); // Stirrer motor amp
    m_stirrerMotorAmp = std::stof(token);

    std::getline(ss, token, '#'); // target Temp
    m_assignedTemp = std::stof(token);

    std::getline(ss, token, '#'); // target RPM
    m_assignedRPM = std::stoi(token);

    if (std::getline(ss, token, '#')) {
        m_flowRateLpm = std::stof(token);
    }

    if (std::getline(ss, token, '#')) {
        m_flowTemp = std::stof(token);
    }

}

std::string Cell::generateUpdateDataStringToBoard(float targetTemp, float targetTempFuture, unsigned int targetRPM, unsigned int motorSelect)
{
    ///  string:   positionIdx#targetTemp#targetRPM#motorSelect#targetTempFuture#checksum
    std::stringstream ss;
    ss << ">" << m_positionIdx
       << "#" << targetTemp
       << "#" << targetRPM
       << "#" << motorSelect
       << "#" << targetTempFuture
       << "#" << (int)(m_positionIdx + (int)targetTemp + targetRPM + motorSelect + (int)targetTempFuture)
       << "<";
    return ss.str();
}

unsigned long long Cell::getCurrentTimeMillis() {
    // Get the current time point using system_clock
    auto now = std::chrono::system_clock::now();

    // Convert the time point to milliseconds since the epoch
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

CellVisuals Cell::toCellVisuals()
{
    CellVisuals cellVs;
    cellVs.setRpm(m_currentRPM);
    cellVs.setStirrerAmp(m_stirrerMotorAmp);
    cellVs.setTimestamp(getCurrentTimeMillis());

    // Graph visuals should reflect external temperature.
    cellVs.setTemperature(m_currentTempExt);
    return cellVs;
}

bool Cell::isExtTempPlugged() const
{
    return m_isExtTempPlugged;
}

void Cell::setIsExtTempPlugged(bool newIsExtTempPlugged)
{
    m_isExtTempPlugged = newIsExtTempPlugged;
}

float Cell::assignedTemp() const
{
    return m_assignedTemp;
}

void Cell::setAssignedTemp(float newAssignedTemp)
{
    m_assignedTemp = newAssignedTemp;
}

int Cell::assignedRPM() const
{
    return m_assignedRPM;
}

void Cell::setAssignedRPM(int newAssignedRPM)
{
    m_assignedRPM = newAssignedRPM;
}

float Cell::currentTempInner() const
{
    return m_currentTempInner;
}

void Cell::setCurrentTempInner(float newCurrentTempInner)
{
    m_currentTempInner = newCurrentTempInner;
}

float Cell::currentTempExt() const
{
    return m_currentTempExt;
}

void Cell::setCurrentTempExt(float newCurrentTempExt)
{
    m_currentTempExt = newCurrentTempExt;
}

unsigned long Cell::lastUpdatedTimestamp() const
{
    return m_lastUpdatedTimestamp;
}

void Cell::setLastUpdatedTimestamp(unsigned long newLastUpdatedTimestamp)
{
    m_lastUpdatedTimestamp = newLastUpdatedTimestamp;
}



float Cell::calculateTargetTemp()
{
    unsigned long currentMSecs = Cell::getCurrentTimeMillis();
    Profile profile = asignedExperiment().profile();
    float targetTemp = profile.calculateTemp(currentMSecs);
    return targetTemp;

}


int Cell::calculateTargetRPM()
{
    unsigned long currentMSecs = getCurrentTimeMillis();
    Profile profile = asignedExperiment().profile();
    int targetRPM = profile.calculateRPM(currentMSecs);
    return targetRPM;
}




float Cell::calculateTargetTempDummy()
{
    Profile profile = asignedExperiment().profile();
    float targetTemp = profile.calculateTemp(profile.totalMaxProfileDuration());
    return targetTemp;

}


int Cell::calculateTargetRPMDummy()
{
    Profile profile = asignedExperiment().profile();
    int targetRPM = profile.calculateRPM(profile.totalMaxProfileDuration());
    return targetRPM;
}

bool Cell::updateBoardRelatedAttributes(Cell other)
{
    if(m_cellID.empty() && !other.m_cellID.empty()){
        m_cellID = other.m_cellID;
    }
    m_positionIdx = other.m_positionIdx;
    m_isExtTempPlugged = other.m_isExtTempPlugged;
    m_isPlugged = other.m_isPlugged;
    m_assignedRPM = other.m_assignedRPM;
    m_assignedTemp = other.m_assignedTemp;
    m_currentRPM = other.m_currentRPM;
    m_currentTempInner = other.m_currentTempInner;
    m_currentTempExt = other.m_currentTempExt;
    m_stirrerMotorAmp = other.m_stirrerMotorAmp;
    m_flowRateLpm = other.m_flowRateLpm;
    m_flowTemp = other.m_flowTemp;
    m_heaterDutyPercent = other.m_heaterDutyPercent;
    m_peltierDutyPercent = other.m_peltierDutyPercent;
    m_lastUpdatedTimestamp = getCurrentTimeMillis();

    return true;
}

float Cell::stirrerMotorAmp() const
{
    return m_stirrerMotorAmp;
}

void Cell::setStirrerMotorAmp(float newStirrerMotorAmp)
{
    m_stirrerMotorAmp = newStirrerMotorAmp;
}

float Cell::flowRateLpm() const
{
    return m_flowRateLpm;
}

void Cell::setFlowRateLpm(float newFlowRateLpm)
{
    m_flowRateLpm = newFlowRateLpm;
}

float Cell::flowTemp() const
{
    return m_flowTemp;
}

void Cell::setFlowTemp(float newFlowTemp)
{
    m_flowTemp = newFlowTemp;
}

int Cell::heaterDutyPercent() const
{
    return m_heaterDutyPercent;
}

void Cell::setHeaterDutyPercent(int newHeaterDutyPercent)
{
    m_heaterDutyPercent = newHeaterDutyPercent;
}

int Cell::peltierDutyPercent() const
{
    return m_peltierDutyPercent;
}

void Cell::setPeltierDutyPercent(int newPeltierDutyPercent)
{
    m_peltierDutyPercent = newPeltierDutyPercent;
}

int Cell::targetSyncState() const
{
    return m_targetSyncState;
}

void Cell::setTargetSyncState(int newTargetSyncState)
{
    m_targetSyncState = newTargetSyncState;
}
