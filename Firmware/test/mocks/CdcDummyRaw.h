#pragma once

#include "Stream.h"
#include <queue>
#include <vector>

class CdcDummyRaw : public Stream {
  public:
    uint8_t RawBuffer[32];
    uint8_t RawBufferLength;

    std::vector<uint8_t> OutBuffer;
    std::queue<std::vector<uint8_t>> OutQueue;
    uint8_t *ReadOutputBuffer(void) {
        if (OutQueue.size() != 0) {
            auto msg = OutQueue.front();
            OutQueue.pop();
            RawBufferLength = msg.size();

            for (size_t i = 0; i < RawBufferLength; i++) {
                RawBuffer[i] = msg[i];
            }

            return RawBuffer;
        }

        auto msg(OutBuffer);
        OutBuffer.clear();
        RawBufferLength = msg.size();
        for (size_t i = 0; i < RawBufferLength; i++) {
            RawBuffer[i] = msg[i];
        }

        return RawBuffer;
    }

    void begin(void) {}
    virtual int available(void) override { return 0; }
    virtual int peek(void) override { return 0; }
    virtual int read(void) override { return 0; }
    virtual size_t readBytes(char *buffer, size_t length) override { return length; }
    virtual size_t readBytesUntil(char terminator, char *buffer, size_t length) override {
        return length;
    }
    virtual void flush(void) override{};

    unsigned _length = 0;
    const unsigned HeaderLen = 3;

    virtual size_t write(uint8_t c) override {
        OutBuffer.push_back(c);
        if (OutBuffer.size() == HeaderLen)
            _length = c;

        if (OutBuffer.size() == (HeaderLen + _length)) {
            OutQueue.push(OutBuffer);
            OutBuffer.clear();
        }
        return 1;
    }

    virtual size_t write(const uint8_t *buffer, size_t size) override {
        for (size_t i = 0; i < size; i++) {
            write(buffer[i]);
        }

        return size;
    }
};