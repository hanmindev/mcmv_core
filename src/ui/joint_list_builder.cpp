#include "joint_list_builder.h"

#include "import/bvh/bvh_importer.h"
#include "import/bb/bb_importer.h"
#include "armature/armature_format_adapter.h"
#include "export/bedrock/bedrock_exporter.h"


void JointList::ParseBBModel()
{
  BBImporter bbFile(this->BBModelFP);
  Model modelJoints = bbFile.get_model();
  this->BBModelJoints = std::move(modelJoints.joints);
  this->JCList.resize(this->BBModelJoints.size());
  this->jointMapping.reserve(this->BBModelJoints.size());
}

void JointList::ParseBvh()
{
  BvhImporter bvhFile(this->BvhFP);
  Model animJoints = bvhFile.get_model();
  this->BvhJoints = std::move(animJoints.joints);
}

bool JointList::SaveAnimationData(std::string Dir){

  if(this->BBModelFP.empty()||this->BvhFP.empty())
    return false;

  for(auto map : this->jointMapping)
  {
    if(map.second == "UNBOUND")
      return false;
  }

  BBImporter bbFile(this->BBModelFP); 
  Model BBModel = bbFile.get_model();

  BvhImporter bvhFile(this->BvhFP);
  Model bvhModel = bvhFile.get_model();
  Animation bvhAnim = bvhFile.get_animation();

  ProcessorConfig config = {
      bvhModel,
      BBModel,
      build_joint_map(bvhModel, BBModel, this->jointMapping)
  };

  auto FormattedModel = ArmatureFormatAdapter(config);

  for (auto& frame : bvhAnim.frames) {
      FormattedModel.push_motion_frame(frame);
  }

  auto formattedAnim = FormattedModel.get_animation();

  auto be = BedrockExporter(BBModel, formattedAnim);

  ExporterConfig ec = {
    bvhAnim.fps,
    true,
    false
  };

  be.export_armature_animation(Dir.c_str(), "test", ec);
  return true;
}


void JointList::DrawJointList(){
  for (int i = 0; i < this->BBModelJoints.size(); i++)
  {
    // dont draw unless model file is selected
    if(BBModelFP.empty())
      continue;

    if (this->JCList[i] == NULL)
    {
      std::unique_ptr<JointCombo> jc = std::make_unique<JointCombo>();
      this->JCList[i] = std::move(jc);
      
    }

    if (!this->BvhJoints.empty())
    {
      this->JCList[i]->joints = this->BvhJoints;
    }

    jointMapping[this->BBModelJoints[i].name] = this->JCList[i]->label;

    if (!filter.PassFilter(this->BBModelJoints[i].name.c_str()))
      continue;
    
    ImGui::Text("%s", this->BBModelJoints[i].name.c_str());
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(100, 0));
    ImGui::SameLine();
    ImGui::NextColumn();


    ImGui::PushID(i);
    this->JCList[i]->MakeCombo();
    ImGui::PopID();


    ImGui::NextColumn();
  }
}