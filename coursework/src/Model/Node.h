#pragma once

#include <assimp/scene.h>

#include <Common.h>


class Node
{
public:
	using Indices = std::vector<UInt>;


public:
	Node();

	Node(Indices&& children, Indices&& meshes, String&& name);

	Node(const Node& node) = delete;

	Node(Node&& node);


	~Node();


	Node& operator = (const Node& node) = delete;

	Node& operator = (Node&& node);


	//Node
	const Indices& children() const;

	const Indices& meshes() const;

	const String& name() const;


private:
	Indices mChildren;
	Indices mMeshes;

	String mName;
};