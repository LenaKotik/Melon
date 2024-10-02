

add_executable(BlendScene "BlendScene.cpp")

#target_include_directories(BlendScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(BlendScene PRIVATE Melon)