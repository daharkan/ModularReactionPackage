#include "CellVisualsHistory.h"

CellVisualsHistory::CellVisualsHistory() {}


Value CellVisualsHistory::toJSON(Document::AllocatorType& allocator) const {
    Value historyObject(kObjectType);
    Value visualsArray(kArrayType);
    for (const auto& visuals : m_visuals) {
        visualsArray.PushBack(visuals.toJSON(allocator), allocator);
    }
    historyObject.AddMember("visuals", visualsArray, allocator);
    return historyObject;
}

// Method to deserialize JSON to object
void CellVisualsHistory::fromJSON(const Value& json) {
    const Value& visualsArray = json["visuals"];
    for (SizeType i = 0; i < visualsArray.Size(); ++i) {
        CellVisuals visuals;
        visuals.fromJSON(visualsArray[i]);
        m_visuals.push_back(visuals);
    }
}

void CellVisualsHistory::addCellVisuals(CellVisuals cellVisuals)
{
    m_visuals.push_back(cellVisuals);
}
