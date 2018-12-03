#include "utils/CommunicationProtocol.h"


int WriteBin(int8_t *dst_ptr, uint32_t version, uint32_t proto_len,
             int8_t *proto_bin_ptr, uint32_t jpeg_len, int8_t *jpeg_bin_ptr) {
  if (!dst_ptr)
    return -1;
  // 写入version，四字节
  Serialize(version, dst_ptr + VERSION_PTR_INDEX);
  // 写入head，四字节
  Serialize(VERSION_SIZE_BYTE + HEAD_SIZE_BYTE + PROTO_SIZE_BYTE,
            dst_ptr + HEAD_PTR_INDEX);
  // 写入proto_len，四字节
  Serialize(proto_len, dst_ptr + PROTO_SIZE_PTR_INDEX);
  // 写入proto_bin，多字节
  if (proto_bin_ptr)
    memcpy(dst_ptr + PROTO_BIN_PTR_INDEX, proto_bin_ptr, proto_len);
  // 写入jpeg_len，四字节
  Serialize(jpeg_len, dst_ptr + VERSION_SIZE_BYTE + HEAD_SIZE_BYTE +
                          PROTO_SIZE_BYTE + proto_len);
  // 写入jpeg_bin，多字节
  if (jpeg_bin_ptr)
    memcpy(dst_ptr + PROTO_SIZE_PTR_INDEX + proto_len + JPEG_SIZE_BYTE,
           jpeg_bin_ptr, jpeg_len);
  return 0;
}

int ReadBin(int8_t *src_ptr, uint32_t &version, uint32_t &head,
            uint32_t &proto_len, int8_t *proto_bin_ptr, uint32_t &jpeg_len,
            uint8_t *jpeg_bin_ptr) {
  if (!src_ptr)
    return -1;
  Deserialize(src_ptr, version);
  Deserialize(src_ptr + HEAD_PTR_INDEX, head);
  Deserialize(src_ptr + PROTO_SIZE_PTR_INDEX, proto_len);
  if (proto_bin_ptr)
    memcpy(proto_bin_ptr, src_ptr + PROTO_BIN_PTR_INDEX, proto_len);
  Deserialize(src_ptr + PROTO_SIZE_PTR_INDEX + proto_len, jpeg_len);
  if (jpeg_bin_ptr)
    memcpy(jpeg_bin_ptr,
           src_ptr + PROTO_BIN_PTR_INDEX + proto_len + JPEG_SIZE_BYTE,
           jpeg_len);
  return 0;
}


int Serialize(uint32_t value, int8_t *result) {
  if (!value)
    return -1;
  // assert(result.size == 4)
  memset(result, 0x00, 4);
  result[0] = value >> 24 & 0x00FF;
  result[1] = value >> 16 & 0x00FF;
  result[2] = value >> 8 & 0x00FF;
  result[3] = value >> 0 & 0x00FF;
  return 0;
}

int Deserialize(int8_t *value, uint32_t &result) {
  if (!value)
    return -1;
  result = (value[0] << 24) & 0xFF000000;
  result |= (value[1] << 16) & 0x00FF0000;
  result |= (value[2] << 8) & 0x0000FF00;
  result |= (value[3]) & 0x000000FF;
  return 0;
}
