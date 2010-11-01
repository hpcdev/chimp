#CHIMP:  Chemical Interactions, Materials, and Particles
#               Database and Simulation Framework
#    - Co-authored by Air Force Research Lab (U.S. Government)
#    - See AUTHORS for contributors

cmake_minimum_required( VERSION 2.6 )


project( chimp )

set( VERSION chimp-0.1.2-13-gc89373f )

include( CTest )

set( CHIMP_PARTICLEDB_XML
    "${CMAKE_CURRENT_SOURCE_DIR}/data/particledb.xml" CACHE STRING
    "Path to standard distribution of CHIMP Data"
)
mark_as_advanced( CHIMP_PARTICLEDB_XML )

add_definitions(
    -DCHMIP_VERSION=\"${VERSION}\"
    -DPARTICLEDB_XML=\"${CHIMP_PARTICLEDB_XML}\"
)


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

find_package( LibXml2 REQUIRED )

include_directories( ${LIBXML2_INCLUDE_DIR} )
add_definitions( ${LIBXML2_DEFINITIONS} )

add_library( ${PROJECT_NAME} 
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_SOURCES}
)


include_directories( ${Boost_INCLUDE_DIRS} )
target_link_libraries( ${PROJECT_NAME}
    physical_calc
    Xylose
)


# include the current directory for framework style includes
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )


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

