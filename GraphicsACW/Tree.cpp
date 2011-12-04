#include "Tree.h"
#include "Util.h"
#include "Renderer.h"
#include "Maths.h"
#include "Util.h"
#include <stack>

Tree::Tree() : 
	_maxDepth(0),
	_drawDepth(0),
	_currentBranchProgram(&_branchTexturedLitProgram),
	_branchCurrentUniforms(&_branchTexturedLitUniforms),
	_currentLeafProgram(&_leafProgram),
	_leafCurrentUniforms(&_leafStandardUniforms),
	_currentDrawMode(DRAW_TEXTURED_LIT)
{

}

void Tree::Create(const Renderer& renderer, const std::string& treestring, unsigned int leafDepth, unsigned int numLeaves)
{
	assert(numLeaves > 0);
	ParseTree(treestring, leafDepth, numLeaves);

	CreateBranches(renderer);
	CreateLeaves(renderer);
}

void Tree::CreateBranches(const Renderer& renderer)
{
	Util::CreateObjFileWithBuffers("cylinder.obj", renderer, _branchModel, _branchBuffer, _branchIndices);
	assert(_branchModel.HasTextureCoordinates());
	assert(_branchModel.HasVertexNormals());

	_branchVert.CreateFromFile(renderer, "tree.vsh");
	_branchVertFlat.CreateFromFile(renderer, "tree_flat.vsh");

	_branchFlatShadedFrag.CreateFromFile(renderer, "lit_flat.fsh");
	_branchTexturedLitFrag.CreateFromFile(renderer, "textured_lit.fsh");
	_branchUnlitFrag.CreateFromFile(renderer, "unlit.fsh");

	_branchTexturedLitProgram.Create(renderer, _branchVert, _branchTexturedLitFrag);
	_branchFlatProgram.Create(renderer, _branchVertFlat, _branchFlatShadedFrag);
	_branchUnlitProgram.Create(renderer, _branchVert, _branchUnlitFrag);

	renderer.GetStandardUniforms(_branchTexturedLitProgram, _branchTexturedLitUniforms);
	renderer.GetStandardUniforms(_branchFlatProgram, _branchFlatUniforms);
	renderer.GetStandardUniforms(_branchUnlitProgram, _branchUnlitUniforms);

	Uniform diffuseMap = _branchTexturedLitProgram.GetUniform("diffuseMap");
	_uniformDrawDepth = _branchTexturedLitProgram.GetUniform("drawDepth");

	_branchTexturedLitProgram.Use();
	_branchTexturedLitProgram.SetUniform(diffuseMap, 0);

	diffuseMap = _branchUnlitProgram.GetUniform("diffuseMap");
	_branchUnlitProgram.Use();
	_branchUnlitProgram.SetUniform(diffuseMap, 0);

	CreateBranchInstanceBuffer(renderer);

	unsigned int stride = _branchModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_branchBuffer, _branchTexturedLitProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _branchModel.GetTexCoordOffset(), 0),
		ArrayElement(_branchBuffer, _branchTexturedLitProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _branchModel.GetNormalOffset(), 0),
		ArrayElement(_branchBuffer, _branchTexturedLitProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _branchModel.GetVertexOffset(), 0),
	
		ArrayElement(_branchInstanceBuffer, _branchTexturedLitProgram.GetAttributeIndex("in_modelRow0"), 4, AE_FLOAT, 13 * sizeof(float), 0, 1),
		ArrayElement(_branchInstanceBuffer, _branchTexturedLitProgram.GetAttributeIndex("in_modelRow1"), 4, AE_FLOAT, 13 * sizeof(float), 4 * sizeof(float), 1),
		ArrayElement(_branchInstanceBuffer, _branchTexturedLitProgram.GetAttributeIndex("in_modelRow2"), 4, AE_FLOAT, 13 * sizeof(float), 8 * sizeof(float), 1),
		ArrayElement(_branchInstanceBuffer, _branchTexturedLitProgram.GetAttributeIndex("in_branchDepth"), 1, AE_FLOAT, 13 * sizeof(float), 12 * sizeof(float), 1),
	};

	_branchBinding.Create(renderer, vertexLayout, 7, _branchIndices, AE_UINT);
	_barkTexture.Create(renderer, "tree_bark.jpg");

	_textureOff.Create(renderer, "white.tga");
}

