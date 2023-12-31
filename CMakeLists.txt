####
# This sets up the build system for the 'fprime-zephyr-led-blinker' project, including
# components and deployments from project.cmake. In addition, it imports the core F Prime components.
####

cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0048 NEW)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
set(CMAKE_SYSTEM_NAME "Zephyr")

project(fprime-zephyr-led-blinker C CXX)

###
# F' Core Setup
# This includes all of the F prime core components, and imports the make-system.
###
# set(FPRIME_SKIP_TOOLS_VERSION_CHECK ON)
# set(FPRIME_FRAMEWORK_PATH "${CMAKE_CURRENT_LIST_DIR}/fprime" CACHE PATH "Location of F prime framework" FORCE)
# set(FPRIME_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Root path of F prime project" FORCE)
include("${CMAKE_CURRENT_LIST_DIR}/fprime/cmake/FPrime.cmake")
# NOTE: register custom targets between these two lines
include("${FPRIME_FRAMEWORK_PATH}/cmake/FPrime-Code.cmake")


# This includes project-wide objects
include("${CMAKE_CURRENT_LIST_DIR}/project.cmake")
