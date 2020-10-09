add_executable(module_datastructures_arrlist
        test_arrlist.c)
target_link_libraries(module_datastructures_arrlist
        module_datastructures
        Unity)
add_test(datastructures_arrlist module_datastructures_arrlist)

add_executable(module_datastructures_linkedlist
        test_linkedlist.c)
target_link_libraries(module_datastructures_linkedlist
        module_datastructures
        Unity)
add_test(datastructures_linkedlist module_datastructures_linkedlist)
