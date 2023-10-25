#ifndef MCMV_CORE_UI_FILE_DIALOG_
#define MCMV_CORE_UI_FILE_DIALOG_
#include "joint_headers.h"

class FileDialog
{
  public:
    std::string fileName;
    std::string LastDirectory = ".";

  public:
    bool OpenFileButton(const std::string UUID);
    bool OpenFileButton(const std::string UUID, std::string BtnText, std::string FileDialogTitle, std::string FileFilters);
};

#endif