add_executable(module_datastructures_arrlist
        test_arrlist.c)
target_link_libraries(module_datastructures_arrlist
        module_datastructures
        Unity)
add_test(datastructures_arrlist module_datastructures_arrlist)
