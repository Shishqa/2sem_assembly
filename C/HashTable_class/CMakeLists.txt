cmake_minimum_required(VERSION 3.16)
project(HashTable_class)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall")

add_executable(HashTable_class main.cpp HashTable.h Bucket.h Value.h HashTableIterator.h Node.h c_HashTable.h)