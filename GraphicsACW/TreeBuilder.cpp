// David Hart - 2012

#include "TreeBuilder.h"

#include <stack>
#include <cassert>

TreeBuilder::TreeBuilder() :
	_maxDepth(0)
{
}

TreeBuilder::~TreeBuilder()
{
	for (int i = 0; i < _patterns.size(); ++i)
	{
		delete _patterns[i];
	}
}


void TreeBuilder::CreateTree()
{
	TreePattern& pattern = *_patterns[rand() % _patterns.size()];

	std::string treestring;

	pattern._lsystem.EvaluateRules(pattern._seed, treestring, pattern._iterations);

	ParseTree(treestring, pattern._numLeaves);
	PackLeafBuffer();
	PackBranchBuffer();
}

void TreeBuilder::AddPattern(const std::string& seed, const LSystem& lsystem, unsigned int numLeaves, unsigned int iterations)
{
	TreePattern* pattern = new TreePattern;
	pattern->_seed = seed;
	pattern->_lsystem = lsystem;
	pattern->_numLeaves = numLeaves;
	pattern->_iterations = iterations;

	_patterns.push_back(pattern);
}

void TreeBuilder::ParseTree(const std::string& treestring, unsigned int numLeaves)
{
	_maxDepth = 1;

	unsigned int branchCount = 0;
	unsigned int leafyBranchCount = 0;
	for (unsigned int i = 0; i < treestring.size(); ++i)
	{
		if (treestring[i] == 'B')
		{
			++branchCount;
		}
		else if (treestring[i] == 'L')
		{
			++branchCount;
			++leafyBranchCount;
		}
	}

	_branches.resize(branchCount);
	_leafyBranchesIndices.clear();
	_leafyBranchesIndices.reserve(leafyBranchCount);

	Matrix4 base;
	Matrix4::Scale(base, Vector3(1.3f, 1, 1.3f));

	int parentBranch = 0;
	int currentBranch = 0;

	std::stack<float> yawstack;
	std::stack<float> pitchstack;
	std::stack<float> scalestack;

	float yawAngle = 0;
	float pitchAngle = 0;
	float uniformScale = 1.0f;

	const float rotationIncrement = PI / 6;

	for (unsigned int i = 0; i < treestring.size(); ++i)
	{
		if (treestring[i] == 'B' || treestring[i] == 'L')
		{
			Matrix4 translation;
			Matrix4::Translation(translation, Vector3(0, 1, 0));

			Matrix4 scale;
			Matrix4::Scale(scale, uniformScale);

			Matrix4 pitch;
			Matrix4::RotationAxis(pitch, Vector3(0, 1, 0), pitchAngle);

			Matrix4 yaw;
			Matrix4::RotationAxis(yaw, Vector3(1, 0, 0), yawAngle);

			if (currentBranch == 0)
			{
				_branches[currentBranch] = Branch(base * pitch * yaw * scale);
			}
			else
			{
				_branches[currentBranch]
					= Branch(parentBranch, _branches[parentBranch], translation * pitch * yaw * scale);
			}

			unsigned int depth = _branches[currentBranch].Depth();
			if (depth + 1 > _maxDepth)
				_maxDepth = depth + 1;

			if (treestring[i] == 'L')
			{
				_leafyBranchesIndices.push_back(currentBranch);
			}
			
			++currentBranch;
		}
		else if (treestring[i] == '[')
		{
			pitchstack.push(pitchAngle);
			yawstack.push(yawAngle);
			scalestack.push(uniformScale);

			yawAngle = 0;
			pitchAngle = 0;
			uniformScale = 1.0f;

			parentBranch = currentBranch - 1;
		}
		else if (treestring[i] == ']')
		{
			parentBranch = _branches[parentBranch].Parent();
			
			pitchAngle = pitchstack.top();
			pitchstack.pop();

			yawAngle = yawstack.top();
			yawstack.pop();

			uniformScale = scalestack.top();
			scalestack.pop();
		}
		else if (treestring[i] == '>')
		{
			pitchAngle += rotationIncrement;
		}
		else if (treestring[i] == '<')
		{
			pitchAngle -= rotationIncrement;
		}
		else if (treestring[i] == '^')
		{
			yawAngle += rotationIncrement;
		}
		else if (treestring[i] == 'v')
		{
			yawAngle -= rotationIncrement;
		}
		else if (treestring[i] == '+')
		{
			uniformScale += 0.1f;
		}
		else if (treestring[i] == '-')
		{
			uniformScale -= 0.1f;
		}
	}
	
	_leaves.reserve(numLeaves);

	if (_leafyBranchesIndices.size() != 0)
	{
		_leaves.clear();
		_leaves.reserve(numLeaves);

		for (unsigned int i = 0; i < numLeaves; ++i)
		{
			// pick a random parent branch from the set of branches above the leafy branch depth
			int parentBranch = rand() % _leafyBranchesIndices.size();

			_leaves.push_back(Leaf(_branches[_leafyBranchesIndices[parentBranch]]));
		}
	}
}

