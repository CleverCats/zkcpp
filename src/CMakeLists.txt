add_library(zkcpp SHARED zkcpp_sync_crud.cxx 
        zkcpp_sync_lock.cxx 
        zkcpp_async_crud.cxx)

target_link_libraries(zkcpp
    PUBLIC pthread 
    PUBLIC zookeeper_mt
    PUBLIC log)

target_include_directories(zkcpp
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})