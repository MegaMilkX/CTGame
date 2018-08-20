#ifndef FBX_GEOMETRY_H
#define FBX_GEOMETRY_H

#include "fbx_object.h"
#include <string>

class FbxGeometry : public FbxObject
{
public:
    void SetName(const std::string& name) { this->name = name; }
    const std::string& GetName() const { return name; }

    const std::vector<float>& GetVertices() const { return vertices; }
    const std::vector<uint32_t>& GetIndices() const { return indices; }

    size_t NormalLayerCount() const { return normal_layers.size(); }
    size_t UVLayerCount() const { return uv_layers.size(); }
    size_t RGBLayerCount() const { return rgb_layers.size(); }

    const std::vector<float>& GetNormals(int layer = 0) const { return normal_layers[layer]; }
    const std::vector<float>& GetUV(int layer = 0) const { return uv_layers[layer]; }
    const std::vector<float>& GetRGB(int layer = 0) const { return rgb_layers[layer]; }

    bool Make(FbxNode& node)
    {
        int64_t uid = node.GetProperty(0).GetInt64();
        std::string name = node.GetProperty(0).GetString();
        SetUid(uid);
        SetName(name);
        /*
        struct VertexMapping
        {
            int32_t vertex;
            std::vector<int32_t> normals;
            std::vector<int32_t> uvs;

            bool operator==(const VertexMapping& other) {
                if(vertex != other.vertex)
                    return false;
                if(normals.size() != other.normals.size())
                    return false;
                for(unsigned i = 0; i < normals.size(); ++i) {
                    if(normals[i] != other.normals[i])
                        return false;
                }
                if(uvs.size() != other.uvs.size())
                    return false;
                for(unsigned i = 0; i < uvs.size(); ++i) {
                    if(uvs[i] != other.uvs[i])
                        return false;
                }
                return true;
            }
        };
        std::vector<VertexMapping> vertexMappings;

        std::vector<int32_t> fbxIndices;
        const std::string indexNodeName = "PolygonVertexIndex";
        const std::string vertexNodeName = "Vertices";
        if(!node.ChildCount(indexNodeName))
            return false;

        fbxIndices =
            node.GetNode(indexNodeName, 0)
                .GetProperty(0)
                .GetArray<int32_t>();
        vertexMappings.resize(fbxIndices.size());
        */

        return true;
    }
private:
    std::string name;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    std::vector<std::vector<float>> normal_layers;
    std::vector<std::vector<float>> uv_layers;
    std::vector<std::vector<float>> rgb_layers;
};

#endif
