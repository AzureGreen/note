# cmake常用语法记录

## 基本格式

```cmake
cmake_minimum_required(VERSION 2.8)

project(project_name)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -Wall")

include_directories(.)
add_subdirectory(util)
add_subdirectory(statistic)
add_subdirectory(preprocess)
add_subdirectory(cachereplacement)

```


## 设置release/debug模式
```shell
cmake -DCMAKE_BUILD_TYPE=Release ..
```

##  编译可执行文件与静态库文件
```cmake
add_executable(cachereplacement_test cachereplacement_test.cpp)

add_library(filemgr STATIC filemgr.cpp)

target_link_libraries(cachereplacement_test filemgr)
```
