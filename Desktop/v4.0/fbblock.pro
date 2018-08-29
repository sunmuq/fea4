## Generated by HaiQ on Tue Nov 16 2010

TEMPLATE =	app

OBJECTS_DIR = ./build
MOC_DIR = ./build
DESTDIR = bin
TARGET = fbblock
CONFIG += qt release console qtestlib

HEADERS += fbblock.h arrays.h fbblocksolver.h
SOURCES += main.cpp ma.cpp fbblock.cpp fbblocksolver.cpp
FORMS +=

HEADERS += fbglobal.h mda_io.h fbparameterfile.h fbsparsearray4d.h fbsparsearray1d.h fberrorestimator.h fbtimer.h
SOURCES += fbglobal.cpp mda_io.cpp fbparameterfile.cpp fbsparsearray4d.cpp fbsparsearray1d.cpp fberrorestimator.cpp fbtimer.cpp

# COMMON CODE ###################
COMMONCODE += mda utils
message($$(DEVCOMMON))
include($$(DEVCOMMON)/devcommon.pri)
#utils
HEADERS += textfile.h
SOURCES += textfile.cpp
#mda
HEADERS += mda.h
SOURCES += mda.cpp
###################################
