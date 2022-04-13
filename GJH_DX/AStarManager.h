#pragma once
#include <GJHGameEngineMath.h>
#include <GJHGameEngineMemory.h>
#include <vector>
#include <list>
#include <GJHSpecialTileMapRenderer.h>

class AStarManager
{
public:
	enum class TYPE
	{
		NONE,
		BLOCK,
		STAIRWAY,
		STAIRWAYINOUT,
		DOOR,
	};

private:
	enum class COLTILETYPE
	{
		NONE = -1,
		BLOCK,
		PLATFORM,
		WALL,
		STAIRWAY1 = 20,
		STAIRWAY2 = 24,
		MIXBLOCK1 = 28,
		MIXBLOCK2 = 29
	};

	enum class SPECIALTILETYPE
	{
		STAIRWAY,
		STAIRWAYINOUT,
		DOOR
	};

public:
	class Tile
	{
	public:
		union
		{
			struct
			{
				int x;
				int y;
			};

			__int64 Key;
		};

		TYPE Type;
		float4 WorldPos;

		void Clear()
		{
			x = 0;
			y = 0;
			Type = TYPE::NONE;
			WorldPos = float4::ZERO;
		}

		Tile() :
			x(0), y(0), Type(TYPE::NONE), WorldPos(float4::ZERO)
		{
		}

		Tile(int _x, int _y, TYPE _Type) :
			x(_x), y(_y), Type(_Type), WorldPos(float4::ZERO)
		{
			WorldPos = float4((float)x * AStarManager::m_TileSize.x, (float)y * -AStarManager::m_TileSize.y);
		}

		Tile(const Tile& _Tile)
		{
			x = _Tile.x;
			y = _Tile.y;
			WorldPos = float4((float)x * AStarManager::m_TileSize.x, (float)y * -AStarManager::m_TileSize.y);
			Type = _Tile.Type;
		}

		Tile& operator=(const Tile& _Tile)
		{
			x = _Tile.x;
			y = _Tile.y;
			WorldPos = float4((float)x * AStarManager::m_TileSize.x, (float)y * -AStarManager::m_TileSize.y);
			Type = _Tile.Type;

			return *this;
		}

		bool operator==(const Tile& _Other) const
		{
			return Key == _Other.Key && Type == _Other.Type;
		}

		bool operator!=(const Tile& _Other) const
		{
			return Key != _Other.Key && Type != _Other.Type;
		}

		bool operator<(const Tile& _Other) const
		{
			return Key < _Other.Key;
		}
	};

	struct Node
	{
		Tile TileData;

		int G;
		int H;
		int F;

		std::shared_ptr<Node> Parent;

		void Clear()
		{
			TileData.Clear();
			G = 0;
			H = 0;
			F = 0;
			Parent = nullptr;
		}

		Node() : TileData(), G(0), H(0), F(0), Parent(nullptr)
		{
		}

		Node(Tile& _Tile, std::shared_ptr<Node> _Parent = nullptr)
		{
			TileData = _Tile;
			Parent = _Parent;
		}

		bool operator==(const Node& _Other)
		{
			return TileData == _Other.TileData;
		}

		bool operator!=(const Node& _Other)
		{
			return TileData != _Other.TileData;
		}

		bool operator<(const Node& _Other)
		{
			return TileData < _Other.TileData;
		}
	};

private:
	static float4 m_TileSize;
	static float4 m_MapSize;

private:
	static Tile m_StartTile;
	static Tile m_EndTile;

	static std::vector<std::vector<Tile>> m_TileMap;
	static std::map<TileInfo, std::set<TileInfo>> m_DoorLinkList;
	
private:
	static std::multimap<float, std::shared_ptr<Node>> m_OpenList;
	static std::map<__int64, std::shared_ptr<Node>> m_FindOpenList;
	static std::map<__int64, std::shared_ptr<Node>> m_CloseList;

public:
	static std::vector<std::pair<float4, int>> FindPath(const float4& _StartPos, const float4& _EndPos);
	static void CreateMapData(std::vector<std::vector<int>> _ColTileMap, std::map<TileInfo, TILETYPE> _SpecialTileMap, std::map<TileInfo, std::set<TileInfo>> _DoorLinkList, float4& _MapSize);

private:
	static std::shared_ptr<Node> PopNode(Tile& _FindTile, std::shared_ptr<Node> _Parent);
	static std::shared_ptr<Node> PopNode(float4& _FindPos, float4 _AddTilePos, std::shared_ptr<Node> _Parent);

private:
	static TYPE GetColTileType(int _Tile);
	static TYPE GetSpecialTileType(int _Tile);

public:
	static Tile GetTile(const float4& _Pos);

public:
	static float4 GetNearNoneTile(float4& _Pos);

private:
	static std::vector<std::pair<float4, int>> CreatePath(std::shared_ptr<Node> _EndNode);

public:
	AStarManager();
	~AStarManager();
};