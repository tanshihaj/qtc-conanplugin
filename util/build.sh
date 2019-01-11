#!/bin/bash

set -e

SELF_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

export PATH="$PWD/qt/bin/:$PATH"
#if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then
#    export PATH="/c/Qt/Tools/mingw730_64/bin:$PATH"
#fi
ROOT="$SELF_PATH/.."
echo `qmake --version`

export QTC_SOURCE="$PWD/qtcreator"
export QTC_BUILD="$PWD/qtcreator"

# Build plugin
rm -rf build && mkdir -p build && cd build
if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then
    NATIVE_ROOT_PATH=$(echo $ROOT | sed 's|/c|C:|g')
    cmd.exe /C '"C:/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/VC/Auxiliary/Build/vcvarsall.bat" x86 && qmake '$NATIVE_ROOT_PATH'/conan.pro && nmake'
else
    qmake "$ROOT/conan.pro"
    make -j2
fi
#if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then
#    mingw32-make
#else
#    make
#fi
cd ..

# Create archive
if [ -z "$VERSION" ]; then VERSION="debug"; fi
PLUGIN_NAME="Conan"
if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
    PLUGIN_FILE_NAME="lib$PLUGIN_NAME.dylib"
    PLUGIN_FILE_SOURCE_DIR="qtcreator/bin/Qt Creator.app/Contents/PlugIns"
    PLUGIN_FILE_TARGET_DIR="dist/Contents/PlugIns"
    ARCHIVE_FILE_NAME=$PLUGIN_NAME'-'$VERSION'_qtcreator-'$QTC_VERSION'_'$TRAVIS_OS_NAME'-x64.tar.gz'
fi
if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    PLUGIN_FILE_NAME="lib$PLUGIN_NAME.so"
    PLUGIN_FILE_SOURCE_DIR="qtcreator/lib/qtcreator/plugins"
    PLUGIN_FILE_TARGET_DIR="dist/lib/qtcreator/plugins"
    ARCHIVE_FILE_NAME=$PLUGIN_NAME'-'$VERSION'_qtcreator-'$QTC_VERSION'_'$TRAVIS_OS_NAME'-x64.tar.gz'
fi
if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then
    PLUGIN_FILE_NAME="$PLUGIN_NAME"4".dll"
    PLUGIN_FILE_SOURCE_DIR="qtcreator/lib/qtcreator/plugins"
    PLUGIN_FILE_TARGET_DIR="dist/lib/qtcreator/plugins"
    ARCHIVE_FILE_NAME=$PLUGIN_NAME'-'$VERSION'_qtcreator-'$QTC_VERSION'_'$TRAVIS_OS_NAME'-x86.zip'
fi

ls -l "$PLUGIN_FILE_SOURCE_DIR/$PLUGIN_FILE_NAME"

rm -rf "dist"
mkdir -p "$PLUGIN_FILE_TARGET_DIR"
cp "$PLUGIN_FILE_SOURCE_DIR/$PLUGIN_FILE_NAME" "$PLUGIN_FILE_TARGET_DIR"
if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
    strip "$PLUGIN_FILE_TARGET_DIR/$PLUGIN_FILE_NAME"
fi
ls -l "$PLUGIN_FILE_TARGET_DIR/$PLUGIN_FILE_NAME"

rm -f "$ARCHIVE_FILE_NAME"
cd dist
if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then
    7z a "../$ARCHIVE_FILE_NAME" *
else
    tar czf "../$ARCHIVE_FILE_NAME" .
fi
cd ..

ls -l "$ARCHIVE_FILE_NAME"



