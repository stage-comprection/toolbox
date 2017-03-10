TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/access_reads.cpp \
    src/compare_reads_files.cpp \
    src/compare_reads_mem.cpp \
    src/count_insertions.cpp \
    src/format_reads_file.cpp \
    src/get_bad_reads.cpp \
    src/get_reads_size.cpp \
    src/read.cpp \
    src/sort_reads.cpp \
    src/utils.cpp

HEADERS += \
    src/read.h \
    src/utils.h
