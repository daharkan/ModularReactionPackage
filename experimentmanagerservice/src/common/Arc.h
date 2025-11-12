#ifndef ARC_H
#define ARC_H

class Arc
{
public:
    Arc(float A, float B, float C, unsigned long startTimeMsec, unsigned long durationMSec);
    // Copy constructor
    Arc(const Arc& other)
        : m_coeffA(other.m_coeffA),
        m_coeffB(other.m_coeffB),
        m_coeffC(other.m_coeffC),
        m_durationMSec(other.m_durationMSec),
        m_finishTimeMsec(other.m_finishTimeMsec),
        m_startTimeMsec(other.m_startTimeMsec)
    {
        // No need for deep copy since all members are primitive types
    }

    // Assignment operator
    Arc& operator=(const Arc& other)
    {
        if (this != &other) // Check for self-assignment
        {
            // Copy each member variable
            m_coeffA = other.m_coeffA;
            m_coeffB = other.m_coeffB;
            m_coeffC = other.m_coeffC;
            m_durationMSec = other.m_durationMSec;
            m_finishTimeMsec = other.m_finishTimeMsec;
            m_startTimeMsec = other.m_startTimeMsec;
        }
        return *this;
    }

    double calculateY(unsigned long currentSystemT);

    float coeffA() const;
    void setCoeffA(float newCoeffA);

    float coeffB() const;
    void setCoeffB(float newCoeffB);

    float coeffC() const;
    void setCoeffC(float newCoeffC);

    unsigned long durationMSec() const;
    void setDurationMSec(unsigned long newDurationMSec);

    unsigned long finishTimeMsec() const;
    void setFinishTimeMsec(unsigned long newFinishTimeMsec);

    unsigned long startTimeMsec() const;
    void setStartTimeMsec(unsigned long newStartTimeMsec);

protected:
    float m_coeffA = 0;
    float m_coeffB = 0;
    float m_coeffC = 0;
    unsigned long m_durationMSec = 0;
    unsigned long m_finishTimeMsec;
    unsigned long m_startTimeMsec;

};

#endif // ARC_H
