#include "RedisDBManager.h"
#include <chrono>
#include <optional>

namespace {
constexpr std::chrono::seconds kRedisTimeout(1);

bool replyTimedOut(std::future<cpp_redis::reply>& future_reply) {
    if (future_reply.wait_for(kRedisTimeout) == std::future_status::timeout) {
        std::cerr << "Error: Timeout occurred while waiting for reply" << std::endl;
        return true;
    }
    return false;
}

std::vector<std::string> parseCellIdArray(const Value& arrayValue) {
    std::vector<std::string> cellIds;
    if (!arrayValue.IsArray()) {
        return cellIds;
    }

    for (SizeType i = 0; i < arrayValue.Size(); ++i) {
        if (arrayValue[i].IsString()) {
            std::string id = arrayValue[i].GetString();
            if (!id.empty()) {
                cellIds.push_back(id);
            }
        }
    }
    return cellIds;
}
} // namespace


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
    if (!m_client.is_connected()) {
        return false;
    }
    return initializeSchema(false);
}

bool RedisDBManager::initializeSchema(bool reset)
{
    if (!m_client.is_connected()) {
        return false;
    }

    if (reset) {
        m_client.flushdb();
        m_client.sync_commit();
    }

    auto getKeyType = [&](const std::string& key) -> std::optional<std::string> {
        std::future<cpp_redis::reply> future_reply = m_client.type(key);
        m_client.sync_commit();
        if (replyTimedOut(future_reply)) {
            return std::nullopt;
        }

        cpp_redis::reply reply = future_reply.get();
        if (!reply.is_string()) {
            return std::nullopt;
        }
        return reply.as_string();
    };

    auto ensureHashKey = [&](const std::string& key) -> bool {
        std::optional<std::string> keyType = getKeyType(key);
        if (!keyType.has_value()) {
            return false;
        }
        if (keyType.value() == "none") {
            m_client.hset(key, "__schema__", "1");
            m_client.sync_commit();
            return true;
        }
        if (keyType.value() == "hash") {
            m_client.hsetnx(key, "__schema__", "1");
            m_client.sync_commit();
            return true;
        }
        return false;
    };

    if (!ensureHashKey(DB_CELLTABLE_KEY)) {
        return false;
    }
    if (!ensureHashKey(DB_TARGETTABLE_KEY)) {
        return false;
    }
    if (!ensureHashKey(DB_USERS_TABLE_KEY)) {
        return false;
    }
    if (!ensureHashKey(DB_EXPERIMENTS_TABLE_KEY)) {
        return false;
    }
    if (!ensureHashKey(DB_BUSBOARD_TABLE_KEY)) {
        return false;
    }

    std::optional<std::string> schemaKeyType = getKeyType(DB_SCHEMA_VERSION_KEY);
    if (!schemaKeyType.has_value()) {
        return false;
    }

    if (schemaKeyType.value() == "none") {
        m_client.set(DB_SCHEMA_VERSION_KEY, DB_SCHEMA_VERSION_VALUE);
        m_client.sync_commit();
    } else if (schemaKeyType.value() != "string") {
        return false;
    }

    return true;
}

