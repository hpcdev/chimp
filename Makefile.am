# Copyright 2004 Spencer Olson

MAKECONF=@OLSON_TOOLS_MAKECONF@
include $(MAKECONF)

lib_LIBRARIES = libparticledb.a

libparticledb_a_SOURCES = \
        ParticleDB.C

# Make it easy to install the headers
libparticledb_a_includedir = $(includedir)/particledb
nobase_libparticledb_a_include_HEADERS = \
	DSMCProperties.h \
        ParticleDB.h \
	Properties.h \
	VHSProperties.h
