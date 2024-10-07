

add_executable(PostProcessingScene "PostProcessingScene.cpp" "../movement3d.h")

#target_include_directories(PostProcessingScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(PostProcessingScene PRIVATE Melon)