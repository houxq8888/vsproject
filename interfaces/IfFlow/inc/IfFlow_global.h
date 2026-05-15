#ifndef IFLOW_GLOBAL_H
#define IFLOW_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(IFLOW_LIBRARY)
#  define IFLOW_EXPORT Q_DECL_EXPORT
#else
#  define IFLOW_IMPORT Q_DECL_IMPORT
#endif

#endif // IFLOW_GLOBAL_H
