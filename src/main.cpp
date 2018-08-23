#include <aurora2/aurora2.h>
#include "world_controller.h"

#include "serialize_scene.h"
#include "deserialize_scene.h"

#include "fbx_read/fbx_read.h"

#include "mesh_buffer.h"

#include <scene_controller.h>

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

void ResourcesFromFbxScene(FbxScene& fbxScene)
{
    for(unsigned i = 0; i < fbxScene.GeometryCount(); ++i)
    {
        FbxGeometry& geom = fbxScene.GetGeometry(i);
        
        mz_zip_archive zip;
        memset(&zip, 0, sizeof(zip));
        mz_zip_writer_init_heap(&zip, 0, 0);

        int32_t vertexCount = (int32_t)geom.VertexCount();
        int32_t indexCount = (int32_t)geom.IndexCount();
        mz_zip_writer_add_mem(&zip, "VertexCount", (void*)&vertexCount, sizeof(vertexCount), 0);
        mz_zip_writer_add_mem(&zip, "IndexCount", (void*)&indexCount, sizeof(indexCount), 0);
        mz_zip_writer_add_mem(&zip, "Indices", (void*)geom.GetIndices().data(), indexCount * sizeof(uint32_t), 0);
        mz_zip_writer_add_mem(&zip, "Vertices", (void*)geom.GetVertices().data(), vertexCount * 3 * sizeof(float), 0);
        if(geom.UVLayerCount() > 0)
            mz_zip_writer_add_mem(&zip, "Normals.0", (void*)geom.GetNormals(0).data(), vertexCount * 3 * sizeof(float), 0);
        if(geom.NormalLayerCount() > 0)
            mz_zip_writer_add_mem(&zip, "UV.0", (void*)geom.GetUV(0).data(), vertexCount * 2 * sizeof(float), 0);


        void* bufptr;
        size_t sz;
        mz_zip_writer_finalize_heap_archive(&zip, &bufptr, &sz);
        // TODO: Copy data, make resource
        g_resourceRegistry.Add(
            MKSTR(geom.GetUid() << geom.GetName() << ".geo"), 
            new ResourceRawMemory((char*)bufptr, sz)
        );
        mz_zip_writer_end(&zip);
    }
}

void SceneFromFbxModel(FbxModel& fbxModel, FbxScene& fbxScene, SceneObject* sceneObject){
    for(unsigned i = 0; i < fbxModel.ChildCount(); ++i)
    {
        SceneObject* child = sceneObject->CreateObject();
        sceneObject->Get<Transform>()->Attach(child->Get<Transform>());

        SceneFromFbxModel(fbxModel.GetChild(i, fbxScene), fbxScene, child);
    }

    sceneObject->Name(fbxModel.GetName());
    //LOG("Created object: " << fbxModel.GetName());
    //LOG("Type: " << fbxModel.GetType());

    sceneObject->Get<Transform>()->SetTransform(*(gfxm::mat4*)&fbxModel.GetTransform());
    if(fbxModel.GetType() == FbxMesh::Type())
    {
        FbxMesh& fbxMesh = fbxScene.GetMesh(fbxModel.GetUid());
        FbxGeometry& fbxGeometry = fbxScene.GetGeometryByUid(fbxMesh.GetGeometryUid());
        sceneObject->Get<Model>()->SetMesh(
            MKSTR(fbxGeometry.GetUid() << fbxGeometry.GetName() << ".geo")
        );

        auto& vertices = fbxGeometry.GetVertices();
        auto& indices = fbxGeometry.GetIndices();
    }
    else if(fbxModel.GetType() == FbxLight::Type())
    {
        LightOmni* o = sceneObject->Get<LightOmni>();
        o->Color(
            1.0f,
            1.0f,
            1.0f
            /*((rand() % 50 + 50) * 0.01f), 
            ((rand() % 50 + 50) * 0.01f), 
            ((rand() % 50 + 50) * 0.01f)*/
        );
        o->Intensity(1.0f);
    }
}

void SceneFromFbx(FbxScene& fbxScene, SceneObject* scene){
    for(unsigned i = 0; i < fbxScene.ModelCount(); ++i)
    {
        SceneObject* child = scene->CreateObject();
        scene->Get<Transform>()->Attach(child->Get<Transform>());
        SceneFromFbxModel(fbxScene.GetModel(i), fbxScene, child);
    }
}

bool SceneFromFbx(const char* data, size_t size, SceneObject* scene)
{
    FbxScene fbxScene;
    if(!FbxReadMem(fbxScene, data, size))
        return false;
    ResourcesFromFbxScene(fbxScene);
    SceneFromFbx(fbxScene, scene);
    return true;
}

bool SceneFromFbx(const std::string& filename, SceneObject* scene)
{    
    FbxScene fbxScene;
    if(!FbxReadFile(fbxScene, filename))
        return false;
    ResourcesFromFbxScene(fbxScene);
    SceneFromFbx(fbxScene, scene);
    fbxScene._dumpFile(filename);
    return true;
}

SceneObject myScene;
SceneObject so;
SceneObject fbx_so;

class Rotator : public Updatable
{
    RTTR_ENABLE(Updatable)
public:
    void OnUpdate()
    {
        Get<Transform>()->Rotate(0.1f * GameState::DeltaTime(), gfxm::vec3(0.0f, 0.0f, 1.0f));
    }
};

void Aurora2Init()
{
    auto gc = myScene.Get<WorldController>();

    SerializeScene(&myScene, "test.scn");
    DeserializeScene("test.scn", so);

    SceneObject* fbx_child = myScene.CreateObject();
    //fbx_child->Get<Rotator>();
    SceneFromFbx("character.fbx", fbx_child);
    SerializeScene(fbx_child, "fbx_so.scn");

    std::vector<char> buf;
    MakeGeometryResourceData(buf);
    std::ofstream f("geometry.geo");
    f.write(buf.data(), buf.size());
    f.close();
/*
    ResourceRef ref("General.Script.scene");
    ref.Get<ResTest>()->Print();
*/
    GameState::GetSceneController().SetScene(&myScene);
}