#ifndef PROFILE_H
#define PROFILE_H
#include <vector>

#include "TempArc.h"
#include "RPMArc.h"

class Profile
{
public:
    Profile();

    Profile(const Profile& other) {
        // Deep copy m_tempArcsInSeq
        m_tempArcsInSeq.reserve(other.m_tempArcsInSeq.size());
        for (const auto& tempArc : other.m_tempArcsInSeq) {
            m_tempArcsInSeq.push_back(TempArc(tempArc));
        }

        // Deep copy m_rpmArcsInSeq
        m_rpmArcsInSeq.reserve(other.m_rpmArcsInSeq.size());
        for (const auto& rpmArc : other.m_rpmArcsInSeq) {
            m_rpmArcsInSeq.push_back(RPMArc(rpmArc));
        }

        // Copy other member variables
        m_totalMaxProfileDuration = other.m_totalMaxProfileDuration;
    }

    bool addTempArcInSequence(TempArc tArc);
    bool addRPMArcInSequence(RPMArc rArc);

    long totalMaxProfileDuration() const;
    void setTotalMaxProfileDuration(long newTotalMaxProfileDuration);

    std::vector<TempArc> tempArcsInSeq() const;

    std::vector<RPMArc> rpmArcsInSeq() const;


    Value toJSON(Document::AllocatorType& allocator) const {
        Value profile(kObjectType);

        Value tempArcs(kArrayType);
        for (const auto& tempArc : m_tempArcsInSeq) {
            tempArcs.PushBack(tempArc.toJSON(allocator), allocator);
        }
        profile.AddMember("tempArcsInSeq", tempArcs, allocator);

        Value rpmArcs(kArrayType);
        for (const auto& rpmArc : m_rpmArcsInSeq) {
            rpmArcs.PushBack(rpmArc.toJSON(allocator), allocator);
        }
        profile.AddMember("rpmArcsInSeq", rpmArcs, allocator);

        profile.AddMember("totalMaxProfileDuration", m_totalMaxProfileDuration, allocator);
        return profile;
    }


    void fromJSON(const Value& json) {
        const Value& tempArcsArray = json["tempArcsInSeq"];
        for (SizeType i = 0; i < tempArcsArray.Size(); i++) {
            TempArc tempArc(0,0,0,0,0);
            tempArc.fromJSON(tempArcsArray[i]);
            m_tempArcsInSeq.push_back(tempArc);
        }

        const Value& rpmArcsArray = json["rpmArcsInSeq"];
        for (SizeType i = 0; i < rpmArcsArray.Size(); i++) {
            RPMArc rpmArc(0,0,0,0,0);;
            rpmArc.fromJSON(rpmArcsArray[i]);
            m_rpmArcsInSeq.push_back(rpmArc);
        }

        m_totalMaxProfileDuration = json["totalMaxProfileDuration"].GetInt64();
    }

    Profile& operator=(const Profile& other) {
        if (this != &other) {
            m_tempArcsInSeq = other.m_tempArcsInSeq;
            m_rpmArcsInSeq = other.m_rpmArcsInSeq;
            m_totalMaxProfileDuration = other.m_totalMaxProfileDuration;
        }
        return *this;
    }

    // Equality operator
    bool operator==(const Profile& other) const {
        return (m_tempArcsInSeq == other.m_tempArcsInSeq &&
                m_rpmArcsInSeq == other.m_rpmArcsInSeq &&
                m_totalMaxProfileDuration == other.m_totalMaxProfileDuration);
    }

    float calculateTemp(unsigned long currentTimeMSec);
    int calculateRPM(unsigned long currentTimeMSec);

    void clearProfile();

private:
    std::vector<TempArc> m_tempArcsInSeq;
    std::vector<RPMArc> m_rpmArcsInSeq;
    long m_totalMaxProfileDuration = 0;

};

#endif // PROFILE_H
