# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
# Set the project name
project (HashMap)


# Create a sources variable with a link to all cpp files to compile
set(SOURCES
        src/main.cpp
        src/student_main.cpp
        src/tests.cpp
        )

# Add an executable with the above sources
add_executable(HashMap ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(HashMap
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include/
        )