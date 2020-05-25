#ifndef MESSAGE__LOGGER__H
#define MESSAGE__LOGGER__H

void SetFunctionCallBackHash( QVariantHash callbackhash );
void(*GetCallBackFuncByName(QString funcname))(const QVariantHash &args);

#endif
