project(test)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

include(GoogleTest)

find_package(GTest CONFIG REQUIRED)


set(LIBS server_lib common gtest gtest_main pthread galay-shared ssl crypto)
set(SRC_LIST test_string.cpp test_vector.cpp test_list.cpp test_rbtree.cpp test_hash.cpp)

foreach(SRC ${SRC_LIST})
    string(REPLACE ".cpp" "" binfile ${SRC})
    add_executable(${binfile} ${SRC} ${TEST_COMMON_LIST})
    add_test(NAME ${binfile} COMMAND ${binfile})
    target_link_libraries(${binfile} ${LIBS})
    install(TARGETS ${binfile} DESTINATION test)
endforeach()
