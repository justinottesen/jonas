add_library(filesystem
    filesystem.cpp
)

target_include_directories(filesystem PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(filesystem PUBLIC cxx_std_23)
