cmake_minimum_required(VERSION 3.13)

set(COMPONENT_ADD_INCLUDEDIRS csrc)
file(GLOB COMPONENT_SRCS csrc/*.c)
file(GLOB COMPONENT_HDRS csrc/*.h)

#exclude the monolithic font collection
list(REMOVE_ITEM COMPONENT_SRCS
	"${CMAKE_CURRENT_LIST_DIR}/csrc/u8g2_fonts.c"
)

#include only the required standalone font
list(APPEND COMPONENT_SRCS
	"${CMAKE_CURRENT_LIST_DIR}/tools/font/build/single_font_files/u8g2_font_ncenB08_tr.c"
)


if(COMMAND idf_component_register)
    idf_component_register(SRCS "${COMPONENT_SRCS}" INCLUDE_DIRS csrc)
    return()
endif()

project(u8g2)

if(COMMAND register_component)
    set(COMPONENT_NAME ".")
    register_component()
else()
    add_library(u8g2 ${COMPONENT_SRCS})
    target_include_directories(u8g2 PUBLIC 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/csrc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/cppsrc>"
    "$<INSTALL_INTERFACE:include>")
endif()

install(TARGETS u8g2
        EXPORT u8g2-targets
        ARCHIVE DESTINATION lib)

install(FILES ${COMPONENT_HDRS}
        DESTINATION include/u8g2)

install(EXPORT u8g2-targets
        DESTINATION lib/cmake/u8g2)

install(FILES ${u8g2_SOURCE_DIR}/pkg/u8g2-config.cmake
        DESTINATION lib/cmake/u8g2)
