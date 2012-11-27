cmake_minimum_required(VERSION 2.8)

project( tester )
find_package( OpenCV REQUIRED )
add_executable(
	tester
	tester.cpp
	${TESTER_SOURCE_DIR}../BeerCaddy/BeerCaddy/BeerClassifier.cpp
)
target_link_libraries( tester ${OpenCV_LIBS} )
include_directories( ${TESTER_SOURCE_DIR}../BeerCaddy/BeerCaddy )
