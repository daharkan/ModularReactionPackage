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
    if (!ensureHashKey(DB_CELLVISUALS_TABLE_KEY)) {
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
    std::vector<std::string> requestedIds;
    requestedIds.reserve(cellIDList.size());
    for (const auto& cellId : cellIDList) {
        if (!cellId.empty()) {
            requestedIds.push_back(cellId);
        }
    }

    if (requestedIds.empty()) {
        return celltargets;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hmget(DB_TARGETTABLE_KEY, requestedIds);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return celltargets;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_array()) {
        return celltargets;
    }

    const auto& values = reply.as_array();
    for (size_t i = 0; i < values.size() && i < requestedIds.size(); ++i) {
        const auto& valueReply = values.at(i);
        if (!valueReply.is_string()) {
            continue;
        }

        std::string jsonString = valueReply.as_string();
        if (jsonString.empty()) {
            continue;
        }

        CellTarget celltarget;

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

        celltarget.fromJSON(*targetValue);
        celltargets.push_back(celltarget);
    }
    return celltargets;
}

bool RedisDBManager::pushExperiment(const Experiment &experiment)
{
    if (!m_client.is_connected()) {
        return false;
    }

    if (experiment.experimentId().empty()) {
        return false;
    }

    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    Value experimentJSON = experiment.toJSON(allocator);
    Value experimentWrapper(experimentJSON, allocator);
    document.AddMember(DB_EXPERIMENTJSON_KEY, experimentWrapper, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_EXPERIMENTS_TABLE_KEY, experiment.experimentId(), jsonString);
    m_client.sync_commit();
    return true;
}

std::optional<Experiment> RedisDBManager::getExperiment(const std::string &experimentId)
{
    if (!m_client.is_connected()) {
        return std::nullopt;
    }

    if (experimentId.empty()) {
        return std::nullopt;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_EXPERIMENTS_TABLE_KEY, experimentId);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return std::nullopt;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_string()) {
        return std::nullopt;
    }

    std::string jsonString = reply.as_string();
    if (jsonString.empty()) {
        return std::nullopt;
    }

    Document jsonDocument;
    jsonDocument.Parse(jsonString.c_str());

    const Value* experimentValue = nullptr;
    if (jsonDocument.IsObject() && jsonDocument.HasMember(DB_EXPERIMENTJSON_KEY) && jsonDocument[DB_EXPERIMENTJSON_KEY].IsObject()) {
        experimentValue = &jsonDocument[DB_EXPERIMENTJSON_KEY];
    } else if (jsonDocument.IsObject()) {
        experimentValue = &jsonDocument;
    }

    if (!experimentValue) {
        return std::nullopt;
    }

    Experiment experiment;
    experiment.fromJSON(*experimentValue);
    if (experiment.experimentId().empty()) {
        experiment.setExperimentId(experimentId);
    }
    return experiment;
}

std::vector<Experiment> RedisDBManager::getExperiments()
{
    std::vector<Experiment> experiments;
    if (!m_client.is_connected()) {
        return experiments;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hgetall(DB_EXPERIMENTS_TABLE_KEY);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return experiments;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_array()) {
        return experiments;
    }

    const auto& values = reply.as_array();
    for (size_t i = 0; i + 1 < values.size(); i += 2) {
        const auto& keyReply = values.at(i);
        const auto& valueReply = values.at(i + 1);
        if (!keyReply.is_string() || !valueReply.is_string()) {
            continue;
        }

        std::string experimentId = keyReply.as_string();
        std::string jsonString = valueReply.as_string();
        if (jsonString.empty()) {
            continue;
        }

        Document jsonDocument;
        jsonDocument.Parse(jsonString.c_str());

        const Value* experimentValue = nullptr;
        if (jsonDocument.IsObject() && jsonDocument.HasMember(DB_EXPERIMENTJSON_KEY) && jsonDocument[DB_EXPERIMENTJSON_KEY].IsObject()) {
            experimentValue = &jsonDocument[DB_EXPERIMENTJSON_KEY];
        } else if (jsonDocument.IsObject()) {
            experimentValue = &jsonDocument;
        }

        if (!experimentValue) {
            continue;
        }

        Experiment experiment;
        experiment.fromJSON(*experimentValue);
        if (experiment.experimentId().empty()) {
            experiment.setExperimentId(experimentId);
        }
        experiments.push_back(experiment);
    }

    return experiments;
}

