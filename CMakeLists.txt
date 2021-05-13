cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(api-deluge-rasp VERSION 0.1.0 LANGUAGES CXX)


find_package(oatpp 1.2.5 REQUIRED)



set(SOURCE main.cpp manage/manager.cpp)
set(HEADERS dto/torrent.h manage/manager.h manage/handler.h)


add_executable(deluge-rasp ${SOURCE} ${HEADERS})

target_link_libraries(deluge-rasp
        PUBLIC oatpp::oatpp
  
)



set_target_properties(deluge-rasp PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)
