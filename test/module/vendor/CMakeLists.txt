## thread_pool_test
#add_executable(thread_pool_test
#        thread_pool_test.cpp
#)
#
#target_link_libraries(thread_pool_test
#  thread_pool
#  gtest
#)
#
#add_test(
#  NAME thread_pool_test
#  COMMAND $<TARGET_FILE:thread_pool_test>
#)
#
## json test
#add_executable(json_test
#        json_test.cpp
#)
#
#target_link_libraries(json_test
#  json
#  gtest
#)
#add_test(
#  NAME json_test
#  COMMAND $<TARGET_FILE:json_test>
#)

addtest(uvw_test uvw_test.cpp)
target_link_libraries(uvw_test
    uvw
    )

addtest(rxcpp_subject_usage rxcpp_subject_usage.cpp)
target_link_libraries(rxcpp_subject_usage
    rxcpp
    )

addtest(tbb_test tbb_test.cpp)
target_link_libraries(tbb_test
    tbb
    )
