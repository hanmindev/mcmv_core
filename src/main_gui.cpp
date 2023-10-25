#include <stdio.h>
#include "ui/makeui.h"


int main()
{
  MakeUI ui;
  ui.MakeWindow(1000, 1500, "MCMV Bone Mapping Tool", &RenderFrame);
  return 0;
}