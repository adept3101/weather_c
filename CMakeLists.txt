cmake_minimum_required(VERSION 3.15)
project(Project C)

set(CMAKE_C_STANDARD 99)

# Добавляем cJSON
add_library(cjson STATIC cJSON/cJSON.c)
target_include_directories(cjson PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cJSON)

# Добавляем исполняемый файл
add_executable(main main.c)
target_link_libraries(main PRIVATE cjson)

# Указываем пути к заголовкам и библиотекам libcurl
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/curl/include)
target_link_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/curl/lib)
target_link_libraries(main PRIVATE curl)
