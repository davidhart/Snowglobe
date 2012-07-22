// David Hart - 2012
#include "TreeBuilder.h"

#include <stack>
#include <cassert>

Branch::Branch() :
	_parent(-1),
	_depth(0)
{
}

Branch::Branch(const Matrix4& trunkMatrix) :
	_matrix(trunkMatrix),	
	_parent(-1),
	_depth(0)
{

}

Branch::Branch(int parentID, const Branch& parent, const Matrix4& matrix) :
	_matrix(parent._matrix * matrix),
	_parent(parentID),
	_depth(parent._depth + 1)
{
}

void Branch::PackBranch(float* out) const
{
	// Pack 3x4 matrix
	for (unsigned int row = 0; row < 3; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			out[row * 4 + col] = _matrix.cell(col, row);
		}
	}
	// Pack branch depth
	out[12] = (float)_depth;
}

int Branch::Parent() const
{
	return _parent;
}

unsigned int Branch::Depth() const
{
	return _depth;
}

void Branch::MultiplyMatrix(Vector4& inout) const
{
	inout = _matrix * inout;
}

Leaf::Leaf()
{
	Matrix4::Identity(_matrix);
}

// Construct a leaf at a random location with a random orientation along
// the provided parent branch
Leaf::Leaf(const Branch& parent)
{
	// Calculate a random position along the branch between 0 and 1
	float yPos = rand() / (float)RAND_MAX;

	// Calculate a random rotation around the branch between 0 and 2Pi
	float orientation = PI * 2 * rand() / (float)RAND_MAX;

	// Calculate a random amount to tilt the leaf away from the branch between 0 and 1
	float yawNoise = rand() / (float)RAND_MAX;

	Matrix4::Identity(_matrix);

	Matrix4 pitch;
	Matrix4::RotationAxis(pitch, Vector3(0, 1, 0), orientation);

	// Offset the leaf along the branch
	Vector4 pos (0.07f, yPos, 0.0f, 1);
	pitch.Transform(pos);
	parent.MultiplyMatrix(pos);
	Matrix4::Translation(_matrix, Vector3(pos.x(), pos.y(), pos.z()));

	// Rotate leaf around branch
	_matrix *= pitch; 

	 // Rotate leaf away from branch
	Matrix4 yaw;
	Matrix4::RotationAxis(yaw, Vector3(0, 0, 1), 1.0f + 0.4f * yawNoise);
	_matrix *= yaw;

	 // Scale leaf
	Matrix4 scale; 
	Matrix4::Scale(scale, 0.3f);
	_matrix *= scale;
}

void Leaf::PackLeaf(float* out) const
{
	// Pack 3x4 matrix
	for (unsigned int row = 0; row < 3; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			out[row * 4 + col] = _matrix.cell(col, row);
		}
	}
}

TreeBuilder::TreeBuilder() :
	_maxDepth(0),
	_treesBuilt(0)
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
	// Cycle through tree patterns
	TreePattern& pattern = *_patterns[_treesBuilt];

	if (++_treesBuilt >= _patterns.size())
	{
		_treesBuilt = 0;
	}

	// Evaluate l-system for pattern
	std::string treestring;
	pattern._lsystem.EvaluateRules(pattern._seed, treestring, pattern._iterations);

	// Translate tree string into tree structure
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

const float* TreeBuilder::GetPackedLeafBuffer() const
{
	return &(_packedLeafInstances[0]);
}

unsigned int TreeBuilder::GetPackedLeafBufferSize() const
{
	return _packedLeafInstances.size() * sizeof(float);
}

const float* TreeBuilder::GetPackedBranchBuffer() const
{
	return &(_packedBranchInstances[0]);
}

unsigned int TreeBuilder::GetPackedBranchBufferSize() const
{
	return _packedBranchInstances.size() * sizeof(float);
}

unsigned int TreeBuilder::GetNumLeaves() const
{
	return _leaves.size();
}

unsigned int TreeBuilder::GetNumBranches() const
{
	return _branches.size();
}

unsigned int TreeBuilder::GetMaxDepth() const
{
	return _maxDepth;
}

void TreeBuilder::ParseTree(const std::string& treestring, unsigned int numLeaves)
{
	_maxDepth = 1;

	unsigned int branchCount = 0;
	unsigned int leafyBranchCount = 0;

	// Pre process the string, calculate total number of branches/leafy branches to
	// avoid repeat dynamic memory allocation
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

	int parentBranch = 0;
	int currentBranch = 0;

	std::stack<float> yawstack;
	std::stack<float> pitchstack;
	std::stack<float> scalestack;

	float yawAngle = 0;
	float pitchAngle = 0;
	float uniformScale = 1.0f;
	Matrix4 base;
	Matrix4::Scale(base, Vector3(1.3f, 1, 1.3f));

	const float rotationIncrement = PI / 6;

	for (unsigned int i = 0; i < treestring.size(); ++i)
	{
		// Emit a branch at the current location
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
			
			// If this is the first branch emit the default branch
			if (currentBranch == 0)
			{
				_branches[currentBranch] = Branch(base * pitch * yaw * scale);
			}
			// Attach branches to their parents
			else
			{
				_branches[currentBranch]
					= Branch(parentBranch, _branches[parentBranch], translation * pitch * yaw * scale);
			}

			// Record the maximum branch depth
			unsigned int depth = _branches[currentBranch].Depth();
			if (depth + 1 > _maxDepth)
				_maxDepth = depth + 1;

			// Add the new branch to the list of branches that will receive leaves
			if (treestring[i] == 'L')
			{
				_leafyBranchesIndices.push_back(currentBranch);
			}
			
			++currentBranch;
		}
		// Push current state onto stack
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
		// Pop current state from stack
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
		// Incrememnt pitch
		else if (treestring[i] == '>')
		{
			pitchAngle += rotationIncrement;
		}
		// Decrement pitch
		else if (treestring[i] == '<')
		{
			pitchAngle -= rotationIncrement;
		}
		// Increment yaw
		else if (treestring[i] == '^')
		{
			yawAngle += rotationIncrement;
		}
		// Decrement yaw
		else if (treestring[i] == 'v')
		{
			yawAngle -= rotationIncrement;
		}
		// Increment Scale
		else if (treestring[i] == '+')
		{
			uniformScale += 0.1f;
		}
		// DecrementScale
		else if (treestring[i] == '-')
		{
			uniformScale -= 0.1f;
		}
	}
	
	// Generate and attach leaves to branches
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
