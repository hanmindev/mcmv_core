#ifndef MCMV_CORE_UI_JOINT_COMBO_
#define MCMV_CORE_UI_JOINT_COMBO_
#include "joint_headers.h"
#include "armature/joint.h"


class JointCombo
{
  public:
    std::string label = "UNBOUND";
    std::vector<Joint> joints;
    ImGuiTextFilter filter;
    int selected = -1;

  public:
    void MakeCombo();
};


#endif