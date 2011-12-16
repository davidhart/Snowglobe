// David Hart - 2011
// 
// class Tree
//   The responsibility of tree is parsing the tree pattern string,
//   drawing and managing the state of the tree. Each character in the
//   tree pattern string represents a command for the tree's interpretter
//     Available Commands:
//     'B'  Add a branch
//     'L'  Add a branch which leaves may be added to
//     '['  Push and reset the current rotation / scale state on the stack
//     ']'  Pop the current rotation / scale state on the stack
//     '>'  Increase the branch pitch
//     '<'  Decrease the branch pitch
//     '^'  Increase the branch yaw
//     'v'  Descrease the branch yaw
//     '+'  Increase the branch scale
//     '-'  Descrease the branch scale
//   
//   Tree makes use of a number of shaders to implement the draw modes
//   each shader uses a 3x4 matrix of per instance data to represent the 
//   rotation and position of ecah branch. Additionally each branch is packed 
//   with a float representing the depth of the branch node within the tree. 
//   This float is to be used with the draw depth to scale branches at specific 
//   depths to implement the tree growth / death animations.
//
//   The tree contains a number of leaves specified at creation. The positon and
//   orientation is selected randomly along a unit branch and then transformed onto
//   a randomly selected leaf node branch ('L' branch). The leave vertex shader 
//   handles animation of the leaves in autumn.

#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Matrix4.h"
#include "Texture.h"
#include "Renderer.h"

#include <vector>
#include <string>

class Renderer;

class Tree
{

public:

	Tree();

	void Create(const Renderer& renderer, const std::string& treestring, unsigned int numLeaves);
	void Dispose();
	
	void Grow();
	void Die();
	void Update(float dt);

	void Draw(const Renderer& renderer);
	void DrawReflection(const Renderer& renderer);

	unsigned int MaxBranchDepth() const;

	void NextDrawMode();

	float GetFireScale() const;

private:

	void ParseTree(const std::string& treestring, unsigned int numLeaves);
	void CreateBranches(const Renderer& renderer);
	void CreateLeaves(const Renderer& renderer);
	void CreateBranchInstanceBuffer(const Renderer& renderer);
	void CreateLeafInstanceBuffer(const Renderer& renderer);
	void DrawBranches(const Renderer& renderer, const Matrix4& model);
	void DrawLeaves(const Renderer& renderer, const Matrix4& mdoel);
	void FetchNonStandardUniforms();

	void ConstructModelMatrix(Matrix4& out) const;
	float GetDrawDepth() const;
	float GetLeafScale() const;
	float GetLeafColorLookup() const;
	float GetLeafFallTime() const;

	VertexShader _branchVert;
	VertexShader _branchVertFlat;

	FragmentShader _branchFlatShadedFrag;
	FragmentShader _branchTexturedLitFrag;
	FragmentShader _branchUnlitFrag;

	ShaderProgram _branchUnlitProgram;
	ShaderProgram _branchFlatProgram;
	ShaderProgram _branchTexturedLitProgram;
	ShaderProgram* _currentBranchProgram;

	Renderer::StandardUniformBlock _branchUnlitUniforms;
	Renderer::StandardUniformBlock _branchFlatUniforms;
	Renderer::StandardUniformBlock _branchTexturedLitUniforms;
	Renderer::StandardUniformBlock* _branchCurrentUniforms;
	Uniform _uniformDrawDepth;

	VertexBuffer _branchBuffer;
	VertexBuffer _branchIndices;
	VertexBuffer _branchInstanceBuffer;
	VertexBinding _branchBinding;

	Texture _textureOff;
	Texture _barkTexture;
	ObjFile _branchModel;

	VertexShader _leafVertShader;
	FragmentShader _leafFragShader;
	FragmentShader _leafFragUnlit;
	ShaderProgram _leafProgram;
	ShaderProgram _leafProgramUnlit;
	ShaderProgram* _currentLeafProgram;

	Renderer::StandardUniformBlock _leafStandardUniforms;
	Renderer::StandardUniformBlock _leafUnlitUniforms;
	Renderer::StandardUniformBlock* _leafCurrentUniforms;

	Uniform _uniformColorLookup;
	Uniform _uniformLeafScale;
	Uniform _uniformFallTime;
	Uniform _uniformFallSpeed;
	VertexBuffer _leafBuffer;
	VertexBuffer _leafIndices;
	VertexBuffer _leafInstanceBuffer;
	VertexBinding _leafBinding;
	Texture _leafTexture;
	Texture _leafGradient;
	ObjFile _leafModel;

	enum eDrawMode
	{
		DRAW_TEXTURED_LIT = 0,
		DRAW_LIT_SMOOTH = 1,
		DRAW_LIT_FLAT = 2,
		DRAW_WIREFRAME = 3,
	};

	eDrawMode _currentDrawMode;

	enum eTreeState
	{
		TREE_GROWING_BRANCHES,
		TREE_GROWING_LEAVES,
		TREE_ALIVE,
		TREE_DROPPING_LEAVES,
		TREE_LOSING_BRANCHES,
		TREE_DEAD,
	};

	eTreeState _currentState;
	float _time;

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
		explicit Leaf(const Branch& parent);
		void PackLeaf(float* out) const;

	private:

		Matrix4 _matrix;
	};

	std::vector<Branch> _branches;
	std::vector<unsigned int> _leafyBranchesIndices;
	std::vector<Leaf> _leaves;
	unsigned int _maxDepth;

	static const float ANIMATION_LENGTH_LEAF_BROWN;
	static const float ANIMATION_LENGTH_LEAF_FALL;
	static const float ANIMATION_LENGTH_BRANCH_GROW;
	static const float ANIMATION_LENGTH_LEAF_GROW;
	static const float ANIMATION_LENGTH_BRANCH_DIE;
	
};