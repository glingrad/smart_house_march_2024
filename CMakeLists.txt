cmake_minimum_required(VERSION 3.16)

project(gus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(gus main.cpp)
find_package(Threads REQUIRED)
target_link_libraries(gus PRIVATE Threads::Threads)  # Исправлено здесь

include(GNUInstallDirs)
install(TARGETS gus
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
