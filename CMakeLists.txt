cmake_minimum_required(VERSION 3.20)
project(Task8_Union C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Вказуємо, де шукати заголовні файли (.h)
include_directories(8_Union_C 8_Union_CPP)

# --- Ціль 1: Запуск C частини ---
add_executable(Task8_C
        8_Union_C/8_Union_test.c
        8_Union_C/8_Union.c
        8_Union_C/8_Union.h
)

# --- Ціль 2: Запуск C++ частини ---
add_executable(Task8_CPP
        8_Union_CPP/8_Union_test.cpp
        8_Union_CPP/8_Union.cpp
        8_Union_CPP/8_Union.h
)

# --- Ціль 3: Крос-тест (перевірка ідентичності) ---
add_executable(Task8_CrossTest
        8_Union_CPP/8_Union_cross_test.cpp
        8_Union_C/8_Union.c     # Підключаємо реалізацію Сі
        8_Union_CPP/8_Union.cpp # Підключаємо реалізацію C++
)

# --- Fast Test C ---
add_executable(Task8_Fast_C
        8_Union_C/8_Union_fast_test.c
        8_Union_C/8_Union.c
        8_Union_C/8_Union.h
        8_Union_C/8_Union_fast_test.c
)

# --- Fast Test C++ ---
add_executable(Task8_Fast_CPP
        8_Union_CPP/8_Union_fast_test.cpp
        8_Union_CPP/8_Union.cpp
        8_Union_CPP/8_Union.h
        8_Union_C/8_Union_fast_test.c
        8_Union_CPP/8_Union_fast_test.cpp
)