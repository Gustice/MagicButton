#pragma once

#include "Stream.h"
#include <queue>
#include <vector>

class CdcDummy : public Stream {
  public:
    std::string OutBuffer;
    std::queue<std::string> OutQueue;
    std::string ReadOutputBuffer(void) {
        if (OutQueue.size() != 0) {
            auto msg = OutQueue.front();
            OutQueue.pop();
            return msg;
        }

        std::string msg(OutBuffer);
        OutBuffer = "";
        return msg;
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

    virtual size_t write(uint8_t c) override {
        auto s = (char)c;
        if (s == '\r')
            return 1;

        if (s == '\n') {
            OutQueue.push(OutBuffer);
            OutBuffer = "";
        } else {
            OutBuffer.push_back((char)c);
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
