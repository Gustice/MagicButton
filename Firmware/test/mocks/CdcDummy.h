#pragma once

// #include "Stream.h"

// class CdcDummy : public Stream {
//   public:
//     void begin(void);
    
//     virtual int available(void) { return 0;}
//     virtual int availableForWrite(void) { return 0;}
//     virtual int peek(void) { return 0;}
//     virtual int read(void) { return 0;}
//     virtual size_t readBytes(char *buffer, size_t length) {return length;} 
//     virtual size_t readBytesUntil(char terminator, char *buffer, size_t length) {return length;}
//     virtual void flush(void) {};
//     virtual size_t write(uint8_t) {return 1;}
//     virtual size_t write(const uint8_t *buffer, size_t size) { return size;}
// };