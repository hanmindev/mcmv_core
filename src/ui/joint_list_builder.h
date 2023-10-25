#include "joint_combo.h"

#include "import/bvh/bvh_importer.h"
#include "import/bb/bb_importer.h"
#include "armature/armature_format_adapter.h"
#include "export/bedrock/bedrock_exporter.h"

class JointList
{

  public:
    std::string BBModelFP;
    std::string BvhFP;

    std::vector<Joint> BBModelJoints;
    std::vector<Joint> BvhJoints;

    vector<std::unique_ptr<JointCombo>> JCList;
    ImGuiTextFilter filter;
    std::unordered_map<std::string, std::string> jointMapping;

  public: 
    void ParseBBModel();
    void ParseBvh();
    void DrawJointList();
    bool SaveAnimationData(std::string Dir);
};