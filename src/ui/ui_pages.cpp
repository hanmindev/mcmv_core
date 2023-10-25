#include "makeui.h"
#include "file_dialog.h"
#include "joint_combo.h"
#include "joint_list_builder.h"


float Yscale = 2.0F;
bool Style = DARK;
int msgTimer = 0;
std::string logMsg = "";

FileDialog fileBBModel;
FileDialog fileBVH;
JointList jointList;
ImGuiTextFilter filter;

std::string lastDir = ".";

void PageOne(MakeUI* ui);
void PageTwo(MakeUI* ui);


void RenderFrame(MakeUI* ui)
{
  ImGui::Begin(ui->title.c_str(), (bool *)true,  ImGuiWindowFlags_NoCollapse |
                                                    ImGuiWindowFlags_NoResize |
                                                    ImGuiWindowFlags_NoMove |
                                                    ImGuiWindowFlags_NoTitleBar |
                                                    ImGuiWindowFlags_NoBringToFrontOnFocus);

  ImGui::SetWindowSize(ImVec2(ui->width, ui->height));
  ImGui::SetWindowPos(ImVec2(0, 0));

  ImGui::GetIO().FontGlobalScale = Yscale;
  
  if (ImGui::BeginTabBar("MCMV_BONE_TABS"))
  {

    if (ImGui::BeginTabItem("Bone Map"))
    {
      // Page One
      PageOne(ui);
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Settings"))
    {
      // Page Two
      PageTwo(ui);
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  ImGui::End();
}

void PageOne(MakeUI* ui){
  if(!(msgTimer<1))
    msgTimer--;

  ImGui::Columns(2);
  
  // purely visual padding
  ImGui::Text("Search: ");
  ImGui::SameLine();
  filter.Draw("##JointNameFilter");
  ImGui::SameLine();
  ImGui::NextColumn();
  if(ImGui::Button("       Export Animation        "))
  {
    if(!jointList.SaveAnimationData(lastDir.c_str()))
    {
      msgTimer = 1 * 60;
      logMsg = "Fill in UNBOUND Bones!";
    }else
    {
      msgTimer = 2 * 60;
      logMsg = "Saving!";
    }
  }
  ImGui::NextColumn();
  ImGui::Dummy(ImVec2(0.0, 20.0));
  ImGui::NextColumn();
  // under export button
  ImGui::Dummy(ImVec2(0.0, 20.0));
  ImGui::SameLine();
  if(!(msgTimer<1))
  {
    ImGui::Text("%s", logMsg.c_str());
  }
  ImGui::NextColumn();


  if (fileBBModel.OpenFileButton("##diagBBModel", "Open BlockBench Model", "Find BlockBench Model", ".bbmodel"))
  {
    if(!fileBBModel.fileName.empty())
    {
      lastDir = fileBBModel.LastDirectory;
      jointList.BBModelFP = fileBBModel.fileName;
      jointList.ParseBBModel();
    }
  }

  ImGui::NextColumn();

  if(fileBVH.OpenFileButton("##diagBVH", "Open Animation File", "Find Animation File", ".bvh"))
  {
    if(!fileBVH.fileName.empty())
    {
      lastDir = fileBVH.LastDirectory;
      jointList.BvhFP = fileBVH.fileName;
      jointList.ParseBvh();
    }
  }

  ImGui::Separator();
  ImGui::NextColumn();

  // draw Bone Names and Combo Boxes
  jointList.filter = filter;
  jointList.DrawJointList();
}

void PageTwo(MakeUI* ui)
{
  ImGui::Text("UI Scale: ");
  ImGui::SameLine();
  ImGui::SliderFloat("##uiScale", &Yscale, 1.5, 5.0, "%.2f");
  ImGui::Separator();

  ImGui::Text("UI Style: ");
  ImGui::SameLine();
  ImGui::Checkbox("##LightDark", &Style);
  ImGui::SameLine();
  if(Style){
    ImGui::Text(" Dark");
    ImGui::StyleColorsDark();
  }else{
    ImGui::Text(" Light");
    ImGui::StyleColorsLight();
  }

}

