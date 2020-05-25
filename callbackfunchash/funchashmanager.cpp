#include <QString>
#include <QDebug>
#include <QVariant>

QVariantHash FunctionCallBackHash;

void SetFunctionCallBackHash( QVariantHash callbackhash )
{
	FunctionCallBackHash.unite( callbackhash );

	qDebug()<<"SetFunctionCallBackHash FunctionCallBackHash="<<FunctionCallBackHash;
}

void (*GetCallBackFuncByName( QString funcname ))( const QVariantHash &args )
{
	if (FunctionCallBackHash.value(funcname).type() == QMetaType::VoidStar)
	{
		void *callbackfunc = FunctionCallBackHash.value(funcname).value<void *>();
		return ( (void (*)( const QVariantHash & ))callbackfunc );
	}
	else
	{
		return NULL;
	}
}
