#ifndef DOODLELIB_GLOBAL_H
#define DOODLELIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DOODLELIB_LIBRARY)
#  define DOODLELIB_EXPORT Q_DECL_EXPORT
#else
#  define DOODLELIB_EXPORT Q_DECL_IMPORT
#endif

#endif // DOODLELIB_GLOBAL_H
