cmake_minimum_required(VERSION 3.16)
project(double-pendulum VERSION 0.1.0 LANGUAGES C)

# 生成 compile_commands.json（供 LSP/clangd 使用）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 指定编译器
set(CMAKE_C_COMPILER gcc)

# 编译选项
add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -O2)

# 可执行文件
add_executable(double-pendulum
    src/main.c
)

# 头文件路径
target_include_directories(double-pendulum PRIVATE include)

# ── 第三方依赖（deps 目录）──────────────────────────────────────
# raylib
set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/deps/raylib-5.5_linux_amd64)
target_include_directories(double-pendulum PRIVATE ${RAYLIB_DIR}/include)
target_link_directories(double-pendulum PRIVATE ${RAYLIB_DIR}/lib)
target_link_libraries(double-pendulum PRIVATE
    raylib
    m          # math
    pthread    # threading
    dl         # dynamic linking
)

# 复制 raylib 动态库到可执行文件旁边（方便运行时加载）
add_custom_command(TARGET double-pendulum POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${RAYLIB_DIR}/lib/libraylib.so.5.5.0
        $<TARGET_FILE_DIR:double-pendulum>/
)

# 安装规则
install(TARGETS double-pendulum RUNTIME DESTINATION bin)
