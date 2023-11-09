aux_source_directory(./ LOG_CXX)

add_library(log SHARED ${LOG_CXX})

target_link_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)