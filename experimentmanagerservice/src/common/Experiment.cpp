#include "Experiment.h"
#include <iostream>

Experiment::Experiment() {}

Experiment::Experiment(const Experiment& other) :
    m_name(other.m_name),
    m_owner(other.m_owner),
    m_profile(other.m_profile),
    m_cellID(other.m_cellID),
    m_busboardID(other.m_busboardID),
    m_totalProfileDurationMSecs(other.m_totalProfileDurationMSecs),
    m_startSystemTimeMSecs(other.m_startSystemTimeMSecs),
    m_experimentId(other.m_experimentId),
    m_experimentType(other.m_experimentType),
    m_createdAtMSecs(other.m_createdAtMSecs),
    m_settingsJson(other.m_settingsJson),
    m_state(other.m_state)
{
    // Copy constructor implementation
}


Experiment& Experiment::operator=(const Experiment& other) {
    if (this != &other) { // Self-assignment check
        m_name = other.m_name;
        m_owner = other.m_owner;
        m_profile = other.m_profile;
        m_cellID = other.m_cellID;
        m_busboardID = other.m_busboardID;
        m_totalProfileDurationMSecs = other.m_totalProfileDurationMSecs;
        m_startSystemTimeMSecs = other.m_startSystemTimeMSecs;
        m_experimentId = other.m_experimentId;
        m_experimentType = other.m_experimentType;
        m_createdAtMSecs = other.m_createdAtMSecs;
        m_settingsJson = other.m_settingsJson;
        m_state = other.m_state;
    }

    return *this;
}
User Experiment::owner() const
{
    return m_owner;
}

void Experiment::setOwner(const User &newOwner)
{
    m_owner = newOwner;
}

std::string Experiment::name() const
{
    return m_name;
}

void Experiment::setName(const std::string &newName)
{
    m_name = newName;
}

std::string Experiment::experimentId() const
{
    return m_experimentId;
}

void Experiment::setExperimentId(const std::string &newExperimentId)
{
    m_experimentId = newExperimentId;
}

int Experiment::experimentType() const
{
    return m_experimentType;
}

void Experiment::setExperimentType(int newExperimentType)
{
    m_experimentType = newExperimentType;
}

unsigned long Experiment::createdAtMSecs() const
{
    return m_createdAtMSecs;
}

void Experiment::setCreatedAtMSecs(unsigned long newCreatedAtMSecs)
{
    m_createdAtMSecs = newCreatedAtMSecs;
}

std::string Experiment::settingsJson() const
{
    return m_settingsJson;
}

void Experiment::setSettingsJson(const std::string &newSettingsJson)
{
    m_settingsJson = newSettingsJson;
}

std::string Experiment::state() const
{
    return m_state;
}

void Experiment::setState(const std::string &newState)
{
    m_state = newState;
}

Profile& Experiment::profile() {
    return m_profile;
}

void Experiment::setProfile(const Profile &newProfile)
{
    m_profile = newProfile;
}

std::string Experiment::cellID() const
{
    return m_cellID;
}

void Experiment::setCellID(const std::string &newCellID)
{
    m_cellID = newCellID;
}

std::string Experiment::busboardID() const
{
    return m_busboardID;
}

void Experiment::setBusboardID(const std::string &newBusboardID)
{
    m_busboardID = newBusboardID;
}

unsigned long Experiment::totalProfileDurationMSecs() const
{
    if(m_profile.tempArcsInSeq().size() > 0){
        return m_profile.totalMaxProfileDuration();
    }
    return 0;
}

void Experiment::setTotalProfileDurationMSecs(unsigned long newTotalProfileDurationMSecs)
{
    m_totalProfileDurationMSecs = newTotalProfileDurationMSecs;
}

Experiment Experiment::fromJSon(std::string jsonString)
{
    Experiment exp;

    return exp;
}

std::string Experiment::toJSon(Experiment exp)
{
    std::string str;

    return str;
}

unsigned long Experiment::startSystemTimeMSecs() const
{
    return m_startSystemTimeMSecs;
}

void Experiment::setStartSystemTimeMSecs(unsigned long newStartSystemTimeMSecs)
{
    m_startSystemTimeMSecs = newStartSystemTimeMSecs;
}
