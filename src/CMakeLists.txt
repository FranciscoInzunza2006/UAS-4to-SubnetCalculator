
add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-parameter)
include_directories(${CMAKE_ROOT}/include)

add_executable(Calculator main.c)
target_link_libraries(Calculator raylib)