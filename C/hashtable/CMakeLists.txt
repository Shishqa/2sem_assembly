cmake_minimum_required(VERSION 3.15)
project(hashtable)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -O0")

add_executable(hashtable main.cpp HashTable.h HashTable_methods.h Bucket_methods.h HashTable_iterator.h HashTable_class.h)