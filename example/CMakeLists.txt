set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(crud crud.cxx)
add_executable(lock_c1 lock_c1.cxx)
add_executable(lock_c2 lock_c2.cxx)
add_executable(multi_thread multi_thread.cxx)
add_executable(logexample test_log.cxx)

target_link_libraries(crud
    PUBLIC zookeeper_mt
    PUBLIC pthread
    PUBLIC zkcpp)

target_link_libraries(lock_c1
    PUBLIC zookeeper_mt
    PUBLIC pthread
    PUBLIC zkcpp)

target_link_libraries(lock_c2
PUBLIC zookeeper_mt
PUBLIC pthread
PUBLIC zkcpp)

target_link_libraries(multi_thread
PUBLIC zookeeper_mt
PUBLIC pthread
PUBLIC zkcpp)

target_link_libraries(logexample PUBLIC log)