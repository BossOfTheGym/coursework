#pragma once

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
        UInt mParent;

        Mat4 mTransform;

        
        ObjectBlock(const UInt& parent, const aiMatrix4x4& transform);

        ObjectBlock(const ObjectBlock& ob) = default;

        ObjectBlock(ObjectBlock&& ob) = default;


        ObjectBlock& operator = (const ObjectBlock& ob) = default;

        ObjectBlock& operator = (ObjectBlock&& ob) = default;
    };
    using Object = ObjectBlock;


    struct SharedBlock
    {
        UInt mNumChildren;
        Indices mChildren;

        UInt mNumMeshes;
        Indices mMeshes;


        SharedBlock();

        SharedBlock(const SharedBlock& sharedBlock) = delete;

        SharedBlock(SharedBlock&& sharedBlock) = delete;


        SharedBlock& operator = (const SharedBlock& sharedBlock) = delete;

        SharedBlock& operator = (SharedBlock&& sharedBlock) = delete;
    };
    using Shared = std::shared_ptr<SharedBlock>;


public:
    Node();

    Node(const aiNode* node);

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

   
    const UInt& parent() const;


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
        ObjectBlock() = default;

        ObjectBlock(const ObjectBlock& ob) = default;

        ObjectBlock(ObjectBlock&& ob) = default;


        ObjectBlock& operator = (const ObjectBlock& ob) = default;

        ObjectBlock& operator = (ObjectBlock&& ob) = default;
    };
    using Object = ObjectBlock;


    struct SharedBlock
    {
        UInt mMaterialIndex;
        VAB mVertexBuffer;


        SharedBlock(UInt materialIndex, VAB&& vab = VAB());

        SharedBlock(const SharedBlock& sb) = delete;

        SharedBlock(SharedBlock&& sb) = delete;


        SharedBlock& operator = (const SharedBlock& sb) = delete;

        SharedBlock& operator = (SharedBlock&& sb) = delete;
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
    using Meshes = std::unique_ptr<Mesh[]>;
    using Nodes  = std::unique_ptr<Node[]>;


    struct ObjectBlock
    {
        ObjectBlock() = default;

        ObjectBlock(const ObjectBlock& ob) = default;

        ObjectBlock(ObjectBlock&& ob) = default;


        ObjectBlock& operator = (const ObjectBlock& ob) = default;

        ObjectBlock& operator = (ObjectBlock&& ob) = default;
    };
    using Object = ObjectBlock;


    struct SharedBlock
    {
        UInt mNumMeshes;
        Meshes mMeshes;

        UInt mNumNodes;
        Nodes mNodes;


        SharedBlock(UInt numMeshes, UInt numNodes);

        SharedBlock(const SharedBlock& sb) = delete;

        SharedBlock(SharedBlock&& sb) = delete;


        SharedBlock& operator = (const SharedBlock& sb) = delete;

        SharedBlock& operator = (SharedBlock&& sb) = delete;
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