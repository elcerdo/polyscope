cmake_minimum_required(VERSION 2.8.9)

# Create a single library for the project
add_executable(distance-map
    main.cpp
    )

# Link settings
target_link_libraries(distance-map
    polyscope
    )
