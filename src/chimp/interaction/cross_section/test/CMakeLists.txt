set( LOTZ_FILENAME ${CMAKE_CURRENT_SOURCE_DIR}/lotz.xml )

chimp_unit_test( interaction.cross_section.Lotz Lotz.cpp )
add_definitions( -DXML_FILENAME=${LOTZ_FILENAME} )

