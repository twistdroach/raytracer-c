add_executable(main
    main.c
)

target_link_libraries(main PRIVATE
        module_datastructures
        m
)
