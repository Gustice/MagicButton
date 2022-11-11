#pragma once

class TimeOut {
  public:
    TimeOut(unsigned max) : _max(max) {}
    void Reset(void) { _cnt = _max; }
    void Tick(void) {
        if (_cnt > 0)
            --_cnt;
    }

    bool IsTimeOut() {
        return _cnt == 0;
    }

  private:
    const unsigned _max;
    unsigned _cnt;
};

