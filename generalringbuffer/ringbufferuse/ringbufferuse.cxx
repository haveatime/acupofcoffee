#include <QCoreApplication>
#include <QDebug>

#include "ringbufferwrap.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	RingBufferWrap onebuffer;

	onebuffer.ungetChar('a');
	onebuffer.ungetChar('b');
	onebuffer.putChar('c');
	onebuffer.putChar('d');
	onebuffer.append("1234");
	onebuffer.putChar('e');

	int onechar = onebuffer.getChar();

	return app.exec();
}
