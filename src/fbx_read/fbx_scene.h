#ifndef FBX_SCENE_H
#define FBX_SCENE_H

#include "fbx_node.h"
#include "fbx_model.h"
#include "fbx_connection.h"
#include "fbx_math.h"

#include <string>
#include <vector>
#include <iostream>

struct FbxSettings
{
    double scaleFactor = 1.0;
};

class FbxScene
{
public:
    unsigned ModelCount() const;
    FbxModel& GetModel(unsigned i);
    FbxModel& GetModelByUid(int64_t uid);

    void _dumpFile(const std::string& filename);
    /* Don't use this */
    void _finalize();
    /* This is not a spatial node, it's a file node */
    FbxNode& _getRootNode() { return rootNode; }
private:
    void _makeGlobalSettings();
    void _makeModel(FbxNode& node);
    FbxConnection* _findObjectToObjectParentConnection(int64_t uid);

    FbxNode rootNode;

    FbxSettings settings;

    std::vector<int64_t> rootModels;
    std::map<int64_t, FbxModel> models;

    std::vector<FbxConnection> connections;
};

#endif
