cmake_minimum_required(VERSION 3.5)
project(goldcrest-vp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Boost REQUIRED COMPONENTS program_options log iostreams)

#set(MEM_IMPL "Memory Implementation" CACHE STRING "MemoryImplementation")

# modify this to select different memory configs
add_compile_definitions(REG_IMPL REG) #done
#add_compile_definitions(REG_IMPL BRAM) #done
#add_compile_definitions(REG_IMPL MEM) #done
#add_compile_definitions(REG_IMPL TOP) #done
#add_compile_definitions(REG_IMPL TOPPC)
#add_compile_definitions(REG_IMPL RISCV)

add_executable(goldcrest
  src/main.cpp
  src/core.cpp
  src/memory.cpp
  src/microcode.cpp
  src/riscv.cpp
  src/time_model.cpp)

target_include_directories(goldcrest PRIVATE inc)
target_include_directories(goldcrest PRIVATE microcode)

target_link_libraries(goldcrest ${Boost_LIBRARIES})

install(TARGETS goldcrest
  EXPORT goldcrest
)
