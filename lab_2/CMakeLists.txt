cmake_minimum_required(VERSION 4.0)
project(lab_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab_2
        SimpleLinkedList.cpp
        Stack.cpp
        Stack.h
        StackImplementation.h
        Vector.h
        Vector.cpp
        StackImplementationVector.cpp
        StackImplementationList.cpp
        SimpleLinkedList.h)
