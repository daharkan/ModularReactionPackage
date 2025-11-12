#include "RedisDBManager.h"



RedisDBManager* RedisDBManager::m_instance = nullptr;

RedisDBManager::RedisDBManager(): QObject() {

}



RedisDBManager* RedisDBManager::getInstance()
{
    if(m_instance == nullptr){
        m_instance = new RedisDBManager();
    }
    return m_instance;
}

bool RedisDBManager::connectToDB(std::string hostname, int port)
{
    m_client.connect(hostname, port);
    return m_client.is_connected();
}

std::vector<Cell> RedisDBManager::getCellList(std::vector<std::string> cellIDList)
{
    //std::cout << "RedisDBManager::getCellList,  busboardID: " << busboardID << "  cellCount: " << cellCount << std::endl;


    std::vector<Cell> cells;
    for(int i = 0; i < cellIDList.size(); i++){
        //std::cout << "fetching: " << busboardID << "  i: " << i << std::endl;

        std::string jsonString;
        std::future<cpp_redis::reply> future_reply = m_client.hget(DB_CELLTABLE_KEY, cellIDList.at(i));
        m_client.sync_commit();

        // Wait for the reply with a timeout of 1 second
        if (future_reply.wait_for(std::chrono::seconds(1)) == std::future_status::timeout) {
            std::cerr << "Error: Timeout occurred while waiting for reply" << std::endl;
            cells.push_back(Cell());
            continue;
        } else {
            // Get the reply
            cpp_redis::reply reply = future_reply.get();

            // Check if the reply is a string
            if (reply.is_string()) {
                jsonString = reply.as_string();
            } else {
                cells.push_back(Cell());
                continue;
            }
        }
        Cell cell;

        // Parse the JSON string into a RapidJSON Value object
        Document jsonDocument;
        jsonDocument.Parse(jsonString.c_str());

        // Pass the Value object from the parsed JSON to fromJSON method
        cell.fromJSON(jsonDocument[DB_CELLJSON_KEY]);
        cells.push_back(cell);
    }
    return cells;
}

std::vector<CellTarget> RedisDBManager::getCellTargets(std::vector<std::string> cellIDList)
{

    std::vector<CellTarget> celltargets;
    for(int i = 0; i < cellIDList.size(); i++){
        //std::cout << "fetching: " << busboardID << "  i: " << i << std::endl;

        if(cellIDList.at(i).empty()){
            continue;
        }

        std::string jsonString;
        std::future<cpp_redis::reply> future_reply = m_client.hget(DB_TARGETTABLE_KEY, cellIDList.at(i));
        m_client.sync_commit();


        // Wait for the reply with a timeout of 1 second
        if (future_reply.wait_for(std::chrono::seconds(1)) == std::future_status::timeout) {
            std::cerr << "Error: Timeout occurred while waiting for reply" << std::endl;
            // Handle timeout error
        } else {
            // Get the reply
            cpp_redis::reply reply = future_reply.get();

            // Check if the reply is a string
            if (reply.is_string()) {
                jsonString = reply.as_string();

                if(jsonString.empty()){
                    continue;
                }
                CellTarget celltarget;

                // Parse the JSON string into a RapidJSON Value object
                Document jsonDocument;
                jsonDocument.Parse(jsonString.c_str());

                // Pass the Value object from the parsed JSON to fromJSON method

                celltarget.fromJSON(jsonDocument[DB_TARGETJSON_KEY]);
                celltargets.push_back(celltarget);
            } else {
                //std::cerr << "Error: Failed to retrieve value: getCellTargets" << std::endl;
                return celltargets;
                // Handle other types of errors
            }
        }

    }
    return celltargets;
}

std::vector<std::string> RedisDBManager::getBusboardCellIds(std::string busboardID)
{
    std::vector<std::string> cellids;

    std::string jsonString;
    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_CELLIDS_TABLE_KEY, busboardID);
    m_client.sync_commit();
    Document doc;
    // Wait for the reply with a timeout of 1 second
    if (future_reply.wait_for(std::chrono::seconds(1)) == std::future_status::timeout) {
        std::cerr << "Error: Timeout occurred while waiting for reply" << std::endl;
        // Handle timeout error
    } else {
        // Get the reply
        cpp_redis::reply reply = future_reply.get();

        // Check if the reply is a string
        if (reply.is_string()) {
            doc.Parse(reply.as_string().c_str());
        } else {
            std::cerr << "Error: Failed to retrieve value: getBusboardCellIds" << std::endl;
            // Handle other types of errors
            return std::vector<std::string>();

        }
    }

    // Iterate over the JSON array and extract strings
    for (SizeType i = 0; i < doc.Size(); ++i) {
        if (doc[i].IsString()) {
            cellids.push_back(doc[i].GetString());
        }
    }

    return cellids;
}

