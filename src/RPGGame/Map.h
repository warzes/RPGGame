#pragma once

#include "Tile.h"
#include "GameModel.h"

struct TempBlockInfo;

constexpr const size_t MAPCHUNKSIZE_XY = 50;
constexpr const size_t MAPCHUNKSIZE_Z = 10;


constexpr const size_t MAPOFFSET_XY = MAPCHUNKSIZE_XY / 2.0f;

struct TileSelection final
{
	size_t x{ 0 };
	size_t y{ 0 };
	size_t z{ 0 };

	size_t tile{ NoTile };
};

class Map final
{
public:
	Map();

	void RecreateGeometry();
	void DestroyGeometry();

	void SetMapPosition(const glm::vec3& positions);

	void ClearTiles();
	void ClearTile(size_t x, size_t y, size_t z);
	void SetTile(size_t tile, size_t x, size_t y, size_t z);
	size_t GetTile(size_t x, size_t y, size_t z) const;

	// находится ли позиция внутри карты
	bool IsInBounds(size_t x, size_t y, size_t z) const;

	// Найти тайл, над которым находится курсор 
	TileSelection RaycastTile(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;

	bool SaveToFile(const std::string& filename) const;
	bool LoadFromFile(const std::string& filename);

	GameModel* GetModel() noexcept { return &m_model; }
	size_t GetVertexCount() const { return m_vertCount; }
	size_t GetIndexCount() const { return m_indexCount; }

private:
	void generateBufferMap();
	//void setVisibleBlock(const TileInfo& ti, TempBlockInfo& blockModelInfo, size_t x, size_t y, size_t z);

	GameModel m_model;
	size_t m_vertCount;
	size_t m_indexCount;

	size_t m_tiles[MAPCHUNKSIZE_XY][MAPCHUNKSIZE_XY][MAPCHUNKSIZE_Z] = { 0 };
	glm::vec3 m_positions{ 0.0f };
};