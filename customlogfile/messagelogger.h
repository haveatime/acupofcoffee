#ifndef MESSAGE__LOGGER__H
#define MESSAGE__LOGGER__H

#if QT_VERSION>=QT_VERSION_CHECK(5,0,0)
void messageLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg);
#else
void messageLogger(QtMsgType type, const char *msg);
#endif

#endif
