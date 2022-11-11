#include "RawForm.h"

namespace RawShell {
    
const CmdFlags_t EmptyMsgFlag {
    .doneBit = 0,
    .spare = 0,
    .eventBit = 0,
    .errorBit = 0,
};

const CmdFlags_t DoneMsgFlag {
    .doneBit = 1,
    .spare = 0,
    .eventBit = 0,
    .errorBit = 0,
};

const CmdFlags_t ErrorEventMsgFlag {
    .doneBit = 0,
    .spare = 0,
    .eventBit = 1,
    .errorBit = 1,
};
const CmdFlags_t EventMsgFlag {
    .doneBit = 0,
    .spare = 0,
    .eventBit = 1,
    .errorBit = 0,
};
const CmdFlags_t ErrorMsgFlag {
    .doneBit = 0,
    .spare = 0,
    .eventBit = 1,
    .errorBit = 0,
};

}