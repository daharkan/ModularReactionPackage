#ifndef CELL_H
#define CELL_H
#include "Experiment.h"
#include "CellVisuals.h"

class Cell
{

public:
    Cell();

    Cell(const Cell& other);

    Cell& operator=(const Cell& other);

    int positionIdx() const;
    void setPositionIdx(int newPositionIdx);

    bool isPlugged() const;
    void setIsPlugged(bool newIsPlugged);

    std::string cellID() const;
    void setCellID(const std::string &newCellID);

    unsigned int currentRPM() const;
    void setCurrentRPM(unsigned int newCurrentRPM);

    Experiment asignedExperiment() const;
    void setAsignedExperiment(const Experiment &newAsignedExperiment);

    Value toJSON(Document::AllocatorType& allocator) const;

    void fromJSON(const Value& json);

    void updateStatusFromBoard(std::string statusDataStringFromBoard);
    std::string generateUpdateDataStringToBoard(float targetTemp, unsigned int targetRPM);

    float currentTempInner() const;
    void setCurrentTempInner(float newCurrentTempInner);

    float currentTempExt() const;
    void setCurrentTempExt(float newCurrentTempExt);

    unsigned long lastUpdatedTimestamp() const;
    void setLastUpdatedTimestamp(unsigned long newLastUpdatedTimestamp);

    static unsigned long long getCurrentTimeMillis();

    CellVisuals toCellVisuals();

    bool isExtTempPlugged() const;
    void setIsExtTempPlugged(bool newIsExtTempPlugged);

    float assignedTemp() const;
    void setAssignedTemp(float newTargetTemp);

    unsigned int assignedRPM() const;
    void setAssignedRPM(unsigned int newTargetRPM);

    float calculateTargetTemp();
    int calculateTargetRPM();

    float calculateTargetTempDummy();
    int calculateTargetRPMDummy();

    bool updateBoardRelatedAttributes(Cell other);


    float stirrerMotorAmp() const;
    void setStirrerMotorAmp(float newStirrerMotorAmp);

    float flowRateLpm() const;
    void setFlowRateLpm(float newFlowRateLpm);

    float flowTemp() const;
    void setFlowTemp(float newFlowTemp);

private:
    int m_positionIdx = -1;
    bool m_isPlugged = false;
    bool m_isExtTempPlugged = false;
    std::string m_cellID = "";
    int m_currentRPM = 0;
    int m_assignedRPM = 0;
    float m_currentTempInner = 0;
    float m_currentTempExt = 0;
    float m_assignedTemp = 0;
    float m_stirrerMotorAmp = 0;
    float m_flowRateLpm = 0;
    float m_flowTemp = 0;
    Experiment m_assignedExperiment;
    unsigned long m_lastUpdatedTimestamp = 0;
    unsigned long m_peltierUseMinute = 0;
    unsigned long m_heaterUseMinute = 0;
    unsigned long m_motorUseMinute = 0;

};

#endif // CELL_H
