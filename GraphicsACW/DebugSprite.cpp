#include "DebugSprite.h"
#include "Util.h"
#include "Texture.h"

const unsigned VERT_STRIDE = sizeof(float)*5;
const unsigned VERT_BUFFER_SIZE = VERT_STRIDE * 4;

void DebugSprite::Create(const Renderer& renderer)
{
	_vertShader.CreateFromFile(renderer, "assets/textured_sprite.vsh");
	_fragShader.CreateFromFile(renderer, "assets/textured_sprite.fsh");

	_shader.Create(renderer, _vertShader, _fragShader);
	_shader.Use();

	Uniform texture = _shader.GetUniform("diffuseMap");
	_shader.SetUniform(texture, 0);

	_projectionUniform = _shader.GetUniform("projection");

	_vertBuffer.Create(renderer, NULL, VERT_BUFFER_SIZE);

	unsigned indices[] = { 0, 1, 2, 0, 2, 3 };

	_indexBuffer.Create(renderer, indices, sizeof(indices));

	ArrayElement vertexLayout[] =
	{
		ArrayElement(_vertBuffer, "in_tex", 2, AE_FLOAT, VERT_STRIDE, sizeof(float)*3),
		ArrayElement(_vertBuffer, "in_vertex", 3, AE_FLOAT, VERT_STRIDE, 0),
	};

	_vertBinding.Create(renderer, _shader, vertexLayout, 2, _indexBuffer, AE_UINT);
}

void DebugSprite::Dispose()
{
	_vertBinding.Dispose();
	_indexBuffer.Dispose();
	_vertBuffer.Dispose();

	_shader.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
}

void DebugSprite::Draw(const Renderer& renderer, float x, float y, float w, float h, Texture& texture)
{
	float data[5 * 4] = { x, y, 0.0f,			0.0f, 0.0f,
						  x + w, y, 0.0f,		1.0f, 0.0f,
						  x + w, y + h, 0.0f,	1.0f, 1.0f,
						  x, y + h, 0.0f,		0.0f, 1.0f };

	_vertBuffer.UpdateRegion(0, data, sizeof(data)); 

	_shader.Use();

	_shader.SetUniform(_projectionUniform, _projectionMatrix);

	texture.Bind(0, true);

	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, 6);
}

void DebugSprite::WindowSize(unsigned width, unsigned height)
{
	Matrix4::ProjectionOrtho(_projectionMatrix, 0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);
}