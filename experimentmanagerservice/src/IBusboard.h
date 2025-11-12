#ifndef IBUSBOARD_H
#define IBUSBOARD_H

#include "qobject.h"
#include "src/common/Cell.h"

class IBusboard : public QObject
{
    Q_OBJECT

public:
    IBusboard();
    virtual bool connectBoard() = 0;
    virtual void sendExampleString() = 0;
    virtual bool checkHealth() = 0;
    virtual std::vector<Cell>& getCellArray() = 0;
    virtual bool sendUpdateString(QString str) = 0;
    virtual std::vector<std::string> getCellIdList() = 0;

    std::string busboardID() const;
    void setBusboardID(const std::string &newBusboardID);

    int cellCount() const;
    void setCellCount(int newCellCount);

protected:
    std::string m_busboardID;
    int m_cellCount;

};

#endif // IBUSBOARD_H
