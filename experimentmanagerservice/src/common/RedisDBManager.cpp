#include "RedisDBManager.h"
#include <chrono>
#include <optional>
#include <algorithm>
#include <cctype>

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

struct BusboardLocator {
    std::string machineId;
    std::string side;
};

std::optional<BusboardLocator> parseBusboardLocator(const std::string& busboardId) {
    if (busboardId.empty()) {
        return std::nullopt;
    }

    std::string upper = busboardId;
    std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });

    std::string side;
    if (upper.find("LHS") != std::string::npos) {
        side = "LHS";
    } else if (upper.find("RHS") != std::string::npos) {
        side = "RHS";
    } else {
        return std::nullopt;
    }

    std::string digits;
    for (char c : upper) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digits.push_back(c);
        }
    }
    if (digits.empty()) {
        return std::nullopt;
    }
    if (digits.size() < 3) {
        digits.insert(digits.begin(), 3 - digits.size(), '0');
    }

    return BusboardLocator{digits, side};
}

Value* ensureBusboardsObject(Document& doc, Document::AllocatorType& allocator) {
    if (!doc.HasMember(DB_MACHINE_BUSBOARDS_KEY) || !doc[DB_MACHINE_BUSBOARDS_KEY].IsObject()) {
        doc.RemoveMember(DB_MACHINE_BUSBOARDS_KEY);
        doc.AddMember(DB_MACHINE_BUSBOARDS_KEY, Value(kObjectType), allocator);
    }
    return &doc[DB_MACHINE_BUSBOARDS_KEY];
}

Value* ensureBusboardEntry(Value& busboards,
                           const BusboardLocator& locator,
                           Document::AllocatorType& allocator) {
    if (!busboards.HasMember(locator.side.c_str())) {
        Value key(locator.side.c_str(), allocator);
        busboards.AddMember(key, Value(kObjectType), allocator);
    }
    Value& entry = busboards[locator.side.c_str()];
    if (!entry.IsObject()) {
        entry.SetObject();
    }
    return &entry;
}

Value buildSlotsArray(const std::vector<int>& slotStates,
                      Document::AllocatorType& allocator,
                      const Value* existingSlots) {
    Value mslots(kArrayType);
    const SizeType count = static_cast<SizeType>(slotStates.size());
    mslots.Reserve(count, allocator);

    for (SizeType i = 0; i < count; ++i) {
        Value slot(kObjectType);
        bool plugged = slotStates.at(i) == 1;
        std::string cellId;
        if (existingSlots && existingSlots->IsArray() && i < existingSlots->Size()) {
            const Value& existingSlot = (*existingSlots)[i];
            if (existingSlot.IsObject()
                && existingSlot.HasMember(DB_MACHINE_SLOT_CELLID_KEY)
                && existingSlot[DB_MACHINE_SLOT_CELLID_KEY].IsString()) {
                cellId = existingSlot[DB_MACHINE_SLOT_CELLID_KEY].GetString();
            }
        }
        if (!plugged) {
            cellId.clear();
        }
        slot.AddMember(DB_MACHINE_SLOT_PLUGGED_KEY, plugged, allocator);
        slot.AddMember(DB_MACHINE_SLOT_CELLID_KEY, Value(cellId.c_str(), allocator), allocator);
        mslots.PushBack(slot, allocator);
    }
    return mslots;
}
} // namespace


RedisDBManager* RedisDBManager::m_instance = nullptr;
std::string RedisDBManager::m_defaultHost = "127.0.0.1";
int RedisDBManager::m_defaultPort = 6379;

RedisDBManager::RedisDBManager(): QObject() {

}



RedisDBManager* RedisDBManager::getInstance()
{
    if(m_instance == nullptr){
        m_instance = new RedisDBManager();
    }
    return m_instance;
}

