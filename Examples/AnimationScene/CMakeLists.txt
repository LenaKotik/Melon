

add_executable(AnimationScene "AnimationScene.cpp")

#target_include_directories(AnimationScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(AnimationScene PRIVATE Melon)