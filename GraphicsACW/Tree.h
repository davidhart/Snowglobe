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

private:

	void ParseTree(const std::string& treestring);

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	VertexBuffer _cylinderBuffer;
	VertexBuffer _cylinderIndices;
	VertexBinding _vertBinding;
	ObjFile _cylinderFile;

	class Branch
	{
		
	public:

		Branch();
		Branch(int parent, unsigned int depth, const Matrix4& matrix);
		const Matrix4& GetMatrix() const;
		int ParentBranch() const;

	private:

		int _parent;
		unsigned int _depth;
		Matrix4 _matrix;

	};

	std::vector<Branch> _branches;
	
};