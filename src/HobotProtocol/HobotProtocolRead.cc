#include "HobotProtocol/HobotProtocolRead.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>

HobotProtocolRead::HobotProtocolRead(int8_t *buff, int bufflen) {
    m_buff = buff;
    m_bufflen = bufflen;
    IsEdian = false;
    // check  Edian
    int32_t version = 0;
    memcpy(&version, buff + 4, 4);
    if((uint32_t)version > 0x0000FFFFu) {
        IsEdian = true;
    }
    pos = 0;
}

void HobotProtocolRead::Swap16(int16_t &value) {
    value = ((value & 0x00FF) << 8) |
            ((value & 0xFF00) >> 8);
}

void HobotProtocolRead::Swap32(int32_t &value) {
     value = ((value & 0x000000FF) << 24) |  
             ((value & 0x0000FF00) << 8)  |  
             ((value & 0x00FF0000) >> 8)  |  
             ((value & 0xFF000000) >> 24);  
}

void HobotProtocolRead::Swap64(int64_t &value) {
     value = ((value & 0x00000000000000FF) << 56) |  
             ((value & 0x000000000000FF00) << 40) |  
             ((value & 0x0000000000FF0000) << 24) |  
             ((value & 0x00000000FF000000) << 8)  |
             ((value & 0x000000FF00000000) >> 8)  |
             ((value & 0x0000FF0000000000) >> 24) |
             ((value & 0x00FF000000000000) >> 40) |
             ((value & 0xFF00000000000000) >> 56);   
}

void HobotProtocolRead::ReadHead(int32_t &length, int32_t &version, 
int64_t &stamp, int64_t &seq) {
    memcpy(&length, m_buff, 4);
    memcpy(&version, m_buff + 4, 4);
    memcpy(&stamp, m_buff + 8, 8);
    memcpy(&seq, m_buff + 16, 8);
    pos += 24;
    if(IsEdian) {
        Swap32(length);
        Swap32(version);
        Swap64(stamp);
        Swap64(seq);
    }
}

void HobotProtocolRead::ReadTLV(valueType &type, int32_t &length, 
int8_t *value) {
    memcpy(&type, m_buff + pos, 4);
    memcpy(&length, m_buff + pos + 4, 4);
    memcpy(value, m_buff + pos + 8, length);
    pos = pos + 8 + length;
    if(IsEdian) {
        int32_t tp = type;
        Swap32(tp);
        type = valueType(tp);
        Swap32(length);
    }
}