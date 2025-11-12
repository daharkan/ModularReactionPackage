#include "IBusboard.h"

IBusboard::IBusboard(): QObject()  {}

std::string IBusboard::busboardID() const
{
    return m_busboardID;
}

void IBusboard::setBusboardID(const std::string &newBusboardID)
{
    m_busboardID = newBusboardID;
}

int IBusboard::cellCount() const
{
    return m_cellCount;
}

void IBusboard::setCellCount(int newCellCount)
{
    m_cellCount = newCellCount;
}
