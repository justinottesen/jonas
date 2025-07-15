add_executable(jonas-fusefs
    main.cpp
    fusefs.cpp
)

target_link_libraries(jonas-fusefs PRIVATE filesystem)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE REQUIRED fuse3)

target_include_directories(jonas-fusefs PRIVATE ${FUSE_INCLUDE_DIRS})
target_link_libraries(jonas-fusefs PRIVATE ${FUSE_LIBRARIES})
target_compile_options(jonas-fusefs PRIVATE ${FUSE_CFLAGS_OTHER})

target_compile_definitions(jonas-fusefs PRIVATE FUSE_USE_VERSION=35)
