#include <aurora2/aurora2.h>
#include "world_controller.h"

#include "serialize_scene.h"
#include "deserialize_scene.h"

#include "fbx_read/fbx_read.h"

bool MakeGeometryResourceData(std::vector<char>& out_data)
{
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    mz_zip_writer_init_heap(&zip, 0, 0);

    int mem;
    mz_zip_writer_add_mem(&zip, "Vertices", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "Indices", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "Normals/0", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "UV/0", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "BoneIndices", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "BoneWeights", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "Submeshes/Cube", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "Submeshes/Teapot", &mem, sizeof(int), 0);

    void* bufPtr;
    size_t bufSize;
    mz_zip_writer_finalize_heap_archive(&zip, &bufPtr, &bufSize);
    out_data = std::vector<char>((char*)bufPtr, (char*)bufPtr + bufSize);
    mz_zip_writer_end(&zip);
    return true;
}

void SceneFromFbxModel(FbxModel& fbxModel, FbxScene& fbxScene, SceneObject* sceneObject){
    for(unsigned i = 0; i < fbxModel.ChildCount(); ++i)
    {
        SceneObject* child = sceneObject->CreateObject();
        sceneObject->Get<Transform>()->Attach(child->Get<Transform>());

        SceneFromFbxModel(fbxModel.GetChild(i, fbxScene), fbxScene, child);
    }

    sceneObject->Name(fbxModel.GetName());
    LOG("Created object: " << fbxModel.GetName());
    LOG("Type: " << fbxModel.GetType());

    sceneObject->Get<Transform>()->SetTransform(*(gfxm::mat4*)&fbxModel.GetTransform());
}

void SceneFromFbx(FbxScene& fbxScene, SceneObject* scene){
    for(unsigned i = 0; i < fbxScene.ModelCount(); ++i)
        SceneFromFbxModel(fbxScene.GetModel(i), fbxScene, scene->CreateObject());
}

bool SceneFromFbx(const char* data, size_t size, SceneObject* scene)
{
    FbxScene fbxScene;
    if(!FbxReadMem(fbxScene, data, size))
        return false;
    SceneFromFbx(fbxScene, scene);
    return true;
}

bool SceneFromFbx(const std::string& filename, SceneObject* scene)
{    
    FbxScene fbxScene;
    if(!FbxReadFile(fbxScene, filename))
        return false;
    SceneFromFbx(fbxScene, scene);
    fbxScene._dumpFile(filename);
    return true;
}

SceneObject myScene;
SceneObject so;

void Aurora2Init()
{
    auto gc = myScene.Get<WorldController>();

    SerializeScene(&myScene, "test.scn");
    DeserializeScene("test.scn", so);

    SceneFromFbx("character.fbx", myScene.CreateObject());

    std::vector<char> buf;
    MakeGeometryResourceData(buf);
    std::ofstream f("geometry.geo");
    f.write(buf.data(), buf.size());
    f.close();

    GameState::SetScene(&myScene);
}