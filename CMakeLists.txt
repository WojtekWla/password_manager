cmake_minimum_required(VERSION 3.24)
project(password_manager)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FETCHCONTENT_DECLARE(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50
)

FETCHCONTENT_MAKEAVAILABLE(fmt)


add_executable(password_manager main.cpp functions.cpp)
target_link_libraries(password_manager fmt)