void RedisDBManager::setDefaultConnection(const std::string &hostname, int port)
{
    if (!hostname.empty()) {
        m_defaultHost = hostname;
    }
    if (port > 0 && port <= 65535) {
        m_defaultPort = port;
    }
}

const std::string &RedisDBManager::defaultHost()
{
    return m_defaultHost;
}

int RedisDBManager::defaultPort()
{
    return m_defaultPort;
}

bool RedisDBManager::connectToDB(std::string hostname, int port)
{
    m_client.connect(hostname, port);
    if (!m_client.is_connected()) {
        return false;
    }
    return initializeSchema(false);
}

bool RedisDBManager::connectToDefault()
{
    if (m_client.is_connected()) {
        return true;
    }
    return connectToDB(m_defaultHost, m_defaultPort);
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
    if (!ensureHashKey(DB_USERS_TABLE_KEY)) {
        return false;
    }
    if (!ensureHashKey(DB_EXPERIMENTS_TABLE_KEY)) {
        return false;
    }
    if (!ensureHashKey(DB_MACHINE_TABLE_KEY)) {
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

std::vector<std::string> RedisDBManager::getMachineIds()
{
    std::vector<std::string> ids;
    std::future<cpp_redis::reply> future_reply = m_client.hkeys(DB_MACHINE_TABLE_KEY);
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
            std::string id = item.as_string();
            if (id == "__schema__") {
                continue;
            }
            ids.push_back(id);
        }
    }

    return ids;
}

bool RedisDBManager::getMachineBusboardIds(const std::string &machineId, std::string *lhsId, std::string *rhsId)
{
    if (lhsId) {
        lhsId->clear();
    }
    if (rhsId) {
        rhsId->clear();
    }

    if (machineId.empty()) {
        return false;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_MACHINE_TABLE_KEY, machineId);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return false;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_string()) {
        return false;
    }

    Document doc;
    doc.Parse(reply.as_string().c_str());
    if (!doc.IsObject() || !doc.HasMember(DB_MACHINE_BUSBOARDS_KEY)) {
        return false;
    }

    const Value& busboards = doc[DB_MACHINE_BUSBOARDS_KEY];
    if (!busboards.IsObject()) {
        return false;
    }

    auto readBusboardId = [&busboards](const char *side) -> std::string {
        if (!busboards.HasMember(side)) {
            return {};
        }
        const Value& entry = busboards[side];
        if (!entry.IsObject() || !entry.HasMember("busboardID") || !entry["busboardID"].IsString()) {
            return {};
        }
        return entry["busboardID"].GetString();
    };

    if (lhsId) {
        *lhsId = readBusboardId("LHS");
    }
    if (rhsId) {
        *rhsId = readBusboardId("RHS");
    }

    return true;
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

bool RedisDBManager::pushMachineStatus(const std::string& busboardID, const std::vector<int>& slotStates)
{
    auto locator = parseBusboardLocator(busboardID);
    if (!locator.has_value() || slotStates.empty()) {
        return false;
    }

    Document doc;
    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_MACHINE_TABLE_KEY, locator->machineId);
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
    if (!doc.HasMember("machineId")) {
        doc.AddMember("machineId", Value(locator->machineId.c_str(), allocator), allocator);
    }

    Value *busboards = ensureBusboardsObject(doc, allocator);
    Value *busboardEntry = ensureBusboardEntry(*busboards, locator.value(), allocator);

    const Value* existingSlots = nullptr;
    if (busboardEntry->HasMember(DB_MACHINE_SLOTS_KEY)) {
        existingSlots = &(*busboardEntry)[DB_MACHINE_SLOTS_KEY];
    }

    Value mslots = buildSlotsArray(slotStates, allocator, existingSlots);
    if (busboardEntry->HasMember(DB_MACHINE_SLOTS_KEY)) {
        (*busboardEntry)[DB_MACHINE_SLOTS_KEY].CopyFrom(mslots, allocator);
    } else {
        busboardEntry->AddMember(DB_MACHINE_SLOTS_KEY, mslots, allocator);
    }

    if (!busboardEntry->HasMember("busboardID")) {
        busboardEntry->AddMember("busboardID", Value(busboardID.c_str(), allocator), allocator);
    }

    busboardEntry->RemoveMember(DB_BUSBOARD_LAST_UPDATED_KEY);
    busboardEntry->AddMember(DB_BUSBOARD_LAST_UPDATED_KEY,
                             static_cast<uint64_t>(Cell::getCurrentTimeMillis()),
                             allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_MACHINE_TABLE_KEY, locator->machineId, jsonString);
    m_client.sync_commit();
    return true;
}