bool RedisDBManager::deleteExperiment(const std::string &experimentId)
{
    if (!m_client.is_connected()) {
        return false;
    }

    if (experimentId.empty()) {
        return false;
    }

    m_client.hdel(DB_EXPERIMENTS_TABLE_KEY, {experimentId});
    m_client.sync_commit();
    return true;
}

bool RedisDBManager::pushUser(const User &user)
{
    if (!m_client.is_connected()) {
        return false;
    }

    if (user.username().empty()) {
        return false;
    }

    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    Value userJSON = user.toJSON(allocator);
    Value userWrapper(userJSON, allocator);
    document.AddMember(DB_USERJSON_KEY, userWrapper, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_USERS_TABLE_KEY, user.username(), jsonString);
    m_client.sync_commit();
    return true;
}

std::optional<User> RedisDBManager::getUser(const std::string &username)
{
    if (!m_client.is_connected()) {
        return std::nullopt;
    }

    if (username.empty()) {
        return std::nullopt;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_USERS_TABLE_KEY, username);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return std::nullopt;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_string()) {
        return std::nullopt;
    }

    std::string jsonString = reply.as_string();
    if (jsonString.empty()) {
        return std::nullopt;
    }

    Document jsonDocument;
    jsonDocument.Parse(jsonString.c_str());

    const Value* userValue = nullptr;
    if (jsonDocument.IsObject() && jsonDocument.HasMember(DB_USERJSON_KEY) && jsonDocument[DB_USERJSON_KEY].IsObject()) {
        userValue = &jsonDocument[DB_USERJSON_KEY];
    } else if (jsonDocument.IsObject()) {
        userValue = &jsonDocument;
    }

    if (!userValue) {
        return std::nullopt;
    }

    User user;
    user.fromJSON(*userValue);
    if (user.username().empty()) {
        user.setUsername(username);
    }
    return user;
}

std::vector<User> RedisDBManager::getUsers()
{
    std::vector<User> users;
    if (!m_client.is_connected()) {
        return users;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hgetall(DB_USERS_TABLE_KEY);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return users;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_array()) {
        return users;
    }

    const auto& values = reply.as_array();
    for (size_t i = 0; i + 1 < values.size(); i += 2) {
        const auto& keyReply = values.at(i);
        const auto& valueReply = values.at(i + 1);
        if (!keyReply.is_string() || !valueReply.is_string()) {
            continue;
        }

        std::string username = keyReply.as_string();
        std::string jsonString = valueReply.as_string();
        if (jsonString.empty()) {
            continue;
        }

        Document jsonDocument;
        jsonDocument.Parse(jsonString.c_str());

        const Value* userValue = nullptr;
        if (jsonDocument.IsObject() && jsonDocument.HasMember(DB_USERJSON_KEY) && jsonDocument[DB_USERJSON_KEY].IsObject()) {
            userValue = &jsonDocument[DB_USERJSON_KEY];
        } else if (jsonDocument.IsObject()) {
            userValue = &jsonDocument;
        }

        if (!userValue) {
            continue;
        }

        User user;
        user.fromJSON(*userValue);
        if (user.username().empty()) {
            user.setUsername(username);
        }
        users.push_back(user);
    }

    return users;
}