void TreeBuilder::PackBranchBuffer()
{
	_packedBranchInstances.resize(_branches.size() * 13);

	for (unsigned int i = 0; i < _branches.size(); ++i)
	{
		_branches[i].PackBranch(&_packedBranchInstances[i*13]);
	}
}

void TreeBuilder::PackLeafBuffer()
{
	_packedLeafInstances.resize(_leaves.size() * 12);

	for (unsigned int i = 0; i < _leaves.size(); ++i)
	{
		_leaves[i].PackLeaf(&_packedLeafInstances[i*12]);
	}
}


TreeBuilder::Branch::Branch() :
	_parent(-1),
	_depth(0)
{
}

TreeBuilder::Branch::Branch(const Matrix4& trunkMatrix) :
	_matrix(trunkMatrix),	
	_parent(-1),
	_depth(0)
{

}

TreeBuilder::Branch::Branch(int parentID, const Branch& parent, const Matrix4& matrix) :
	_matrix(parent._matrix * matrix),
	_parent(parentID),
	_depth(parent._depth + 1)
{
}

void TreeBuilder::Branch::PackBranch(float* out) const
{
	for (unsigned int row = 0; row < 3; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			out[row * 4 + col] = _matrix.cell(col, row);
		}
	}
	out[12] = (float)_depth;
}

int TreeBuilder::Branch::Parent() const
{
	return _parent;
}

unsigned int TreeBuilder::Branch::Depth() const
{
	return _depth;
}

void TreeBuilder::Branch::MultiplyMatrix(Vector4& inout) const
{
	inout = _matrix * inout;
}

TreeBuilder::Leaf::Leaf()
{
	Matrix4::Identity(_matrix);
}

TreeBuilder::Leaf::Leaf(const Branch& parent)
{
	float yPos = rand() / (float)RAND_MAX;
	float orientation = rand() / (float)RAND_MAX;
	float yawNoise = 0.4f * rand() / (float)RAND_MAX;

	Matrix4::Identity(_matrix);

	Matrix4 scale; 
	Matrix4::Scale(scale, 0.3f);

	Matrix4 pitch;
	Matrix4::RotationAxis(pitch, Vector3(0, 1, 0), orientation * PI * 2);

	Vector4 pos (0.07f, yPos, 0.0f, 1);

	pitch.Transform(pos);

	parent.MultiplyMatrix(pos);

	Matrix4 yaw;
	Matrix4::RotationAxis(yaw, Vector3(0, 0, 1), 1.0f + yawNoise);

	Matrix4::Translation(_matrix, Vector3(pos.x(), pos.y(), pos.z()));

	_matrix *= pitch;
	_matrix *= yaw;
	_matrix *= scale;
}

void TreeBuilder::Leaf::PackLeaf(float* out) const
{
	for (unsigned int row = 0; row < 3; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			out[row * 4 + col] = _matrix.cell(col, row);
		}
	}
}