cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(api-deluge-rasp VERSION 0.1.0 LANGUAGES CXX)

set(SOURCE main.cpp)

add_executable(deluge-rasp ${SOURCE})