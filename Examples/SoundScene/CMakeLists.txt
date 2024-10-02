

add_executable(SoundScene "SoundScene.cpp")

#target_include_directories(SoundScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(SoundScene PRIVATE Melon)