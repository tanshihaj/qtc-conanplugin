#!/bin/bash

set -e

SELF_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mkdir -p download

# Qt Creator
if [ ! -d "qtcreator" ]; then
    mkdir qtcreator

    QTC_VERSION_MAJOR=`echo $QTC_VERSION | cut -d. -f1`
    QTC_VERSION_MINOR=`echo $QTC_VERSION | cut -d. -f2`
    QTC_VERSION_PATCH=`echo $QTC_VERSION | cut -d. -f3`
    QTC_VERSION_SHORT=$QTC_VERSION_MAJOR.$QTC_VERSION_MINOR

    if [[ "$ARCH" == "x64" ]]; then
        case $TRAVIS_OS_NAME in
            osx) SUBFOLDER="mac_x64" ;;
            linux) SUBFOLDER="linux_gcc_64_rhel72" ;;
            windows) SUBFOLDER="windows_vs2015_64" ;;
            *)
                echo $TRAVIS_OS_NAME" is unknown os"
                exit 1
            ;;
        esac
    elif [[ "$ARCH" == "x86" ]]; then
        case $TRAVIS_OS_NAME in
            osx)
                echo $TRAVIS_OS_NAME"-"$ARCH" builds is unsupported"
                exit 1
            ;;
            linux)
                echo $TRAVIS_OS_NAME"-"$ARCH" builds is unsupported"
                exit 1
            ;;
            windows) SUBFOLDER="windows_vs2015_32" ;;
            *)
                echo $TRAVIS_OS_NAME" is unknown os"
                exit 1
            ;;
        esac
    else
        echo $ARCH" builds is unsupported"
        exit 1
    fi

    wget -cq http://download.qt.io/official_releases/qtcreator/$QTC_VERSION_SHORT/$QTC_VERSION/installer_source/$SUBFOLDER/qtcreator.7z -O download/qtcreator.7z
    if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
        7z x download/qtcreator.7z -oqtcreator/bin > /dev/null
    else
        7z x download/qtcreator.7z -oqtcreator > /dev/null
    fi

    # since qt-creator binaries by default doesn't contain sources and .lib files we need to download dev package
    wget -cq http://download.qt.io/official_releases/qtcreator/$QTC_VERSION_SHORT/$QTC_VERSION/installer_source/$SUBFOLDER/qtcreator_dev.7z -O download/qtcreator_dev.7z
    7z x download/qtcreator_dev.7z -oqtcreator -y > /dev/null
fi

# Qt
if [ ! -d "qt" ]; then
    QT_VERSION_MAJOR=`echo $QT_VERSION | cut -d. -f1`
    QT_VERSION_MINOR=`echo $QT_VERSION | cut -d. -f2`
    QT_VERSION_PATCH=`echo $QT_VERSION | cut -d. -f3`
    QT_VERSION_JOINED=$QT_VERSION_MAJOR$QT_VERSION_MINOR$QT_VERSION_PATCH


    if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
        if [[ "$ARCH" != "x64" ]]; then
            echo $TRAVIS_OS_NAME"-"$ARCH" builds is unsupported"
            exit 1
        fi
        QT_MODULE_NAME=clang_64
        QT_MODULE_DIR=clang_64

        wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-mac-x64-online.dmg -O download/qt-installer.dmg
        hdiutil attach -mountpoint qt-installer download/qt-installer.dmg
        ln -s qt-installer/qt-*/Contents/MacOS/qt-* run
        sed -i -e 's|INSTALL_PATH|/opt/qt|g' "$SELF_PATH/qt_install.qs"
        sed -i -e 's|MODULE|qt.qt5.'$QT_VERSION_JOINED'.'$QT_MODULE_NAME'|g' "$SELF_PATH/qt_install.qs"
        sudo ./run --script "$SELF_PATH/qt_install.qs" --no-force-installations
        rm run
        hdiutil unmount qt-installer
        ln -s /opt/qt/$QT_VERSION/$QT_MODULE_DIR qt
    fi
    if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
        if [[ "$ARCH" != "x64" ]]; then
            echo $TRAVIS_OS_NAME"-"$ARCH" builds is unsupported"
            exit 1
        fi
        QT_MODULE_NAME=gcc_64
        QT_MODULE_DIR=gcc_64

        export QT_QPA_PLATFORM="minimal"
        wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run -O download/qt-installer.run
        chmod +x download/qt-installer.run
        sed -i -e 's|INSTALL_PATH|/opt/qt|g' "$SELF_PATH/qt_install.qs"
        sed -i -e 's|MODULE|qt.qt5.'$QT_VERSION_JOINED'.'$QT_MODULE_NAME'|g' "$SELF_PATH/qt_install.qs"
        sudo download/qt-installer.run --script "$SELF_PATH/qt_install.qs" --no-force-installations
        ln -s /opt/qt/$QT_VERSION/$QT_MODULE_DIR qt
    fi
    if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then
        if [[ "$ARCH" == "x64" ]]; then
#            QT_MODULE_NAME=win64_mingw73
            QT_MODULE_NAME=win64_msvc2017_64
#            QT_MODULE_DIR=mingw73_64
            QT_MODULE_DIR=msvc2017_64
        elif [[ "$ARCH" == "x86" ]]; then
            QT_MODULE_NAME=win32_msvc2017
            QT_MODULE_DIR=msvc2017
        else
            echo $ARCH" builds is unsupported"
            exit 1
        fi

        wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-windows-x86-online.exe -O download/qt-installer.exe
        sed -i -e 's|INSTALL_PATH|C:/Qt|g' "$SELF_PATH/qt_install.qs"
        sed -i -e 's|MODULE|qt.qt5.'$QT_VERSION_JOINED'.'$QT_MODULE_NAME'|g' "$SELF_PATH/qt_install.qs"

        download/qt-installer.exe --script "$SELF_PATH/qt_install.qs" --no-force-installations
        ln -s /c/Qt/$QT_VERSION/$QT_MODULE_DIR qt
    fi
fi
