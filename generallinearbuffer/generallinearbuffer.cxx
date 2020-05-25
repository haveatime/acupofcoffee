#include <QDebug>
#include <QCoreApplication>

#define MAX_LINEAR_BUFFER_SIZE  16384

class LinearBufferWrap
{
public:
	LinearBufferWrap(int) : len(0), first(0), buf(0), capacity(0) {
	}
	~LinearBufferWrap() {
		delete[] buf;
	}
	void clear() {
		len = 0;
		delete[] buf;
		buf = 0;
		first = buf;
		capacity = 0;
	}
	qint64 size() const {
		return len;
	}
	bool isEmpty() const {
		return len == 0;
	}
	void skip(qint64 n) {
		if (n >= len) {
			clear();
		}
		else {
			len -= n;
			first += n;
		}
	}
	int getChar() {
		if (len == 0)
			return -1;
		int ch = uchar(*first);
		len--;
		first++;
		return ch;
	}
	qint64 read(char* target, qint64 size) {
		qint64 r = qMin(size, len);
		if (r) {
			memcpy(target, first, r);
			len -= r;
			first += r;
		}
		return r;
	}
	qint64 peek(char* target, qint64 size) {
		qint64 r = qMin(size, len);
		if (r)
			memcpy(target, first, r);
		return r;
	}
	char* reserve(qint64 size) {
		makeSpace(size + len, freeSpaceAtEnd);
		char* writePtr = first + len;
		len += size;
		return writePtr;
	}
	void chop(qint64 size) {
		if (size >= len) {
			clear();
		}
		else {
			len -= size;
		}
	}
	QByteArray readAll() {
		QByteArray retVal(first, len);
		clear();
		return retVal;
	}
	qint64 readLine(char* target, qint64 size) {
		qint64 r = qMin(size, len);
		char* eol = static_cast<char*>(memchr(first, '\n', r));
		if (eol)
			r = 1 + (eol - first);
		memcpy(target, first, r);
		len -= r;
		first += r;
		return r;
	}
	bool canReadLine() const {
		return first && memchr(first, '\n', len);
	}
	void ungetChar(char c) {
		if (first == buf) {
			// underflow, the existing valid data needs to move to the end of the (potentially bigger) buffer
			makeSpace(len + 1, freeSpaceAtStart);
		}
		first--;
		len++;
		*first = c;
	}
	void ungetBlock(const char* block, qint64 size) {
		if ((first - buf) < size) {
			// underflow, the existing valid data needs to move to the end of the (potentially bigger) buffer
			makeSpace(len + size, freeSpaceAtStart);
		}
		first -= size;
		len += size;
		memcpy(first, block, size);
	}

private:
	enum FreeSpacePos { freeSpaceAtStart, freeSpaceAtEnd };
	void makeSpace(size_t required, FreeSpacePos where) {
		size_t newCapacity = qMax(capacity, size_t(MAX_LINEAR_BUFFER_SIZE));
		while (newCapacity < required)
			newCapacity *= 2;
		const size_t moveOffset = (where == freeSpaceAtEnd) ? 0 : newCapacity - size_t(len);
		if (newCapacity > capacity) {
			// allocate more space
			char* newBuf = new char[newCapacity];
			if (first)
				memmove(newBuf + moveOffset, first, len);
			delete[] buf;
			buf = newBuf;
			capacity = newCapacity;
		}
		else {
			// shift any existing data to make space
			memmove(buf + moveOffset, first, len);
		}
		first = buf + moveOffset;
	}

private:
	// length of the unread data
	qint64 len;
	// start of the unread data
	char* first;
	// the allocated buffer
	char* buf;
	// allocated buffer size
	size_t capacity;
};

int main(int argc, char **argv)
{
	QCoreApplication qtapp(argc, argv);

	LinearBufferWrap onebuffer(100);

	char *writep = onebuffer.reserve(1);
	*writep = 'a';

	onebuffer.ungetChar('b');
	onebuffer.ungetBlock("1234",4);

	int onechar = onebuffer.getChar();

	qtapp.exec();

	return 0;
}

