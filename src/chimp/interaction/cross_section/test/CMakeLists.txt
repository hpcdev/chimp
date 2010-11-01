path-constant LOTZ_FILENAME : ./lotz.xml ;

chimp_unit_test( Lotz Lotz.cpp )
set_target_properties( chimp.Lotz.test PROPERTIES
    COMPILE_DEFINITIONS XML_FILENAME=\"${LOTZ_FILENAME}\"
)

