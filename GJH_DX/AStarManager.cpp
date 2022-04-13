#include "AStarManager.h"
#include <algorithm>

float4 AStarManager::m_TileSize = float4(16.f, 16.f);
float4 AStarManager::m_MapSize;
AStarManager::Tile AStarManager::m_StartTile;
AStarManager::Tile AStarManager::m_EndTile;
std::vector<std::vector<AStarManager::Tile>> AStarManager::m_TileMap;
std::map<TileInfo, std::set<TileInfo>> AStarManager::m_DoorLinkList;
std::multimap<float, std::shared_ptr<AStarManager::Node>> AStarManager::m_OpenList;
std::map<__int64, std::shared_ptr<AStarManager::Node>> AStarManager::m_FindOpenList;
std::map<__int64, std::shared_ptr<AStarManager::Node>> AStarManager::m_CloseList;

AStarManager::AStarManager()
{

}

AStarManager::~AStarManager()
{

}

std::vector<std::pair<float4, int>> AStarManager::FindPath(const float4& _StartPos, const float4& _EndPos)
{
	float4 StartPos = _StartPos;
	StartPos = GetNearNoneTile(StartPos);

	float4 EndPos = _EndPos;
	EndPos = GetNearNoneTile(EndPos);

	if (StartPos == float4::ZERO || EndPos == float4::ZERO)
		return std::vector<std::pair<float4, int>>();

	m_StartTile = GetTile(StartPos);
	m_EndTile = GetTile(EndPos);

	std::shared_ptr<Node> EndNode = nullptr;

	EndNode = PopNode(m_StartTile, nullptr);
	if (EndNode != nullptr)
	{
		m_OpenList.clear();
		m_FindOpenList.clear();
		m_CloseList.clear();

		return CreatePath(EndNode);
	}

	while (m_OpenList.size())
	{
		std::shared_ptr<Node> CurNode = m_OpenList.begin()->second;
		m_OpenList.erase(m_OpenList.begin());
		m_CloseList.insert(std::make_pair(CurNode->TileData.Key, CurNode));

		if (CurNode->TileData.Type == TYPE::STAIRWAY
		|| CurNode->TileData.Type == TYPE::STAIRWAYINOUT)
		{
			float4 Dir[6] = { { -1.f, -1.f }, { -1.f, 1.f }, { 1.f, -1.f },
							  { 1.f, 1.f }, { -1.f, 0.f }, { 1.f, 0.f } };

			for (int i = 0; i < 6; i++)
			{
				EndNode = PopNode(CurNode->TileData.WorldPos, Dir[i], CurNode);
				if (EndNode != nullptr)
					break;
			}
		}
		else if (CurNode->TileData.Type == TYPE::DOOR)
		{
			float4 Dir[2] = { { -1.f, 0.f }, { 1.f, 0.f } };

			for (int i = 0; i < 2; i++)
			{
				EndNode = PopNode(CurNode->TileData.WorldPos, Dir[i], CurNode);
				if (EndNode != nullptr)
					break;
			}

			auto Find = m_DoorLinkList.find(TileInfo(CurNode->TileData.x, CurNode->TileData.y));
			if (Find != m_DoorLinkList.end())
			{
				for (auto& List : Find->second)
				{
					float4 TilePos = float4(List.x * m_TileSize.x, List.y * m_TileSize.y);
					Tile ConvertTile = GetTile(TilePos);

					EndNode = PopNode(ConvertTile, CurNode);
					if (EndNode != nullptr)
						break;
				}
			}
		}
		else if (CurNode->TileData.Type == TYPE::NONE)
		{
			float4 Dir[2] = { { -1.f, 0.f }, { 1.f, 0.f } };

			for (int i = 0; i < 2; i++)
			{
				EndNode = PopNode(CurNode->TileData.WorldPos, Dir[i], CurNode);
				if (EndNode != nullptr)
					break;
			}
		}

		if (EndNode != nullptr)
			break;
	}

	m_OpenList.clear();
	m_FindOpenList.clear();
	m_CloseList.clear();

	return CreatePath(EndNode);
}

void AStarManager::CreateMapData(std::vector<std::vector<int>> _ColTileMap, std::map<TileInfo, TILETYPE> _SpecialTileMap, std::map<TileInfo, std::set<TileInfo>> _DoorLinkList, float4& _MapSize)
{
	m_TileMap.clear();

	for (int i = 0; i < _ColTileMap.size(); i++)
	{
		m_TileMap.push_back(std::vector<Tile>());
		for (int j = 0; j < _ColTileMap[i].size(); j++)
		{
			m_TileMap[i].push_back(Tile(j, i, GetColTileType(_ColTileMap[i][j])));
		}
	}

	for (auto& Map : _SpecialTileMap)
	{
		TYPE Type = GetSpecialTileType((int)Map.second);
		if (Type != TYPE::NONE)
		{
			m_TileMap[Map.first.y][Map.first.x].Type = Type;
		}
	}

	for (int i = 0; i < m_TileMap.size() - 1; i++)
	{
		for (int j = 1; j < m_TileMap[i].size() - 1; j++)
		{
			if (i == 18 && j == 56)
			{
				int a = 0;
			}

			if (m_TileMap[i][j].Type == TYPE::STAIRWAYINOUT)
			{
				continue;
			}

			if ((m_TileMap[i + 1][j].Type == TYPE::STAIRWAY || m_TileMap[i + 1][j].Type != TYPE::BLOCK)
				&& m_TileMap[i][j - 1].Type != TYPE::STAIRWAYINOUT
				&& m_TileMap[i][j + 1].Type != TYPE::STAIRWAYINOUT)
			{
				m_TileMap[i][j].Type = TYPE::BLOCK;
				continue;
			}
		}
	}

	m_DoorLinkList = _DoorLinkList;
	m_MapSize = _MapSize;
}

