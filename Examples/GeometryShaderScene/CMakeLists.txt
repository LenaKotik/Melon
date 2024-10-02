

add_executable(GeometryShaderScene "GeometryShaderScene.cpp" "../movement3d.h")

#target_include_directories(GeometryShaderScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(GeometryShaderScene PRIVATE Melon)