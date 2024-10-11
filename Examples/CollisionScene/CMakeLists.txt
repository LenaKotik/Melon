

add_executable(CollisionScene "CollisionScene.cpp" "../movement3d.h")

#target_include_directories(CollisionScene PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(CollisionScene PRIVATE Melon)