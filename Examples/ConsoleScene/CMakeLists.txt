

add_executable(RandomTextureCubeScene "RandomTextureCubeScene.cpp" "../movement3d.h")

#target_include_directories(RandomTextureCubeScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(RandomTextureCubeScene PRIVATE Melon)