#include <stdio.h>
#include <string.h>
#include <iostream>
#include "HobotProtocol/HobotProtocolRead.h"
#include "HobotProtocol/HobotProtocolWrite.h"

using namespace Modo;
struct SyncFreatureRequest {
  int32_t camera_id;
  int32_t traking_id;
};
int main() {
  //    int mybuffleng = 30;
  //    int8_t *mybuff = new int8_t(mybuffleng);
  //    HobotProtocolWrite protoWrite(mybuff, mybuffleng);
  //    int32_t len = 0x12345678;
  //    int32_t ver = 0xFFFF0000;
  //    int64_t stmp = 0x0000000012345678;
  //    int64_t sq = 0x0000000012345678;
  //    protoWrite.WriteHead(len,ver,stmp,sq);
  //    int  tp = 6;
  //    int32_t valLen = 0x00000002;
  //    int8_t val[2] = {9, 9};
  //    protoWrite.WriteTLV(tp, valLen, val);


  //    HobotProtocolRead protoRead(mybuff, mybuffleng);
  //    int32_t lenRd = 0;
  //    int32_t verRd = 0;
  //    int64_t stmpRd = 0;
  //    int64_t sqRd = 0;
  //    int tpRd = 9;
  //    int32_t valLenRd = 0;
  //    int8_t valRd[2] = {0, 0};

  //    protoRead.ReadHead(lenRd, verRd, stmpRd, sqRd);
  //    protoRead.ReadTLV(tpRd, valLenRd, valRd);
  // //    std::cout << lenRd  <<  std::endl;
  // //    std::cout << verRd  <<  std::endl;
  // //    std::cout << stmpRd <<  std::endl;
  // //    std::cout << sqRd   <<  std::endl;
  // //    std::cout << valLenRd   <<  std::endl;
  //    printf("lenRd: 0x%x\n", lenRd);
  //    printf("verRd: 0x%x\n",verRd);
  //    printf("stmpRd: 0x%lux\n",stmpRd);
  //    printf("sqRd: 0x%lux\n",sqRd);
  //    printf("valLenRd: 0x%x\n",valLenRd);
  //    std::cout << protoRead.GetIsEdian() << std::endl;

  //    printf("val[0] is %d\n", val[0]);
  //    printf("val[1] is %d\n", val[1]);
  {
    int8_t buff[128];
    Modo::HobotProtocolWrite write(buff, 128);
    char data[128] = "test";
    write.WriteHead(0, 0, 0);
    write.WriteTLV(0, strlen(data), (int8_t *)data);
    printf("write length: %d\n", write.GetPackageLength());
    HobotProtocolRead read(buff, 128);
    int type, length;
    int8_t *out;
    read.ReadTLV(type, length, &out);
    printf("out: %s\n", (char *)out);
  }


  {
    SyncFreatureRequest request;
    request.camera_id = 1;
    request.traking_id = 3;
    int8_t buff[128];
    Modo::HobotProtocolWrite write(buff, 128);
    write.WriteHead(0, 0, 0);
    write.WriteTLV(1009, sizeof(request), (int8_t *)&request);
    printf("write length: %d\n", write.GetPackageLength());
    HobotProtocolRead read(buff, 128);
    int type, length;
    int8_t *out;
    read.ReadTLV(type, length, &out);
    printf("out: %d,%d,%d,%d\n", type, length,
           ((SyncFreatureRequest *)out)->camera_id,
           ((SyncFreatureRequest *)out)->traking_id);
  }


  return 0;
}
