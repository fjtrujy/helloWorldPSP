cmake_minimum_required(VERSION 3.11)

project(texture)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pspgu
    pspge
    pspdisplay
)

#Make sure we copy the grass.png file to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/grass.png
    ${CMAKE_BINARY_DIR}/grass.png
    COMMENT "Copying grass.png to build directory"
)

#Add -pg flag for profiling
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -g")

# Create an EBOOT.PBP file
# set(BUILD_PRX ON)
create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_NAME}
    VERSION 01.00
)
