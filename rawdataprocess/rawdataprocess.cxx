#include <QDebug>
#include <QCoreApplication>
#include<QVector>

#include "rawdatafastcache.h"

/* 链路ID类型 */
typedef long LINK_ID_TYPE;

/* 点ID类型 */
typedef int POINT_ID_TYPE;

/* 点数据类型 */
typedef struct DATA_VALUE_TYPE
{
	int value;
	int quality;
}DataValueType;

inline bool operator==(const DataValueType &left, const DataValueType &right)
{
	if (left.value == right.value && left.quality == right.quality)
		return true;
	else
		return false;
}

int main(int argc, char **argv)
{
	int datacount, index;

	QCoreApplication qtapp(argc, argv);

	RawDataFastCache<LINK_ID_TYPE,POINT_ID_TYPE,DataValueType> cachetest("mycache1");

	typedef RawDataFastCache<LINK_ID_TYPE,POINT_ID_TYPE,DataValueType>::ForwardRawDataType ForwardRawDataType;

	QVector<ForwardRawDataType> datagroup{
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 3}, {1, 0} },   /* 链路11,采集点号3,值1,质量码0 */
			{ {3, 7}, {1, 0} },   /* 链路3,采集点号7,值1,质量码0 */
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 3}, {0, 0} },   /* 链路11,采集点号3,值0,质量码0 */
			{ {3, 3}, {1, 0} },   /* 链路3,采集点号3,值1,质量码0 */
			{ {3, 7}, {1, 0} },   /* 链路3,采集点号7,值1,质量码0 */
			{ {11, 5}, {1, 0} },   /* 链路11,采集点号5,值1,质量码0 */
			{ {11, 3}, {0, 0} },   /* 链路11,采集点号3,值0,质量码0 */
			{ {3, 3}, {0, 0} },   /* 链路3,采集点号3,值0,质量码0 */
			{ {3, 5}, {1, 0} },   /* 链路3,采集点号5,值1,质量码0 */
			{ {3, 3}, {0, 0} },   /* 链路3,采集点号3,值0,质量码0 */
			{ {11, 6}, {1, 0} },   /* 链路11,采集点号6,值1,质量码0 */
			{ {3, 6}, {1, 0} },   /* 链路3,采集点号6,值1,质量码0 */
			{ {11, 13}, {1, 0} },   /* 链路11,采集点号13,值1,质量码0 */
			{ {11, 5}, {1, 0} },   /* 链路11,采集点号5,值1,质量码0 */
			{ {11, 6}, {1, 0} },   /* 链路11,采集点号6,值1,质量码0 */
			{ {3, 13}, {1, 0} },   /* 链路3,采集点号13,值1,质量码0 */
			{ {3, 5}, {1, 0} },   /* 链路3,采集点号5,值1,质量码0 */
			{ {11, 5}, {0, 0} },   /* 链路11,采集点号5,值0,质量码0 */
			{ {3, 6}, {1, 0} },   /* 链路3,采集点号6,值1,质量码0 */
			{ {3, 5}, {0, 0} },   /* 链路3,采集点号5,值0,质量码0 */
			{ {11, 3}, {0, 0} },   /* 链路11,采集点号3,值0,质量码0 */
			{ {11, 4}, {1, 0} },   /* 链路11,采集点号4,值1,质量码0 */
			{ {3, 3}, {0, 0} },   /* 链路3,采集点号3,值0,质量码0 */
			{ {3, 4}, {1, 0} },   /* 链路3,采集点号4,值1,质量码0 */
			{ {3, 5}, {1, 0} },   /* 链路3,采集点号5,值1,质量码0 */
			{ {11, 5}, {1, 0} },   /* 链路11,采集点号5,值1,质量码0 */
			{ {11, 26}, {1, 0} },  /* 链路11,采集点号26,值1,质量码0 */
			{ {3, 26}, {1, 0} }   /* 链路3,采集点号26,值1,质量码0 */
	};

	QVector<ForwardRawDataType> datachange{
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {1, 0} },   /* 链路11,采集点号7,值1,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {0, 0} },   /* 链路11,采集点号7,值0,质量码0 */
			{ {11, 7}, {0, 0} }   /* 链路11,采集点号7,值0,质量码0 */
	};

	/*  这两个链路的数据,缓存处理两次后的结果如下:
	---stage1---
	link_id= 11 point_id= 7 value= 1 quality= 0
	link_id= 11 point_id= 3 value= 1 quality= 0
	link_id= 3 point_id= 7 value= 1 quality= 0
	link_id= 11 point_id= 3 value= 0 quality= 0
	link_id= 3 point_id= 3 value= 1 quality= 0
	link_id= 11 point_id= 5 value= 1 quality= 0
	link_id= 3 point_id= 3 value= 0 quality= 0
	link_id= 3 point_id= 5 value= 1 quality= 0
	link_id= 11 point_id= 6 value= 1 quality= 0
	link_id= 3 point_id= 6 value= 1 quality= 0
	link_id= 11 point_id= 13 value= 1 quality= 0
	link_id= 3 point_id= 13 value= 1 quality= 0
	link_id= 11 point_id= 5 value= 0 quality= 0
	link_id= 3 point_id= 5 value= 0 quality= 0
	link_id= 11 point_id= 4 value= 1 quality= 0
	link_id= 3 point_id= 4 value= 1 quality= 0
	link_id= 3 point_id= 5 value= 1 quality= 0
	link_id= 11 point_id= 5 value= 1 quality= 0
	link_id= 11 point_id= 26 value= 1 quality= 0
	link_id= 3 point_id= 26 value= 1 quality= 0
	---stage2---
	link_id= 11 point_id= 3 value= 1 quality= 0
	link_id= 11 point_id= 3 value= 0 quality= 0
	link_id= 3 point_id= 3 value= 1 quality= 0
	link_id= 3 point_id= 3 value= 0 quality= 0
	link_id= 11 point_id= 5 value= 0 quality= 0
	link_id= 3 point_id= 5 value= 0 quality= 0
	link_id= 3 point_id= 5 value= 1 quality= 0
	link_id= 11 point_id= 5 value= 1 quality= 0
	link_id= 11 point_id= 7 value= 1 quality= 0
	link_id= 3 point_id= 7 value= 1 quality= 0
	link_id= 11 point_id= 6 value= 1 quality= 0
	link_id= 3 point_id= 6 value= 1 quality= 0
	link_id= 11 point_id= 13 value= 1 quality= 0
	link_id= 3 point_id= 13 value= 1 quality= 0
	link_id= 11 point_id= 4 value= 1 quality= 0
	link_id= 3 point_id= 4 value= 1 quality= 0
	link_id= 11 point_id= 26 value= 1 quality= 0
	link_id= 3 point_id= 26 value= 1 quality= 0
	---stage3---
	link_id= 11 point_id= 7 value= 0 quality= 0
	link_id= 11 point_id= 7 value= 1 quality= 0
	link_id= 11 point_id= 7 value= 0 quality= 0
	link_id= 11 point_id= 7 value= 1 quality= 0
	link_id= 11 point_id= 7 value= 0 quality= 0
	link_id= 11 point_id= 7 value= 1 quality= 0
	link_id= 11 point_id= 7 value= 0 quality= 0
	*/

	qDebug() << "---stage1---";
	datacount = datagroup.size();

	for (index=0; index < datacount; index++)
		cachetest.AddOneData(datagroup[index]);

	while (true)
	{
		ForwardRawDataType onddata;
		if (cachetest.ReadOneData(onddata) == 1)
		{
			qDebug() << "link_id=" << onddata.data_key.link_id << "point_id=" << onddata.data_key.point_id << "value=" << onddata.data_value.value << "quality=" << onddata.data_value.quality;
			cachetest.DelOneData();
		}
		else
			break;
	}

	qDebug() << "---stage2---";
	datacount = datagroup.size();

	for (index = 0; index < datacount; index++)
		cachetest.AddOneData(datagroup[index]);

	while (true)
	{
		ForwardRawDataType onddata;
		if (cachetest.ReadOneData(onddata) == 1)
		{
			qDebug() << "link_id=" << onddata.data_key.link_id << "point_id=" << onddata.data_key.point_id << "value=" << onddata.data_value.value << "quality=" << onddata.data_value.quality;
			cachetest.DelOneData();
		}
		else
			break;
	}

	qDebug() << "---stage3---";
	datacount = datachange.size();

	for (index = 0; index < datacount; index++)
		cachetest.AddOneData(datachange[index]);

	while (true)
	{
		ForwardRawDataType onddata;
		if (cachetest.ReadOneData(onddata) == 1)
		{
			qDebug() << "link_id=" << onddata.data_key.link_id << "point_id=" << onddata.data_key.point_id << "value=" << onddata.data_value.value << "quality=" << onddata.data_value.quality;
			cachetest.DelOneData();
		}
		else
			break;
	}

	qtapp.exec();

	return 0;
}

