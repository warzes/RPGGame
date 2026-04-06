#include "stdafx.h"
#include "Map.h"
#include "MapLoadObjTile.h"
//=============================================================================
inline std::string getFileNameBlock(TileGeometryType type)
{
	switch (type)
	{
	case TileGeometryType::Block00: return "data/tiles/Block00.obj";
	case TileGeometryType::Block01: return "data/tiles/Block01.obj";
	case TileGeometryType::Block02: return "data/tiles/Block02.obj";
	case TileGeometryType::Block03: return "data/tiles/Block03.obj";
	case TileGeometryType::Block04: return "data/tiles/Block04.obj";
	case TileGeometryType::Block05: return "data/tiles/Block05.obj";
	case TileGeometryType::Block06: return "data/tiles/Block06.obj";
	case TileGeometryType::Block07: return "data/tiles/Block07.obj";
	case TileGeometryType::Block08: return "data/tiles/Block08.obj";
	case TileGeometryType::Block09: return "data/tiles/Block09.obj";
	case TileGeometryType::Block10: return "data/tiles/Block10.obj";
	default: std::unreachable();
	}
}
//=============================================================================
inline glm::vec3 getRotateAngle(RotateAngleY angle)
{
	glm::vec3 r(0.0f);
	if (angle == RotateAngleY::Rotate0)        r.y = 0.0f;
	else if (angle == RotateAngleY::Rotate90)  r.y = glm::radians(90.0f);
	else if (angle == RotateAngleY::Rotate180) r.y = glm::radians(180.0f);
	else if (angle == RotateAngleY::Rotate270) r.y = glm::radians(270.0f);
	return r;
}
//=============================================================================
bool testVisBlock(Map& map, TileGeometryType tile, size_t x, size_t y, size_t z)
{
	if ((x >= MAPCHUNKSIZE_XY) || (y >= MAPCHUNKSIZE_XY) || (z >= MAPCHUNKSIZE_Z)) return false;
	if (map.GetTile(x, y, z) == NoTile) return false;

	if (tile == TileGeometryType::Block00)
	{
		const auto& b = TileBank::GetTileInfo(map.GetTile(x, y, z));
		if (b.type == TileGeometryType::Block00)
		{
			return true;
		}
		else
		{
			// TODO: другие варианты блоков
		}
	}
	return false;
}
//=============================================================================
void setVisibleBlock(Map& map, const TileInfo& ti, TempBlockInfo& blockModelInfo, size_t x, size_t y, size_t z)
{
	blockModelInfo.rightVisible = blockModelInfo.leftVisible = true;
	blockModelInfo.forwardVisible = blockModelInfo.backVisible = true;
	blockModelInfo.bottomVisible = blockModelInfo.topVisible = true;

	if (x > 0)                   blockModelInfo.rightVisible = !testVisBlock(map, ti.type, x - 1, y, z);
	if (x < MAPCHUNKSIZE_XY - 1) blockModelInfo.leftVisible = !testVisBlock(map, ti.type, x + 1, y, z);
	if (y > 0)                   blockModelInfo.forwardVisible = !testVisBlock(map, ti.type, x, y - 1, z);
	if (y < MAPCHUNKSIZE_XY - 1) blockModelInfo.backVisible = !testVisBlock(map, ti.type, x, y + 1, z);
	if (z > 0)                   blockModelInfo.bottomVisible = !testVisBlock(map, ti.type, x, y, z - 1);
	if (z < MAPCHUNKSIZE_Z - 1)  blockModelInfo.topVisible = !testVisBlock(map, ti.type, x, y, z + 1);
}
//=============================================================================
Map::Map()
{
	ClearTiles();
}
//=============================================================================
void Map::RecreateGeometry()
{
	m_model.model.Free();
	generateBufferMap();
}
//=============================================================================
void Map::DestroyGeometry()
{}
//=============================================================================
void Map::SetMapPosition(const glm::vec3& positions)
{
	m_positions = positions;
}
//=============================================================================
void Map::ClearTiles()
{
	for (size_t y = 0; y < MAPCHUNKSIZE_XY; y++)
	{
		for (size_t x = 0; x < MAPCHUNKSIZE_XY; x++)
		{
			for (size_t z = 0; z < MAPCHUNKSIZE_Z; z++)
			{
				m_tiles[x][y][z] = NoTile;
			}
		}
	}
}
//=============================================================================
void Map::ClearTile(size_t x, size_t y, size_t z)
{
	SetTile(NoTile, x, y, z);
}
//=============================================================================
void Map::SetTile(size_t tile, size_t x, size_t y, size_t z)
{
	if (IsInBounds(x, y, z))
		m_tiles[x][y][z] = tile;
}
//=============================================================================
size_t Map::GetTile(size_t x, size_t y, size_t z) const
{
	if (IsInBounds(x, y, z))
		return m_tiles[x][y][z];

	return NoTile;
}
//=============================================================================
bool Map::IsInBounds(size_t x, size_t y, size_t z) const
{
	if (x < MAPCHUNKSIZE_XY && y < MAPCHUNKSIZE_XY && z < MAPCHUNKSIZE_Z) return true;
	else return false;
}
//=============================================================================
TileSelection Map::RaycastTile(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const
{
	// Алгоритм лучевого пересечения с воксельной сеткой (алгоритм цифровой дифференциальной аналого-вычислительной машины)
	glm::ivec3 currentPos = glm::ivec3(
		static_cast<int>(std::floor(rayOrigin.x)),
		static_cast<int>(std::floor(rayOrigin.y)),
		static_cast<int>(std::floor(rayOrigin.z))
	);

	// Определяем шаги
	glm::ivec3 step(
		rayDirection.x > 0 ? 1 : (rayDirection.x < 0 ? -1 : 0),
		rayDirection.y > 0 ? 1 : (rayDirection.y < 0 ? -1 : 0),
		rayDirection.z > 0 ? 1 : (rayDirection.z < 0 ? -1 : 0)
	);

	// Вычисляем расстояния до следующих плоскостей
	glm::vec3 tMax(
		(step.x > 0) ? (currentPos.x + 1 - rayOrigin.x) / rayDirection.x :
		(rayDirection.x != 0 ? (currentPos.x - rayOrigin.x) / rayDirection.x : std::numeric_limits<float>::max()),
		(step.y > 0) ? (currentPos.y + 1 - rayOrigin.y) / rayDirection.y :
		(rayDirection.y != 0 ? (currentPos.y - rayOrigin.y) / rayDirection.y : std::numeric_limits<float>::max()),
		(step.z > 0) ? (currentPos.z + 1 - rayOrigin.z) / rayDirection.z :
		(rayDirection.z != 0 ? (currentPos.z - rayOrigin.z) / rayDirection.z : std::numeric_limits<float>::max())
	);

	// Вычисляем шаги по осям
	glm::vec3 tDelta(
		(rayDirection.x != 0) ? std::abs(1.0f / rayDirection.x) : std::numeric_limits<float>::max(),
		(rayDirection.y != 0) ? std::abs(1.0f / rayDirection.y) : std::numeric_limits<float>::max(),
		(rayDirection.z != 0) ? std::abs(1.0f / rayDirection.z) : std::numeric_limits<float>::max()
	);

	const float mapOffsetXY = MAPCHUNKSIZE_XY / 2.0f;
	currentPos.x += mapOffsetXY;
	currentPos.y -= 0.5;
	currentPos.z += mapOffsetXY;

	// Проходим по вокселям, пока не найдем блок или не выйдем за границы
	for (int i = 0; i < MAPCHUNKSIZE_XY * MAPCHUNKSIZE_XY * MAPCHUNKSIZE_Z; ++i)
	{
		// Проверяем, находится ли текущая позиция в пределах карты
		if (IsInBounds(currentPos.x, currentPos.y, currentPos.z))
		{
			size_t tile = GetTile(currentPos.x, currentPos.y, currentPos.z);
			// Если в этой позиции есть блок, возвращаем его
			if (tile != NoTile)
			{
				TileSelection sel;
				sel.tile = tile;
				sel.x = currentPos.x;
				sel.y = currentPos.y;
				sel.z = currentPos.z;
				return sel;
			}
		}
		else
		{
			// Если вышли за границы, прекращаем поиск
			break;
		}

		// Определяем, по какой оси двигаться дальше
		if (tMax.x < tMax.y && tMax.x < tMax.z)
		{
			if (tMax.x > 100.0f) break; // Ограничение дистанции
			currentPos.x += step.x;
			tMax.x += tDelta.x;
		}
		else if (tMax.y < tMax.z) {
			if (tMax.y > 100.0f) break; // Ограничение дистанции
			currentPos.y += step.y;
			tMax.y += tDelta.y;
		}
		else {
			if (tMax.z > 100.0f) break; // Ограничение дистанции
			currentPos.z += step.z;
			tMax.z += tDelta.z;
		}
	}

	// Если блок не найден, возвращаем невалидный результат
	return {};
}
//=============================================================================
bool Map::SaveToFile(const std::string& filename) const
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		Error("Could not open file for writing: " + filename);
		return false;
	}

	// Записываем размеры карты
	file.write(reinterpret_cast<const char*>(&MAPCHUNKSIZE_XY), sizeof(int));
	file.write(reinterpret_cast<const char*>(&MAPCHUNKSIZE_XY), sizeof(int));
	file.write(reinterpret_cast<const char*>(&MAPCHUNKSIZE_Z), sizeof(int));

	// Записываем данные блоков
	for (int x = 0; x < MAPCHUNKSIZE_XY; ++x)
	{
		for (int y = 0; y < MAPCHUNKSIZE_XY; ++y)
		{
			for (int z = 0; z < MAPCHUNKSIZE_Z; ++z)
			{
				auto tile = GetTile(x, y, z);
				file.write(reinterpret_cast<const char*>(&tile), sizeof(size_t));
			}
		}
	}

	file.close();
	return true;
}
//=============================================================================
bool Map::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		Error("Could not open file for reading: " + filename);
		return false;
	}

	// Читаем размеры карты
	int sizeX, sizeY, sizeZ;
	file.read(reinterpret_cast<char*>(&sizeX), sizeof(int));
	file.read(reinterpret_cast<char*>(&sizeY), sizeof(int));
	file.read(reinterpret_cast<char*>(&sizeZ), sizeof(int));

	// Проверяем совместимость размеров
	if (sizeX != MAPCHUNKSIZE_XY || sizeY != MAPCHUNKSIZE_XY || sizeZ != MAPCHUNKSIZE_Z)
	{
		Error("Map dimensions in file don't match expected dimensions.");
		file.close();
		return false;
	}

	// Читаем данные блоков
	for (int x = 0; x < MAPCHUNKSIZE_XY; ++x)
	{
		for (int y = 0; y < MAPCHUNKSIZE_XY; ++y)
		{
			for (int z = 0; z < MAPCHUNKSIZE_Z; ++z)
			{
				size_t tile;
				file.read(reinterpret_cast<char*>(&tile), sizeof(size_t));
				SetTile(tile, x, y, z);
			}
		}
	}

	file.close();
	return true;
}
//=============================================================================
void getTempBlockInfo(Map& map, TempBlockInfo& blockInfo, const TileInfo& tile, size_t ix, size_t iy, size_t iz)
{
	const glm::vec3 position = glm::vec3(
		float(ix) - MAPOFFSET_XY,
		float(iz) + 0.5f,
		float(iy) - MAPOFFSET_XY);

	blockInfo.color = tile.color;
	blockInfo.center = position;
	blockInfo.rotate = getRotateAngle(tile.rotate);
	setVisibleBlock(map, tile, blockInfo, ix, iy, iz);
	blockInfo.modelPath = getFileNameBlock(tile.type);
	blockInfo.textureWall = tile.textureWall;
	blockInfo.textureFloor = tile.textureFloor;
	blockInfo.textureCeil = tile.textureCeil;
}
//=============================================================================
void Map::generateBufferMap()
{
	std::vector<MeshInfo> meshInfo;
	TempBlockInfo blockInfo{};

	// формирование информации о тайлах
	for (size_t iy = 0; iy < MAPCHUNKSIZE_XY; iy++)
	{
		for (size_t ix = 0; ix < MAPCHUNKSIZE_XY; ix++)
		{
			for (size_t iz = 0; iz < MAPCHUNKSIZE_Z; iz++)
			{
				size_t currentTile = GetTile(ix, iy, iz);
				if (currentTile == NoTile) continue;
				const auto& tile = TileBank::GetTileInfo(currentTile);

				getTempBlockInfo(*this, blockInfo, tile, ix, iy, iz);
				AddObjModel(blockInfo, meshInfo);
			}
		}
	}

	m_vertCount = 0;
	m_indexCount = 0;
	for (size_t i = 0; i < meshInfo.size(); i++)
	{
		m_vertCount += meshInfo[i].vertices.size();
		m_indexCount += meshInfo[i].indices.size();
	}

	// TODO: можно ведь не пересоздавать модель, если количество вершин/индексов осталось прежним
	m_model.model.Create(meshInfo);
}
//=============================================================================