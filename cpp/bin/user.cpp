#include "user.h"

#include <pwd.h>
#include <zconf.h>

#include <string>

using std::string;

User::User()
    : User{geteuid()} { }

User::User(uid_t uid)
    : passwd{getpwuid(uid)} { }

string User::name() {
  return passwd->pw_name;
}

User::~User()               = default;
User::User(User&&) noexcept = default;
User& User::operator=(User&&) noexcept = default;
