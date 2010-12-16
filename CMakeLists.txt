#CHIMP:  Chemical Interactions, Materials, and Particles
#               Database and Simulation Framework
#    - Co-authored by Air Force Research Lab (U.S. Government)
#    - See AUTHORS for contributors

cmake_minimum_required( VERSION 2.6 )


project( chimp )

include( CTest )

################################################################################
# Obtain the chimp version
################################################################################
set( VERSION )

find_program( GIT_EXECUTABLE NAMES git )
if( GIT_EXECUTABLE )
  exec_program(
    ${GIT_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR} ARGS describe
    OUTPUT_VARIABLE VERSION
    RETURN_VALUE GIT_RETVAL )
  if( NOT ${GIT_RETVAL} EQUAL 0 )
    # Something went wrong, fall back on other methods.
    # We still didn't find a version, so it's really unknown.
    set( VERSION "chimp-unknownversion" )
  endif()
endif()

message( STATUS "chimp VERSION: ${VERSION}" )


set( CHIMP_PARTICLEDB_XML
    "${CMAKE_CURRENT_SOURCE_DIR}/data/particledb.xml" CACHE STRING
    "Path to standard distribution of CHIMP Data"
)
mark_as_advanced( CHIMP_PARTICLEDB_XML )

LIST(APPEND ${PROJECT_NAME}_DEFINITIONS
    -DCHIMP_VERSION=${VERSION}
    -DCHIMP_PARTICLEDB_XML=${CHIMP_PARTICLEDB_XML}
)

# include the current directory for framework style includes
LIST(APPEND ${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src )

# FIXME:  physical needs a find module
# find_package( physical )
add_definitions( ${physical_DEFINITIONS} )
include_directories( ${physical_INCLUDE_DIRS} )


find_package( LibXml2 REQUIRED )
add_definitions( ${LIBXML2_DEFINITIONS} )
include_directories( ${LIBXML2_INCLUDE_DIR} )

find_package( Boost REQUIRED )
add_definitions( ${Boost_DEFINITIONS} )
include_directories( ${Boost_INCLUDE_DIRS} )

# /chimp//particledb configuration
set( ${PROJECT_NAME}_HEADERS
    src/chimp/RuntimeDB.h
    src/chimp/make_options.h
    src/chimp/interaction/Term.h
    src/chimp/interaction/Input.h
    src/chimp/interaction/Driver.h
    src/chimp/interaction/model/Elastic.h
    src/chimp/interaction/model/InElastic.h
    src/chimp/interaction/model/detail/vss_helpers.h
    src/chimp/interaction/model/test/diagnostics.h
    src/chimp/interaction/model/Base.h
    src/chimp/interaction/model/VSSElastic.h
    src/chimp/interaction/selectRandomPair.h
    src/chimp/interaction/Particle.h
    src/chimp/interaction/cross_section/Constant.h
    src/chimp/interaction/cross_section/VHS.h
    src/chimp/interaction/cross_section/Lotz.h
    src/chimp/interaction/cross_section/AveragedDiameters.h
    src/chimp/interaction/cross_section/DATA.h
    src/chimp/interaction/cross_section/detail/VHSInfo.h
    src/chimp/interaction/cross_section/detail/LotzDetails.h
    src/chimp/interaction/cross_section/detail/AvgEasy.h
    src/chimp/interaction/cross_section/detail/logE_E.h
    src/chimp/interaction/cross_section/Base.h
    src/chimp/interaction/Set.h
    src/chimp/interaction/Equation.h
    src/chimp/interaction/v_rel_fnc.h
    src/chimp/interaction/detail/sort_terms.h
    src/chimp/interaction/detail/DriverRetval.h
    src/chimp/interaction/filter/Null.h
    src/chimp/interaction/filter/Section.h
    src/chimp/interaction/filter/And.h
    src/chimp/interaction/filter/Elastic.h
    src/chimp/interaction/filter/EqIO.h
    src/chimp/interaction/filter/Not.h
    src/chimp/interaction/filter/detail/EqPair.h
    src/chimp/interaction/filter/Base.h
    src/chimp/interaction/filter/Or.h
    src/chimp/interaction/filter/Label.h
    src/chimp/interaction/ReducedMass.h
    src/chimp/property/charge.h
    src/chimp/property/size.h
    src/chimp/property/Null.h
    src/chimp/property/name.h
    src/chimp/property/polarizability.h
    src/chimp/property/aggregate.h
    src/chimp/property/DefaultSet.h
    src/chimp/property/detail/list.h
    src/chimp/property/detail/check.h
    src/chimp/property/Comparator.h
    src/chimp/property/define.h
    src/chimp/property/mass.h
    src/chimp/accessors.h
    src/chimp/physical_calc.h
)

set( ${PROJECT_NAME}_SOURCES
    src/chimp/physical_calc.cpp
    src/chimp/interaction/filter/Base.cpp
    src/chimp/interaction/cross_section/DATA.cpp
    src/chimp/interaction/cross_section/Constant.cpp
    src/chimp/interaction/cross_section/detail/VHSInfo.cpp
    src/chimp/interaction/cross_section/detail/LotzDetails.cpp
    src/chimp/interaction/model/detail/vss_helpers.cpp
)

add_definitions( ${${PROJECT_NAME}_DEFINITIONS} )
include_directories( ${${PROJECT_NAME}_INCLUDE_DIRS} )
add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries( ${PROJECT_NAME}
    physical_calc
    Xylose
)

install( DIRECTORY data DESTINATION share/chimp )

# Bogus stuff to help simulate a find module for this project
set( ${PROJECT_NAME}_DEFINITIONS ${${PROJECT_NAME}_DEFINITIONS} PARENT_SCOPE )
set( ${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} PARENT_SCOPE )


# utility macro to add a unit test to be shared by all subdirectories.
macro( chimp_unit_test test_name )
    set(BOOST_USE_STATIC_LIBS_TMP ${Boost_USE_STATIC_LIBS})
    set(Boost_USE_STATIC_LIBS ON)
    find_package( Boost REQUIRED COMPONENTS unit_test_framework )
    set(BOOST_USE_STATIC_LIBS ${Boost_USE_STATIC_LIBS_TMP})
    include_directories( ${Boost_INCLUDE_DIRS} )
    add_executable( chimp.${test_name}.test ${ARGN} )
    target_link_libraries( chimp.${test_name}.test
        ${PROJECT_NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    add_test( chimp.${test_name} chimp.${test_name}.test )
endmacro()

# add source directory to get the unit tests recursively
add_subdirectory( src )

