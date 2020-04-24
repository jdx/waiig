cmake_minimum_required(VERSION 3.17)
project(monkey)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -gdwarf-3")
if ("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage -fcolor-diagnostics")
endif ()

find_package(fmt CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

add_library(lib lib/lexer.cpp lib/lexer.cpp lib/token.cpp lib/repl.cpp)
target_link_libraries(lib fmt::fmt)
target_include_directories(lib PUBLIC lib/include)

add_executable(monkey bin/main.cpp bin/user.cpp)
target_link_libraries(monkey lib)

add_executable(testlib test/main.cpp test/lexer_test.cpp test/repl_test.cpp)
#target_include_directories(testlib PRIVATE lib)
target_link_libraries(testlib PRIVATE lib Catch2::Catch2)

#include(CTest)
include(Catch)