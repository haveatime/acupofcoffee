
#include <QDebug>
#include "dliec104frame.h"

DlIec104Frame::DlIec104Frame(const QVariantHash &parameter)
:DataLinkframeInterface(parameter), currParseStep(PARSE_FRAME_HEAD)
{
}

DlIec104Frame::~DlIec104Frame()
{
}

void DlIec104Frame::restore()
{
	currParseStep = PARSE_FRAME_HEAD;
	frameParsing.clear();
	parsedFrames.clear();
}

void DlIec104Frame::feedBytes(const QList<QByteArray> &data)
{
	int everypos = 0;
	foreach(const QByteArray& everydata, data)
	{
		int everylen = everydata.size();

		do
		{
			if (currParseStep == PARSE_FRAME_HEAD)
			{
				if (everylen <= everypos)
				{
					everypos = 0;
					continue;
				}

				everypos = everydata.indexOf((char)0x68, everypos);
				if (-1 == everypos)
				{
					everypos = 0;
					continue;
				}
				else
				{
					currParseStep = PARSE_FRMAE_LENGTH;
					frameParsing.append((char)0x68);
					everypos ++;
				}
			}

			if (currParseStep == PARSE_FRMAE_LENGTH)
			{
				if (everylen <= everypos)
				{
					everypos = 0;
					continue;
				}

				frameParsing.append(everydata[everypos]);
				currParseStep = PARSE_FRAME_BODY;
				holdFrameBytes = 0;
				everypos++;
			}

			if (currParseStep == PARSE_FRAME_BODY)
			{
				if (everylen <= everypos)
				{
					everypos = 0;
					continue;
				}

				int everyLeaveBytes = everylen - everypos;
				int wholeFrameBytes = (unsigned char)frameParsing[1];
				int needCopyBytes = wholeFrameBytes - holdFrameBytes;
				if (needCopyBytes > everyLeaveBytes)
					needCopyBytes = everyLeaveBytes;

				frameParsing.append(everydata.mid(everypos, needCopyBytes));
				holdFrameBytes += needCopyBytes;
				everypos += needCopyBytes;

				if (holdFrameBytes >= wholeFrameBytes)
				{
					currParseStep = PARSE_FRMAE_COMPLETE;
				}
				else
				{
					everypos = 0;
					continue;
				}
			}

			if (currParseStep >= PARSE_FRMAE_COMPLETE)
			{
				parsedFrames.append(frameParsing);
				frameParsing.clear();
				currParseStep = PARSE_FRAME_HEAD;
			}
		} while (everypos>0);
	}

	if (parsedFrames.size() > 0)
	{
		emit framesReady(parsedFrames);
		parsedFrames.clear();
	}
}
