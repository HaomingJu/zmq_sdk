#ifndef __HOBOTPROTOCOLWRITE_H__
#define __HOBOTPROTOCOLWRITE_H__
#include <stdint.h>
#include <vector>

// enum valueType {
// JPEG  = 0,
// MP4   = 1,
// PROTO = 2,
// JSON  = 3,
// };

namespace Modo {
class HobotProtocolWrite {
public:
  HobotProtocolWrite(int8_t *buff, int bufflen) : m_buff_(buff), 
  m_bufflen_(bufflen) { pos_ = 0; }
  void WriteHead(int32_t length, int32_t version, int64_t stamp, int64_t seq);
  void WriteTLV(int type, int32_t length, int8_t *value);
  int GetPackageLength();
  int8_t *GetPackageBuff();

private:
  int8_t *m_buff_;
  int m_bufflen_;
  int pos_;
  int packageLength_;
};
}
#endif
