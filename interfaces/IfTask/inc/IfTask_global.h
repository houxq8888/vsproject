#ifndef IFTASK_GLOBAL_H
#define IFTASK_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(IFTASK_LIBRARY)
#  define IFTASK_EXPORT Q_DECL_EXPORT
#else
#  define IFTASK_IMPORT Q_DECL_IMPORT
#endif

#endif // IFTASK_GLOBAL_H
