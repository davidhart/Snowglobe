#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Matrix4.h"
#include "Texture.h"

#include <vector>
#include <string>

class Renderer;

class Tree
{

public:

	Tree();

	void Create(const Renderer& renderer, const std::string& treestring, unsigned int leafDepth, unsigned int numLeaves);
	void Dispose();

	void Grow(float elapsed);
	void Shrink(float elapsed);

	void Draw(const Renderer& renderer);
	void DrawReflection(const Renderer& renderer);

	unsigned int MaxBranchDepth() const;

private:

	void ParseTree(const std::string& treestring, unsigned int leafDepth, unsigned int numLeaves);
	void CreateBranches(const Renderer& renderer);
	void CreateLeaves(const Renderer& renderer);
	void CreateBranchInstanceBuffer(const Renderer& renderer);
	void CreateLeafInstanceBuffer(const Renderer& renderer);
	void DrawBranches(const Renderer& renderer, const Matrix4& model);
	void DrawLeaves(const Renderer& renderer, const Matrix4& mdoel);

	void ConstructModelMatrix(Matrix4& out);

	VertexShader _branchVertShader;
	FragmentShader _branchFragShader;
	ShaderProgram _branchProgram;
	VertexBuffer _branchBuffer;
	VertexBuffer _branchIndices;
	VertexBuffer _branchInstanceBuffer;
	VertexBinding _branchBinding;
	Texture _barkTexture;
	ObjFile _branchModel;

	VertexShader _leafVertShader;
	FragmentShader _leafFragShader;
	ShaderProgram _leafProgram;
	VertexBuffer _leafBuffer;
	VertexBuffer _leafIndices;
	VertexBuffer _leafInstanceBuffer;
	VertexBinding _leafBinding;
	Texture _leafTexture;
	Texture _leafGradient;
	ObjFile _leafModel;

	class Branch
	{
		
	public:

		Branch();
		explicit Branch(const Matrix4& trunkMatrix);
		Branch(int parentID, const Branch& parent, const Matrix4& matrix);
		void PackBranch(float* out) const;
		int Parent() const;
		unsigned int Depth() const;
		void MultiplyMatrix(Vector4& inout) const;

	private:

		Matrix4 _matrix;
		int _parent;
		unsigned int _depth;

	};

	class Leaf
	{
		
	public:

		Leaf();
		Leaf(const Branch& parent);
		void PackLeaf(float* out) const;

	private:

		Matrix4 _matrix;
	};

	std::vector<Branch> _branches;
	std::vector<unsigned int> _leafyBranchesIndices;
	std::vector<Leaf> _leaves;
	unsigned int _maxDepth;
	float _drawDepth;
	
};