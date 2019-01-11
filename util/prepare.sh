#!/bin/bash

set -e

SELF_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mkdir -p download

# Qt Creator
if [ ! -d "qtcreator" ]; then
    mkdir qtcreator

    if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then SUBFOLDER="mac_x64" ; fi
    if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then SUBFOLDER="linux_gcc_64_rhel72" ; fi
    if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then SUBFOLDER="windows_vs2015_32" ; fi

    QTC_VERSION_MAJOR=`echo $QTC_VERSION | cut -d. -f1`
    QTC_VERSION_MINOR=`echo $QTC_VERSION | cut -d. -f2`
    QTC_VERSION_PATCH=`echo $QTC_VERSION | cut -d. -f3`
    QTC_VERSION_SHORT=$QTC_VERSION_MAJOR.$QTC_VERSION_MINOR

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
    if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then
        wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-mac-x64-online.dmg -O download/qt-installer.dmg
        hdiutil attach -mountpoint qt-installer download/qt-installer.dmg
        ln -s qt-installer/qt-*/Contents/MacOS/qt-* run
        sed -i -e 's|INSTALL_PATH|/opt/qt|g' "$SELF_PATH/qt_install.qs"
        sed -i -e 's|MODULE|qt.qt5.5120.clang_64|g' "$SELF_PATH/qt_install.qs"
        sudo ./run --script "$SELF_PATH/qt_install.qs" --no-force-installations
        rm run
        hdiutil unmount qt-installer
        ln -s /opt/qt/5.12.0/clang_64 qt
    fi
    if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
        export QT_QPA_PLATFORM="minimal"
        wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run -O download/qt-installer.run
        chmod +x download/qt-installer.run
        sed -i -e 's|INSTALL_PATH|/opt/qt|g' "$SELF_PATH/qt_install.qs"
        sed -i -e 's|MODULE|qt.qt5.5120.gcc_64|g' "$SELF_PATH/qt_install.qs"
        sudo download/qt-installer.run --script "$SELF_PATH/qt_install.qs" --no-force-installations
        ln -s /opt/qt/5.12.0/gcc_64 qt
    fi
    if [[ "$TRAVIS_OS_NAME" == "windows" ]]; then
        wget -cq http://download.qt.io/official_releases/online_installers/qt-unified-windows-x86-online.exe -O download/qt-installer.exe
        sed -i -e 's|INSTALL_PATH|C:/Qt|g' "$SELF_PATH/qt_install.qs"
#        sed -i -e 's|MODULE|qt.qt5.5120.win64_mingw73|g' "$SELF_PATH/qt_install.qs"
        sed -i -e 's|MODULE|qt.qt5.5120.win32_msvc2017|g' "$SELF_PATH/qt_install.qs"

        download/qt-installer.exe --script "$SELF_PATH/qt_install.qs" --no-force-installations
#        ln -s C:/Qt/5.12.0/mingw73_64 qt
        ln -s C:/Qt/5.12.0/msvc2017 qt
    fi
fi
