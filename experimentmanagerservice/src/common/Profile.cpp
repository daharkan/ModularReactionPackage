#include "Profile.h"
#include <iostream>

Profile::Profile() : m_totalMaxProfileDuration(0) {}


bool Profile::addTempArcInSequence(TempArc tArc)
{
    std::cout << "addTempArcInSequence :: fin time before: " << tArc.finishTemp() << std::endl;

    m_tempArcsInSeq.push_back(tArc);
    m_totalMaxProfileDuration = m_tempArcsInSeq.back().finishTimeMsec();

    std::cout << "addTempArcInSequence :: fin time after: " << m_tempArcsInSeq.at(m_tempArcsInSeq.size()-1).finishTemp() << std::endl;
    return true;
}

bool Profile::addRPMArcInSequence(RPMArc rArc)
{
    m_rpmArcsInSeq.push_back(rArc);
    return true;
}

bool Profile::removeLastTempArc()
{
    if (m_tempArcsInSeq.empty()) {
        return false;
    }

    m_tempArcsInSeq.pop_back();
    if (m_tempArcsInSeq.empty()) {
        m_totalMaxProfileDuration = 0;
    } else {
        m_totalMaxProfileDuration = m_tempArcsInSeq.back().finishTimeMsec();
    }

    return true;
}

long Profile::totalMaxProfileDuration() const
{

    //make this smarter to choose max time duration in RPM and Temp sequences...
    return m_totalMaxProfileDuration;
}

void Profile::setTotalMaxProfileDuration(long newTotalMaxProfileDuration)
{
    m_totalMaxProfileDuration = newTotalMaxProfileDuration;
}

std::vector<TempArc> Profile::tempArcsInSeq() const
{
    return m_tempArcsInSeq;
}

std::vector<RPMArc> Profile::rpmArcsInSeq() const
{
    return m_rpmArcsInSeq;
}

float Profile::calculateTemp(unsigned long currentTimeMSec)
{
    float temp = 0.0;
    for(int i = 0; i < m_tempArcsInSeq.size(); i++){
        if(currentTimeMSec < m_tempArcsInSeq.at(i).finishTimeMsec()){
            temp = m_tempArcsInSeq.at(i).calculateY(currentTimeMSec);
            return temp;
        }
    }
    return temp;
}

int Profile::calculateRPM(unsigned long currentTimeMSec)
{
    int RPM = 0;
    for(int i = 0; i < m_rpmArcsInSeq.size(); i++){
        if(currentTimeMSec < m_rpmArcsInSeq.at(i).finishTimeMsec()){
            RPM = m_rpmArcsInSeq.at(i).calculateY(currentTimeMSec);
            return RPM;
        }
    }
    return RPM;
}

void Profile::clearProfile()
{
    m_totalMaxProfileDuration = 0;
    m_rpmArcsInSeq.clear();
    m_tempArcsInSeq.clear();
}
