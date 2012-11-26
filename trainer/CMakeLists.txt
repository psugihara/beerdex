cmake_minimum_required(VERSION 2.8)

project( trainer )
find_package( OpenCV REQUIRED )
add_executable(
	trainer
	trainer.cpp
	${TRAINER_SOURCE_DIR}../BeerCaddy/BeerCaddy/BeerClassifier.cpp
)
target_link_libraries( trainer ${OpenCV_LIBS} )
include_directories( ${TRAINER_SOURCE_DIR}../BeerCaddy/BeerCaddy )
