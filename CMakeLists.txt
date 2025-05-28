cmake_minimum_required(VERSION 3.30)
project(ConwayGameOfLife C)

set(CMAKE_C_STANDARD 23)

add_executable(gameoflife
        src/main.c
        src/shape_editor.c src/shape_editor.h
        src/rules.c src/rules.h
        src/render.c src/render.h
        src/grid.c src/grid.h
        src/ui_menu.c src/ui_menu.h
        src/mouse_state.h
)

target_link_libraries(gameoflife
        m
        GL glut GLU
)
