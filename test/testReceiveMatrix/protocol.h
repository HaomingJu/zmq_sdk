/*
 * protocol.h
 *
 *  Created on: Feb 25, 2019
 *      Author: user
 */

#ifndef HOBOT_DMS_FRAMEWORK_SRC_BASE_INCLUDE_HOBOT_DMS_BASE_PROTOCOL_H_
#define HOBOT_DMS_FRAMEWORK_SRC_BASE_INCLUDE_HOBOT_DMS_BASE_PROTOCOL_H_
#pragma pack(push, 1)
namespace Modo {
/*
 * Message type
 */
enum ModoMsgType {
  // Percetion 1~1000
  MSG_PERCEPTION_JPEG_PREVIEW_DRIVER = 1,  // byte array
  MSG_PERCEPTION_JPEG_PREVIEW_FULL = 2,    // byte array
  MSG_PERCEPTION_JPEG_CAPTURE_DRIVER = 3,  // byte array
  MSG_PERCEPTION_JPEG_CAPTURE_FULL = 4,    // byte array
  MSG_PERCEPTION_POROTO = 5,               // byte array


};

}
#pragma pack(pop)

#endif /* HOBOT_DMS_FRAMEWORK_SRC_BASE_INCLUDE_HOBOT_DMS_BASE_PROTOCOL_H_ */
