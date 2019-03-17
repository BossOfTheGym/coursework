#pragma once

#include <functional>
#include <map>
#include <vector>


#include <assimp/scene.h>


#include <Common.h>

#include <Texture/Texture2D.h>

#include <Shader/ShaderProgram.h>

#include <Model/Model.h>
#include <Model/VertexArrayBuffer.h>


class Mesh;
class Node;
class Model;


class Node
{
public:
    using UInt = unsigned int;
    using Indices  = std::unique_ptr<UInt[]>;


    struct ObjectBlock
    {
    public:
        ObjectBlock(const aiMatrix4x4& transform = aiMatrix4x4());

        ObjectBlock(const ObjectBlock& ob) = default;

        ObjectBlock(ObjectBlock&& ob) = default;


        ObjectBlock& operator = (const ObjectBlock& ob) = default;

        ObjectBlock& operator = (ObjectBlock&& ob) = default;


    public:
        Mat4 mTransform;
    };
    using Object = ObjectBlock;


    struct SharedBlock
    {
    public:
        SharedBlock(UInt numChildren = 0, UInt numMeshes = 0);

        SharedBlock(const SharedBlock& sharedBlock) = delete;

        SharedBlock(SharedBlock&& sharedBlock) = delete;


        SharedBlock& operator = (const SharedBlock& sharedBlock) = delete;

        SharedBlock& operator = (SharedBlock&& sharedBlock) = delete;


    public:
        UInt mNumChildren;
        Indices mChildren;

        UInt mNumMeshes;
        Indices mMeshes;


    };
    using Shared = std::shared_ptr<SharedBlock>;


public:
    Node();

    Node(std::map<const aiNode*, UInt>& mapping, const aiNode* node);

    Node(const Node& node);

    Node(Node&& node);


    ~Node();


    Node& operator = (const Node& node);

    Node& operator = (Node&& node);



    const UInt& numChildren() const;

    const Indices& children() const;


    const UInt& numMeshes() const;

    const Indices& meshes() const;


    Mat4& transform();

    const Mat4& transform() const;


private:
    Object mObjectBlock;
    Shared mSharedBlock;
};



class Mesh
{
public:
    using UInt = unsigned int;
    using VAB = VertexArrayBuffer;

    struct ObjectBlock
    {
    public:
        ObjectBlock() = default;

        ObjectBlock(const ObjectBlock& ob) = default;

        ObjectBlock(ObjectBlock&& ob) = default;


        ObjectBlock& operator = (const ObjectBlock& ob) = default;

        ObjectBlock& operator = (ObjectBlock&& ob) = default;
    };
    using Object = ObjectBlock;


    struct SharedBlock
    {
    public:
        SharedBlock(UInt materialIndex = 0, VAB&& vab = VAB());

        SharedBlock(const SharedBlock& sb) = delete;

        SharedBlock(SharedBlock&& sb) = delete;


        SharedBlock& operator = (const SharedBlock& sb) = delete;

        SharedBlock& operator = (SharedBlock&& sb) = delete;


    public:
        UInt mMaterialIndex;
        VAB mVertexBuffer;
    };
    using Shared = std::shared_ptr<SharedBlock>;


public:
    Mesh();

    Mesh(const aiMesh* mesh);

    Mesh(const Mesh& mesh);

    Mesh(Mesh&& mesh);


    ~Mesh();


    Mesh& operator = (const Mesh& mesh);

    Mesh& operator = (Mesh&& mesh);


    const VAB& vab() const;

    const UInt& material() const;


private:
    Object mObjectBlock;
    Shared mSharedBlock;
};



class Model
{
public:
    using UInt = unsigned int;
    using Meshes   = std::unique_ptr<Mesh[]>;
    using Nodes    = std::unique_ptr<Node[]>;
    using Materials = std::unique_ptr<Texture2D[]>;


    struct ObjectBlock
    {
    public:
        ObjectBlock() = default;

        ObjectBlock(const ObjectBlock& ob) = default;

        ObjectBlock(ObjectBlock&& ob) = default;


        ObjectBlock& operator = (const ObjectBlock& ob) = default;

        ObjectBlock& operator = (ObjectBlock&& ob) = default;
    };
    using Object = ObjectBlock;


    struct SharedBlock
    {
    public:
        SharedBlock(UInt numMeshes = 0, UInt numNodes = 0, UInt numMaterials = 0);

        SharedBlock(const SharedBlock& sb) = delete;

        SharedBlock(SharedBlock&& sb) = delete;


        SharedBlock& operator = (const SharedBlock& sb) = delete;

        SharedBlock& operator = (SharedBlock&& sb) = delete;


    public:
        UInt mNumMeshes;
        Meshes mMeshes;

        UInt mNumNodes;
        Nodes mNodes;

        UInt mNumMaterials;
        Materials mMaterials;
    };
    using Shared = std::shared_ptr<SharedBlock>;
    

public:
    Model();

    Model(const aiScene* scene);

    Model(const Model& model);

    Model(Model&& model);


    ~Model();


    Model& operator = (const Model& model);

    Model& operator = (Model&& model);



    const UInt& numMeshes() const;

    const Meshes& meshes() const;
    

    const UInt& numNodes() const;

    const Nodes& nodes() const;


    const Node& root() const;


private:
    Object mObjectBlock;
    Shared mSharedBlock;
};