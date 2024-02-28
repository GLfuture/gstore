project(client)
set(SRC_LIST main.cc)
set(TEST_SRC_LIST test_client.cc)
add_executable(client ${SRC_LIST})
target_link_libraries(client common galay-shared ssl crypto)

add_executable(test_client ${TEST_SRC_LIST})
target_link_libraries(test_client common galay-shared ssl crypto)