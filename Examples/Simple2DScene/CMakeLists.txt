

add_executable(Simple2DScene "Simple2DScene.cpp")

#target_include_directories(Simple2DScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(Simple2DScene PRIVATE Melon)