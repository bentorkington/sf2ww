include_directories("${PROJECT_SOURCE_DIR}/RedHammer")
include_directories("${PROJECT_SOURCE_DIR}/FistBlue")
include_directories("/usr/local/include")
link_directories("/usr/local/lib")
add_executable(test-redhammer main.c test_redhammer.c test_sf2types.c ${PROJECT_SOURCE_DIR}/RedHammer/redhammer.c)
add_test(test-redhammer test-redhammer)
add_dependencies(test-redhammer test-redhammer)
target_link_libraries(test-redhammer cmocka)


