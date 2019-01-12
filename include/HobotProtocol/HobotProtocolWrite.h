#ifndef __HOBOTPROTOCOLWRITE_H__
#define __HOBOTPROTOCOLWRITE_H__
#include <stdint.h>
#include <vector>

enum valueType {
JPEG  = 0,
MP4   = 1,
PROTO = 2,
JSON  = 3,
};

class HobotProtocolWrite {
public:
  HobotProtocolWrite(int8_t *buff, int bufflen) : m_buff(buff), 
  m_bufflen(bufflen) { pos = 0; }
  void WriteHead(int32_t length, int32_t version, int64_t stamp, int64_t seq);
  void WriteTLV(valueType type, int32_t length, int8_t *value);
  int GetPackageLength();
  int8_t *GetPackageBuff();

private:
  int8_t *m_buff;
  int m_bufflen;
  int pos;
  int packageLength;
};

#endif
