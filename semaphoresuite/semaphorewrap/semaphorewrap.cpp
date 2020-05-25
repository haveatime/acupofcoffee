
#include <QDebug>

#include "semaphorewrap.h"

SemaphoreWrap::~SemaphoreWrap()
{
}

bool SemaphoreWrap::create(const QVariantHash &args)
{
	bool flag = false;

	if( category.size() > 0 )
	{
		qDebug() << "SemaphoreWrap::create already created category=" << category;
		return flag;
	}
	
	QString demand_category = args["category"].toString();
	int demand_value = args["value"].toInt();

	if( "system" == demand_category )
	{
		syswidekey = args["key"].toString();
		syswidemode = args["mode"].toString();

		if( "open" == syswidemode )
			syswidesem = QSharedPointer<QSystemSemaphore>(new QSystemSemaphore(syswidekey, demand_value, QSystemSemaphore::Open));
		else if( "create" == syswidemode )
			syswidesem = QSharedPointer<QSystemSemaphore>(new QSystemSemaphore(syswidekey, demand_value, QSystemSemaphore::Create));
		else
		{
			qDebug() << "SemaphoreWrap::create access is wrong! syswidemode=" << syswidemode;
			return flag;
		}

		flag = true;
	}
	else if( "local" == demand_category )
	{
		localsem = QSharedPointer<QSemaphore>(new QSemaphore(demand_value));

		flag = true;
	}
	else
	{
		qDebug() << "SemaphoreWrap::create category is wrong! args=" << args;
	}

	if( flag )
	{
		category = demand_category;
		value = demand_value;
	}

	return flag;
}

bool SemaphoreWrap::acquire( int n )
{
	bool flag = false;

	if( category.size() <= 0 )
	{
		qDebug() << "SemaphoreWrap::acquire error! category.size()<=0";
		return flag;
	}

	if( n <= 0 )
	{
		qDebug() << "SemaphoreWrap::acquire error! n<=0";
		return flag;
	}

	if( "system" == category )
	{
		int try_value = 0;
		bool one_flag = false;

		for( try_value=0; try_value<n; try_value++ )
		{
			one_flag = syswidesem->acquire( );
			if( !one_flag )
			{
				qDebug() << "SemaphoreWrap::acquire system acquire error="<<syswidesem->errorString()<<"try_value="<<try_value;
				break;
			}
		}

		if( one_flag )
			flag = true;
		else
		{
			if( try_value>0 )
				syswidesem->release( try_value );
		}
	}
	else if( "local" == category )
	{
		flag = localsem->tryAcquire( n );
	}
	else
	{
		qDebug() << "SemaphoreWrap::acquire category="<<category<<"is wrong!";
	}

	return flag;
}

bool SemaphoreWrap::release( int n )
{
	bool flag = false;

	if( category.size() <= 0 )
	{
		qDebug() << "SemaphoreWrap::release error! category.size()<=0";
		return flag;
	}

	if( n <= 0 )
	{
		qDebug() << "SemaphoreWrap::release error! n<=0";
		return flag;
	}
	
	if( "system" == category )
	{
		flag = syswidesem->release( n );
		if( !flag )
		{
			qDebug() << "SemaphoreWrap::release system release error="<<syswidesem->errorString()<<"n="<<n;
		}
	}
	else if( "local" == category )
	{
		localsem->release( n );
		flag = true;
	}
	else
	{
		qDebug() << "SemaphoreWrap::release category="<<category<<"is wrong!";
	}
	return flag;
}

