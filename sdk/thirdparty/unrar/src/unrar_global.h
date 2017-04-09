#ifndef UNRAR_GLOBAL_H
#define UNRAR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UNRAR_LIBRARY)
#  define UNRARSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UNRARSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UNRAR_GLOBAL_H
