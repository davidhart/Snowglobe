// David Hart - 2012
// 
// class TreeBuilder
//   The responsibility of treebuilder is evaluating the l system and parsing the resulting 
//   tree pattern string into a tree structure (Using helper classes leaf and branch. 
//   
//   The matrices for each instance of a branch and leaf are packed into buffers ready for
//   loading into the vertex buffers used by Tree
// 
//   Each character in the
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
//    Future imrpovements:
//      -Move tree design into a separate class and have treebuilder configure instances of 
//       that class
//
#pragma once

#include <string>
#include <vector>
#include "Matrix4.h"
#include "LSystem.h"
#include "Uncopyable.h"

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


class TreeBuilder : public Uncopyable // TODO: copy constructor
{

public:

	TreeBuilder();
	~TreeBuilder();

	// Populate buffers with a new tree instance constructed through randomly selecting and 
	// applying one of the configured patterns
	void CreateTree();

	// Add a new tree configuration to the builder
	//   seed:       initial string that will be fed into the lsystem
	//   lsystem:    system preconfigured with rules for the tree
	//   numleaves:  total number of leaves to attach to the tree
	//   iterations: number of iterations of lsystem rule to perform on seed string
	void AddPattern(const std::string& seed, const LSystem& lsystem, unsigned int numLeaves, unsigned int iterations);

	// Return pointer to packed leaf instance buffer which contains the following properties
	// per leaf:
	//   4x3 floats - transformation matrix
	const float* GetPackedLeafBuffer() const;
	// Return the size of the pakced leaf buffer in bytes
	unsigned int GetPackedLeafBufferSize() const;
	// Return the number of leaf instances packed into the leaf buffer
	unsigned int GetNumLeaves() const;
	
	// Return pointer to packed branch instance buffer which contains the following properties
	// per branch:
	//   4x3 floats - tansformation matrix
	//   1 float    - branch depth in tree structure
	const float* GetPackedBranchBuffer() const;
	unsigned int GetPackedBranchBufferSize() const;
	// Return the number of branch instances packed into the branch buffer
	unsigned int GetNumBranches() const;

	// Return the maximum branch depth of the constructed tree instance
	unsigned int GetMaxDepth() const;

private:

	void ParseTree(const std::string& treestring, unsigned int numLeaves);
	void PackLeafBuffer();
	void PackBranchBuffer();

	struct TreePattern
	{
		std::string _seed;
		LSystem _lsystem;
		unsigned _numLeaves;
		unsigned _iterations;
	};

	std::vector<Branch> _branches;
	std::vector<unsigned int> _leafyBranchesIndices;
	std::vector<Leaf> _leaves;
	unsigned int _maxDepth;

	std::vector<float> _packedLeafInstances;
	std::vector<float> _packedBranchInstances;

	std::vector<TreePattern*> _patterns;
};