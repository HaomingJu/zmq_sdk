#ifndef __STATUS_RENDER_HELPER_H__
#define __STATUS_RENDER_HELPER_H__

#include <string>
#include <vector>
#include "DMSSDKOutput.pb.h"

using namespace DMSOutputProtocol;

class StatusRenderHelper {
 public:
  StatusRenderHelper(PerceptResult &proto) : mProto(proto) { renderVision(); }
  virtual ~StatusRenderHelper() {}

  std::string Roi2String(const Rect &roi);
  std::string Point2String(const Point &point);
  std::string Vector3f2String(const Vector_3f &vec);
  std::string RotRPY2String(const RotRPY &rpy);


  void renderPerson(
      const google::protobuf::RepeatedPtrField<PersonInfo> &person);
  void renderDAA(const DMSOutputProtocol::DAAResult &daaResult);
  void renderEyeResult(const EyeResult &eyeResult);
  void renderEventResult(
      const google::protobuf::RepeatedPtrField<EventResult> &eventResult);
  void renderStateTrigger(const DMSOutputProtocol::StateTrigger &trigger);
  void renderSmokeResult(const SmokeResult &smokeResult);
  void renderFaceFeature(const std::string &faceFeature);

  void renderLDMK(const LDMK &ldmk);

  void renderFaceDirection(const FaceDirection &faceDir);
  void renderFaceInfo(const FaceInfo &faceInfo);
  void renderVision();

  void renderRegisterIdenty(const std::string &registerIdenty);
  void renderRecogIdenty(const std::string &recogIdenty);
  void renderModoCommand(const std::string &modoCommand);


  std::vector<std::string> &getResult() { return mResult; }

 private:
  PerceptResult mProto;
  std::vector<std::string> mResult;
};

#endif
