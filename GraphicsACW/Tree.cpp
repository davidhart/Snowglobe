// David Hart - 2012

#include "Tree.h"
#include "TreeBuilder.h"
#include "Util.h"
#include "Renderer.h"
#include "Maths.h"
#include "Util.h"
#include <stack>

const float Tree::ANIMATION_LENGTH_LEAF_BROWN = 3.0f;
const float Tree::ANIMATION_LENGTH_LEAF_FALL = 22.0f;
const float Tree::ANIMATION_LENGTH_BRANCH_GROW = 13.0f;
const float Tree::ANIMATION_LENGTH_LEAF_GROW = 9.0f;
const float Tree::ANIMATION_LENGTH_BRANCH_DIE = 15.0f;

Tree::Tree() : 
	_currentBranchProgram(&_branchTexturedLitProgram),
	_branchCurrentUniforms(&_branchTexturedLitUniforms),
	_currentLeafProgram(&_leafProgram),
	_leafCurrentUniforms(&_leafStandardUniforms),
	_currentDrawMode(DRAW_TEXTURED_LIT),
	_currentState(TREE_GROWING_BRANCHES),
	_time(0),
	_maxDepth(0),
	_numBranches(0),
	_numLeaves(0)
{

}

void Tree::CreateAssets(const Renderer& renderer)
{
	CreateBranchAssets(renderer);
	CreateLeafAssets(renderer);

	_textureOff.Create(renderer, "assets/white.tga");

	FetchNonStandardUniforms();
}

void Tree::CreateBranchAssets(const Renderer& renderer)
{
	Util::CreateObjFileWithBuffers("assets/cylinder.obj", renderer, _branchModel, _branchBuffer, _branchIndices);
	assert(_branchModel.HasTextureCoordinates());
	assert(_branchModel.HasVertexNormals());

	_branchVert.CreateFromFile(renderer, "assets/tree.vsh");
	_branchVertFlat.CreateFromFile(renderer, "assets/tree_flat.vsh");

	_branchFlatShadedFrag.CreateFromFile(renderer, "assets/lit_flat.fsh");
	_branchTexturedLitFrag.CreateFromFile(renderer, "assets/textured_lit.fsh");
	_branchUnlitFrag.CreateFromFile(renderer, "assets/unlit.fsh");

	_branchTexturedLitProgram.Create(renderer, _branchVert, _branchTexturedLitFrag);
	_branchFlatProgram.Create(renderer, _branchVertFlat, _branchFlatShadedFrag);
	_branchUnlitProgram.Create(renderer, _branchVert, _branchUnlitFrag);

	renderer.GetStandardUniforms(_branchTexturedLitProgram, _branchTexturedLitUniforms);
	renderer.GetStandardUniforms(_branchFlatProgram, _branchFlatUniforms);
	renderer.GetStandardUniforms(_branchUnlitProgram, _branchUnlitUniforms);

	Uniform diffuseMap = _branchTexturedLitProgram.GetUniform("diffuseMap");
	Uniform matSpecular = _branchTexturedLitProgram.GetUniform("matSpecular");

	_branchTexturedLitProgram.Use();
	_branchTexturedLitProgram.SetUniform(diffuseMap, 0);
	_branchTexturedLitProgram.SetUniform(matSpecular, Vector3(0.1f));

	diffuseMap = _branchUnlitProgram.GetUniform("diffuseMap");
	_branchUnlitProgram.Use();
	_branchUnlitProgram.SetUniform(diffuseMap, 0);

	_barkTexture.Create(renderer, "assets/tree_bark.jpg");
}

void Tree::CreateLeafAssets(const Renderer& renderer)
{
	Util::CreateObjFileWithBuffers("assets/leaf.obj", renderer, _leafModel, _leafBuffer, _leafIndices);
	assert(_leafModel.HasTextureCoordinates());
	assert(_leafModel.HasVertexNormals());

	_leafVertShader.CreateFromFile(renderer, "assets/leaf.vsh");
	_leafFragShader.CreateFromFile(renderer, "assets/leaf.fsh");
	_leafFragUnlit.CreateFromFile(renderer, "assets/unlit.fsh");

	_leafProgram.Create(renderer, _leafVertShader, _leafFragShader);
	_leafProgramUnlit.Create(renderer, _leafVertShader, _leafFragUnlit);

	renderer.GetStandardUniforms(_leafProgram, _leafStandardUniforms);
	renderer.GetStandardUniforms(_leafProgramUnlit, _leafUnlitUniforms);

	Uniform diffuseMap = _leafProgram.GetUniform("diffuseMap");
	Uniform gradientMap = _leafProgram.GetUniform("gradientMap");

	_leafProgram.Use();
	_leafProgram.SetUniform(diffuseMap, 0);
	_leafProgram.SetUniform(gradientMap, 1);

	_leafTexture.Create(renderer, "assets/leaf.tga", T_CLAMP_EDGE);
	_leafGradient.Create(renderer, "assets/leaf_gradient.tga");
}