std::vector<Cell> RedisDBManager::getCellList(std::vector<std::string> cellIDList)
{
    //std::cout << "RedisDBManager::getCellList,  busboardID: " << busboardID << "  cellCount: " << cellCount << std::endl;


    std::vector<Cell> cells;
    for(int i = 0; i < cellIDList.size(); i++){
        //std::cout << "fetching: " << busboardID << "  i: " << i << std::endl;
        if (cellIDList.at(i).empty()) {
            continue;
        }

        std::string jsonString;
        std::future<cpp_redis::reply> future_reply = m_client.hget(DB_CELLTABLE_KEY, cellIDList.at(i));
        m_client.sync_commit();

        // Wait for the reply with a timeout of 1 second
        if (replyTimedOut(future_reply)) {
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

        const Value* cellValue = nullptr;
        if (jsonDocument.IsObject() && jsonDocument.HasMember(DB_CELLJSON_KEY) && jsonDocument[DB_CELLJSON_KEY].IsObject()) {
            cellValue = &jsonDocument[DB_CELLJSON_KEY];
        } else if (jsonDocument.IsObject()) {
            cellValue = &jsonDocument;
        }

        if (!cellValue) {
            cells.push_back(Cell());
            continue;
        }

        // Pass the Value object from the parsed JSON to fromJSON method
        cell.fromJSON(*cellValue);
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
        if (replyTimedOut(future_reply)) {
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

                const Value* targetValue = nullptr;
                if (jsonDocument.IsObject() && jsonDocument.HasMember(DB_TARGETJSON_KEY) && jsonDocument[DB_TARGETJSON_KEY].IsObject()) {
                    targetValue = &jsonDocument[DB_TARGETJSON_KEY];
                } else if (jsonDocument.IsObject()) {
                    targetValue = &jsonDocument;
                }

                if (!targetValue) {
                    continue;
                }

                // Pass the Value object from the parsed JSON to fromJSON method
                celltarget.fromJSON(*targetValue);
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
    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_BUSBOARD_TABLE_KEY, busboardID);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return {};
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_string()) {
        return {};
    }

    Document doc;
    doc.Parse(reply.as_string().c_str());
    if (!doc.IsObject() || !doc.HasMember(DB_BUSBOARD_CELLIDS_KEY)) {
        return {};
    }

    return parseCellIdArray(doc[DB_BUSBOARD_CELLIDS_KEY]);
}

std::vector<std::string> RedisDBManager::getBusboardIds()
{
    std::vector<std::string> ids;
    std::future<cpp_redis::reply> future_reply = m_client.smembers(DB_BUSBOARD_IDS_KEY);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return ids;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_array()) {
        return ids;
    }

    for (const auto& item : reply.as_array()) {
        if (item.is_string()) {
            ids.push_back(item.as_string());
        }
    }

    return ids;
}

bool RedisDBManager::pushCellTarget(CellTarget celltarget)
{
    Document document;
    document.SetObject();

    // Pass the document's allocator to toJSON method to allocate memory for JSON objects
    Document::AllocatorType& allocator = document.GetAllocator();

    // Convert the Cell object to JSON
    Value cellJSON = celltarget.toJSON(allocator);

    Value cellJSONWrapper(cellJSON, allocator);

    // Add the cellJSON object to the main JSON document
    document.AddMember(DB_TARGETJSON_KEY, cellJSONWrapper, allocator);

    // Prepare writer and string buffer to convert JSON to string
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    // Get the JSON string
    std::string jsonString = buffer.GetString();
    std::future<cpp_redis::reply> reply = m_client.hset(DB_TARGETTABLE_KEY, celltarget.cellID(), jsonString);

    //std::cout << "FROMBOARD <--- pushing jsonString: " << jsonString << std::endl;

    m_client.sync_commit();
    return true;
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

        Value cellJSONWrapper(cellJSON, allocator);

        // Add the cellJSON object to the main JSON document
        document.AddMember(DB_TARGETJSON_KEY, cellJSONWrapper, allocator);

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
    return true;
}

