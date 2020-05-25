#include <QDebug>
#include <QCoreApplication>

#include "funchashmanager.h"

void Function1( const QVariantHash &args )
{
	qDebug()<<"Function1 args="<<args;
}

void Function2( const QVariantHash &args )
{
	qDebug()<<"Function2 args="<<args;
}

void Function3( const QVariantHash &args )
{
	qDebug()<<"Function3 args="<<args;
}

void Function4( const QVariantHash &args )
{
	qDebug()<<"Function4 args="<<args;
}

static QVariantHash AssembleCallBackFuncHash( )
{
	QVariant onecallbackfunc;
	QVariantHash callbackfunchash;

	onecallbackfunc.setValue((void*)&Function1);
	callbackfunchash.insert( "Function1", onecallbackfunc );

	onecallbackfunc.setValue((void*)&Function2);
	callbackfunchash.insert( "Function2", onecallbackfunc );

	onecallbackfunc.setValue((void*)&Function3);
	callbackfunchash.insert( "Function3", onecallbackfunc );

	onecallbackfunc.setValue((void*)&Function4);
	callbackfunchash.insert( "Function4", onecallbackfunc );

	return callbackfunchash;
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QVariantHash funchash = AssembleCallBackFuncHash();

	SetFunctionCallBackHash( funchash );

	void (*onefunction)( const QVariantHash &args ) = NULL;

	onefunction = GetCallBackFuncByName("Function4");

	if( onefunction )
	{
		QVariantHash args;
		args["arg1"] = "127.0.0.1";
		args["arg2"] = 11111;
		
		(*onefunction)(args);
	}

	return app.exec();
}