void Tree::CreateLeaves(const Renderer& renderer)
{
	Util::CreateObjFileWithBuffers("leaf.obj", renderer, _leafModel, _leafBuffer, _leafIndices);
	assert(_leafModel.HasTextureCoordinates());
	assert(_leafModel.HasVertexNormals());

	_leafVertShader.CreateFromFile(renderer, "leaf.vsh");
	_leafFragShader.CreateFromFile(renderer, "leaf.fsh");
	_leafFragUnlit.CreateFromFile(renderer, "unlit.fsh");

	_leafProgram.Create(renderer, _leafVertShader, _leafFragShader);
	_leafProgramUnlit.Create(renderer, _leafVertShader, _leafFragUnlit);

	renderer.GetStandardUniforms(_leafProgram, _leafStandardUniforms);
	renderer.GetStandardUniforms(_leafProgramUnlit, _leafUnlitUniforms);

	Uniform diffuseMap = _leafProgram.GetUniform("diffuseMap");
	Uniform gradientMap = _leafProgram.GetUniform("gradientMap");
	_uniformColorLookup = _leafProgram.GetUniform("colorLookup");

	_leafProgram.Use();
	_leafProgram.SetUniform(diffuseMap, 0);
	_leafProgram.SetUniform(gradientMap, 1);

	CreateLeafInstanceBuffer(renderer);

	unsigned int stride = _branchModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_leafBuffer, _leafProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _leafModel.GetTexCoordOffset(), 0),
		ArrayElement(_leafBuffer, _leafProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _leafModel.GetNormalOffset(), 0),
		ArrayElement(_leafBuffer, _leafProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _leafModel.GetVertexOffset(), 0),
	
		ArrayElement(_leafInstanceBuffer, _leafProgram.GetAttributeIndex("in_modelRow0"), 4, AE_FLOAT, 12 * sizeof(float), 0, 1),
		ArrayElement(_leafInstanceBuffer, _leafProgram.GetAttributeIndex("in_modelRow1"), 4, AE_FLOAT, 12 * sizeof(float), 4 * sizeof(float), 1),
		ArrayElement(_leafInstanceBuffer, _leafProgram.GetAttributeIndex("in_modelRow2"), 4, AE_FLOAT, 12 * sizeof(float), 8 * sizeof(float), 1),
	};

	_leafBinding.Create(renderer, vertexLayout, 6, _leafIndices, AE_UINT);
	_leafTexture.Create(renderer, "leaf.tga", T_CLAMP_EDGE);
	_leafGradient.Create(renderer, "leaf_gradient.tga");
}

void Tree::Dispose()
{
	_branchBinding.Dispose();
	_branchBuffer.Dispose();
	_branchIndices.Dispose();
	_branchInstanceBuffer.Dispose();
	_branchTexturedLitProgram.Dispose();
	_branchVert.Dispose();
	_branchTexturedLitFrag.Dispose();
	_branchFlatProgram.Dispose();
	_branchFlatShadedFrag.Dispose();
	_branchVertFlat.Dispose();
	_branchUnlitProgram.Dispose();
	_branchUnlitFrag.Dispose();

	_barkTexture.Dispose();
	_textureOff.Dispose();

	_leafBinding.Dispose();
	_leafBuffer.Dispose();
	_leafIndices.Dispose();
	_leafInstanceBuffer.Dispose();
	_leafProgram.Dispose();
	_leafProgramUnlit.Dispose();
	_leafFragShader.Dispose();
	_leafFragUnlit.Dispose();
	_leafVertShader.Dispose();
	_leafTexture.Dispose();
	_leafGradient.Dispose();
}

void Tree::Grow(float elapsed)
{
	_drawDepth += elapsed;
	_drawDepth = Util::Min(_drawDepth, (float)_maxDepth);
}

void Tree::Shrink(float elapsed)
{
	_drawDepth -= elapsed;
	_drawDepth = Util::Max(_drawDepth, 0.0f);
}

