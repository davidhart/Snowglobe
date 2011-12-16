#pragma once

#include <vector>

class IndexRange
{

public:

	IndexRange(unsigned int min, unsigned int max);

	inline unsigned int GetMin() const
	{
		return _min;
	}

	inline unsigned int GetMax() const
	{
		return _max;
	}

private:

	unsigned int _min;
	unsigned int _max;

};

class ObjFile
{

public:

	ObjFile();

	void Read(const char* filename);

	bool HasVertexNormals() const;
	bool HasTextureCoordinates() const;

	const float* GetVertexData();
	unsigned int GetNumVertices() const;
	unsigned int GetVertexStride() const;

	unsigned int GetVertexOffset() const;
	unsigned int GetNormalOffset() const;
	unsigned int GetTexCoordOffset() const;

	const unsigned int* GetIndexData();
	unsigned int GetNumIndices() const;

	const IndexRange* GetMeshRanges();
	unsigned int GetNumMeshes() const;

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

inline bool ObjFile::HasTextureCoordinates() const
{
	return _hasTextureCoordinates;
}

inline bool ObjFile::HasVertexNormals() const
{
	return _hasNormals;
}