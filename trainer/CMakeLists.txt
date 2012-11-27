cmake_minimum_required(VERSION 2.8)

project( trainer )
find_package( OpenCV REQUIRED )

find_package( Boost 1.52 COMPONENTS filesystem program_options system REQUIRED )

add_executable(
	trainer
	trainer.cpp
	${TRAINER_SOURCE_DIR}../BeerCaddy/BeerCaddy/BeerClassifier.cpp
)
target_link_libraries( trainer ${OpenCV_LIBS} ${Boost_LIBRARIES})
include_directories( ${TRAINER_SOURCE_DIR}../BeerCaddy/BeerCaddy )
