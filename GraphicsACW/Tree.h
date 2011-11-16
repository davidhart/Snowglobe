#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Matrix4.h"

#include <vector>
#include <string>

class Renderer;

class Tree
{

public:

	Tree();

	void Create(const Renderer& renderer, const std::string& treestring);
	void Dispose();

	void Draw(const Renderer& renderer);
	void DrawReflection(const Renderer& renderer);

private:

	void ParseTree(const std::string& treestring);
	void CreateBranchInstanceBuffer(const Renderer& renderer);

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	VertexBuffer _cylinderBuffer;
	VertexBuffer _cylinderIndices;

	VertexBuffer _branchInstanceBuffer;

	VertexBinding _vertBinding;
	ObjFile _cylinderFile;

	class Branch
	{
		
	public:

		Branch();
		Branch(int parent, unsigned int depth, const Matrix4& matrix);
		Matrix4 GetMatrix() const;
		int ParentBranch() const;

	private:

		Matrix4 _matrix;
		int _parent;
		unsigned int _depth;

	};

	std::vector<Branch> _branches;
	
};