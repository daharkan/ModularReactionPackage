#ifndef USER_H
#define USER_H

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/ostreamwrapper.h"

using namespace rapidjson;
enum UserRole{
    ROLE_ROOT,
    ROLE_ADMIN,
    ROLE_OPERATOR
};

class User
{
public:
    User();
    User(const User& other);

    std::string username() const;
    void setUsername(const std::string &newUsername);

    std::string password() const;
    void setPassword(const std::string &newPassword);

    UserRole role() const;
    void setRole(UserRole newRole);

    User& operator=(const User& other);


    Value toJSON(Document::AllocatorType& allocator) const {
        Value user(kObjectType);
        user.AddMember("username", Value().SetString(m_username.c_str(), static_cast<SizeType>(m_username.length()), allocator), allocator);
        user.AddMember("password", Value().SetString(m_password.c_str(), static_cast<SizeType>(m_password.length()), allocator), allocator);
        user.AddMember("role", m_role, allocator);
        return user;
    }

    void fromJSON(const Value& json) {
        m_username = json["username"].GetString();
        m_password = json["password"].GetString();
        m_role = static_cast<UserRole>(json["role"].GetInt());
    }

private:
    std::string m_username;
    std::string m_password;
    UserRole m_role;
};

#endif // USER_H