std::shared_ptr<AStarManager::Node> AStarManager::PopNode(Tile& _FindTile, std::shared_ptr<Node> _Parent)
{
	if (_FindTile.Type == TYPE::BLOCK)
	{
		return nullptr;
	}
	
	if (m_FindOpenList.find(_FindTile.Key) != m_FindOpenList.end())
	{
		return nullptr;
	}

	if (m_CloseList.find(_FindTile.Key) != m_CloseList.end())
	{
		return nullptr;
	}

	std::shared_ptr<Node> NewNode = std::make_shared<Node>();
	NewNode->TileData = _FindTile;
	NewNode->Parent = _Parent;

	if (_Parent != nullptr)
	{
		NewNode->G = (int)(_Parent->G + float4::CalLen(NewNode->TileData.WorldPos - _Parent->TileData.WorldPos));
	}
	else
	{
		NewNode->G = 0;
	}

	NewNode->H = (int)(float4::CalLen(_FindTile.WorldPos - m_EndTile.WorldPos));
	NewNode->F = NewNode->G + NewNode->H;

	m_OpenList.insert(std::make_pair(NewNode->F, NewNode));
	m_FindOpenList.insert(std::make_pair(NewNode->TileData.Key, NewNode));

	if (_FindTile == m_EndTile)
	{
		return NewNode;
	}

	return nullptr;
}

std::shared_ptr<AStarManager::Node> AStarManager::PopNode(float4& _FindPos, float4 _AddTilePos, std::shared_ptr<Node> _Parent)
{
	Tile FindTile = GetTile(_FindPos + float4(_AddTilePos.x * m_TileSize.x, _AddTilePos.y * -m_TileSize.y));

	return PopNode(FindTile, _Parent);
}

AStarManager::TYPE AStarManager::GetColTileType(int _Tile)
{
	TYPE Type;

	switch (_Tile)
	{
	case (int)COLTILETYPE::NONE:
	case (int)COLTILETYPE::STAIRWAY1:
	case (int)COLTILETYPE::STAIRWAY2:
		Type = TYPE::NONE;
		break;
	case (int)COLTILETYPE::BLOCK:
	case (int)COLTILETYPE::PLATFORM:
	case (int)COLTILETYPE::WALL:
	case (int)COLTILETYPE::MIXBLOCK1:
	case (int)COLTILETYPE::MIXBLOCK2:
		Type = TYPE::BLOCK;
		break;
	default:
		Type = TYPE::NONE;
		break;
	}

	return Type;
}

AStarManager::TYPE AStarManager::GetSpecialTileType(int _Tile)
{
	TYPE Type;

	switch (_Tile)
	{
	case (int)SPECIALTILETYPE::STAIRWAY:
		Type = TYPE::STAIRWAY;
		break;
	case (int)SPECIALTILETYPE::STAIRWAYINOUT:
		Type = TYPE::STAIRWAYINOUT;
		break;
	case (int)SPECIALTILETYPE::DOOR:
		Type = TYPE::DOOR;
		break;
	default:
		Type = TYPE::NONE;
		break;
	}

	return Type;
}

AStarManager::Tile AStarManager::GetTile(const float4& _Pos)
{
	int X = abs(static_cast<int>(_Pos.x / m_TileSize.x));
	int Y = abs(static_cast<int>(_Pos.y / m_TileSize.y)); 

	if (m_TileMap.size() - 1 < X
		|| m_TileMap.size() - 1 < Y)
	{
		return Tile();
	}

	return Tile(X, Y, m_TileMap[Y][X].Type);
}

std::vector<std::pair<float4, int>> AStarManager::CreatePath(std::shared_ptr<Node> _EndNode)
{
	std::vector<std::pair<float4, int>> PathVector;

	if (_EndNode == nullptr)
	{
		return PathVector;
	}

	while (_EndNode->Parent)
	{
		PathVector.push_back(std::pair<float4, int>(_EndNode->TileData.WorldPos, (int)_EndNode->TileData.Type));
		_EndNode = _EndNode->Parent;
	}

	std::reverse(PathVector.begin(), PathVector.end());

	return PathVector;
}

float4 AStarManager::GetNearNoneTile(float4& _Pos)
{
	int Gap = 0;
	float4 Pos = _Pos;

	while (true)
	{
		float4 FindPos = float4(Pos.x, Pos.y - (m_TileSize.y * Gap));
		if (FindPos.y < -m_MapSize.y)
		{
			Pos = float4::ZERO;
			break;
		}

		Tile FindTile = GetTile(FindPos);
		
		if (FindTile.Type != TYPE::BLOCK)
		{
			Pos = FindTile.WorldPos;
			break;
		}

		Gap++;
	}

	return Pos;
}