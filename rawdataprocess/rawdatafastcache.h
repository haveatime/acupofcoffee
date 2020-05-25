#ifndef RAW__DATA__FAST__CACHE__H
#define RAW__DATA__FAST__CACHE__H

#include <QString>
#include <QHash>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker> 
#include <QDateTime> 

/************************
* LINKID_T 链路ID类型
* POINTID_T 点ID类型
* DATAVALUE_T 点数据类型
************************/
template <typename LINKID_T, typename POINTID_T, typename DATAVALUE_T>
class RawDataFastCache
{
	public:
		typedef LINKID_T LinkIdType;
		typedef POINTID_T PointIdType;

	public:
		struct DataKeyType
		{
			LinkIdType link_id;
			PointIdType point_id;
			inline bool operator==( const DataKeyType &right )
			{
				if( this->link_id==right.link_id && this->point_id==right.point_id )
					return true;
				else
					return false;
			}
		};
 
		struct ForwardRawDataType
		{
			DataKeyType data_key;
			DATAVALUE_T data_value;
			inline bool operator==( const ForwardRawDataType &right )
			{
				if( this->data_key==right.data_key && this->data_value==right.data_value )
					return true;
				else
					return false;
			}
		};

	private:
		typedef QHash<PointIdType, ForwardRawDataType> SingleLinkData;

		QString cachename;
		QMutex cachelocker;
		QHash< LinkIdType, SingleLinkData > rawdatacache;
		QQueue< DataKeyType > normalfifo;
		QQueue< ForwardRawDataType > fastfifo;
		int datanumber;
		qint64 firstjointime;  /* 第一个数据加入cache的时间  */
		qint64 emptyleavetime; /* 最后一个数据清空cache的时间  */
		int endureinterval;    /* 清空整个cache的最大忍受时间  */
		int capacitymax;       /* 整个cache的数据最大容量  */

	public:
		RawDataFastCache<LINKID_T,POINTID_T,DATAVALUE_T>( const char* name, int delay=900000, int capacity=100000 );
		int ReadOneData( ForwardRawDataType &data );
		int AddOneData( ForwardRawDataType &data );
		int DelOneData( void );
};

template <typename LINKID_T, typename POINTID_T, typename DATAVALUE_T>
RawDataFastCache<LINKID_T,POINTID_T,DATAVALUE_T>::RawDataFastCache( const char* name, int delay, int capacity )
	:cachename(name),datanumber(0),firstjointime(0),emptyleavetime(0),endureinterval(delay),capacitymax(capacity)
{
}

template <typename LINKID_T, typename POINTID_T, typename DATAVALUE_T>
int RawDataFastCache<LINKID_T,POINTID_T,DATAVALUE_T>::ReadOneData( ForwardRawDataType &data )
{
	QMutexLocker locker(&cachelocker);

	if( !fastfifo.isEmpty() )
	{
		data = fastfifo.head();

		return 1;
	}
	else if( !normalfifo.isEmpty() )
	{
		DataKeyType keyword = normalfifo.head();

		data = rawdatacache[keyword.link_id][keyword.point_id];

		return 1;
	}
	else
	{
		return 0;
	}
}

template <typename LINKID_T, typename POINTID_T, typename DATAVALUE_T>
int RawDataFastCache<LINKID_T,POINTID_T,DATAVALUE_T>::AddOneData( ForwardRawDataType &data )
{
	QMutexLocker locker(&cachelocker);

	if( datanumber > capacitymax )
	{
		qint64 currtime = QDateTime::currentMSecsSinceEpoch();
		int interval = currtime-firstjointime;
		qDebug() << "RawDataFastCache::AddOneData FULL interval=" << interval << "cachename=" << cachename;
	}

	if( datanumber == 0 )
	{
		firstjointime = QDateTime::currentMSecsSinceEpoch();
	}

	if( rawdatacache.contains(data.data_key.link_id) )
	{
		SingleLinkData &savedlink = rawdatacache[data.data_key.link_id];

		if( savedlink.contains(data.data_key.point_id) )
		{
			ForwardRawDataType &savedpos = savedlink[data.data_key.point_id];
			DataKeyType keyword{data.data_key.link_id,data.data_key.point_id};

			if( savedpos.data_value==data.data_value )
			{
				if( !normalfifo.contains(keyword) && !fastfifo.contains(data) )
				{
					normalfifo.enqueue(keyword);
					datanumber ++;
				}
			}
			else
			{
				savedlink[data.data_key.point_id] = data;

				fastfifo.enqueue(data);
				datanumber ++;

				int count = normalfifo.removeAll(keyword);
				datanumber -= count;
			}
		}
		else
		{
			savedlink[data.data_key.point_id] = data;

			fastfifo.enqueue(data);
			datanumber ++;
		}
	}
	else
	{
		SingleLinkData newlink;
		newlink[data.data_key.point_id] = data;

		rawdatacache[data.data_key.link_id] = newlink;

		fastfifo.enqueue(data);
		datanumber ++;
	}

	return 1;
}

template <typename LINKID_T, typename POINTID_T, typename DATAVALUE_T>
int RawDataFastCache<LINKID_T,POINTID_T,DATAVALUE_T>::DelOneData( void )
{
	QMutexLocker locker(&cachelocker);

	if( !fastfifo.isEmpty() )
	{
		fastfifo.dequeue();
		datanumber --;
	}
	else if( !normalfifo.isEmpty() )
	{
		normalfifo.dequeue();
		datanumber --;
	}

	if( datanumber == 0 )
	{
		emptyleavetime = QDateTime::currentMSecsSinceEpoch();

		int interval = emptyleavetime - firstjointime;
		if( interval > endureinterval )
		{
			qDebug() << "RawDataFastCache::DelOneData DELAY interval=" << interval << "cachename=" << cachename;
		}
	}

	return 1;
}


#endif
