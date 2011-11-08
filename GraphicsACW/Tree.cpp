#include "Tree.h"
#include "Util.h"
#include "Renderer.h"
#include <stack>

Tree::Tree()
{

}

void Tree::Create(const Renderer& renderer, const std::string& treestring)
{
	_cylinderFile.Read("cylinder.obj");
	_cylinderBuffer.Create(renderer, _cylinderFile.GetVertexData(), _cylinderFile.GetNumVertices() * _cylinderFile.GetVertexStride());
	_cylinderIndices.Create(renderer, _cylinderFile.GetIndexData(), _cylinderFile.GetNumIndices() * sizeof(unsigned int));

	std::string shadersource;
	Util::ReadTextFileToString("tree.vsh", shadersource);
	_vertShader.Create(renderer, shadersource.c_str());

	Util::ReadTextFileToString("tree.fsh", shadersource);
	_fragShader.Create(renderer, shadersource.c_str());

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	assert(_cylinderFile.HasTextureCoordinates());
	assert(_cylinderFile.HasVertexNormals());

	unsigned int stride = _cylinderFile.GetVertexStride();

	ArrayElement vertexLayout[] =
	{
		{ _cylinderBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _cylinderFile.GetTexCoordOffset() },
		{ _cylinderBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _cylinderFile.GetNormalOffset() },
		{ _cylinderBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _cylinderFile.GetVertexOffset() },
	};

	_vertBinding.Create(renderer, vertexLayout, 3, _cylinderIndices, AE_UINT);

	ParseTree(treestring);

}

void Tree::Dispose()
{
	_vertBinding.Dispose();
	_cylinderBuffer.Dispose();
	_cylinderIndices.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
}

void Tree::Draw(const Renderer& renderer)
{
	_shaderProgram.Use();
	renderer.UpdateViewProjectionUniforms(_shaderProgram);

	for (unsigned int i = 0; i < _branches.size(); ++i)
	{
		_shaderProgram.SetUniform("model", _branches[i].GetMatrix());
		renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _cylinderFile.GetNumIndices());
	}
}

void Tree::ParseTree(const std::string& treestring)
{
	unsigned int branchCount = 1;
	for (unsigned int i = 0; i < treestring.size(); ++i)
	{
		if (treestring[i] == 'B')
		{
			++branchCount;
		}
	}

	_branches.resize(branchCount);

	Matrix4 translate;
	Matrix4::Translation(translate, Vector3(1, -3, 3));
	Matrix4 scale;
	Matrix4::Scale(scale, Vector3(0.5, 1.5, 0.5));

	_branches[0] = Branch(-1, 0, translate * scale);

	int parentBranch = 0;
	int currentBranch = 1;
	int depth = 1;

	Matrix4 yaw;
	Matrix4::RotationAxis(yaw, Vector3(1, 0, 0), 0.8f);

	std::stack<float> pitchangle;
	pitchangle.push(0);

	for (unsigned int i = 0; i < treestring.size(); ++i)
	{
		if (treestring[i] == 'B')
		{
			Matrix4 translation;
			Matrix4::Translation(translation, Vector3(0, 1, 0));

			Matrix4 scale;
			Matrix4::Scale(scale, 0.8f);

			Matrix4 pitch;
			Matrix4::RotationAxis(pitch, Vector3(0, 1, 0), pitchangle.top());

			_branches[currentBranch]
				= Branch(parentBranch, depth, _branches[parentBranch].GetMatrix() * translation * scale * pitch * yaw);
			
			++currentBranch;
		}
		else if (treestring[i] == '[')
		{
			pitchangle.push(0);
			parentBranch = currentBranch - 1;
			++depth;
		}
		else if (treestring[i] == ']')
		{
			--depth;
			parentBranch = _branches[parentBranch].ParentBranch();
			pitchangle.pop();
		}
		else if (treestring[i] == '>')
		{
			pitchangle.top() += 0.8f;
		}
	}
}

Tree::Branch::Branch() :
	_parent(-1),
	_depth(0)
{

}

Tree::Branch::Branch(int parent, unsigned int depth, const Matrix4& matrix) :
	_parent(parent),
	_depth(depth),
	_matrix(matrix)
{
}

const Matrix4& Tree::Branch::GetMatrix() const
{
	return _matrix;
}

int Tree::Branch::ParentBranch() const
{
	return _parent;
}