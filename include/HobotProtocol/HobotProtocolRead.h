#ifndef __HOBOTPROTOCOLREAD_H__
#define __HOBOTPROTOCOLREAD_H__

#include <stdint.h>
#include <vector>

#include "HobotProtocol/HobotProtocolWrite.h"

#define LENGTH_PTR_INDEX  0
#define VERSION_PTR_INDEX 4
#define STAMP_PTR_INDEX   8
#define SEQ_PTR_INDEX    16

namespace Modo {
class HobotProtocolRead {
public:
  HobotProtocolRead(int8_t *buff, int bufflen);
  bool GetIsEdianDiff() { return IsEdianDiff_; }
  void ReadHead(int32_t &length, int32_t &version, int64_t &stamp, int64_t &seq);
  // void ReadTLV(int &type, int32_t &length, int8_t *value);
  void ReadTLV(int &type, int32_t &length, int8_t **value); 
  void Swap16(int16_t &value);
  void Swap32(int32_t &value);
  void Swap64(int64_t &value);

private:
  int8_t *m_buff_;
  int m_bufflen_;
  bool IsEdianDiff_;
  int pos_;
};
}
#endif