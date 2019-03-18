#include "statusRenderHelper.h"
#include <string>

std::string StatusRenderHelper::Roi2String(const Rect &roi) {
  return std::string("(") + std::to_string(roi.left()) + "," +
         std::to_string(roi.top()) + "," + std::to_string(roi.right()) + "," +
         std::to_string(roi.bottom()) + ")";
}

std::string StatusRenderHelper::Point2String(const Point &point) {
  return std::string("(") + std::to_string(point.x()) + "," +
         std::to_string(point.y()) + ")";
}

std::string StatusRenderHelper::Vector3f2String(const Vector_3f &vec) {
  return std::string("(") + std::to_string(vec.x()) + "," +
         std::to_string(vec.y()) + "," + std::to_string(vec.z()) + ")";
}

std::string StatusRenderHelper::RotRPY2String(const RotRPY &rpy) {
  return std::string("(") + "roll=" + std::to_string(rpy.roll()) + "," +
         "pitch=" + std::to_string(rpy.pitch()) + "," + "yaw=" +
         std::to_string(rpy.yaw()) + ")";
}

void StatusRenderHelper::renderEyeResult(const EyeResult &eyeResult) {
  // intent 4
  mResult.push_back("        |-eye_result");
  mResult.push_back(std::string("          |-left=") +
                    std::to_string(eyeResult.left()));
  mResult.push_back(std::string("          |-right=") +
                    std::to_string(eyeResult.right()));
  mResult.push_back(std::string("          |-left_roi=") +
                    Roi2String(eyeResult.left_roi()));
  mResult.push_back(std::string("          |-right_roi=") +
                    Roi2String(eyeResult.right_roi()));

  if (eyeResult.has_score()) {
    mResult.push_back(std::string("            |-left_open_score=") +
                      std::to_string(eyeResult.score().left_open_score()));
    mResult.push_back(std::string("            |-left_close_score=") +
                      std::to_string(eyeResult.score().left_close_score()));
    mResult.push_back(std::string("            |-left_unknow_score=") +
                      std::to_string(eyeResult.score().left_unknow_score()));
    mResult.push_back(std::string("            |-right_open_score=") +
                      std::to_string(eyeResult.score().right_open_score()));
    mResult.push_back(std::string("            |-right_close_score=") +
                      std::to_string(eyeResult.score().right_close_score()));
    mResult.push_back(std::string("            |-right_unknow_score=") +
                      std::to_string(eyeResult.score().right_unknow_score()));
  }
}

void StatusRenderHelper::renderSmokeResult(const SmokeResult &smokeResult) {
  // intent 4
  mResult.push_back("        |-smoke_result");
  // Render smoke_result
  mResult.push_back(std::string("          |-smoke_state=") +
                    std::to_string(smokeResult.smoke_state()));
  mResult.push_back(std::string("          |-smoke_roi=") +
                    Roi2String(smokeResult.smoke_roi()));

  if (smokeResult.has_score()) {
    mResult.push_back(std::string("          |-score"));
    mResult.push_back(std::string("            |-no_smoke_normal=") +
                      std::to_string(smokeResult.score().no_smoke_normal()));
    mResult.push_back(std::string("            |-smoke_normal=") +
                      std::to_string(smokeResult.score().smoke_normal()));
    mResult.push_back(std::string("            |-no_smoke_suspect=") +
                      std::to_string(smokeResult.score().no_smoke_suspect()));
  }
}


void StatusRenderHelper::renderEventResult(
    const google::protobuf::RepeatedPtrField<EventResult> &eventResult) {
  // intent 4
  std::string events = "        |-event_result=(";
  for (int i = 0; i < eventResult.size(); i++) {
    events += std::to_string(eventResult.Get(i).event()) + ",";
  }
  events += ")";
  mResult.push_back(events);
}

