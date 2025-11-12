#include "TempArc.h"
#include <iostream>

TempArc::TempArc(float A, float B, float C, unsigned long startTimeMsec, unsigned long durationMSec) :
    Arc(A, B, C, startTimeMsec, durationMSec)
{
    unsigned long finishTimeMSec = startTimeMsec + durationMSec;
    double finishTimeSec = finishTimeMSec/1000.0;
    double startTimeSec = startTimeMsec/1000.0;
    m_finishTemp = A * finishTimeSec * finishTimeSec + B * finishTimeSec + C;
    m_startTemp = A * startTimeSec * startTimeSec + B * startTimeSec + C;
    std::cout << std::endl;
    std::cout << "TempArc::TempArc A: " << A << std::endl;
    std::cout << "TempArc::TempArc B: " << B << std::endl;
    std::cout << "TempArc::TempArc C: " << C << std::endl;
    std::cout << "TempArc::TempArc m_startTemp: " << m_startTemp << std::endl;
    std::cout << "TempArc::TempArc m_finishTemp: " << m_finishTemp << std::endl;
    std::cout << "TempArc::TempArc m_startTimeMsec: " << m_startTimeMsec << std::endl;
    std::cout << "TempArc::TempArc m_finishTimeMsec: " << m_finishTimeMsec << std::endl;


}

float TempArc::startTemp() const
{
    return m_startTemp;
}

void TempArc::setStartTemp(float newStartTemp)
{
    m_startTemp = newStartTemp;
}

float TempArc::finishTemp() const
{
    return m_finishTemp;
}

void TempArc::setFinishTemp(float newFinishTemp)
{
    m_finishTemp = newFinishTemp;
}
