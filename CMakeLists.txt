cmake_minimum_required(VERSION 3.2)
project(terminator CXX)

find_package(PkgConfig)
pkg_search_module(KYOTOCABINET REQUIRED kyotocabinet)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)

set ( CMAKE_CXX_FLAGS -std=c++17 )
set ( CMAKE_CXX_FLAGS "-Wl,--as-needed -Wl,--allow-shlib-undefined")
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set ( TERMINATOR_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set ( TERMINATOR_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set ( TERMINATOR_BUILD_DIR "${CMAKE_BINARY_DIR}/build" CACHE STRING "Set build dir")

set ( TERMINATOR_BUILD_LIB_DIR ${TERMINATOR_BUILD_DIR}/lib)
set ( TERMINATOR_BUILD_INCLUDE_DIR ${TERMINATOR_BUILD_DIR}/include)
set ( LIBRARY_OUTPUT_PATH ${TERMINATOR_BUILD_LIB_DIR} CACHE PATH "Build directory" FORCE)

include_directories(${TERMINATOR_INCLUDE})

set(SOURCE_LIBTERMINATOR
	${TERMINATOR_SRC}/terminator.cc
	${TERMINATOR_SRC}/terminator_classifier_base.cc
	${TERMINATOR_SRC}/terminator_classifier_bwinnow.cc
	${TERMINATOR_SRC}/terminator_classifier_hit.cc
	${TERMINATOR_SRC}/terminator_classifier_lr.cc
	${TERMINATOR_SRC}/terminator_classifier_nb.cc
	${TERMINATOR_SRC}/terminator_classifier_nsnb.cc
	${TERMINATOR_SRC}/terminator_classifier_owv.cc
	${TERMINATOR_SRC}/terminator_classifier_pa.cc
	${TERMINATOR_SRC}/terminator_classifier_pam.cc
	${TERMINATOR_SRC}/terminator_classifier_winnow.cc
)



add_library(
    ${PROJECT_NAME}_obj
    OBJECT
    ${SOURCE_LIBTERMINATOR}
)

add_library(
    ${PROJECT_NAME}_static
    STATIC
    $<TARGET_OBJECTS:${PROJECT_NAME}_obj>
)

add_library(
    ${PROJECT_NAME}
    SHARED
    $<TARGET_OBJECTS:${PROJECT_NAME}_obj>
)

target_link_libraries(${PROJECT_NAME} ${KYOTOCABINET_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_static PUBLIC ${KYOTOCABINET_LIBRARIES})

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_static DESTINATION lib)
install(FILES 	${TERMINATOR_INCLUDE}/terminator_classifier_base.h
				${TERMINATOR_INCLUDE}/terminator_classifier_bwinnow.h
				${TERMINATOR_INCLUDE}/terminator_classifier_hit.h
				${TERMINATOR_INCLUDE}/terminator_classifier_lr.h
				${TERMINATOR_INCLUDE}/terminator_classifier_nb.h
				${TERMINATOR_INCLUDE}/terminator_classifier_nsnb.h
				${TERMINATOR_INCLUDE}/terminator_classifier_owv.h
				${TERMINATOR_INCLUDE}/terminator_classifier_pa.h
				${TERMINATOR_INCLUDE}/terminator_classifier_pam.h
				${TERMINATOR_INCLUDE}/terminator_classifier_winnow.h
				${TERMINATOR_INCLUDE}/terminator_common.h
				${TERMINATOR_INCLUDE}/terminator.h

				DESTINATION include)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_NAME "mint-terminator")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A tool for spam filtering")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Aleksandr Matuzok (sherilynhope@gmail.com)")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS	"libkyotocabinet-dev")
INCLUDE(CPack)
