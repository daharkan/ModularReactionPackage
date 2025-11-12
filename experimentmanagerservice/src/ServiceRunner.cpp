#include "ServiceRunner.h"
#include "src/BusboardV1.h"
#include "src/common/CellTarget.h"
#include "src/common/CellVisualsHistory.h"
#include <QCoreApplication>
#include <chrono>
#include <thread>
#include <QtConcurrent/QtConcurrent>

#define HOSTNAME "127.0.0.1"
#define PORT 6379

#define MIN_CELL_UPDATE_INTERVAL_MSECS 100
#define TEMP_EPSILON 0.5


ServiceRunner::ServiceRunner()
{
    m_busboard = std::make_shared<BusboardV1>();
}

bool ServiceRunner::initService()
{
    bool succ = false;
    succ = m_busboard->connectBoard();

    bool succDB = false;

    if(RedisDBManager::getInstance()->isConnected()){
        return succ;
    }
    succDB = RedisDBManager::getInstance()->connectToDB(HOSTNAME, PORT);
    return succ && succDB;
}

void ServiceRunner::startServiceLoop()
{
    while(m_busboard->checkHealth()){



        //qDebug() << "----1----";

        QCoreApplication::processEvents();

        //Getting board cells with all BOARD RELATED data----------------------------------------
        std::vector<Cell> cellsFromBoard = m_busboard->getCellArray();
        std::vector<std::string> cellIDList = m_busboard->getCellIdList();
        //qDebug() << "-----2-----";

        if(cellsFromBoard.size() != cellIDList.size()){
            qDebug() << "cellsFromBoard size mismatch";
            continue;
        }
        std::vector<Cell> cellsFromDB = RedisDBManager::getInstance()->getCellList(cellIDList);
        //qDebug() << "-----3-----";

        std::vector<CellTarget> celltargets = RedisDBManager::getInstance()->getCellTargets(cellIDList);

       // qDebug() << "-----4----- celltargets size: " << celltargets.size();


        std::map<std::string, Cell> boardCellMap;
        //cellsFromBoard got copying to cellMap by cellID........
        for(int i = 0; i < cellsFromBoard.size(); i++){
            if(cellsFromBoard.at(i).cellID().empty()){
                continue;
            }
            boardCellMap[cellsFromBoard.at(i).cellID()] = cellsFromBoard.at(i);
        }
        QCoreApplication::processEvents();
        //qDebug() << "-----5-----";

        std::map<std::string, Cell> dbCellMap;
        //cellsFromBoard got copying to cellMap by cellID........
        for(int i = 0; i < cellsFromDB.size(); i++){
            if(cellsFromDB.at(i).cellID().empty()){
                continue;
            }
            dbCellMap[cellsFromDB.at(i).cellID()] = cellsFromDB.at(i);
        }
        QCoreApplication::processEvents();

        //qDebug() << "-----6-----";

        std::map<std::string, CellTarget> cellTargetsMap;
        //cellsFromBoard got copying to cellMap by cellID........
        for(int i = 0; i < celltargets.size(); i++){
            if(celltargets.at(i).cellID().empty()){
                continue;
            }
            cellTargetsMap[celltargets.at(i).cellID()] = celltargets.at(i);
        }
        QCoreApplication::processEvents();

        //qDebug() << "-----7-----";

        for(int i = 0; i < cellIDList.size(); i++){
            std::string cellID = cellIDList.at(i);
            if(cellID.empty()){
                continue;
            }
            dbCellMap[cellID].updateBoardRelatedAttributes(boardCellMap[cellID]);
        }
        QCoreApplication::processEvents();
        //qDebug() << "-----8-----";


        for(int i = 0; i < cellIDList.size(); i++){
            if(cellTargetsMap.size() < 1){
                continue;
            }
            std::string cellID = cellIDList.at(i);

            if(cellID.empty()){
                continue;
            }

            auto it = cellTargetsMap.find(cellID);

            if (it != cellTargetsMap.end()) {
                CellTarget celltarget = cellTargetsMap[cellID];

                if(celltarget.targetRPM() != boardCellMap[cellID].assignedRPM()
                    || abs( celltarget.targetTemp() - boardCellMap[cellID].assignedTemp() ) > TEMP_EPSILON){
                    int targetRPM = celltarget.targetRPM();
                    double targetTemp = celltarget.targetTemp();
                    std::string updateString = boardCellMap[cellID].generateUpdateDataStringToBoard(targetTemp, targetRPM);
                    QCoreApplication::processEvents();

                    QString str = QString::fromStdString(updateString);
                    m_busboard->sendUpdateString(str);
                    /*QtConcurrent::run([str, this]() {
                        m_busboard->sendUpdateString(str);
                    });*/
                }
                QCoreApplication::processEvents();
            }//
        }



/*
        //this will get current visuals in DB and update and push again...
        for(int i = 0; i < cellsFromBoard.size(); i++){
            CellVisualsHistory cellVisualsHistory = RedisDBManager::getInstance()->getCellVisualsHistory(cellsFromBoard.at(i).cellID());
            CellVisuals visuals = cellsFromBoard.at(i).toCellVisuals();
            cellVisualsHistory.addCellVisuals(visuals);
            RedisDBManager::getInstance()->pushCellVisuals(cellsFromBoard.at(i).cellID(), cellVisualsHistory);
        }
        //------------------------------------------------------------------------------------------------------------------------
*/
        std::vector<Cell> cellsToDB;
        for (const auto& pair : dbCellMap) {
            cellsToDB.push_back(pair.second);
        }
        //pushing updated cells to the DB...
        RedisDBManager::getInstance()->pushCellList(cellsToDB);
        QCoreApplication::processEvents();

        RedisDBManager::getInstance()->pushBusboardCellIds(m_busboard->busboardID(), cellIDList);
        QCoreApplication::processEvents();


    }



}
