cmake_minimum_required(VERSION 3.2)

option(HUNTER_STATUS_DEBUG "Hunter debug info" ON)

# Hack, we have to use Clang
set(
    CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_LIST_DIR}/toolchain.cmake
    CACHE
    PATH
    "Default toolchain file"
)

include("${CMAKE_CURRENT_LIST_DIR}/../HunterGate.cmake")

HunterGate(
    URL "${HUNTER_HOST_URL}"
    SHA1 "${HUNTER_HOST_SHA1}"
)

project(v8_host)

hunter_add_package(v8)
