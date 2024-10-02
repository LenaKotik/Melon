

add_executable(RotationScene "RotationScene.cpp" "../movement3d.h")

#target_include_directories(RotationScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(RotationScene PRIVATE Melon)