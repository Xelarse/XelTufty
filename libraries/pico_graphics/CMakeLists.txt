if(NOT TARGET bitmap_fonts)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../bitmap_fonts)
endif()

if(NOT TARGET hershey_fonts)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../hershey_fonts)
endif()

add_library(pico_graphics 
    ${CMAKE_CURRENT_LIST_DIR}/types.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_1bit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_1bitY.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_3bit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_p4.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_p8.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_rgb332.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_rgb565.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_rgb888.cpp
    ${CMAKE_CURRENT_LIST_DIR}/pico_graphics_pen_inky7.cpp
)

include_directories(${CMAKE_SOURCE_DIR})

target_include_directories(pico_graphics INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(pico_graphics bitmap_fonts hershey_fonts pico_stdlib)