#ifndef CELLVISUALSHISTORY_H
#define CELLVISUALSHISTORY_H
#include "CellVisuals.h"
#include <vector>

class CellVisualsHistory
{
public:
    CellVisualsHistory();
    CellVisualsHistory(const std::vector<CellVisuals>& visuals = {}) : m_visuals(visuals) {}

    // Copy constructor
    CellVisualsHistory(const CellVisualsHistory& other) : m_visuals(other.m_visuals) {}

    // Assignment operator
    CellVisualsHistory& operator=(const CellVisualsHistory& other) {
        if (this != &other) {
            m_visuals = other.m_visuals;
        }
        return *this;
    }

    Value toJSON(Document::AllocatorType& allocator) const;
    void fromJSON(const Value& json);

    void addCellVisuals(CellVisuals cellVisuals);

private:
    std::vector<CellVisuals> m_visuals;
};

#endif // CELLVISUALSHISTORY_H
