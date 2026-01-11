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
    m_visuals.clear();
    if (!json.HasMember("visuals") || !json["visuals"].IsArray()) {
        return;
    }
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

void CellVisualsHistory::appendHistory(const CellVisualsHistory& other)
{
    m_visuals.insert(m_visuals.end(), other.m_visuals.begin(), other.m_visuals.end());
}

const std::vector<CellVisuals>& CellVisualsHistory::visuals() const
{
    return m_visuals;
}
