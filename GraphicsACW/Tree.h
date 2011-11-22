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
		explicit Branch(const Matrix4& trunkMatrix);
		Branch(int parentID, const Branch& parent, const Matrix4& matrix);
		void Pack4x3Matrix(float* out) const;
		int ParentBranch() const;

	private:

		Matrix4 _matrix;
		int _parent;
		unsigned int _depth;

	};

	std::vector<Branch> _branches;
	
};