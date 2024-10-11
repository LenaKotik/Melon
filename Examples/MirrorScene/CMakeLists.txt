

add_executable(MirrorScene "MirrorScene.cpp" "../movement3d.h")

#target_include_directories(MirrorScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(MirrorScene PRIVATE Melon)