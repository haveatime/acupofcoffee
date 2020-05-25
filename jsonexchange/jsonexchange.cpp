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
ֻ����QJsonObject��fromVariantHash,fromVariantMap��toVariantHash,toVariantMap����ת��.
��Ϊmap��hash,��key��value,�����ı�����һ������,����ת����json�ַ���,�����100%��һ�Ի����ű�ʾ.
������QJsonDocument��fromQVariant��toQVariant,��Ϊת�����QJsonDocument��һ����һ��QJsonObject.
QJsonDocumentҪô��һ��QJsonArrayҪô��һ��QJsonObject,������Ч��JSON document����[]��{}Ϊ���.
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

