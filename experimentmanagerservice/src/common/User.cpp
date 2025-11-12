#include "User.h"

User::User() {}

User::User(const User& other) :
    m_username(other.m_username),
    m_password(other.m_password),
    m_role(other.m_role)
{
    // Copy constructor implementation
}

User& User::operator=(const User& other) {
    if (this != &other) { // Avoid self-assignment
        m_username = other.m_username;
        m_password = other.m_password;
        m_role = other.m_role;
    }
    return *this;
}
std::string User::username() const
{
    return m_username;
}

void User::setUsername(const std::string &newUsername)
{
    m_username = newUsername;
}

std::string User::password() const
{
    return m_password;
}

void User::setPassword(const std::string &newPassword)
{
    m_password = newPassword;
}

UserRole User::role() const
{
    return m_role;
}

void User::setRole(UserRole newRole)
{
    m_role = newRole;
}
