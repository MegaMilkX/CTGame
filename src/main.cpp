#include <aurora2/aurora2.h>
#include "world_controller.h"

#include "serialize_scene.h"
#include "deserialize_scene.h"

#include "fbx_read/fbx_read.h"

#include "mesh_buffer.h"

bool MakeGeometryResourceData(std::vector<char>& out_data)
{
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    mz_zip_writer_init_heap(&zip, 0, 0);

    int mem;
    mz_zip_writer_add_mem(&zip, "Vertices", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "Indices", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "Normals.0", &mem, sizeof(int), 0);
    mz_zip_writer_add_mem(&zip, "UV.0", &mem, sizeof(int), 0);
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
    if(fbxModel.GetType() == FbxMesh::Type())
    {
        FbxMesh& fbxMesh = fbxScene.GetMesh(fbxModel.GetUid());
        FbxGeometry& fbxGeometry = fbxScene.GetGeometry(fbxMesh.GetGeometryUid());
        sceneObject->Get<Model>()->SetMesh(
            MKSTR(fbxGeometry.GetUid() << fbxGeometry.GetName())
        );

        LOG(fbxGeometry.GetUid() << fbxGeometry.GetName());
    }
    /*
    else if(fbxModel.GetType() == FbxLight::Type())
    {
        FbxLight& fbxLight = fbxScene.GetLight(fbxModel.GetUid());
    }
    */
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

class ResourceRef
{
public:
    typedef std::function<void(ResourceRef&)> on_change_func_t;

    ResourceRef() {}
    ResourceRef(const std::string& name) {}
    template<typename T>
    T* Get()
    {
        return 0;
    }
    void SetOnChangeCallback(on_change_func_t cb);
private:
    on_change_func_t onChangeFunc;
};

SceneObject myScene;
SceneObject so;

void Aurora2Init()
{
    auto gc = myScene.Get<WorldController>();

    SerializeScene(&myScene, "test.scn");
    DeserializeScene("test.scn", so);

    SceneObject fbx_so;
    SceneFromFbx("character.fbx", &fbx_so);
    SerializeScene(&fbx_so, "fbx_so.scn");

    std::vector<char> buf;
    MakeGeometryResourceData(buf);
    std::ofstream f("geometry.geo");
    f.write(buf.data(), buf.size());
    f.close();

    MeshBuffer mb;
    std::vector<char> vertices;
    std::vector<char> normals;
    mb.SetAttribArray<Position>(vertices.data(), vertices.size());
    mb.SetAttribArray<Normal>(normals.data(), normals.size());

    ResourceRef ref("General.Character");
    ref.Get<SceneObject>();

    GameState::SetScene(&myScene);
}