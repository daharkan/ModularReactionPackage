#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include "User.h"
#include "Profile.h"
#include "rapidjson/rapidjson.h"

using namespace rapidjson;

class Experiment
{
public:
    Experiment();
    Experiment(const Experiment& other);
    Experiment& operator=(const Experiment& other);
    User owner() const;
    void setOwner(const User &newOwner);

    std::string name() const;
    void setName(const std::string &newName);

    std::string experimentId() const;
    void setExperimentId(const std::string &newExperimentId);

    int experimentType() const;
    void setExperimentType(int newExperimentType);

    unsigned long createdAtMSecs() const;
    void setCreatedAtMSecs(unsigned long newCreatedAtMSecs);

    std::string settingsJson() const;
    void setSettingsJson(const std::string &newSettingsJson);

    Profile& profile();
    void setProfile(const Profile &newProfile);

    std::string cellID() const;
    void setCellID(const std::string &newCellID);

    std::string busboardID() const;
    void setBusboardID(const std::string &newBusboardID);

    unsigned long totalProfileDurationMSecs() const;
    void setTotalProfileDurationMSecs(unsigned long newTotalProfileDurationMSecs);

    static Experiment fromJSon(std::string jsonString);
    std::string toJSon(Experiment exp);

    Value toJSON(Document::AllocatorType& allocator) const {
        Value experiment(kObjectType);
        experiment.AddMember("name", Value().SetString(m_name.c_str(), static_cast<SizeType>(m_name.length()), allocator), allocator);
        experiment.AddMember("owner", m_owner.toJSON(allocator), allocator);
        experiment.AddMember("profile", m_profile.toJSON(allocator), allocator);
        experiment.AddMember("cellID", Value().SetString(m_cellID.c_str(), static_cast<SizeType>(m_cellID.length()), allocator), allocator);
        experiment.AddMember("busboardID", Value().SetString(m_busboardID.c_str(), static_cast<SizeType>(m_busboardID.length()), allocator), allocator);
        experiment.AddMember("totalProfileDurationMSecs", m_totalProfileDurationMSecs, allocator);
        experiment.AddMember("startSystemTimeMSecs", m_startSystemTimeMSecs, allocator);
        experiment.AddMember("experimentId", Value().SetString(m_experimentId.c_str(), static_cast<SizeType>(m_experimentId.length()), allocator), allocator);
        experiment.AddMember("experimentType", m_experimentType, allocator);
        experiment.AddMember("createdAtMSecs", m_createdAtMSecs, allocator);
        experiment.AddMember("settingsJson", Value().SetString(m_settingsJson.c_str(), static_cast<SizeType>(m_settingsJson.length()), allocator), allocator);

        return experiment;
    }

    void fromJSON(const Value& json) {
        if (json.HasMember("name") && json["name"].IsString()) {
            m_name = json["name"].GetString();
        }
        if (json.HasMember("owner") && json["owner"].IsObject()) {
            m_owner.fromJSON(json["owner"]);
        }
        if (json.HasMember("profile") && json["profile"].IsObject()) {
            m_profile.fromJSON(json["profile"]);
        }
        if (json.HasMember("cellID") && json["cellID"].IsString()) {
            m_cellID = json["cellID"].GetString();
        }
        if (json.HasMember("busboardID") && json["busboardID"].IsString()) {
            m_busboardID = json["busboardID"].GetString();
        }
        if (json.HasMember("totalProfileDurationMSecs") && json["totalProfileDurationMSecs"].IsUint64()) {
            m_totalProfileDurationMSecs = json["totalProfileDurationMSecs"].GetUint64();
        }
        if (json.HasMember("startSystemTimeMSecs") && json["startSystemTimeMSecs"].IsUint64()) {
            m_startSystemTimeMSecs = json["startSystemTimeMSecs"].GetUint64();
        }
        if (json.HasMember("experimentId") && json["experimentId"].IsString()) {
            m_experimentId = json["experimentId"].GetString();
        }
        if (json.HasMember("experimentType") && json["experimentType"].IsInt()) {
            m_experimentType = json["experimentType"].GetInt();
        }
        if (json.HasMember("createdAtMSecs") && json["createdAtMSecs"].IsUint64()) {
            m_createdAtMSecs = json["createdAtMSecs"].GetUint64();
        }
        if (json.HasMember("settingsJson") && json["settingsJson"].IsString()) {
            m_settingsJson = json["settingsJson"].GetString();
        }
    }



    unsigned long startSystemTimeMSecs() const;
    void setStartSystemTimeMSecs(unsigned long newStartSystemTimeMSecs);

private:
    std::string m_name;
    User m_owner;
    Profile m_profile;
    std::string m_cellID;
    std::string m_busboardID;
    unsigned long m_totalProfileDurationMSecs;
    unsigned long m_startSystemTimeMSecs;
    std::string m_experimentId;
    int m_experimentType = 0;
    unsigned long m_createdAtMSecs = 0;
    std::string m_settingsJson;

};

#endif // EXPERIMENT_H
