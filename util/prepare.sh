#!/bin/bash

set -e

case $ARCH in
    x64)
        case $TRAVIS_OS_NAME in
            linux)
                QTC_SUBFOLDER=linux_gcc_64_rhel72
                QT_INSTALL_PATH=$TRAVIS_BUILD_DIR/qt
                QT_MODULE_NAME=gcc_64
                QT_MODULE_DIR=gcc_64
            ;;
            windows)
                QTC_SUBFOLDER=windows_vs2015_64
                QT_INSTALL_PATH=$( echo $TRAVIS_BUILD_DIR/qt | sed -e 's|^\/c|C:|g' )
                QT_MODULE_NAME=win64_msvc2017_64
                QT_MODULE_DIR=msvc2017_64
            ;;
            osx)
                QTC_SUBFOLDER=mac_x64
                QT_INSTALL_PATH=$TRAVIS_BUILD_DIR/qt
                QT_MODULE_NAME=clang_64
                QT_MODULE_DIR=clang_64
            ;;
            *) echo $TRAVIS_OS_NAME" is unknown os" ; exit 1 ;;
        esac ;;
    x86)
        case $TRAVIS_OS_NAME in
            linux) echo $TRAVIS_OS_NAME"-"$ARCH" builds is unsupported" ; exit 1 ;;
            windows)
                QTC_SUBFOLDER=windows_vs2015_32
                QT_INSTALL_PATH=$( echo $TRAVIS_BUILD_DIR/qt | sed -e 's|^\/c|C:|g' )
                QT_MODULE_NAME=win32_msvc2017
                QT_MODULE_DIR=msvc2017
            ;;
            osx) echo $TRAVIS_OS_NAME"-"$ARCH" builds is unsupported" ; exit 1 ;;
            *) echo $TRAVIS_OS_NAME" is unknown os" ; exit 1 ;;
        esac ;;
    *) echo $ARCH" builds is unsupported" ; exit 1 ;;
esac


SELF_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mkdir -p download

# Qt Creator
if [ ! -d $TRAVIS_BUILD_DIR/qtcreator ]; then
    mkdir qtcreator

    QTC_VERSION_MAJOR=`echo $QTC_VERSION | cut -d. -f1`
    QTC_VERSION_MINOR=`echo $QTC_VERSION | cut -d. -f2`
    QTC_VERSION_PATCH=`echo $QTC_VERSION | cut -d. -f3`
    QTC_VERSION_SHORT=$QTC_VERSION_MAJOR.$QTC_VERSION_MINOR

    wget -cq http://download.qt.io/official_releases/qtcreator/$QTC_VERSION_SHORT/$QTC_VERSION/installer_source/$QTC_SUBFOLDER/qtcreator.7z -O download/qtcreator.7z
    if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
        7z x download/qtcreator.7z -oqtcreator/bin > /dev/null
    else
        7z x download/qtcreator.7z -oqtcreator > /dev/null
    fi

    # since qt-creator binaries by default doesn't contain sources and .lib files we need to download dev package
    wget -cq http://download.qt.io/official_releases/qtcreator/$QTC_VERSION_SHORT/$QTC_VERSION/installer_source/$QTC_SUBFOLDER/qtcreator_dev.7z -O download/qtcreator_dev.7z
    7z x download/qtcreator_dev.7z -oqtcreator -y > /dev/null
fi

# Qt
if [ ! -d $TRAVIS_BUILD_DIR/qt ]; then
    QT_VERSION_MAJOR=`echo $QT_VERSION | cut -d. -f1`
    QT_VERSION_MINOR=`echo $QT_VERSION | cut -d. -f2`
    QT_VERSION_PATCH=`echo $QT_VERSION | cut -d. -f3`
    QT_VERSION_JOINED=$QT_VERSION_MAJOR$QT_VERSION_MINOR$QT_VERSION_PATCH

    sed -i -e 's|INSTALL_PATH|'$QT_INSTALL_PATH'|g' "$SELF_PATH/qt_install.qs"
    sed -i -e 's|MODULE|qt.qt5.'$QT_VERSION_JOINED'.'$QT_MODULE_NAME'|g' "$SELF_PATH/qt_install.qs"

    case $TRAVIS_OS_NAME in
        linux)
            export QT_QPA_PLATFORM="minimal"
            wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run -O download/qt-installer.run
            chmod +x download/qt-installer.run
            sudo download/qt-installer.run --script "$SELF_PATH/qt_install.qs" --no-force-installations
        ;;
        windows)
            wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-windows-x86-online.exe -O download/qt-installer.exe
            download/qt-installer.exe --script "$SELF_PATH/qt_install.qs" --no-force-installations
        ;;
        osx)
            wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-mac-x64-online.dmg -O download/qt-installer.dmg
            hdiutil attach -mountpoint qt-installer download/qt-installer.dmg
            ln -s qt-installer/qt-*/Contents/MacOS/qt-* run
            sudo ./run --script "$SELF_PATH/qt_install.qs" --no-force-installations
            rm run
            hdiutil unmount qt-installer
        ;;
    esac
fi

ln -s $QT_INSTALL_PATH/$QT_VERSION/$QT_MODULE_DIR qtmodule
