#include <QDebug>
#include <QByteArray>
#include <QCoreApplication>

#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#include <QJsonObject>
#else
#include "QJsonDocument.h"
#include "QJsonObject.h"
#endif

/**********************************************************************************************
只能用QJsonObject的fromVariantHash,fromVariantMap和toVariantHash,toVariantMap互相转换.
因为map和hash,有key和value,这样的必须是一个对象,而且转换成json字符串,最外层100%是一对花括号表示.
不能用QJsonDocument的fromQVariant和toQVariant,因为转换后的QJsonDocument不一定是一个QJsonObject.
QJsonDocument要么是一个QJsonArray要么是一个QJsonObject,所以有效的JSON document是以[]或{}为界的.
**********************************************************************************************/

int main( int argc, char ** argv)
{
	QCoreApplication coreApplication(argc, argv);

	QJsonValue jsonvaluea;
	QJsonValue jsonvalueb(true);
	QJsonValue jsonvaluec(100);
	QJsonValue jsonvalued("zifuchuan");
	qDebug()<<"jsonvaluea.toVariant="<<jsonvaluea.toVariant();
	qDebug()<<"jsonvalueb.toVariant="<<jsonvalueb.toVariant();
	qDebug()<<"jsonvaluec.toVariant="<<jsonvaluec.toVariant();
	qDebug()<<"jsonvalued.toVariant="<<jsonvalued.toVariant();

	qDebug()<<endl;

	QByteArray jsonstringa ( 
	"{\
      \"Array\": [\
          true,\
          999,\
          \"string\"\
      ],\
      \"Key\": \"Value\",\
      \"null\": null\
  }"
	);
	qDebug()<<"jsonstringa="<<jsonstringa;

	QJsonDocument jsondocumenta = QJsonDocument::fromJson(jsonstringa);
	if(!jsondocumenta.isNull())
	{
		qDebug()<<"jsondocumenta.toJson="<<jsondocumenta.toJson(QJsonDocument::Compact);
		qDebug()<<"jsondocumenta.toVariant="<<jsondocumenta.toVariant();
	}

	if( jsondocumenta.isObject() )
	{
		QJsonObject jsonobjecta = jsondocumenta.object();
		qDebug()<<"jsonobjecta.toVariantHash="<<jsonobjecta.toVariantHash();
	}
	qDebug()<<endl;

	QVariantHash jsonhash;
	jsonhash["key"] = "value";
	QVariantList injsonlist;
	injsonlist.append(true);
	injsonlist.append(999);
	injsonlist.append("string");
	jsonhash["array"] = (QVariant)injsonlist;
	jsonhash["null"] = QVariant();
	qDebug()<<"jsonhash="<<jsonhash;

	QJsonObject jsonobjectb = QJsonObject::fromVariantHash(jsonhash);
	qDebug()<<"jsonobjectb.toVariantHash="<<jsonobjectb.toVariantHash();

	QJsonDocument jsondocumentb(jsonobjectb);
	qDebug()<<"jsondocumentb.toJson="<<jsondocumentb.toJson(QJsonDocument::Compact);
	qDebug()<<"jsondocumentb.toVariant="<<jsondocumentb.toVariant();

	qDebug()<<endl;

	QByteArray jsonstringc ( "[true,2,\"abc\"]" 	);
	qDebug()<<"jsonstringc="<<jsonstringc;

	QJsonDocument jsondocumentc = QJsonDocument::fromJson(jsonstringc);
	if(!jsondocumentc.isNull())
	{
		qDebug()<<"jsondocumentc.toJson="<<jsondocumentc.toJson(QJsonDocument::Compact);
		qDebug()<<"jsondocumentc.toVariant="<<jsondocumentc.toVariant();
	}

	qDebug()<<endl;

	QVariantHash hashfrom;
	hashfrom["one"] = 1;
	hashfrom["two"] = 2;
	hashfrom["three"] = 3;
	hashfrom["good"] = true;
	hashfrom["bad"] = false;
	hashfrom["name"] = "sunyang";
	qDebug()<<"hashfrom="<<hashfrom;
	QJsonObject exchangejson = QJsonObject::fromVariantHash(hashfrom);
	QVariantMap tomap = exchangejson.toVariantMap();
	qDebug()<<"tomap="<<tomap;

	return coreApplication.exec();
}

