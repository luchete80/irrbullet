include_directories("./include")
include_directories("d:/Luciano/bullet3/src")
include_directories("d:/Luciano/irrlicht-1.8.4/include")

file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.hpp")


add_library (irrbullet SHARED ${SOURCES})
target_link_libraries (irrbullet irrlicht bullet3Common)
target_link_libraries (irrbullet irrlicht Bullet3Common BulletSoftBody BulletDynamics BulletCollision BulletInverseDynamicsUtils BulletInverseDynamics LinearMath)

add_executable (helloworld "examples/HelloWorld/helloworldexample.cpp"
							"examples/HelloWorld/main.cpp"
							"examples/framework/exampleframework.cpp")
							
target_link_libraries (helloworld irrbullet)

add_executable (softbody "examples/SoftBody/softbodyexample.cpp"
							"examples/Softbody/main.cpp"
							"examples/framework/exampleframework.cpp")
target_link_libraries (softbody irrbullet)

add_executable (softbody2 "examples/SoftBody2/softbodyexample2.cpp"
							"examples/Softbody2/main.cpp"
							"examples/framework/exampleframework.cpp")							
							
target_link_libraries (softbody2 irrbullet)

add_executable (character "examples/Character/characterexample.cpp"
							"examples/Character/main.cpp"
							"examples/framework/exampleframework.cpp")
							
target_link_libraries (character irrbullet)

add_executable (collision "examples/Collision/collisionexample.cpp"
							"examples/collision/main.cpp"
							"examples/framework/exampleframework.cpp")
							
target_link_libraries (collision irrbullet)