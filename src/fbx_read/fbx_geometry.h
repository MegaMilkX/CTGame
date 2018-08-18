#ifndef FBX_GEOMETRY_H
#define FBX_GEOMETRY_H

#include "fbx_object.h"
#include <string>

class FbxGeometry : public FbxObject
{
public:
    void SetName(const std::string& name) { this->name = name; }
    const std::string& GetName() const { return name; }
private:
    std::string name;
};

#endif
