#include "HobotProtocol/HobotProtocolRead.h"
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include "base/base.h"

namespace Modo {
HobotProtocolRead::HobotProtocolRead(int8_t *buff, int bufflen) {
  m_buff_ = buff;
  m_bufflen_ = bufflen;
  IsEdianDiff_ = false;
  /* check  EdianDiff */
  int32_t version = 0;
  memcpy(&version, buff + 4, 4);
  if ((uint32_t)version > 0x0000FFFFu) {
    IsEdianDiff_ = true;
    LOGD << "IsEdianDiff_= " << IsEdianDiff_;
  }
  ReadHead(length_, version_, stamp_, seq_);
  LOGD << "length_= " << length_;
  pos_ = 24;
}

void HobotProtocolRead::Swap16(int16_t &value) {
  value = ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

void HobotProtocolRead::Swap32(int32_t &value) {
  value = ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) |
          ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

void HobotProtocolRead::Swap64(int64_t &value) {
  value = ((value & 0x00000000000000FF) << 56) |
          ((value & 0x000000000000FF00) << 40) |
          ((value & 0x0000000000FF0000) << 24) |
          ((value & 0x00000000FF000000) << 8) |
          ((value & 0x000000FF00000000) >> 8) |
          ((value & 0x0000FF0000000000) >> 24) |
          ((value & 0x00FF000000000000) >> 40) |
          ((value & 0xFF00000000000000) >> 56);
}

int HobotProtocolRead::ReadHead(int32_t &length, int32_t &version,
                                int64_t &stamp, int64_t &seq) {
  memcpy(&length, m_buff_, 4);
  memcpy(&version, m_buff_ + 4, 4);
  memcpy(&stamp, m_buff_ + 8, 8);
  memcpy(&seq, m_buff_ + 16, 8);
  if (IsEdianDiff_) {
    Swap32(length);
    Swap32(version);
    Swap64(stamp);
    Swap64(seq);
  }
  return 0;
}

// void HobotProtocolRead::ReadTLV(int &type, int32_t &length,
// int8_t *value) {
//     memcpy(&type, m_buff_ + pos_, 4);
//     memcpy(&length, m_buff_ + pos_ + 4, 4);
//     memcpy(value, m_buff_ + pos_ + 8, length);
//     pos_ = pos_ + 8 + length;
//     if(IsEdianDiff_) {
//         Swap32(type);
//         Swap32(length);
//     }
// }

int HobotProtocolRead::ReadTLV(int &type, int32_t &length, int8_t **value) {
  if (pos_ >= length_) {
    LOGD << "ReadTLV over,length_=" << length_ << ",pos_" << pos_;
    return TRANSFER_OUT_MEMORY;
  }
  memcpy(&type, m_buff_ + pos_, 4);
  memcpy(&length, m_buff_ + pos_ + 4, 4);

  if (IsEdianDiff_) {
    Swap32(type);
    Swap32(length);
  }
  if (length > 0) {
    *value = m_buff_ + pos_ + 8;
  } else {
    *value = nullptr;
  }

  pos_ = pos_ + 8 + length;
  return 0;
}
}
