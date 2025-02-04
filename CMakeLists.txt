cmake_minimum_required(VERSION 3.30)
project(hash_table C)

set(CMAKE_C_STANDARD 90)

string(APPEND CMAKE_C_FLAGS_DEBUG " -fsanitize=address,undefined -Werror=all -Werror=pedantic -Wextra")

add_library(ht_hash ht_hash.c)