bool RedisDBManager::deleteUser(const std::string &username)
{
    if (!m_client.is_connected()) {
        return false;
    }

    if (username.empty()) {
        return false;
    }

    m_client.hdel(DB_USERS_TABLE_KEY, {username});
    m_client.sync_commit();
    return true;
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
    std::future<cpp_redis::reply> future_reply = m_client.hkeys(DB_BUSBOARD_TABLE_KEY);
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

std::vector<std::string> RedisDBManager::getCellIds()
{
    std::vector<std::string> ids;
    std::future<cpp_redis::reply> future_reply = m_client.hkeys(DB_CELLTABLE_KEY);
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
    document.AddMember(DB_BUSBOARD_LAST_UPDATED_KEY, static_cast<uint64_t>(Cell::getCurrentTimeMillis()), allocator);
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_BUSBOARD_TABLE_KEY, busboardID, jsonString);
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
    doc.AddMember(DB_BUSBOARD_LAST_UPDATED_KEY, static_cast<uint64_t>(Cell::getCurrentTimeMillis()), allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_BUSBOARD_TABLE_KEY, busboardID, jsonString);
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
    if (cellID.empty()) {
        return false;
    }

    CellVisualsHistory mergedHistory;
    if (!history.visuals().empty()) {
        std::future<cpp_redis::reply> future_reply = m_client.hget(DB_CELLVISUALS_TABLE_KEY, cellID);
        m_client.sync_commit();

        if (!replyTimedOut(future_reply)) {
            cpp_redis::reply reply = future_reply.get();
            if (reply.is_string()) {
                const std::string jsonString = reply.as_string();
                if (!jsonString.empty()) {
                    Document jsonDocument;
                    jsonDocument.Parse(jsonString.c_str());
                    const Value* historyValue = nullptr;
                    if (jsonDocument.IsObject() && jsonDocument.HasMember(DB_CELLVISUALS_JSON_KEY)
                        && jsonDocument[DB_CELLVISUALS_JSON_KEY].IsObject()) {
                        historyValue = &jsonDocument[DB_CELLVISUALS_JSON_KEY];
                    } else if (jsonDocument.IsObject()) {
                        historyValue = &jsonDocument;
                    }
                    if (historyValue && historyValue->IsObject()) {
                        mergedHistory.fromJSON(*historyValue);
                    }
                }
            }
        }
    }

    mergedHistory.appendHistory(history);

    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();
    Value historyJSON = mergedHistory.toJSON(allocator);
    Value historyWrapper(historyJSON, allocator);
    document.AddMember(DB_CELLVISUALS_JSON_KEY, historyWrapper, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string jsonString = buffer.GetString();
    std::future<cpp_redis::reply> reply = m_client.hset(DB_CELLVISUALS_TABLE_KEY, cellID, jsonString);
    m_client.sync_commit();
    return true;
}

CellVisualsHistory RedisDBManager::getCellVisualsHistory(std::string cellID)
{
    CellVisualsHistory hist;
    if (cellID.empty()) {
        return hist;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_CELLVISUALS_TABLE_KEY, cellID);
    m_client.sync_commit();
    if (replyTimedOut(future_reply)) {
        return hist;
    }
    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_string()) {
        return hist;
    }

    const std::string jsonString = reply.as_string();
    if (jsonString.empty()) {
        return hist;
    }

    Document jsonDocument;
    jsonDocument.Parse(jsonString.c_str());

    const Value* historyValue = nullptr;
    if (jsonDocument.IsObject() && jsonDocument.HasMember(DB_CELLVISUALS_JSON_KEY)
        && jsonDocument[DB_CELLVISUALS_JSON_KEY].IsObject()) {
        historyValue = &jsonDocument[DB_CELLVISUALS_JSON_KEY];
    } else if (jsonDocument.IsObject()) {
        historyValue = &jsonDocument;
    }

    if (!historyValue || !historyValue->IsObject()) {
        return hist;
    }

    hist.fromJSON(*historyValue);
    return hist;
}


bool RedisDBManager::isConnected()
{
    return m_client.is_connected();
}