bool RedisDBManager::removeBusboardFromMachine(const std::string& busboardID)
{
    auto locator = parseBusboardLocator(busboardID);
    if (!locator.has_value()) {
        return false;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_MACHINE_TABLE_KEY, locator->machineId);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return false;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_string()) {
        return false;
    }

    Document doc;
    doc.Parse(reply.as_string().c_str());
    if (!doc.IsObject() || !doc.HasMember(DB_MACHINE_BUSBOARDS_KEY)) {
        return false;
    }

    Value& busboards = doc[DB_MACHINE_BUSBOARDS_KEY];
    if (!busboards.IsObject() || !busboards.HasMember(locator->side.c_str())) {
        return false;
    }

    busboards.RemoveMember(locator->side.c_str());
    if (busboards.MemberCount() == 0) {
        m_client.hdel(DB_MACHINE_TABLE_KEY, {locator->machineId});
        m_client.sync_commit();
        return true;
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    m_client.hset(DB_MACHINE_TABLE_KEY, locator->machineId, buffer.GetString());
    m_client.sync_commit();
    return true;
}

std::vector<std::string> RedisDBManager::getBusboardCellIds(std::string busboardID)
{
    auto locator = parseBusboardLocator(busboardID);
    if (!locator.has_value()) {
        return {};
    }

    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_MACHINE_TABLE_KEY, locator->machineId);
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
    if (!doc.IsObject() || !doc.HasMember(DB_MACHINE_BUSBOARDS_KEY)) {
        return {};
    }

    const Value& busboards = doc[DB_MACHINE_BUSBOARDS_KEY];
    if (!busboards.IsObject() || !busboards.HasMember(locator->side.c_str())) {
        return {};
    }

    const Value& busboardEntry = busboards[locator->side.c_str()];
    if (!busboardEntry.IsObject() || !busboardEntry.HasMember(DB_MACHINE_SLOTS_KEY)) {
        return {};
    }

    const Value& mslots = busboardEntry[DB_MACHINE_SLOTS_KEY];
    if (!mslots.IsArray()) {
        return {};
    }

    std::vector<std::string> cellIds;
    for (SizeType i = 0; i < mslots.Size(); ++i) {
        const Value& slot = mslots[i];
        if (!slot.IsObject()) {
            continue;
        }
        if (slot.HasMember(DB_MACHINE_SLOT_PLUGGED_KEY) && slot[DB_MACHINE_SLOT_PLUGGED_KEY].IsBool()
            && !slot[DB_MACHINE_SLOT_PLUGGED_KEY].GetBool()) {
            continue;
        }
        if (slot.HasMember(DB_MACHINE_SLOT_CELLID_KEY) && slot[DB_MACHINE_SLOT_CELLID_KEY].IsString()) {
            std::string id = slot[DB_MACHINE_SLOT_CELLID_KEY].GetString();
            if (!id.empty()) {
                cellIds.push_back(id);
            }
        }
    }

    return cellIds;
}

