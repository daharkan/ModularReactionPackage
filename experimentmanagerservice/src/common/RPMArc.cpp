#include "RPMArc.h"

RPMArc::RPMArc(float A, float B, float C, unsigned long startTimeMsec, unsigned long durationMSec) :
    Arc(A, B, C, startTimeMsec, durationMSec)
{
}

int RPMArc::startRPM() const
{
    return m_startRPM;
}

void RPMArc::setStartRPM(int newStartRPM)
{
    m_startRPM = newStartRPM;
}

int RPMArc::finishRPM() const
{
    return m_finishRPM;
}

void RPMArc::setFinishRPM(int newFinishRPM)
{
    m_finishRPM = newFinishRPM;
}
