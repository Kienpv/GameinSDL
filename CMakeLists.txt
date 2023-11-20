cmake_minimum_required(VERSION 3.10)

add_compile_options(-gdwarf-2 -gstrict-dwarf)

project("gSDL")

set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/buildx) 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(SDL2 REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/hdr
)

link_directories (
#    /cygdrive/c/GL/SDL/lib/x64
)

# file(GLOB_RECURSE SOURCE_DEPENDENCIES 
#     ${PROJECT_SOURCE_DIR}/src/*.cpp
# )
# file(GLOB_RECURSE HEADER_DEPENDENCIES 
#     ${PROJECT_SOURCE_DIR}/hdr/*.h
# )

# Create a sources variable with a link to all .h/.cpp files to compile
set(SOURCES
    src/main.cpp
    src/game.cpp
)
set(HEADERS
    hdr/game.h
)

# IF(UNIX)
#     link_libraries(stdc++fs)
# ENDIF()

#add_executable(${PROJECT_NAME} ${SOURCE_DEPENDENCIES} ${HEADER_DEPENDENCIES})

add_executable(${PROJECT_NAME} 
    # ${PROJECT_SOURCE_DIR}/src/main.cpp
    # ${PROJECT_SOURCE_DIR}/src/game.cpp
    ${SOURCES}

    # ${PROJECT_SOURCE_DIR}/hdr/game.h
    ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} 
                      ${SDL2_LIBRARIES}
                        # SDL2main SDL2 
                     )
