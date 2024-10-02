

add_executable(LightingScene "LightingScene.cpp" "../movement3d.h")

#target_include_directories(LightingScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(LightingScene PRIVATE Melon)