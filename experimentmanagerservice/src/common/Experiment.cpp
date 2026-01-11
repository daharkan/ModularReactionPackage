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
    m_startSystemTimeMSecs(other.m_startSystemTimeMSecs)
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
