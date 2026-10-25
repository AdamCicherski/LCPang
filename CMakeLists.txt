cmake_minimum_required(VERSION 3.10)
project(LCPang VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (ENABLE_ASAN)                                                                                                                                                                             
    message(STATUS "Building with AddressSanitizer")                                                                                                                                         
    add_compile_options(                                                                                                                                                                     
        -g                                                                                                                                                                                   
                -O1                                                                                                                                                                          
               -fno-omit-frame-pointer                                                                                                                                                       
        -fsanitize=address                                                                                                                                                                   
         )                                                                                                                                                                                   
         add_link_options(                                                                                                                                                                   
        -fsanitize=address                                                                                                                                                                   
        )                                                                                                                                                                                    
        endif()                                                                                                                                                                              
        if (ENABLE_UBSAN)                                                                                                                                                                    
           message(STATUS "Building with UBSanitizer")                                                                                                                                       
        add_compile_options(                                                                                                                                                                 
        -g                                                                                                                                                                                   
        -O1                                                                                                                                                                                  
        -fno-omit-frame-pointer                                                                                                                                                              
        -fsanitize=undefined                                                                                                                                                                 
         )                                                                                                                                                                                   
                                                                                                                                                                                              
        add_link_options(                                                                                                                                                                    
        -fsanitize=undefined                                                                                                                                                                 
        )                                                                                                                                                                                    
        endif()     

add_compile_options(-march=native -mavx2 -mfma)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_library(DIVSUFSORT_LIB divsufsort)
if(NOT DIVSUFSORT_LIB)
    message(FATAL_ERROR "libdivsufsort not found")
endif()

find_package(OpenMP REQUIRED)

add_custom_target(psascan_build ALL
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/externals/psascan
)


include_directories(
    include
    ${CMAKE_SOURCE_DIR}/externals/unordered_dense/include
)



set(LIBSAIS_USE_OPENMP ON CACHE BOOL "Use OpenMP for parallelization" FORCE)
add_subdirectory(externals/libsais)


add_library(FastaUtils
    src/fasta_utils.cpp
)
add_library(SAMPLE
	src/build_samples.cpp)

add_library(SequenceUtils
    src/sequence_utils.cpp
)

add_library(QUOTIENT_E
    src/quotient_external.cpp
)
target_compile_definitions(QUOTIENT_E PRIVATE
    CONSTRUCT_SA_PATH="${CMAKE_SOURCE_DIR}/externals/psascan/construct_sa"
)

target_link_libraries(QUOTIENT_E
    PRIVATE SAMPLE
)

add_library(Indexing
	src/Indexing.cpp
)
target_include_directories(Indexing
    PRIVATE
        ${CMAKE_SOURCE_DIR}/externals/libsais
)

target_link_libraries(Indexing
    PRIVATE libsais
)




add_executable(LCPang main.cpp src/build_external.cpp src/build_rapid.cpp)
target_compile_definitions(LCPang PUBLIC
    CONSTRUCT_SA_PATH="${CMAKE_SOURCE_DIR}/externals/psascan/construct_sa"
)

target_link_libraries(LCPang
    PRIVATE FastaUtils
    PRIVATE SequenceUtils
    PRIVATE ${DIVSUFSORT_LIB}
    PRIVATE libsais           # libsais z add_subdirectory
    PRIVATE OpenMP::OpenMP_CXX
    PRIVATE SAMPLE
    PRIVATE QUOTIENT_E
    PRIVATE Indexing
)


target_include_directories(LCPang
    PRIVATE ${CMAKE_SOURCE_DIR}/externals/libsais/include
)


target_link_libraries(FastaUtils PRIVATE libsais OpenMP::OpenMP_CXX)
target_link_libraries(SequenceUtils PRIVATE libsais OpenMP::OpenMP_CXX)


target_compile_definitions(LCPang PRIVATE LIBSAIS_OPENMP)


