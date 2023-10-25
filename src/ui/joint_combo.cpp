#include "joint_combo.h"

void JointCombo::MakeCombo()
{
  if (this->joints.empty())
  {
    ImGui::Text("Import BVH file :)");
    return;
  }
  
  if(ImGui::BeginCombo("##BvhCombo", this->label.c_str()))
    {
      this->filter.Draw("##BvhSearch", 300.0f);

      if (ImGui::Selectable("UNBOUND", (this->selected == -1))) 
      {
        this->label = "UNBOUND";
        this->selected = -1;
      }
      for (int index = 0; index < this->joints.size(); index++)
      {
        std::string jointName = this->joints[index].name;
        if (!this->filter.PassFilter(jointName.c_str()))
          continue;

        if (ImGui::Selectable(jointName.c_str(), (this->selected == index))) 
        {
          this->label = jointName;
          this->selected = index;
        }

        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
        if (this->selected == index)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
}