#pragma once

#include <pwd.h>

#include <memory>

using std::unique_ptr;

struct User {
  User();
  ~User();
  User(uid_t uid);
  User(User&&) noexcept;
  User& operator=(User&&) noexcept;

  std::string name();

private:
  passwd* passwd;
};
