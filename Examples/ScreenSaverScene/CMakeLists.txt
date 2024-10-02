

add_executable(ScreenSaverScene "ScreenSaverScene.cpp")

#target_include_directories(ScreenSaverScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(ScreenSaverScene PRIVATE Melon)