void Tree::FetchNonStandardUniforms()
{
	_uniformDrawDepth = _currentBranchProgram->GetUniform("drawDepth");

	_uniformColorLookup = _currentLeafProgram->GetUniform("colorLookup");
	_uniformLeafScale = _currentLeafProgram->GetUniform("leafScale");
	_uniformFallTime = _currentLeafProgram->GetUniform("fallTime");
	_uniformFallSpeed = _currentLeafProgram->GetUniform("fallSpeed");
}

void Tree::Dispose()
{
	_textureOff.Dispose();

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

void Tree::Grow()
{
	_currentState = TREE_GROWING_BRANCHES;
	_time = 0;
}

void Tree::Die()
{
	_currentState = TREE_DROPPING_LEAVES;
	_time = 0;
}

void Tree::Update(float dt)
{
	_time += dt;

	if (_currentState == TREE_GROWING_BRANCHES)
	{
		if (_time > ANIMATION_LENGTH_BRANCH_GROW)
		{
			_time -= ANIMATION_LENGTH_BRANCH_GROW;
			_currentState = TREE_GROWING_LEAVES;
		}
	}

	if (_currentState == TREE_GROWING_LEAVES)
	{
		if (_time > ANIMATION_LENGTH_LEAF_GROW)
		{
			_currentState = TREE_ALIVE;
		}
	}

	if (_currentState == TREE_ALIVE)
	{
		_time = 0;
	}

	if (_currentState == TREE_DROPPING_LEAVES)
	{
		if (_time > ANIMATION_LENGTH_LEAF_BROWN + ANIMATION_LENGTH_LEAF_FALL)
		{
			_time -= ANIMATION_LENGTH_LEAF_BROWN + ANIMATION_LENGTH_LEAF_FALL;
			_currentState = TREE_LOSING_BRANCHES;
		}
	}

	if (_currentState == TREE_LOSING_BRANCHES)
	{
		if (_time > ANIMATION_LENGTH_BRANCH_DIE)
		{
			_currentState = TREE_DEAD;
		}
	}

	if (_currentState == TREE_DEAD)
	{
		_time = 0;
	}
}

void Tree::Draw(const Renderer& renderer)
{
	if (_currentState != TREE_DEAD)
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
}

void Tree::DrawReflection(const Renderer& renderer)
{
	if (_currentState != TREE_DEAD)
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


	float drawDepth = GetDrawDepth();

	_currentBranchProgram->SetUniform(_uniformDrawDepth, drawDepth);

	renderer.DrawInstances(_branchBinding, PT_TRIANGLES, 0, _branchModel.GetNumIndices(), _numBranches);
}

void Tree::DrawLeaves(const Renderer& renderer, const Matrix4& model)
{
	if (_currentState != TREE_GROWING_BRANCHES && _currentState != TREE_LOSING_BRANCHES)
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

		_currentLeafProgram->SetUniform(_uniformLeafScale, GetLeafScale());
		_currentLeafProgram->SetUniform(_uniformColorLookup, GetLeafColorLookup());

		_currentLeafProgram->SetUniform(_uniformFallSpeed, 1.0f);
		_currentLeafProgram->SetUniform(_uniformFallTime, GetLeafFallTime());

		renderer.EnableCullFace(false);
		renderer.DrawInstances(_leafBinding, PT_TRIANGLES, 0, _leafModel.GetNumIndices(), _numLeaves);
		renderer.EnableCullFace(true);
	}
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

	FetchNonStandardUniforms();
}

float Tree::GetFireScale() const
{
	if (_currentState != TREE_LOSING_BRANCHES)
		return 0;

	return GetDrawDepth() / (float)_maxDepth;
}

void Tree::ConstructModelMatrix(Matrix4& out) const
{
	float drawscale = 0.8f * GetDrawDepth() / (float)_maxDepth + 0.2f;

	Matrix4 translate;
	Matrix4::Translation(translate, Vector3(2, 0, 0.0f));
	Matrix4 scale;
	Matrix4::Scale(scale, Vector3(0.9f, 1.6f, 0.9f) * drawscale);

	out = translate * scale;
}

