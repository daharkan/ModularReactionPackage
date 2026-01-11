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

    Profile& profile();
    void setProfile(const Profile &newProfile);

    std::string cellID() const;
    void setCellID(const std::string &newCellID);

    std::string busboardID() const;
    void setBusboardID(const std::string &newBusboardID);

    unsigned long totalProfileDurationMSecs() const;

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

        return experiment;
    }

    void fromJSON(const Value& json) {
        if (json.HasMember("name") && json["name"].IsString()) {
            m_name = json["name"].GetString();
        }
        m_owner.fromJSON(json["owner"]);
        m_profile.fromJSON(json["profile"]);
        m_cellID = json["cellID"].GetString();
        m_busboardID = json["busboardID"].GetString();
        m_totalProfileDurationMSecs = json["totalProfileDurationMSecs"].GetUint64();
        m_startSystemTimeMSecs = json["startSystemTimeMSecs"].GetUint64();
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

};

#endif // EXPERIMENT_H
