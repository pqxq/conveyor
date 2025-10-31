cmake_minimum_required(VERSION 3.10)
project(GoldConveyor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Основна програма ---
add_executable(GoldConveyor conveyor.cpp)

# --- Ціль для юніт-тестів ---
add_executable(GoldConveyorTests conveyor.cpp)
target_compile_definitions(GoldConveyorTests PRIVATE RUN_TESTS)
