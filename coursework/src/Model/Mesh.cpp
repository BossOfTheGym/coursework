#include "Mesh.h"

Mesh::Mesh(Model& model, const aiMesh * mesh)
    : mModel(std::addressof(model))
{}
