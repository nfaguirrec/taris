cmake_minimum_required(VERSION 2.8)
project(taris)
add_definitions(-static -static-libgcc)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/lib)

find_library(GTL_LIB NAMES libGTL.so libGTL.a HINTS deps/lib DOC "The GTL library")
MESSAGE(${GTL_LIB})

add_library(taris STATIC
	src/molekel.h src/molekel.cpp
	src/Point.h src/Point.cpp
	src/TarisApplication.h src/TarisApplication.cpp
	src/Triangle.h src/Triangle.cpp
	src/HyperSurface.h src/HyperSurface.cpp
	src/NodeWeight.h src/NodeWeight.cpp
	src/Surface.h src/Surface.cpp
	src/Tree.h src/Tree.cpp
)

add_executable(TARIS-BuildIsosurface src/TARIS-BuildIsosurface.cpp)
add_executable(TARIS-BuildTree src/TARIS-BuildTree.cpp)
add_executable(TARIS-Matrices src/TARIS-Matrices.cpp)
add_executable(TARIS-TreesDistance src/TARIS-TreesDistance.cpp)

target_link_libraries(TARIS-BuildIsosurface taris ${GTL_LIB})
target_link_libraries(TARIS-BuildTree taris ${GTL_LIB})
target_link_libraries(TARIS-Matrices taris ${GTL_LIB})
target_link_libraries(TARIS-TreesDistance taris ${GTL_LIB})

install(TARGETS taris DESTINATION lib)
install(TARGETS TARIS-BuildIsosurface DESTINATION bin)
install(TARGETS TARIS-BuildTree DESTINATION bin)
install(TARGETS TARIS-Matrices DESTINATION bin)
install(TARGETS TARIS-TreesDistance DESTINATION bin)
install(FILES src/TARIS-Dendrogram DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
file(COPY src/TARIS-Dendrogram DESTINATION bin)
