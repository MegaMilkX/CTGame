#ifndef MESH_BUFFER_H
#define MESH_BUFFER_H

#include <vector>
#include <map>
#include <aurora/gfx.h>

class VertexAttribDesc
{
public:
    virtual const char* Name() const = 0;
    virtual int ElementCount() const = 0;
    virtual GLenum ElementType() const = 0;
    virtual bool IsNormalized() const = 0;
};

#define DECL_VERTEX_ATTRIB(NAME, ELEM_COUNT, ELEM_TYPE, NORMALIZED) \
    class NAME : public VertexAttribDesc \
    { \
    public: \
        virtual const char* Name() { return #NAME; } \
        virtual int ElementCount() const { return ELEM_COUNT; } \
        virtual GLenum ElementType() const { return ELEM_TYPE; } \
        virtual bool IsNormalized() const { return NORMALIZED; } \
    }

DECL_VERTEX_ATTRIB(Position, 3, GL_FLOAT, false);
DECL_VERTEX_ATTRIB(Normal, 3, GL_FLOAT, false);
DECL_VERTEX_ATTRIB(UV, 3, GL_FLOAT, false);
DECL_VERTEX_ATTRIB(ColorRGB, 3, GL_UNSIGNED_BYTE, true);
DECL_VERTEX_ATTRIB(ColorRGBA, 4, GL_UNSIGNED_BYTE, true);
DECL_VERTEX_ATTRIB(BoneIndices, 4, GL_INT, false);
DECL_VERTEX_ATTRIB(BoneWeights, 4, GL_FLOAT, false);

class VertexDesc
{
public:

};

class GpuMesh
{
public:
    void Draw();
};

class MeshBuffer
{
public:
    template<typename T>
    void SetAttribArray(const void* data, size_t size)
    {

    }
    unsigned AttribCount() const;
    VertexAttribDesc* GetAttribDesc(unsigned i);
    template<typename T>
    int GetAttribIndex(unsigned instance);
    const std::vector<char>& GetAttribArray(unsigned i);
    
    void SetIndices(uint32_t* data, size_t count);
private:
    std::vector<VertexAttribDesc*> attribs;
    std::vector<std::vector<char>> buffers;
    std::map<unsigned, unsigned> attribToBuffer;
};

#endif