void StatusRenderHelper::renderStateTrigger(
    const DMSOutputProtocol::StateTrigger &trigger) {
  // intent 4
  mResult.push_back("        |-state_trigger");
  mResult.push_back(std::string("          |-ddw_l=") +
                    std::to_string(trigger.ddw_l()));
  mResult.push_back(std::string("          |-ddw_r=") +
                    std::to_string(trigger.ddw_r()));
  mResult.push_back(std::string("          |-ddw_u=") +
                    std::to_string(trigger.ddw_u()));
  mResult.push_back(std::string("          |-ddw_d=") +
                    std::to_string(trigger.ddw_d()));
  mResult.push_back(std::string("          |-ddw_lu=") +
                    std::to_string(trigger.ddw_lu()));
  mResult.push_back(std::string("          |-ddw_ld=") +
                    std::to_string(trigger.ddw_ld()));
  mResult.push_back(std::string("          |-ddw_ru=") +
                    std::to_string(trigger.ddw_ru()));
  mResult.push_back(std::string("          |-ddw_rd=") +
                    std::to_string(trigger.ddw_rd()));
  mResult.push_back(std::string("          |-dfw=") +
                    std::to_string(trigger.dfw()));
  mResult.push_back(std::string("          |-dya=") +
                    std::to_string(trigger.dya()));
  mResult.push_back(std::string("          |-dca=") +
                    std::to_string(trigger.dca()));
  mResult.push_back(std::string("          |-dsa=") +
                    std::to_string(trigger.dsa()));
}

void StatusRenderHelper::renderFaceFeature(const std::string &faceFeature) {
  // intent 4
  std::string str = "        |-face_feature=(";
  for (int i = 0; i < faceFeature.size(); i++) {
    str += std::to_string(faceFeature.at(i)) + ",";
    if (i != 0 && i % 20 == 0) {
      // break line
      mResult.push_back(str);
      str = "                        ";
    }
  }
  str += ")";
  mResult.push_back(str);
}

void StatusRenderHelper::renderLDMK(const LDMK &ldmk) {
  // intent 5
  std::string str = "          |-ldmk=[";
  for (int i = 0; i < ldmk.face_feature_points().size(); i++) {
    str += Point2String(ldmk.face_feature_points().Get(i)) + ",";
    if (i != 0 && i % 10 == 0) {
      // break line
      mResult.push_back(str);
      str = "                 ";
    }
  }
  str += "]";
  mResult.push_back(str);
}


void StatusRenderHelper::renderFaceDirection(const FaceDirection &faceDir) {
  // intent 5
  mResult.push_back("          |-face_direction");
  mResult.push_back(std::string("            |-face_dir=") +
                    std::to_string(faceDir.face_dir()));

  if (faceDir.has_cur_angle_rpy()) {
    mResult.push_back(std::string("            |-cur_angle_rpy=") +
                      RotRPY2String(faceDir.cur_angle_rpy()));
  }

  if (faceDir.has_avr_angle_rpy()) {
    mResult.push_back(std::string("            |-avr_angle_rpy=") +
                      RotRPY2String(faceDir.avr_angle_rpy()));
  }

  if (faceDir.has_cur_angle_unit_vector()) {
    mResult.push_back(
        std::string("            |-cur_angle_unit_vector=") +
        Vector3f2String(faceDir.cur_angle_unit_vector().unit_vector()));
  }

  if (faceDir.has_avr_angle_unit_vector()) {
    mResult.push_back(
        std::string("            |-avr_angle_unit_vector=") +
        Vector3f2String(faceDir.avr_angle_unit_vector().unit_vector()));
  }
}

void StatusRenderHelper::renderFaceInfo(const FaceInfo &faceInfo) {
  // intent 4
  mResult.push_back("        |-face_info");
  mResult.push_back(std::string("          |-type=") +
                    std::to_string(faceInfo.type()));
  mResult.push_back(std::string("          |-face_roi=") +
                    Roi2String(faceInfo.face_roi().face_roi()));
  if (faceInfo.face_roi().has_conf()) {
    mResult.push_back(std::string("          |-conf=") +
                      std::to_string(faceInfo.face_roi().conf()));
  }
  if (faceInfo.has_ldmk()) {
    renderLDMK(faceInfo.ldmk());
  }
  if (faceInfo.has_tracking_id()) {
    mResult.push_back(std::string("          |-tracking_id=") +
                      std::to_string(faceInfo.tracking_id()));
  }
  if (faceInfo.has_face_direction()) {
    renderFaceDirection(faceInfo.face_direction());
  }
}

