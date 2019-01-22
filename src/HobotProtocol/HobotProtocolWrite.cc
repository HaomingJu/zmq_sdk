#include "HobotProtocol/HobotProtocolWrite.h"
#include <memory.h>

namespace Modo{
void HobotProtocolWrite::WriteHead(int32_t length, int32_t version, 
int64_t stamp, int64_t seq) {
    memcpy(m_buff_, &length, 4);
    memcpy(m_buff_ + 4, &version, 4);
    memcpy(m_buff_ + 8, &stamp, 8);
    memcpy(m_buff_ + 16, &seq, 8);
    pos_ += 24;
}   

void HobotProtocolWrite::WriteTLV(int type, int32_t length, 
int8_t *value) {
    memcpy(m_buff_ + pos_, &type, 4);
    memcpy(m_buff_ + pos_ + 4, &length, 4);
    memcpy(m_buff_ + pos_ + 8, &value, length);
    pos_  = pos_ + 8 + length;
}

int HobotProtocolWrite::GetPackageLength() {
    return pos_;
}

int8_t *HobotProtocolWrite::GetPackageBuff() {
    return m_buff_;
}
}