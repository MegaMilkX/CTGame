#ifndef FBX_READER_H
#define FBX_READER_H

#include "fbx_scene.h"
#include "fbx_node.h"

typedef uint8_t Byte;
typedef uint32_t Word;
typedef uint64_t DoubleWord;

class FbxReader
{
public:
    bool ReadMem(const char* data, size_t size, FbxScene& scene);
    bool ReadFile(const std::string& filename, FbxScene& scene);

    FbxNode& GetRootNode() { return rootNode; }
private:
    void ReadData(FbxProp& prop, std::vector<char>& out, const char* data, const char*& cursor, const char* end);
    bool ReadBlock(FbxNode& node, const char* data, const char*& cursor, const char* end, Word flags);

    FbxNode rootNode;
};

#endif
