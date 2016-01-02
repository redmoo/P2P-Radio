#ifndef COMMON_GLOBAL_H
#define COMMON_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(COMMON_LIBRARY)
#  define COMMONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMMONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMON_GLOBAL_H
