#include "Tree.h"
#include "Util.h"
#include "Renderer.h"
#include "Maths.h"
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

	ParseTree(treestring);

	CreateBranchInstanceBuffer(renderer);

	ArrayElement vertexLayout[] =
	{
		ArrayElement(_cylinderBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _cylinderFile.GetTexCoordOffset(), 0),
		ArrayElement(_cylinderBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _cylinderFile.GetNormalOffset(), 0),
		ArrayElement(_cylinderBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _cylinderFile.GetVertexOffset(), 0),
	
		ArrayElement(_branchInstanceBuffer, _shaderProgram.GetAttributeIndex("in_modelRow0"), 4, AE_FLOAT, 12 * sizeof(float), 0, 1),
		ArrayElement(_branchInstanceBuffer, _shaderProgram.GetAttributeIndex("in_modelRow1"), 4, AE_FLOAT, 12 * sizeof(float), 4 * sizeof(float), 1),
		ArrayElement(_branchInstanceBuffer, _shaderProgram.GetAttributeIndex("in_modelRow2"), 4, AE_FLOAT, 12 * sizeof(float), 8 * sizeof(float), 1),
	};

	_vertBinding.Create(renderer, vertexLayout, 6, _cylinderIndices, AE_UINT);

}

void Tree::Dispose()
{
	_vertBinding.Dispose();
	_cylinderBuffer.Dispose();
	_cylinderIndices.Dispose();
	_branchInstanceBuffer.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
}

void Tree::Draw(const Renderer& renderer)
{
	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram);

	Matrix4 translate;
	Matrix4::Translation(translate, Vector3(2, 0, 0.0f));
	Matrix4 scale;
	Matrix4::Scale(scale, Vector3(0.8f, 1.6f, 0.8f));

	_shaderProgram.SetUniform("model", translate * scale);

	renderer.DrawInstances(_vertBinding, PT_TRIANGLES, 0, _cylinderFile.GetNumIndices(), _branches.size());
}

void Tree::DrawReflection(const Renderer& renderer)
{
	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1, -1, 1));

	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram);

	Matrix4 translate;
	Matrix4::Translation(translate, Vector3(2, 0, 0.0f));
	Matrix4 scale;
	Matrix4::Scale(scale, Vector3(0.8f, -1.6f, 0.8f));

	_shaderProgram.SetUniform("model", translate * scale);

	renderer.DrawInstances(_vertBinding, PT_TRIANGLES, 0, _cylinderFile.GetNumIndices(), _branches.size());
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

	Matrix4 identity;
	Matrix4::Identity(identity);

	_branches[0] = Branch(-1, 0, identity);

	int parentBranch = 0;
	int currentBranch = 1;
	int depth = 1;

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
				= Branch(parentBranch, depth, _branches[parentBranch].GetMatrix() * translation * pitch * yaw * scale);
			
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
			++depth;
		}
		else if (treestring[i] == ']')
		{
			--depth;
			parentBranch = _branches[parentBranch].ParentBranch();
			
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
}

void Tree::CreateBranchInstanceBuffer(const Renderer& renderer)
{
	std::vector<float> packed3x4Matrices(_branches.size() * 12);

	for (unsigned int i = 0; i < _branches.size(); ++i)
	{
		const Matrix4& t = _branches[i].GetMatrix();

		for (unsigned int row = 0; row < 3; row++)
		{
			for (unsigned int col = 0; col < 4; col++)
			{
				packed3x4Matrices[i*12 + row * 4 + col] = t.cell(col, row);
			}
		}
	}

	_branchInstanceBuffer.Create(renderer, &packed3x4Matrices[0], sizeof(float) * packed3x4Matrices.size());
}

Tree::Branch::Branch() :
	_parent(-1),
	_depth(0)
{

}

Tree::Branch::Branch(int parent, unsigned int depth, const Matrix4& matrix) :
	_matrix(matrix),
	_parent(parent),
	_depth(depth)
{
}

Matrix4 Tree::Branch::GetMatrix() const
{
	return _matrix;
}

int Tree::Branch::ParentBranch() const
{
	return _parent;
}