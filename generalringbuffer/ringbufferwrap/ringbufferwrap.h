#ifndef RING__BUFFER__WRAP__H
#define RING__BUFFER__WRAP__H

#include <QByteArray>
#include <QList>

#ifdef RINGBUFFER_WRAP_EXPORT
#define RINGBUFFER_WRAP_DECL Q_DECL_EXPORT
#else
#define RINGBUFFER_WRAP_DECL Q_DECL_IMPORT
#endif

class RINGBUFFER_WRAP_DECL RingBufferWrap
{
public:
    explicit inline RingBufferWrap(int growth = 4096) :
        head(0), tail(0), tailBuffer(0), basicBlockSize(growth), bufferSize(0) {
        buffers.append(QByteArray());
    }

    inline qint64 nextDataBlockSize() const {
        return (tailBuffer == 0 ? tail : buffers.first().size()) - head;
    }

    inline const char *readPointer() const {
        return bufferSize == 0 ? NULL : (buffers.first().constData() + head);
    }

    const char *readPointerAtPosition(qint64 pos, qint64 &length) const;
    void free(qint64 bytes);
    char *reserve(qint64 bytes);
    char *reserveFront(qint64 bytes);

    inline void truncate(qint64 pos) {
        if (pos < size())
            chop(size() - pos);
    }

    void chop(qint64 bytes);

    inline bool isEmpty() const {
        return bufferSize == 0;
    }

    inline int getChar() {
        if (isEmpty())
            return -1;
        char c = *readPointer();
        free(1);
        return int(uchar(c));
    }

    inline void putChar(char c) {
        char *ptr = reserve(1);
        *ptr = c;
    }

    void ungetChar(char c)
    {
        if (head > 0) {
            --head;
            buffers.first()[head] = c;
            ++bufferSize;
        } else {
            char *ptr = reserveFront(1);
            *ptr = c;
        }
    }


    inline qint64 size() const {
        return bufferSize;
    }

    void clear();
    inline qint64 indexOf(char c) const { return indexOf(c, size()); }
    qint64 indexOf(char c, qint64 maxLength) const;
    qint64 read(char *data, qint64 maxLength);
    QByteArray read();
    qint64 peek(char *data, qint64 maxLength, qint64 pos = 0) const;
    void append(const QByteArray &qba);

    inline qint64 skip(qint64 length) {
        return read(0, length);
    }

    qint64 readLine(char *data, qint64 maxLength);

    inline bool canReadLine() const {
        return indexOf('\n') >= 0;
    }

private:
    QList<QByteArray> buffers;
    int head, tail;
    int tailBuffer; // always buffers.size() - 1
    const int basicBlockSize;
    qint64 bufferSize;
};

#endif
