TEMPLATE = app
CONFIG += c++14
CONFIG -= qt app_bundle

SOURCES += main.cpp

OTHER_FILES += conanfile.txt
CONFIG += conan_basic_setup
include($$OUT_PWD/conanbuildinfo.pri)
