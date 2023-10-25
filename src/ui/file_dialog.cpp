#include "file_dialog.h"


bool FileDialog::OpenFileButton(const std::string UUID){
  
  if (ImGui::Button("Open File Dialog"))
    ImGuiFileDialog::Instance()->OpenDialog(UUID, "Choose File", ".*", this->LastDirectory);
  // display
  if (ImGuiFileDialog::Instance()->Display(UUID, ImGuiWindowFlags_None, ImVec2(750,750))) 
  {
    // action if OK

    if (ImGuiFileDialog::Instance()->IsOk())
    {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      this->LastDirectory = ImGuiFileDialog::Instance()->GetCurrentPath()+"/";
      this->fileName = std::move(filePathName);
    }
    
    // close
    ImGuiFileDialog::Instance()->Close();
    return true;
  }
  return false;
}

//customizable dialog
bool FileDialog::OpenFileButton(const std::string UUID, std::string BtnText, std::string FileDialogTitle, std::string FileFilters)
{

  if (ImGui::Button(BtnText.c_str()))
    ImGuiFileDialog::Instance()->OpenDialog(UUID, FileDialogTitle, FileFilters.c_str(), this->LastDirectory);

  // display
  if (ImGuiFileDialog::Instance()->Display(UUID, ImGuiWindowFlags_None, ImVec2(750,750))) 
  {
    // action if OK

    if (ImGuiFileDialog::Instance()->IsOk())
    {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      this->LastDirectory = ImGuiFileDialog::Instance()->GetCurrentPath()+"/";
      this->fileName = std::move(filePathName);
    }
    
    // close
    ImGuiFileDialog::Instance()->Close();
    return true;
  }
  return false;
}