std::vector<std::string> RedisDBManager::getBusboardIds()
{
    std::vector<std::string> ids;
    std::future<cpp_redis::reply> future_reply = m_client.hkeys(DB_MACHINE_TABLE_KEY);
    m_client.sync_commit();

    if (replyTimedOut(future_reply)) {
        return ids;
    }

    cpp_redis::reply reply = future_reply.get();
    if (!reply.is_array()) {
        return ids;
    }

    for (const auto& item : reply.as_array()) {
        if (!item.is_string()) {
            continue;
        }

        std::future<cpp_redis::reply> machine_reply = m_client.hget(DB_MACHINE_TABLE_KEY, item.as_string());
        m_client.sync_commit();
        if (replyTimedOut(machine_reply)) {
            continue;
        }
        cpp_redis::reply machineValue = machine_reply.get();
        if (!machineValue.is_string()) {
            continue;
        }

        Document doc;
        doc.Parse(machineValue.as_string().c_str());
        if (!doc.IsObject() || !doc.HasMember(DB_MACHINE_BUSBOARDS_KEY)) {
            continue;
        }
        const Value& busboards = doc[DB_MACHINE_BUSBOARDS_KEY];
        if (!busboards.IsObject()) {
            continue;
        }
        for (const char *side : {"LHS", "RHS"}) {
            if (!busboards.HasMember(side)) {
                continue;
            }
            const Value& entry = busboards[side];
            if (entry.IsObject() && entry.HasMember("busboardID") && entry["busboardID"].IsString()) {
                std::string id = entry["busboardID"].GetString();
                if (!id.empty()) {
                    ids.push_back(id);
                }
            }
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


bool RedisDBManager::pushFlowStatus(const std::string& busboardID, const FlowStatus& flowStatus)
{
    auto locator = parseBusboardLocator(busboardID);
    if (!locator.has_value()) {
        return false;
    }

    Document document;
    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_MACHINE_TABLE_KEY, locator->machineId);
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
    if (!document.HasMember("machineId")) {
        document.AddMember("machineId", Value(locator->machineId.c_str(), allocator), allocator);
    }

    Value *busboards = ensureBusboardsObject(document, allocator);
    Value *busboardEntry = ensureBusboardEntry(*busboards, locator.value(), allocator);

    Value flowJSON = flowStatus.toJSON(allocator);
    if (busboardEntry->HasMember(DB_BUSBOARD_FLOW_KEY)) {
        (*busboardEntry)[DB_BUSBOARD_FLOW_KEY].CopyFrom(flowJSON, allocator);
    } else {
        busboardEntry->AddMember(DB_BUSBOARD_FLOW_KEY, flowJSON, allocator);
    }
    if (!busboardEntry->HasMember("busboardID")) {
        busboardEntry->AddMember("busboardID", Value(busboardID.c_str(), allocator), allocator);
    }

    busboardEntry->RemoveMember(DB_BUSBOARD_LAST_UPDATED_KEY);
    busboardEntry->AddMember(DB_BUSBOARD_LAST_UPDATED_KEY,
                             static_cast<uint64_t>(Cell::getCurrentTimeMillis()),
                             allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_MACHINE_TABLE_KEY, locator->machineId, jsonString);
    m_client.sync_commit();
    return true;
}

FlowStatus RedisDBManager::getFlowStatus(const std::string& busboardID)
{
    FlowStatus flowStatus;
    auto locator = parseBusboardLocator(busboardID);
    if (!locator.has_value()) {
        return flowStatus;
    }

    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_MACHINE_TABLE_KEY, locator->machineId);
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
    if (document.IsObject() && document.HasMember(DB_MACHINE_BUSBOARDS_KEY)) {
        const Value& busboards = document[DB_MACHINE_BUSBOARDS_KEY];
        if (busboards.IsObject() && busboards.HasMember(locator->side.c_str())) {
            const Value& entry = busboards[locator->side.c_str()];
            if (entry.IsObject() && entry.HasMember(DB_BUSBOARD_FLOW_KEY)
                && entry[DB_BUSBOARD_FLOW_KEY].IsObject()) {
                flowValue = &entry[DB_BUSBOARD_FLOW_KEY];
            }
        }
    }

    if (flowValue) {
        flowStatus.fromJSON(*flowValue);
    }
    return flowStatus;
}

bool RedisDBManager::pushBusboardCellIds(std::string busboardID, std::vector<std::string> cellIDs)
{
    auto locator = parseBusboardLocator(busboardID);
    if (!locator.has_value()) {
        return false;
    }

    Document doc;
    std::future<cpp_redis::reply> future_reply = m_client.hget(DB_MACHINE_TABLE_KEY, locator->machineId);
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
    if (!doc.HasMember("machineId")) {
        doc.AddMember("machineId", Value(locator->machineId.c_str(), allocator), allocator);
    }

    Value *busboards = ensureBusboardsObject(doc, allocator);
    Value *busboardEntry = ensureBusboardEntry(*busboards, locator.value(), allocator);

    const Value* existingSlots = nullptr;
    if (busboardEntry->HasMember(DB_MACHINE_SLOTS_KEY)) {
        existingSlots = &(*busboardEntry)[DB_MACHINE_SLOTS_KEY];
    }

    Value mslots(kArrayType);
    mslots.Reserve(static_cast<SizeType>(cellIDs.size()), allocator);
    for (SizeType i = 0; i < cellIDs.size(); ++i) {
        const auto& cellId = cellIDs.at(i);
        bool plugged = false;
        std::string storedCellId = cellId;

        if (existingSlots && existingSlots->IsArray() && i < existingSlots->Size()) {
            const Value& existingSlot = (*existingSlots)[i];
            if (existingSlot.IsObject()) {
                if (existingSlot.HasMember(DB_MACHINE_SLOT_PLUGGED_KEY)
                    && existingSlot[DB_MACHINE_SLOT_PLUGGED_KEY].IsBool()) {
                    plugged = existingSlot[DB_MACHINE_SLOT_PLUGGED_KEY].GetBool();
                }
                if (storedCellId.empty()
                    && existingSlot.HasMember(DB_MACHINE_SLOT_CELLID_KEY)
                    && existingSlot[DB_MACHINE_SLOT_CELLID_KEY].IsString()) {
                    storedCellId = existingSlot[DB_MACHINE_SLOT_CELLID_KEY].GetString();
                }
            }
        }

        if (!storedCellId.empty()) {
            plugged = true;
        }

        Value slot(kObjectType);
        slot.AddMember(DB_MACHINE_SLOT_PLUGGED_KEY, plugged, allocator);
        slot.AddMember(DB_MACHINE_SLOT_CELLID_KEY, Value(storedCellId.c_str(), allocator), allocator);
        mslots.PushBack(slot, allocator);
    }

    if (busboardEntry->HasMember(DB_MACHINE_SLOTS_KEY)) {
        (*busboardEntry)[DB_MACHINE_SLOTS_KEY].CopyFrom(mslots, allocator);
    } else {
        busboardEntry->AddMember(DB_MACHINE_SLOTS_KEY, mslots, allocator);
    }

    if (!busboardEntry->HasMember("busboardID")) {
        busboardEntry->AddMember("busboardID", Value(busboardID.c_str(), allocator), allocator);
    }

    busboardEntry->RemoveMember(DB_BUSBOARD_LAST_UPDATED_KEY);
    busboardEntry->AddMember(DB_BUSBOARD_LAST_UPDATED_KEY,
                             static_cast<uint64_t>(Cell::getCurrentTimeMillis()),
                             allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::string jsonString = buffer.GetString();
    m_client.hset(DB_MACHINE_TABLE_KEY, locator->machineId, jsonString);
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

bool RedisDBManager::deleteCell(const std::string& cellID)
{
    if (cellID.empty()) {
        return false;
    }
    m_client.hdel(DB_CELLTABLE_KEY, {cellID});
    m_client.sync_commit();
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
