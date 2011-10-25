#pragma once

#include <vector>

class IndexRange
{
	unsigned int Min;
	unsigned int Max;
};

class ObjFile
{

public:

	ObjFile();

	void Read(const char* filename);

	bool HasVertexNormals();
	bool HasTextureCoordinates();

	const float* GetVertexData();
	unsigned int GetNumVertices();
	unsigned int GetVertexStride();

	const unsigned int* GetIndexData();
	unsigned int GetNumIndices();

	const IndexRange* GetMeshRanges();
	unsigned int GetNumMeshes();

private:

	struct ObjFormatIndex
	{
		
	public:

		ObjFormatIndex();
		void Read(std::istream& stream);

		int v;
		int vt;
		int vn;

	};

	struct CachedObjIndex
	{

	public:
		CachedObjIndex();
		ObjFormatIndex index;
		int next;
		int packedPos;

	};

	void PackModelData(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& textureCoordinates,
		const std::vector<ObjFormatIndex>& indices);

	void PackVertex(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& textureCoordinates, 
		const ObjFormatIndex& index, std::vector<CachedObjIndex>& indices);

	std::vector<float> _vertexData;
	std::vector<unsigned int> _indexData;
	std::vector<IndexRange> _meshRanges;
	
	bool _hasNormals;
	bool _hasTextureCoordinates;
};
