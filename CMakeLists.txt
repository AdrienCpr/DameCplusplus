cmake_minimum_required(VERSION 3.29)
project(DameCplusplus)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(
        main
        src/main.cpp
        src/checkers/board.cpp
        src/checkers/piece.cpp
        src/checkers/position.cpp
        src/checkers/soundManager.cpp
        src/checkers/windowManager.cpp
        src/checkers/game.cpp
)

target_sources(
        main
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        src/checkers/modules/board.cppm
        src/checkers/modules/piece.cppm
        src/checkers/modules/position.cppm
        src/checkers/modules/game.cppm
        src/checkers/modules/windowManager.cppm
        src/checkers/modules/soundManager.cppm
)

target_link_libraries(main PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)
target_compile_features(main PRIVATE cxx_std_23)

if(WIN32)
    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
            VERBATIM)

    add_custom_command(
            TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:main>/resources
    )
endif()