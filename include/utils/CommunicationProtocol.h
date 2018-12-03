#ifndef __UTILS_COMMUNICATION_PROTOCOL_H__
#define __UTILS_COMMUNICATION_PROTOCOL_H__
#include <stdint.h>
#include <string.h>

#define VERSION_SIZE_BYTE 4
#define HEAD_SIZE_BYTE 4
#define JPEG_SIZE_BYTE 4
#define PROTO_SIZE_BYTE 4

#define VERSION_PTR_INDEX 0
#define HEAD_PTR_INDEX 4
#define PROTO_SIZE_PTR_INDEX 8
#define PROTO_BIN_PTR_INDEX 12

int WriteBin(int8_t *dst_ptr, uint32_t version, uint32_t proto_len,
             int8_t *proto_bin_ptr, uint32_t jpeg_len, int8_t *jpeg_bin_ptr);

int ReadBin(int8_t *src_ptr, uint32_t &version, uint32_t &head,
            uint32_t &proto_len, int8_t *proto_bin_ptr, uint32_t &jpeg_len,
            uint8_t *jpeg_bin_ptr);

int Serialize(uint32_t value, int8_t *result);

int Deserialize(int8_t *result, uint32_t &value);


#endif  // __UTILS_COMMUNICATION_PROTOCOL_H__
