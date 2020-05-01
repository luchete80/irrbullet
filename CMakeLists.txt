include_directories ("D:/Luciano/bullet3/src")
include_directories ("D:/Luciano/irrlicht/include")

include_directories (${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library (irrbullet SHARED ${SOURCES})

target_link_libraries (irrbullet irrlicht Bullet3Common BulletSoftBody BulletDynamics BulletCollision BulletInverseDynamicsUtils
                                BulletInverseDynamics LinearMath)