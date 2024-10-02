

add_executable(TreeTransformScene "TreeTransformScene.cpp" "../movement3d.h")

#target_include_directories(TreeTransformScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(TreeTransformScene PRIVATE Melon)