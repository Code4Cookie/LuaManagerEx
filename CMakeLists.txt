cmake_minimum_required(VERSION 3.10)

# set the project name
project(
	LuaManager
	VERSION 1.0.0
	LANGUAGES CXX
)

# im using glob. deal with it. lmao
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${HEADERS}
		${SOURCES}
)

add_executable( ${PROJECT_NAME} 
		${HEADERS}
		${SOURCES}
)
		
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/extern/LuaJIT/src")
target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/extern/LuaJIT/src")

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC  "luajit" "lua51")


if(MSVC)
	set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib")
	  
	  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)