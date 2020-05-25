#ifndef DL__IEC104__FRAME__H
#define DL__IEC104__FRAME__H

#include "datalinkframeinterface.h"

#ifdef DL_IEC104_FRAME_EXPORT
#define DL_IEC104_FRAME_DECL Q_DECL_EXPORT 
#else
#define DL_IEC104_FRAME_DECL Q_DECL_IMPORT 
#endif

class DL_IEC104_FRAME_DECL DlIec104Frame : public DataLinkframeInterface
{
	Q_OBJECT

private:
	typedef enum PARSE_STEP_ENUM
	{
		PARSE_FRAME_HEAD,
		PARSE_FRMAE_LENGTH,
		PARSE_FRAME_BODY,
		PARSE_FRMAE_COMPLETE
	}ParseFrameStep;

private:
	ParseFrameStep currParseStep;
	int holdFrameBytes;
	int frameLength;
	QByteArray frameParsing;
	QList<QByteArray> parsedFrames;

public:
	DlIec104Frame(const QVariantHash &parameter);
	virtual ~DlIec104Frame();

	virtual void restore();

public slots:
	virtual void feedBytes(const QList<QByteArray> &data);
};

#endif
