#ifndef FBX_GEOMETRY_H
#define FBX_GEOMETRY_H

#include "fbx_object.h"
#include "fbx_node.h"
#include <string>
#include <vector>
#include <iostream>

inline bool FbxTriangulate(std::vector<uint32_t>& out, std::vector<int32_t>& polygons)
{
    std::vector<int32_t> poly;
    for(auto fbxi : polygons)
    {
        int32_t i = fbxi < 0 ? -fbxi - 1 : fbxi;
        poly.emplace_back(i);

        if(fbxi < 0) {
            if(poly.size() < 3) {
                poly.clear();
                continue;
            }

            for(size_t p = 0; p < poly.size() - 2; ++p)
            {
                out.emplace_back(poly[0]);
                out.emplace_back(poly[p + 1]);
                out.emplace_back(poly[p + 2]);
            }

            poly.clear();
        }
    }

    return true;
}

struct FbxVertexMapping
{
    int32_t vertex;
    std::vector<int32_t> normals;
    std::vector<int32_t> uvs;

    bool operator==(const FbxVertexMapping& other) {
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
        std::string name = node.GetProperty(1).GetString();
        SetUid(uid);
        SetName(name);
        const std::string indexNodeName = "PolygonVertexIndex";
        const std::string vertexNodeName = "Vertices";
        const std::string normalLayerNodeName = "LayerElementNormal";
        const std::string uvLayerNodeName = "LayerElementUV";

        std::vector<int32_t> fbxIndices;
        if(!node.ChildCount(indexNodeName))
            return false;
        fbxIndices =
            node.GetNode(indexNodeName, 0)
                .GetProperty(0)
                .GetArray<int32_t>();
        FbxTriangulate(indices, fbxIndices);

        std::vector<double> fbxVertices;
        if(!node.ChildCount(vertexNodeName))
            return false;
        fbxVertices =
            node.GetNode(vertexNodeName, 0)
                .GetProperty(0)
                .GetArray<double>();
        vertices.reserve(fbxVertices.size());
        for(auto d : fbxVertices) vertices.emplace_back((float)d);

        for(unsigned i = 0; i < node.ChildCount(normalLayerNodeName); ++i) {
            FbxNode& fbxLayer = node.GetNode(normalLayerNodeName, i);
            std::string mapping = 
                fbxLayer.GetNode("MappingInformationType", 0)
                    .GetProperty(0)
                    .GetString();
            std::string refType = 
                fbxLayer.GetNode("ReferenceInformationType", 0)
                    .GetProperty(0)
                    .GetString();
            std::vector<double> fbxNormals =
                fbxLayer.GetNode("Normals", 0)
                    .GetProperty(0)
                    .GetArray<double>();
            
        }

        for(unsigned i = 0; i < node.ChildCount(uvLayerNodeName); ++i) {
            FbxNode& fbxLayer = node.GetNode(uvLayerNodeName, i);
            std::string mapping = 
                fbxLayer.GetNode("MappingInformationType", 0)
                    .GetProperty(0)
                    .GetString();
            std::string refType =
                fbxLayer.GetNode("ReferenceInformationType", 0)
                    .GetProperty(0)
                    .GetString();
            std::vector<double> fbxUV = 
                fbxLayer.GetNode("UV", 0)
                    .GetProperty(0)
                    .GetArray<double>();
            
        }

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
