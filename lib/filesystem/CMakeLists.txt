add_library(fs
    filesystem_interface.cpp
)

target_include_directories(fs PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(fs PUBLIC cxx_std_23)
