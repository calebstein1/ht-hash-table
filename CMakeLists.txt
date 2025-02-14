cmake_minimum_required(VERSION 3.30)
project(hash_table C)

set(CMAKE_C_STANDARD 99)

string(APPEND CMAKE_C_FLAGS_DEBUG " -fsanitize=address,undefined -Werror=all -Werror=pedantic -Wextra")

add_library(hthash ht_hash.c)