bool RedisDBManager::pushCellTarget(CellTarget celltarget)
{
    Document document;
    document.SetObject();

    // Pass the document's allocator to toJSON method to allocate memory for JSON objects
    Document::AllocatorType& allocator = document.GetAllocator();

    // Convert the Cell object to JSON
    Value cellJSON = celltarget.toJSON(allocator);

    // Add the cellJSON object to the main JSON document
    document.AddMember(DB_TARGETJSON_KEY, cellJSON, allocator);

    // Prepare writer and string buffer to convert JSON to string
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    // Get the JSON string
    std::string jsonString = buffer.GetString();
    std::future<cpp_redis::reply> reply = m_client.hset(DB_TARGETTABLE_KEY, celltarget.cellID(), jsonString);

    //std::cout << "FROMBOARD <--- pushing jsonString: " << jsonString << std::endl;

    m_client.sync_commit();
}

bool RedisDBManager::pushCellTargets(std::vector<CellTarget> celltargets)
{
    for(int i = 0; i < celltargets.size(); i++){
        //std::cout << "pushing cell no: " << i << std::endl;
        Document document;
        document.SetObject();

        // Pass the document's allocator to toJSON method to allocate memory for JSON objects
        Document::AllocatorType& allocator = document.GetAllocator();

        // Convert the Cell object to JSON
        Value cellJSON = celltargets.at(i).toJSON(allocator);

        // Add the cellJSON object to the main JSON document
        document.AddMember(DB_TARGETJSON_KEY, cellJSON, allocator);

        // Prepare writer and string buffer to convert JSON to string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);

        // Get the JSON string
        std::string jsonString = buffer.GetString();
        std::future<cpp_redis::reply> reply = m_client.hset(DB_TARGETTABLE_KEY, celltargets.at(i).cellID(), jsonString);

        //std::cout << "FROMBOARD <--- pushing jsonString: " << jsonString << std::endl;

        m_client.sync_commit();
    }
}

bool RedisDBManager::pushBusboardCellIds(std::string busboardID, std::vector<std::string> cellIDs)
{
    Document doc;
    doc.SetArray(); // Set it as an array
    Document::AllocatorType& allocator = doc.GetAllocator();

    // Convert vector of strings to RapidJSON array
    for (const auto& str : cellIDs) {
        Value value(str.c_str(), doc.GetAllocator()); // Convert string to RapidJSON Value
        doc.PushBack(value, doc.GetAllocator()); // Push it into the document array
    }

    // Prepare writer and string buffer to convert JSON to string
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    // Get the JSON string
    std::string jsonString = buffer.GetString();
    std::future<cpp_redis::reply> reply = m_client.hset(DB_CELLIDS_TABLE_KEY, busboardID, jsonString);

    //std::cout << "FROMBOARD <--- pushing jsonString: " << jsonString << std::endl;

    m_client.sync_commit();
}

bool RedisDBManager::pushCellList(std::vector<Cell> cells)
{
    if(cells.size() > 0){
        //std::cout << "RedisDBManager::pushCellLis" << "   cell0 cost: " << cells.at(0).lastUpdatedTimestamp() - m_prevTimestamp << std::endl;

        m_prevTimestamp = cells.at(0).lastUpdatedTimestamp();
    }

    for(int i = 0; i < cells.size(); i++){
        //std::cout << "pushing cell no: " << i << std::endl;

        cells.at(i).setLastUpdatedTimestamp(Cell::getCurrentTimeMillis());
        Document document;
        document.SetObject();

        // Pass the document's allocator to toJSON method to allocate memory for JSON objects
        Document::AllocatorType& allocator = document.GetAllocator();

        // Convert the Cell object to JSON
        Value cellJSON = cells.at(i).toJSON(allocator);

        // Add the cellJSON object to the main JSON document
        document.AddMember(DB_CELLJSON_KEY, cellJSON, allocator);

        // Prepare writer and string buffer to convert JSON to string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);

        // Get the JSON string
        std::string jsonString = buffer.GetString();
        std::future<cpp_redis::reply> reply = m_client.hset(DB_CELLTABLE_KEY, cells.at(i).cellID(), jsonString);

        //std::cout << "FROMBOARD <--- pushing jsonString: " << jsonString << std::endl;

        m_client.sync_commit();
    }
}

bool RedisDBManager::pushCellVisuals(std::string cellID, CellVisualsHistory history)
{
    /*
    for(int i = 0; i < cells.size(); i++){
        //std::cout << "pushing cell no: " << i << std::endl;

        Document document;
        document.SetObject();

        // Pass the document's allocator to toJSON method to allocate memory for JSON objects
        Document::AllocatorType& allocator = document.GetAllocator();

        // Convert the Cell object to JSON
        Value cellJSON = cells.at(i).toJSON(allocator);

        // Add the cellJSON object to the main JSON document
        document.AddMember("cell", cellJSON, allocator);

        // Prepare writer and string buffer to convert JSON to string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        document.Accept(writer);

        // Get the JSON string
        std::string jsonString = buffer.GetString();
        std::future<cpp_redis::reply> reply = m_client.hset(busboardID, std::to_string(cells.at(i).positionIdx()-1), jsonString);

        //std::cout << "FROMBOARD <--- pushing jsonString: " << jsonString << std::endl;

        m_client.sync_commit();
    }
*/
}

CellVisualsHistory RedisDBManager::getCellVisualsHistory(std::string cellID)
{
    CellVisualsHistory hist(std::vector<CellVisuals>(1));
    return hist;
}


bool RedisDBManager::isConnected()
{
    return m_client.is_connected();
}