float Tree::GetDrawDepth() const
{
	float drawDepth;
	if (_currentState == TREE_GROWING_BRANCHES)
		drawDepth = _time * _maxDepth / ANIMATION_LENGTH_BRANCH_GROW;
	else if (_currentState == TREE_LOSING_BRANCHES)
		drawDepth = (ANIMATION_LENGTH_BRANCH_DIE - _time) * _maxDepth / ANIMATION_LENGTH_BRANCH_DIE;
	else
		drawDepth = (float)_maxDepth;

	return drawDepth;
}

float Tree::GetLeafScale() const
{
	float leafScale;
	if (_currentState == TREE_GROWING_LEAVES)
		leafScale = _time / ANIMATION_LENGTH_LEAF_GROW;
	else
		leafScale = 1;

	return leafScale;
}

float Tree::GetLeafColorLookup() const
{
	float color;
	if (_currentState == TREE_DROPPING_LEAVES)
		color = Util::Min(_time / ANIMATION_LENGTH_LEAF_BROWN, 1.0f);
	else
		color = 0;
	return color;
}

float Tree::GetLeafFallTime() const
{
	if (_currentState == TREE_DROPPING_LEAVES)
		return Util::Max(_time - ANIMATION_LENGTH_LEAF_BROWN, 0.0f);
	else
		return 0;
}

void Tree::CopyTreeBuffers(const Renderer& renderer, const TreeBuilder& treebuilder)
{
	_maxDepth = treebuilder.GetMaxDepth();

	CreateBranchInstanceBuffer(renderer, treebuilder);
	CreateLeafInstanceBuffer(renderer, treebuilder);
}

void Tree::CreateBranchInstanceBuffer(const Renderer& renderer, const TreeBuilder& treebuilder)
{
	_numBranches = treebuilder.GetNumBranches();

	_branchInstanceBuffer.Create(renderer, 
								 treebuilder.GetPackedBranchBuffer(), 
								 treebuilder.GetPackedBranchBufferSize());

	unsigned int stride = _branchModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_branchBuffer, "in_tex", 2, AE_FLOAT, stride, _branchModel.GetTexCoordOffset(), 0),
		ArrayElement(_branchBuffer, "in_normal", 3, AE_FLOAT, stride, _branchModel.GetNormalOffset(), 0),
		ArrayElement(_branchBuffer, "in_vertex", 3, AE_FLOAT, stride, _branchModel.GetVertexOffset(), 0),
	
		ArrayElement(_branchInstanceBuffer, "in_modelRow0", 4, AE_FLOAT, 13 * sizeof(float), 0, 1),
		ArrayElement(_branchInstanceBuffer, "in_modelRow1", 4, AE_FLOAT, 13 * sizeof(float), 4 * sizeof(float), 1),
		ArrayElement(_branchInstanceBuffer, "in_modelRow2", 4, AE_FLOAT, 13 * sizeof(float), 8 * sizeof(float), 1),
		ArrayElement(_branchInstanceBuffer, "in_branchDepth", 1, AE_FLOAT, 13 * sizeof(float), 12 * sizeof(float), 1),
	};

	_branchBinding.Create(renderer, _branchTexturedLitProgram, vertexLayout, 7, _branchIndices, AE_UINT);
}

void Tree::CreateLeafInstanceBuffer(const Renderer& renderer, const TreeBuilder& treebuilder)
{
	_numLeaves = treebuilder.GetNumLeaves();

	_leafInstanceBuffer.Create(renderer, 
							   treebuilder.GetPackedLeafBuffer(), 
							   treebuilder.GetPackedLeafBufferSize());

	unsigned int stride = _branchModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_leafBuffer, "in_tex", 2, AE_FLOAT, stride, _leafModel.GetTexCoordOffset(), 0),
		ArrayElement(_leafBuffer, "in_normal", 3, AE_FLOAT, stride, _leafModel.GetNormalOffset(), 0),
		ArrayElement(_leafBuffer, "in_vertex", 3, AE_FLOAT, stride, _leafModel.GetVertexOffset(), 0),
	
		ArrayElement(_leafInstanceBuffer, "in_modelRow0", 4, AE_FLOAT, 12 * sizeof(float), 0, 1),
		ArrayElement(_leafInstanceBuffer, "in_modelRow1", 4, AE_FLOAT, 12 * sizeof(float), 4 * sizeof(float), 1),
		ArrayElement(_leafInstanceBuffer, "in_modelRow2", 4, AE_FLOAT, 12 * sizeof(float), 8 * sizeof(float), 1),
	};

	_leafBinding.Create(renderer, _leafProgram, vertexLayout, 6, _leafIndices, AE_UINT);
}