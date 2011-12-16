// David Hart - 2011

#include "ObjFile.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>

ObjFile::ObjFile() : 
	_hasNormals(false),
	_hasTextureCoordinates(false)
{
}

void ObjFile::Read(const char* filename)
{
	std::ifstream file(filename);
	assert(file.is_open());

	std::vector<float> vertices;
	std::vector<float> vertNormals;
	std::vector<float> vertTextureCoords;
	std::vector<ObjFormatIndex> indices;
	std::string token;
	bool errorEncountered = false;
	std::string material;

	while (!file.eof())
	{
		file >> token;

		if (!file.fail())
		{
			if (token == "v")
			{
				float x, y, z;
				file >> x >> y>> z;

				if (file.fail())
				{
					errorEncountered = true;

					break;
				}

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
			else if (token == "vt")
			{
				float s, t;
				file >> s >> t;

				if (file.fail())
				{
					errorEncountered = true;

					break;
				}

				vertTextureCoords.push_back(s);
				vertTextureCoords.push_back(t);
			}
			else if (token == "vn")
			{
				float x, y, z;
				file >> x >> y>> z;

				if (file.fail())
				{
					errorEncountered = true;

					break;
				}

				vertNormals.push_back(x);
				vertNormals.push_back(y);
				vertNormals.push_back(z);
			}
			else if (token == "f")
			{
				// extract remainder of line to stringstream
				std::stringstream line;

				char c;

				do
				{
					file.get(c);
					line << c;
				}
				while (file.peek() != '\n' && file.good());

				std::string indexstr;
				int firstindex = indices.size();
				int indexcount = 0;

				while (line.good())
				{
					if (!line.fail())
					{
						ObjFormatIndex newIndex;
						newIndex.Read(line);
						indexcount++;

						// triangle fan -> triangles
						if (indexcount > 3)
						{
							int lastindex = indices.size() - 1;

							// need to add 3 vertices to make triangle, first vertex in poly 
							// + last vertex with new vertex
							indices.push_back(indices[firstindex]);
							indices.push_back(indices[lastindex]);
						}

						indices.push_back(newIndex);
					}
				}
			}/*
			else if (token == "usemtl")
			{
				if (indices.size() > 0)
				{
					FloatVector packedVertices;
					UnsignedIntVector packedIndices;
					int vertexFormat;

					ConvertOBJMesh(vertices,
								   vertNormals,
								   vertTextureCoords,
								   indices,
								   packedVertices,
								   packedIndices,
								   vertexFormat);

					Mesh* mesh = new Mesh(vertexFormat, material);
					mesh->SetVertices(&packedVertices[0], packedVertices.size() * sizeof(float));
					mesh->SetIndices(&packedIndices[0], packedIndices.size() * sizeof(unsigned int));

					m_meshes.push_back(mesh);

					indices.clear();
				}

				file >> material;
			}*/
		}
		else
		{
			file.clear();
		}

		file.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	}

	if (indices.size() > 0)
	{
		_hasNormals = indices[0].vn >= 0;
		_hasTextureCoordinates = indices[1].vt >= 0;
		
		PackModelData(vertices, vertNormals, vertTextureCoords, indices);
	}
}

ObjFile::ObjFormatIndex::ObjFormatIndex() :
	v(-1),
	vt(-1),
	vn(-1)
{
}

ObjFile::CachedObjIndex::CachedObjIndex() : 
	next(-1),
	packedPos(-1)
{
}

void ObjFile::ObjFormatIndex::Read(std::istream& stream)
{
	stream >> v;

	assert(v > 0);

	if (stream.get() != '/' || stream.fail())
	{
		return;
	}

	stream >> vt;

	if (stream.fail())
	{
		stream.clear();
	}

	if (stream.get() != '/' || stream.fail())
	{
		return;
	}

	stream >> vn;

	if (stream.fail())
	{
		stream.clear();
	}
}

void ObjFile::PackVertex(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& texCoords, const ObjFormatIndex& index,
	std::vector<CachedObjIndex>& packedIndicesCache)
{
	unsigned int strideInBytes = GetVertexStride();
	unsigned int strideInFloats = strideInBytes / sizeof(float);

	_vertexData.push_back(vertices[(index.v - 1)*3]);
	_vertexData.push_back(vertices[(index.v - 1)*3 + 1]);
	_vertexData.push_back(vertices[(index.v - 1)*3 + 2]);

	if (_hasTextureCoordinates)
	{
		_vertexData.push_back(texCoords[(index.vt - 1)*2]);
		_vertexData.push_back(texCoords[(index.vt - 1)*2 + 1]);
	}

	if (_hasNormals)
	{
		_vertexData.push_back(normals[(index.vn - 1)*3]);
		_vertexData.push_back(normals[(index.vn - 1)*3 + 1]);
		_vertexData.push_back(normals[(index.vn - 1)*3 + 2]);
	}

	packedIndicesCache[index.v - 1].index = index;

	int packedPos = (_vertexData.size() / strideInFloats) - 1;
	_indexData.push_back(packedPos);
	packedIndicesCache[index.v - 1].packedPos = packedPos;
}

void ObjFile::PackModelData(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& texCoords, const std::vector<ObjFormatIndex>& indices)
{
	unsigned int strideInBytes = GetVertexStride();
	unsigned int strideInFloats = strideInBytes / sizeof(float);

	std::vector<CachedObjIndex> packedIndicesCache(vertices.size());

	for (unsigned int i = 0; i < indices.size(); i++)
	{
		int v = indices[i].v - 1;

		if (packedIndicesCache[v].packedPos == -1)
		{
			PackVertex(vertices, normals, texCoords, indices[i], packedIndicesCache);
		}
		else
		{
			bool found = false;
			int index = indices[i].v - 1;

			while (!found)
			{
				if (((!_hasNormals) || (_hasNormals && packedIndicesCache[index].index.vn == indices[i].vn)) && 
					((!_hasTextureCoordinates) || (_hasTextureCoordinates && packedIndicesCache[index].index.vt == indices[i].vt)))
				{
					_indexData.push_back(packedIndicesCache[index].packedPos);
					found = true;

					break;
				}
				else
				{
					if (packedIndicesCache[index].next == -1)
					{
						break;
					}
					else
					{
						index = packedIndicesCache[index].next;
					}
				}
			}

			if (!found)
			{
				packedIndicesCache.push_back(CachedObjIndex());

				PackVertex(vertices, normals, texCoords, indices[i], packedIndicesCache);

				packedIndicesCache[index].next = packedIndicesCache.size() - 1;
			}
		}
	}
}



const float* ObjFile::GetVertexData()
{
	return &_vertexData[0];
}

unsigned int ObjFile::GetNumVertices() const
{
	unsigned int numVertices = _vertexData.size() / (GetVertexStride() / sizeof(float));
	return numVertices;
}

unsigned int ObjFile::GetVertexStride() const
{
	int stride = 12;

	if (_hasNormals)
		stride += 12;

	if (_hasTextureCoordinates)
		stride += 8;

	return stride;
}

unsigned int ObjFile::GetVertexOffset() const
{
	return 0;
}

unsigned int ObjFile::GetNormalOffset() const
{
	int offset = 12;
	if (_hasTextureCoordinates)
		offset += 8;

	return offset;
}

unsigned int ObjFile::GetTexCoordOffset() const
{
	return 12;
}

const unsigned int* ObjFile::GetIndexData()
{
	return &_indexData[0];
}

unsigned int ObjFile::GetNumIndices() const
{
	return _indexData.size();
}

IndexRange::IndexRange(unsigned int min, unsigned int max) :
	_min(min),
	_max(max)
{
}
