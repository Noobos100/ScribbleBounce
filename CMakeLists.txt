cmake_minimum_required(VERSION 4.4)

project(ScribbleBounce)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
# Forcer l'utilisation de C++ 23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Tout doit être parfait
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
# Ne pas utiliser les extensions de CMake
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
    set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)
endif()

# Pour "import std"
include(cmake/std.cmake)
include(cmake/compile_options.cmake)

# Répertoire contenant les sources des exercices
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# Répertoire contenant les sources des tests unitaires
set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Découvrir les sources et relancer CMake lors de leur ajout ou suppression
# Note : en cas d'ajout de sous répertoire, mettre ci-dessous
file(GLOB SRC_FILES CONFIGURE_DEPENDS
        "${SRC_DIR}/*.cpp"
)
file(GLOB MODULE_FILES CONFIGURE_DEPENDS
        "${SRC_DIR}/*.ixx"
)
file(GLOB TEST_FILES CONFIGURE_DEPENDS
        "${TESTS_DIR}/*.cpp"
)

# Les tests utilisent le main fourni par Catch2
set(TEST_SRC_FILES ${SRC_FILES} ${TEST_FILES})
list(FILTER TEST_SRC_FILES EXCLUDE REGEX "(^|/)main\\.cpp$")

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
)

FetchContent_MakeAvailable(SFML)

# Exécutable principal
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ${MODULE_FILES}
)
compile_options(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

if(TEST_FILES)
    # Exécutable et liste des fichiers sources des tests unitaires
    add_executable(tests ${TEST_SRC_FILES})
    target_sources(tests
            PUBLIC
            FILE_SET CXX_MODULES
            FILES
            ${MODULE_FILES}
    )

    # bibliothèque pour les tests unitaires
    include(cmake/catch2.cmake)
    target_link_libraries(tests PRIVATE
            SFML::Graphics
            SFML::Window
            SFML::System
            Catch2::Catch2WithMain
            std-cxx-modules
    )
    include(CTest)
    include(Catch)
    catch_discover_tests(tests)
else()
    message(STATUS "No test files found; skipping the tests target.")
endif()