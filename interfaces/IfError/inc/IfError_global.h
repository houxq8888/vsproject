#ifndef IFERROR_GLOBAL_H
#define IFERROR_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(IFERROR_LIBRARY)
#  define IFERROR_EXPORT Q_DECL_EXPORT
#else
#  define IFERROR_EXPORT Q_DECL_IMPORT
#endif

#endif // IFERROR_GLOBAL_H
