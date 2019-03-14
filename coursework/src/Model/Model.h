#pragma once


#include "../Common.h"

#include "Mesh.h"


class Mesh;
class Node;
class Model;


class Node
{
public:
    Node();

    Node(const Node& node);

    Node(Node&& node);


    ~Node();


    Node& operator = (const Node& node);

    Node& operator = (Node&& node);



    const std::vector<Node>& children() const;

    const std::vector<int>& meshes() const;

    Mat4& transform();

    const Mat4& transform() const;


private:
    //nonassignable
    Model* mModel;
    //nonassignable
    Node* mParent;


    //object
    Mat4 mTransform;

    //object
    std::vector<Node> mChildren;

    //shared
    std::vector<int> mMeshes;
};


class Model
{
public:

private:
    //object
    Node mRoot;

    //shared
    std::vector<Mesh> mMeshes;
};