void StatusRenderHelper::renderPerson(
    const google::protobuf::RepeatedPtrField<PersonInfo> &person) {
  for (int i = 0; i < person.size(); i++) {
    // intent 3
    mResult.push_back(std::string("      |-person[") + std::to_string(i) + "]");

    // Render face_info
    renderFaceInfo(person.Get(i).face_info());

    // Render face_feature
    if (person.Get(i).has_face_feature()) {
      renderFaceFeature(person.Get(i).face_feature());
    }

    // Render eye_result
    if (person.Get(i).has_eye_result()) {
      renderEyeResult(person.Get(i).eye_result());
    }

    // Render smoke_result
    if (person.Get(i).has_smoke_result()) {
      renderSmokeResult(person.Get(i).smoke_result());
    }


    // Render event_result
    renderEventResult(person.Get(i).event_result());

    // Render state_trigger
    if (person.Get(i).has_state_trigger()) {
      renderStateTrigger(person.Get(i).state_trigger());
    }
  }
}

void StatusRenderHelper::renderDAA(
    const DMSOutputProtocol::DAAResult &daaResult) {
  // intent 3
  mResult.push_back(std::string("      |-daa_result"));
  mResult.push_back(std::string("        |-daa_event=") +
                    std::to_string(daaResult.daa_event()));
}


void StatusRenderHelper::renderRegisterIdenty(
    const std::string &registerIdenty) {
  // intent 2
  std::string str = "    |-register_identy=(";
  for (int i = 0; i < registerIdenty.size(); i++) {
    str += std::to_string(registerIdenty.at(i)) + ",";
    if (i != 0 && i % 20 == 0) {
      // break line
      mResult.push_back(str);
      str = "                    ";
    }
  }
  str += ")";
  mResult.push_back(str);
}

void StatusRenderHelper::renderRecogIdenty(const std::string &recogIdenty) {
  // intent 2
  std::string str = "    |-recognition_identy=(";
  for (int i = 0; i < recogIdenty.size(); i++) {
    str += std::to_string(recogIdenty.at(i)) + ",";
    if (i != 0 && i % 20 == 0) {
      // break line
      mResult.push_back(str);
      str = "                    ";
    }
  }
  str += ")";
  mResult.push_back(str);
}

void StatusRenderHelper::renderModoCommand(const std::string &modoCommand) {
  // intent 2
  std::string str = "    |-modo_command=(";
  for (int i = 0; i < modoCommand.size(); i++) {
    str += std::to_string(modoCommand.at(i)) + ",";
    if (i != 0 && i % 20 == 0) {
      // break line
      mResult.push_back(str);
      str = "                    ";
    }
  }
  str += ")";
  mResult.push_back(str);
}


void StatusRenderHelper::renderVision() {
  // intent 1
  mResult.push_back("-vision");
  printf("vision_size=%d \n", mProto.vision_size());

  for (int i = 0; i < mProto.vision_size(); i++) {
    // intent 2
    mResult.push_back(std::string("    |-vision[") + std::to_string(i) + "]");
    mResult.push_back(std::string("      |-camera_type=") +
                      std::to_string(mProto.vision().Get(i).camera_type()));
    mResult.push_back(std::string("      |-frame_id=") +
                      std::to_string(mProto.vision().Get(i).frame_id()));
    mResult.push_back(std::string("      |-time_stamp=") +
                      std::to_string(mProto.vision().Get(i).time_stamp()));
    //   renderHandRoi(mProto.vision().Get(i).hand_roi());
    renderPerson(mProto.vision().Get(i).person());
    renderDAA(mProto.vision().Get(i).daa_result());
  }
}
