cmake_minimum_required(VERSION 3.17)
project(group_project_inheritorplus)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER "/usr/local/bin/gcc-10") #这里写你的gcc路径
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-10") #这里写你的g++路径
set(OPENMP_LIBRARIES "/usr/local/Cellar/libomp/11.0.0/lib") #这里写你的libomp路径, 通过brew安装的默认地址
set(OPENMP_INCLUDES "/usr/local/Cellar/libomp/11.0.0/include")#这里写你的libomp路径

message("here")
message("${CMAKE_CXX_COMPILER}")

OPTION (USE_OpenMP "Use OpenMP to enamble <omp.h>" ON)



# Find OpenMP
if(APPLE AND USE_OpenMP)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C "${CMAKE_C_COMPILER}")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -lomp -Wno-unused-command-line-argument")
        set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
        set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -lomp -Wno-unused-command-line-argument")
        set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")
        set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
        set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
        set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    endif()
endif()

if(USE_OpenMP)
    find_package(OpenMP REQUIRED)
endif(USE_OpenMP)


if (OPENMP_FOUND)
    include_directories("${OPENMP_INCLUDES}")
    link_directories("${OPENMP_LIBRARIES}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif(OPENMP_FOUND)

add_executable(main main.cpp Image.cpp Image.h Pixel.cpp Pixel.h
         PNG.cpp PNG.h Interface.h Interface.cpp Serial.cpp Serial.h Parallel.cpp Parallel.h)
