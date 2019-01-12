#include "HobotProtocol/HobotProtocolWrite.h"
#include <memory.h>

void HobotProtocolWrite::WriteHead(int32_t length, int32_t version, 
int64_t stamp, int64_t seq) {
    memcpy(m_buff, &length, 4);
    memcpy(m_buff + 4, &version, 4);
    memcpy(m_buff + 8, &stamp, 8);
    memcpy(m_buff + 16, &seq, 8);
    pos += 24;
}   

void HobotProtocolWrite::WriteTLV(valueType type, int32_t length, 
int8_t *value) {
    memcpy(m_buff + pos, &type, 4);
    memcpy(m_buff + pos + 4, &length, 4);
    memcpy(m_buff + pos + 8, &value, length);
    pos  = pos + 8 + length;
}

int HobotProtocolWrite::GetPackageLength() {
    return pos;
}

int8_t *HobotProtocolWrite::GetPackageBuff() {
    return m_buff;
}