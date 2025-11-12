#include "Arc.h"
#include <cmath>
#include <iostream>

Arc::Arc(float A, float B, float C, unsigned long startTimeMsec, unsigned long durationMSec) :
    m_coeffA(A),
    m_coeffB(B),
    m_coeffC(C),
    m_durationMSec(durationMSec),
    m_startTimeMsec(startTimeMsec)
{
    m_finishTimeMsec = m_startTimeMsec + m_durationMSec;
}

double Arc::calculateY(unsigned long currentSystemT)
{
    double tSec = currentSystemT / (double)1000.0;
    //std::cout << "currentSystemT: " << currentSystemT << "  tSec: " << tSec << std::endl;
    //std::cout << "m_coeffA: " << m_coeffA << "   m_coeffB: " << m_coeffB << "   m_coeffC: " << m_coeffC << std::endl;

    double y = (double)m_coeffA * (double)(tSec*tSec) + (double)m_coeffB * (double)tSec + (double)m_coeffC;

   // double roundedY = std::round(y * 100.0) / 100.0;
    //std::cout << "y: " << y << std::endl;
    //std::cout << "roundedY: " << roundedY << std::endl;

    return y;
}

float Arc::coeffA() const
{
    return m_coeffA;
}

void Arc::setCoeffA(float newCoeffA)
{
    m_coeffA = newCoeffA;
}

float Arc::coeffB() const
{
    return m_coeffB;
}

void Arc::setCoeffB(float newCoeffB)
{
    m_coeffB = newCoeffB;
}

float Arc::coeffC() const
{
    return m_coeffC;
}

void Arc::setCoeffC(float newCoeffC)
{
    m_coeffC = newCoeffC;
}

unsigned long Arc::durationMSec() const
{
    return m_durationMSec;
}

void Arc::setDurationMSec(unsigned long newDurationMSec)
{
    m_durationMSec = newDurationMSec;
}

unsigned long Arc::finishTimeMsec() const
{
    return m_finishTimeMsec;
}

void Arc::setFinishTimeMsec(unsigned long newFinishTimeMsec)
{
    m_finishTimeMsec = newFinishTimeMsec;
}

unsigned long Arc::startTimeMsec() const
{
    return m_startTimeMsec;
}

void Arc::setStartTimeMsec(unsigned long newStartTimeMsec)
{
    m_startTimeMsec = newStartTimeMsec;
}