bool RedisDBManager::pushFlowStatus(const std::string& busboardID, const FlowStatus& flowStatus)
{
    if (busboardID.empty()) {
        return false;
    }

    Document document;
    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_BUSBOARD_TABLE_KEY, busboardID);
    m_client.sync_commit();

    if (!replyTimedOut(future_reply)) {
        cpp_redis::reply reply = future_reply.get();
        if (reply.is_string()) {
            document.Parse(reply.as_string().c_str());
        }
    }

    if (!document.IsObject()) {
        document.SetObject();
    }

    Document::AllocatorType& allocator = document.GetAllocator();
    document.RemoveMember("flowRateLpm");
    document.RemoveMember("flowTemp");
    document.RemoveMember("timestamp");
    document.RemoveMember("flowstatus");

    Value flowJSON = flowStatus.toJSON(allocator);
    if (document.HasMember(DB_BUSBOARD_FLOW_KEY)) {
        document[DB_BUSBOARD_FLOW_KEY].CopyFrom(flowJSON, allocator);
    } else {
        document.AddMember(DB_BUSBOARD_FLOW_KEY, flowJSON, allocator);
    }
    if (!document.HasMember("busboardID")) {
        document.AddMember("busboardID", Value(busboardID.c_str(), allocator), allocator);
    }

    document.RemoveMember(DB_BUSBOARD_LAST_UPDATED_KEY);
    document.AddMember(DB_BUSBOARD_LAST_UPDATED_KEY, Cell::getCurrentTimeMillis(), allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_BUSBOARD_TABLE_KEY, busboardID, jsonString);
    m_client.sadd(DB_BUSBOARD_IDS_KEY, {busboardID});
    m_client.sync_commit();
    return true;
}

FlowStatus RedisDBManager::getFlowStatus(const std::string& busboardID)
{
    FlowStatus flowStatus;
    if (busboardID.empty()) {
        return flowStatus;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_BUSBOARD_TABLE_KEY, busboardID);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return flowStatus;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_string()) {
        return flowStatus;
    }

    Document document;
    document.Parse(reply.as_string().c_str());
    const Value* flowValue = nullptr;
    if (document.IsObject() && document.HasMember(DB_BUSBOARD_FLOW_KEY) && document[DB_BUSBOARD_FLOW_KEY].IsObject()) {
        flowValue = &document[DB_BUSBOARD_FLOW_KEY];
    }

    if (flowValue) {
        flowStatus.fromJSON(*flowValue);
    }
    return flowStatus;
}

bool RedisDBManager::pushBusboardCellIds(std::string busboardID, std::vector<std::string> cellIDs)
{
    if (busboardID.empty()) {
        return false;
    }

    Document doc;
    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_BUSBOARD_TABLE_KEY, busboardID);
    m_client.sync_commit();

    if (!replyTimedOut(future_reply)) {
        cpp_redis::reply reply = future_reply.get();
        if (reply.is_string()) {
            doc.Parse(reply.as_string().c_str());
        }
    }

    if (!doc.IsObject()) {
        doc.SetObject();
    }

    Document::AllocatorType& allocator = doc.GetAllocator();
    if (!doc.HasMember("busboardID")) {
        doc.AddMember("busboardID", Value(busboardID.c_str(), allocator), allocator);
    }

    Value cellIdArray(kArrayType);
    for (const auto& str : cellIDs) {
        if (str.empty()) {
            continue;
        }
        cellIdArray.PushBack(Value(str.c_str(), allocator), allocator);
    }

    if (doc.HasMember(DB_BUSBOARD_CELLIDS_KEY)) {
        doc[DB_BUSBOARD_CELLIDS_KEY].CopyFrom(cellIdArray, allocator);
    } else {
        doc.AddMember(DB_BUSBOARD_CELLIDS_KEY, cellIdArray, allocator);
    }

    doc.RemoveMember(DB_BUSBOARD_LAST_UPDATED_KEY);
    doc.AddMember(DB_BUSBOARD_LAST_UPDATED_KEY, Cell::getCurrentTimeMillis(), allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_BUSBOARD_TABLE_KEY, busboardID, jsonString);
    m_client.sadd(DB_BUSBOARD_IDS_KEY, {busboardID});
    m_client.sync_commit();
    return true;
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

        Value cellJSONWrapper(cellJSON, allocator);

        // Add the cellJSON object to the main JSON document
        document.AddMember(DB_CELLJSON_KEY, cellJSONWrapper, allocator);

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
    return true;
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
