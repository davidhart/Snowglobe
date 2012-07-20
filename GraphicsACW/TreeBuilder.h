#pragma once

#include <string>
#include <vector>
#include "Matrix4.h"
#include "LSystem.h"
#include "Uncopyable.h"

class TreeBuilder : public Uncopyable // Todo: copy constructor
{
	friend class Tree;

public:

	TreeBuilder();
	~TreeBuilder();

	void CreateTree();

	void AddPattern(const std::string& seed, const LSystem& lsystem, unsigned int numLeaves, unsigned int iterations);

private:

	void ParseTree(const std::string& treestring, unsigned int numLeaves);
	void PackLeafBuffer();
	void PackBranchBuffer();

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