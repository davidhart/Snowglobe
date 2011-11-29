#include "Util.h"
#include "ObjFile.h"
#include "VertexBuffer.h"

#include <fstream>
#include <cassert>

void Util::ReadTextFileToString(const std::string& filename, std::string& text)
{
	std::ifstream file(filename);
	assert(file.is_open());

	text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

bool Util::FloatEquality(float a, float b, float precision)
{
	float difference = a - b;
	return difference < precision && difference > -precision;
}

void Util::CreateObjFileWithBuffers(const char* filename,
									const Renderer& renderer,
									ObjFile& objFile,
									VertexBuffer& vertexBuffer,
									VertexBuffer& indexBuffer)
{
	objFile.Read(filename);
	vertexBuffer.Create(renderer, objFile.GetVertexData(), objFile.GetNumVertices() * objFile.GetVertexStride());
	indexBuffer.Create(renderer, objFile.GetIndexData(), objFile.GetNumIndices() * sizeof(unsigned int));
}