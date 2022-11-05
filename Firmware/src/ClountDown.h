#pragma once

class CntDown {
  public:
    CntDown(unsigned max) : _max(max) {}

    void Tick() {
        _cnt++;
        if (_cnt >= _max) {
            _cnt = 0;
            _pFlag = true;
        }
    }

    bool GetVolatileFlag() {
        if (!_pFlag)
            return false;

        _pFlag = false;
        return true;
    }

  private:
    const unsigned _max;
    unsigned _cnt;
    bool _pFlag;
};