void Tree::Draw(const Renderer& renderer)
{
	Matrix4 model;
	ConstructModelMatrix(model);

	if (_currentDrawMode == DRAW_WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	DrawBranches(renderer, model);
	DrawLeaves(renderer, model);

	if (_currentDrawMode == DRAW_WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Tree::DrawReflection(const Renderer& renderer)
{
	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1, -1, 1));

	Matrix4 model;
	ConstructModelMatrix(model);

	if (_currentDrawMode == DRAW_WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	DrawBranches(renderer, model * mirror);
	DrawLeaves(renderer, model * mirror);

	if (_currentDrawMode == DRAW_WIREFRAME)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Tree::DrawBranches(const Renderer& renderer, const Matrix4& model)
{
	if (_currentDrawMode == DRAW_LIT_SMOOTH || _currentDrawMode == DRAW_WIREFRAME)
	{
		_textureOff.Bind();
	}
	else
	{
		_barkTexture.Bind();
	}
	_currentBranchProgram->Use();

	renderer.UpdateStandardUniforms(*_currentBranchProgram, *_branchCurrentUniforms);
	_currentBranchProgram->SetUniform(_branchCurrentUniforms->Model, model);
	_currentBranchProgram->SetUniform(_uniformDrawDepth, _drawDepth);

	renderer.DrawInstances(_branchBinding, PT_TRIANGLES, 0, _branchModel.GetNumIndices(), _branches.size());
}

void Tree::DrawLeaves(const Renderer& renderer, const Matrix4& model)
{
	_currentLeafProgram->Use();

	if (_currentDrawMode == DRAW_LIT_SMOOTH || _currentDrawMode == DRAW_WIREFRAME || _currentDrawMode == DRAW_LIT_FLAT)
	{
		_textureOff.Bind(0);
		_textureOff.Bind(1);
	}
	else
	{
		_leafTexture.Bind(0);
		_leafGradient.Bind(1);
		_currentLeafProgram->SetUniform(_uniformColorLookup, 0.0f);
	}

	renderer.UpdateStandardUniforms(*_currentLeafProgram, *_leafCurrentUniforms);
	_currentLeafProgram->SetUniform(_leafCurrentUniforms->Model, model);

	glDisable(GL_CULL_FACE);
	renderer.DrawInstances(_leafBinding, PT_TRIANGLES, 0, _leafModel.GetNumIndices(), _leaves.size());
	glEnable(GL_CULL_FACE);
}

unsigned int Tree::MaxBranchDepth() const
{
	return _maxDepth;
}

void Tree::NextDrawMode()
{
	_currentDrawMode = (eDrawMode)((int)_currentDrawMode + 1);

	if (_currentDrawMode > 3)
		_currentDrawMode = DRAW_TEXTURED_LIT;

	if (_currentDrawMode == DRAW_TEXTURED_LIT || _currentDrawMode == DRAW_LIT_SMOOTH)
	{
		_currentBranchProgram = &_branchTexturedLitProgram;
		_branchCurrentUniforms = &_branchTexturedLitUniforms;
	}
	else if (_currentDrawMode == DRAW_LIT_FLAT)
	{
		_currentBranchProgram = &_branchFlatProgram;
		_branchCurrentUniforms = &_branchFlatUniforms;
	}
	else if (_currentDrawMode == DRAW_WIREFRAME)
	{
		_currentBranchProgram = &_branchUnlitProgram;
		_branchCurrentUniforms = &_branchUnlitUniforms;
	}

	if (_currentDrawMode == DRAW_WIREFRAME)
	{
		_currentLeafProgram = &_leafProgramUnlit;
		_leafCurrentUniforms = &_leafUnlitUniforms;
	}
	else
	{
		_currentLeafProgram = &_leafProgram;
		_leafCurrentUniforms = &_leafStandardUniforms;
	}

	_uniformDrawDepth = _currentBranchProgram->GetUniform("drawDepth");
}

void Tree::ConstructModelMatrix(Matrix4& out)
{
	float growfraction = 0.8f * _drawDepth / (float)_maxDepth + 0.2f;

	Matrix4 translate;
	Matrix4::Translation(translate, Vector3(2, 0, 0.0f));
	Matrix4 scale;
	Matrix4::Scale(scale, Vector3(0.9f, 1.6f, 0.9f) * growfraction);

	out = translate * scale;
}

void Tree::ParseTree(const std::string& treestring, unsigned int leafDepth, unsigned int numLeaves)
{
	_maxDepth = 1;

	unsigned int branchCount = 1;
	for (unsigned int i = 0; i < treestring.size(); ++i)
	{
		if (treestring[i] == 'B')
		{
			++branchCount;
		}
	}

	_branches.resize(branchCount);
	_leafyBranchesIndices.reserve(branchCount);

	Matrix4 identity;
	//Matrix4::Identity(identity);
	Matrix4::Scale(identity, Vector3(1.3f, 1, 1.3f));
	_branches[0] = Branch(identity);

	int parentBranch = 0;
	int currentBranch = 1;

	std::stack<float> yawstack;
	std::stack<float> pitchstack;
	std::stack<float> scalestack;

	float yawAngle = 0;
	float pitchAngle = 0;
	float uniformScale = 1.0f;

	const float rotationIncrement = PI / 6;

	for (unsigned int i = 0; i < treestring.size(); ++i)
	{
		if (treestring[i] == 'B')
		{
			Matrix4 translation;
			Matrix4::Translation(translation, Vector3(0, 1, 0));

			Matrix4 scale;
			Matrix4::Scale(scale, uniformScale);

			Matrix4 pitch;
			Matrix4::RotationAxis(pitch, Vector3(0, 1, 0), pitchAngle);

			Matrix4 yaw;
			Matrix4::RotationAxis(yaw, Vector3(1, 0, 0), yawAngle);

			_branches[currentBranch]
				= Branch(parentBranch, _branches[parentBranch], translation * pitch * yaw * scale);

			unsigned int depth = _branches[currentBranch].Depth();
			if (depth + 1 > _maxDepth)
				_maxDepth = depth + 1;

			if (depth >= leafDepth)
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

	_drawDepth = (float)_maxDepth;
	
	_leaves.reserve(numLeaves);

	for (unsigned int i = 0; i < numLeaves; ++i)
	{
		// pick a random parent branch from the set of branches above the leafy branch depth
		int parentBranch = rand() % _leafyBranchesIndices.size();

		//_leaves.push_back(Leaf(_branches[1]));
		_leaves.push_back(Leaf(_branches[_leafyBranchesIndices[parentBranch]]));
	}
}

void Tree::CreateBranchInstanceBuffer(const Renderer& renderer)
{
	std::vector<float> packedBranchInstances(_branches.size() * 13);

	for (unsigned int i = 0; i < _branches.size(); ++i)
	{
		_branches[i].PackBranch(&packedBranchInstances[i*13]);
	}

	_branchInstanceBuffer.Create(renderer, &packedBranchInstances[0], sizeof(float) * packedBranchInstances.size());
}

void Tree::CreateLeafInstanceBuffer(const Renderer& renderer)
{
	std::vector<float> packedLeafInstances(_leaves.size() * 12);

	for (unsigned int i = 0; i < _leaves.size(); ++i)
	{
		_leaves[i].PackLeaf(&packedLeafInstances[i*12]);
	}

	_leafInstanceBuffer.Create(renderer, &packedLeafInstances[0], sizeof(float) * packedLeafInstances.size());
}

Tree::Branch::Branch() :
	_parent(-1),
	_depth(0)
{
}

Tree::Branch::Branch(const Matrix4& trunkMatrix) :
	_matrix(trunkMatrix),	
	_parent(-1),
	_depth(0)
{

}

Tree::Branch::Branch(int parentID, const Branch& parent, const Matrix4& matrix) :
	_matrix(parent._matrix * matrix),
	_parent(parentID),
	_depth(parent._depth + 1)
{
}

void Tree::Branch::PackBranch(float* out) const
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

int Tree::Branch::Parent() const
{
	return _parent;
}

unsigned int Tree::Branch::Depth() const
{
	return _depth;
}

void Tree::Branch::MultiplyMatrix(Vector4& inout) const
{
	inout = _matrix * inout;
}

Tree::Leaf::Leaf()
{
	Matrix4::Identity(_matrix);
}

Tree::Leaf::Leaf(const Branch& parent)
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

void Tree::Leaf::PackLeaf(float* out) const
{
	for (unsigned int row = 0; row < 3; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			out[row * 4 + col] = _matrix.cell(col, row);
		}
	}
}