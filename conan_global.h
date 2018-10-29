/** @author Kamil Zaripov
    @date 28.10.2018 */

#pragma once

#include <QtGlobal>

#if defined(CONAN_LIBRARY)
#  define CONANSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONANSHARED_EXPORT Q_DECL_IMPORT
#endif
