cmake_minimum_required(VERSION 2.8)

project( validate )
find_package( OpenCV REQUIRED )

find_package( Boost 1.52 COMPONENTS filesystem program_options system REQUIRED )

add_executable(
	validate
	validate.cpp
	${VALIDATE_SOURCE_DIR}../BeerCaddy/BeerCaddy/BeerClassifier.cpp
)
target_link_libraries( validate ${OpenCV_LIBS} ${Boost_LIBRARIES})
include_directories( ${VALIDATE_SOURCE_DIR}../BeerCaddy/BeerCaddy )
