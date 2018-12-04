#include <iostream>
#include "utils/CommunicationProtocol.h"

int main(int argc, char **argv) {
  int8_t *dst_ptr = new int8_t[44];
  int8_t *proto_bin_ptr = new int8_t[28];
  int8_t *tmp = new int8_t[100]{100};

  WriteBin(dst_ptr, 2018, 28, proto_bin_ptr, 0, tmp);

  uint32_t version;
  uint32_t head;
  uint32_t proto_len;
  uint32_t jpeg_len;
  int8_t *proto_bin_ptr_2 = new int8_t[28];

  ReadBin(dst_ptr, version, head, proto_len, proto_bin_ptr_2, jpeg_len,
          nullptr);


  std::cout << "version: " << version << std::endl
            << "head:" << head << std::endl
            << "proto_len: " << proto_len << std::endl
            << "jpeg_len: " << jpeg_len << std::endl;
  return 0;
}
