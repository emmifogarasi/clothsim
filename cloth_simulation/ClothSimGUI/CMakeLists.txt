cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

project(ClothSimGUIBUILD LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
find_package(NGL CONFIG REQUIRED)



include_directories(${PROJECT_SOURCE_DIR}/include)


find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
find_package(OpenGL REQUIRED)
add_executable(ClothSimGUI)

target_sources(ClothSimGUI PRIVATE
        src/main.cpp
        src/MainWindow.cpp
        src/Particle.cpp
        src/Constraint.cpp
        src/Cloth.cpp
        src/NGLScene.cpp
        src/NGLSceneMouseControls.cpp
        include/MainWindow.h
        include/particle.h
        include/Constraint.h
        include/Cloth.h
        include/NGLScene.h
        include/WindowParams.h
        ui/MainWindow.ui

)




if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        message("Mac build")
        find_library(MACGL OpenGL)
        set(EXTRALIBS  ${MACGL})
add_definitions(-DGL_SCILENCE_DEPRICATION)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set ( PROJECT_LINK_LIBS -lGL)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        add_compile_definitions(_USE_MATH_DEFINES)
        add_compile_definitions(NOMINMAX)
        target_compile_definitions(ClothSimGUI PRIVATE /wd4251)
endif()

target_link_libraries(ClothSimGUI PRIVATE ${PROJECT_LINK_LIBS} NGL Qt5::Widgets Qt5::OpenGL OpenGL::GLU)
# add exe and link libs that must be after the other defines



