
add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-parameter)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

add_library(Calculator calculator.c ${INCLUDE_DIR}/calculator.h)
target_link_libraries(Calculator raylib)

add_executable(Main main.c)
target_link_libraries(Main Calculator raylib raygui)