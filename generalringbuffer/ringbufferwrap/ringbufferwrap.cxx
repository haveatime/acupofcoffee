#include "ringbufferwrap.h"

#define RingBufferMaxByteArraySize 0x3FFFFFFF

/*!
    \internal

    Access the bytes at a specified position the out-variable length will
    contain the amount of bytes readable from there, e.g. the amount still
    the same QByteArray
*/
const char *RingBufferWrap::readPointerAtPosition(qint64 pos, qint64 &length) const
{
    if (pos >= 0) {
        pos += head;
        for (int i = 0; i < buffers.size(); ++i) {
            length = (i == tailBuffer ? tail : buffers[i].size());
            if (length > pos) {
                length -= pos;
                return buffers[i].constData() + pos;
            }
            pos -= length;
        }
    }

    length = 0;
    return 0;
}

void RingBufferWrap::free(qint64 bytes)
{
    while (bytes > 0) {
        const qint64 blockSize = buffers.first().size() - head;

        if (tailBuffer == 0 || blockSize > bytes) {
            // keep a single block around if it does not exceed
            // the basic block size, to avoid repeated allocations
            // between uses of the buffer
            if (bufferSize <= bytes) {
                if (buffers.first().size() <= basicBlockSize) {
                    bufferSize = 0;
                    head = tail = 0;
                } else {
                    clear(); // try to minify/squeeze us
                }
            } else {
                Q_ASSERT(bytes < RingBufferMaxByteArraySize);
                head += int(bytes);
                bufferSize -= bytes;
            }
            return;
        }

        bufferSize -= blockSize;
        bytes -= blockSize;
        buffers.removeFirst();
        --tailBuffer;
        head = 0;
    }
}

char *RingBufferWrap::reserve(qint64 bytes)
{
    if (bytes <= 0 || bytes >= RingBufferMaxByteArraySize)
        return 0;

    const qint64 newSize = bytes + tail;
    // if need buffer reallocation
    if (newSize > buffers.last().size()) {
        if (newSize > buffers.last().capacity() && (tail >= basicBlockSize
                || newSize >= RingBufferMaxByteArraySize)) {
            // shrink this buffer to its current size
            buffers.last().resize(tail);

            // create a new QByteArray
            buffers.append(QByteArray());
            ++tailBuffer;
            tail = 0;
        }
        buffers.last().resize(qMax(basicBlockSize, tail + int(bytes)));
    }

    char *writePtr = buffers.last().data() + tail;
    bufferSize += bytes;
    Q_ASSERT(bytes < RingBufferMaxByteArraySize);
    tail += int(bytes);
    return writePtr;
}

/*!
    \internal

    Allocate data at buffer head
*/
char *RingBufferWrap::reserveFront(qint64 bytes)
{
    if (bytes <= 0 || bytes >= RingBufferMaxByteArraySize)
        return 0;

    if (head < bytes) {
        buffers.first().remove(0, head);
        if (tailBuffer == 0)
            tail -= head;

        head = qMax(basicBlockSize, int(bytes));
        if (bufferSize == 0) {
            tail = head;
        } else {
            buffers.prepend(QByteArray());
            ++tailBuffer;
        }
        buffers.first().resize(head);
    }

    head -= int(bytes);
    bufferSize += bytes;
    return buffers.first().data() + head;
}

void RingBufferWrap::chop(qint64 bytes)
{
    while (bytes > 0) {
        if (tailBuffer == 0 || tail > bytes) {
            // keep a single block around if it does not exceed
            // the basic block size, to avoid repeated allocations
            // between uses of the buffer
            if (bufferSize <= bytes) {
                if (buffers.first().size() <= basicBlockSize) {
                    bufferSize = 0;
                    head = tail = 0;
                } else {
                    clear(); // try to minify/squeeze us
                }
            } else {
                Q_ASSERT(bytes < RingBufferMaxByteArraySize);
                tail -= int(bytes);
                bufferSize -= bytes;
            }
            return;
        }

        bufferSize -= tail;
        bytes -= tail;
        buffers.removeLast();
        --tailBuffer;
        tail = buffers.last().size();
    }
}

void RingBufferWrap::clear()
{
    buffers.erase(buffers.begin() + 1, buffers.end());
    buffers.first().clear();

    head = tail = 0;
    tailBuffer = 0;
    bufferSize = 0;
}

qint64 RingBufferWrap::indexOf(char c, qint64 maxLength) const
{
    qint64 index = 0;
    qint64 j = head;
    for (int i = 0; index < maxLength && i < buffers.size(); ++i) {
        const char *ptr = buffers[i].constData() + j;
        j = qMin(index + (i == tailBuffer ? tail : buffers[i].size()) - j, maxLength);

        while (index < j) {
            if (*ptr++ == c)
                return index;
            ++index;
        }
        j = 0;
    }
    return -1;
}

qint64 RingBufferWrap::read(char *data, qint64 maxLength)
{
    const qint64 bytesToRead = qMin(size(), maxLength);
    qint64 readSoFar = 0;
    while (readSoFar < bytesToRead) {
        const qint64 bytesToReadFromThisBlock = qMin(bytesToRead - readSoFar,
                                                     nextDataBlockSize());
        if (data)
            memcpy(data + readSoFar, readPointer(), bytesToReadFromThisBlock);
        readSoFar += bytesToReadFromThisBlock;
        free(bytesToReadFromThisBlock);
    }
    return readSoFar;
}

/*!
    \internal

    Read an unspecified amount (will read the first buffer)
*/
QByteArray RingBufferWrap::read()
{
    if (bufferSize == 0)
        return QByteArray();

    QByteArray qba(buffers.takeFirst());

    qba.reserve(0); // avoid that resizing needlessly reallocates
    if (tailBuffer == 0) {
        qba.resize(tail);
        tail = 0;
        buffers.append(QByteArray());
    } else {
        --tailBuffer;
    }
    qba.remove(0, head); // does nothing if head is 0
    head = 0;
    bufferSize -= qba.size();
    return qba;
}

/*!
    \internal

    Peek the bytes from a specified position
*/
qint64 RingBufferWrap::peek(char *data, qint64 maxLength, qint64 pos) const
{
    qint64 readSoFar = 0;

    if (pos >= 0) {
        pos += head;
        for (int i = 0; readSoFar < maxLength && i < buffers.size(); ++i) {
            qint64 blockLength = (i == tailBuffer ? tail : buffers[i].size());

            if (pos < blockLength) {
                blockLength = qMin(blockLength - pos, maxLength - readSoFar);
                memcpy(data + readSoFar, buffers[i].constData() + pos, blockLength);
                readSoFar += blockLength;
                pos = 0;
            } else {
                pos -= blockLength;
            }
        }
    }

    return readSoFar;
}

/*!
    \internal

    Append a new buffer to the end
*/
void RingBufferWrap::append(const QByteArray &qba)
{
    if (tail == 0) {
        buffers.last() = qba;
    } else {
        buffers.last().resize(tail);
        buffers.append(qba);
        ++tailBuffer;
    }
    tail = qba.size();
    bufferSize += tail;
}

qint64 RingBufferWrap::readLine(char *data, qint64 maxLength)
{
    if (!data || --maxLength <= 0)
        return -1;

    qint64 i = indexOf('\n', maxLength);
    i = read(data, i >= 0 ? (i + 1) : maxLength);

    // Terminate it.
    data[i] = '\0';
    return i;
}
