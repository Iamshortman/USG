cmake_minimum_required(VERSION 3.8.0)
project(USG)

#Bring the headers
include_directories(src)

file(GLOB_RECURSE CLIENT_SOURCES "src/Client/*.cpp")
file(GLOB_RECURSE COMMON_SOURCES "src/Common/*.cpp")
file(GLOB_RECURSE SERVER_SOURCES "src/Server/*.cpp")

file(GLOB_RECURSE CLIENT_HEADERS "src/Client/*.hpp")
file(GLOB_RECURSE COMMON_HEADERS "src/Common/*.hpp")
file(GLOB_RECURSE SERVER_HEADERS "src/Server/*.hpp")

source_group("Client" FILES ${CLIENT_SOURCES})
source_group("Common" FILES ${COMMON_SOURCES})
source_group("Server" FILES ${SERVER_SOURCES})

source_group("Client_Headers" FILES ${CLIENT_HEADERS})
source_group("Common_Headers" FILES ${COMMON_HEADERS})
source_group("Server_Headers" FILES ${SERVER_HEADERS})

#libs part 1
link_directories(lib/SDL2-2.0.4/lib/x64/)
link_directories(lib/glew-1.12.0/lib/Release/x64/)

link_directories(lib/assimp-3.3.1/build/code/Debug/)
link_directories(lib/assimp-3.3.1/build/contrib/zlib/Debug/)

link_directories(lib/entityx-current/build/Debug)

link_directories(lib/reactphysics3d-master/build/lib/Debug/)

#Create Projects
add_executable(Client ${CLIENT_SOURCES} ${CLIENT_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})
add_executable(Server ${SERVER_SOURCES} ${SERVER_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})
set_target_properties(Client PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
target_compile_definitions(Client PUBLIC CLIENT)
target_compile_definitions(Server PUBLIC SERVER)

#libs part 2

#GLM
include_directories(lib/glm/)

#jsoncons
include_directories(lib/jsoncons-0.104.0/include)

#OPENGL
target_link_libraries (Client opengl32)
target_link_libraries (Client glu32)

#SDL2
include_directories(lib/SDL2-2.0.4/include/)
target_link_libraries (Client SDL2)

#Glew
include_directories(lib/glew-1.12.0/include/)
target_link_libraries (Client glew32)

#ASSIMP
include_directories(lib/assimp-3.3.1/include/)
target_link_libraries (Client zlibstaticd)
target_link_libraries (Server zlibstaticd)

target_link_libraries (Server assimp-vc140-mt)
target_link_libraries (Client assimp-vc140-mt)

#Entityx
include_directories(lib/entityx-current)
target_link_libraries (Client entityx-d)
target_link_libraries (Server entityx-d)

#REACT_PHYSICS_3D
include_directories(lib/reactphysics3d-master/src)
target_link_libraries (Client reactphysics3d)
target_link_libraries (Server reactphysics3d)
target_compile_definitions(Client PUBLIC IS_DOUBLE_PRECISION_ENABLED)
target_compile_definitions(Server PUBLIC IS_DOUBLE_PRECISION_ENABLED)

