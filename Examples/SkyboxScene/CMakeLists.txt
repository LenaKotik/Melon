

add_executable(SkyboxScene "SkyboxScene.cpp" "../movement3d.h")

#target_include_directories(SkyboxScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(SkyboxScene PRIVATE Melon)