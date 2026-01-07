// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"

#include <queue>

#include "HappyBearVillage.h"
#include "ProceduralGeneration/Noise/HBNoiseSettings.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"
#include "Utils/HBUtils.h"

FHBMapData UHBMapDataGenerator::GenerateMapData(FHBNoiseSettings Settings)
{
	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();
	PerlinNoise->GeneratePerlinNoise(Settings);
	
	GenerateFieldData(PerlinNoise);
	GenerateHouseData(8);
	GenerateForestData();

	UpdateMap();

	return MapData;
}

FHBMapData UHBMapDataGenerator::GenerateFieldData(UHBPerlinNoise* InPerlinNoise)
{
	// 초기화
	Width = InPerlinNoise->GetNoiseSettings().Resolution.X;
	Height = InPerlinNoise->GetNoiseSettings().Resolution.Y;
	AreaCount = 0;

	for (int32 Row=0; Row<MAX_HEIGHT; ++Row)
	{
		for (int32 Col=0; Col<MAX_WIDTH; ++Col)
		{
			Nodes[Row][Col].Row = Row;
			Nodes[Row][Col].Col = Col;
			Nodes[Row][Col].Perlin = UNDEFINED;
			Nodes[Row][Col].AreaID = UNDEFINED;
			Nodes[Row][Col].Type = ' ';
			
			AreaAdj[Row][Col].Src = nullptr;
			AreaAdj[Row][Col].Dest = nullptr;
			AreaAdj[Row][Col].Weight = MAX_WEIGHT;
		}
	}

	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			Nodes[Row][Col].Perlin = InPerlinNoise->GetNoiseElement(Row, Col);
			
			if (Nodes[Row][Col].Perlin >= 1)
			{
				Nodes[Row][Col].Type = 'A';
			}
		}
	}

	AreaBorderNodes.Empty();

	// 영역 구분
	int32 DR[4] = {0, 0, 1, -1};
	int32 DC[4] = {1, -1, 0, 0};

	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			if (Nodes[Row][Col].AreaID != UNDEFINED) continue;
			if (Nodes[Row][Col].Type != 'A') continue;

			++AreaCount;

			TQueue<FMapNode*> BfsQueue;
			FMapNode* Start = &Nodes[Row][Col];
			Start->AreaID = AreaCount;
			BfsQueue.Enqueue(Start);

			while (!BfsQueue.IsEmpty())
			{
				FMapNode* CurNode;
				bool Success = BfsQueue.Dequeue(CurNode);
				if (!Success) break;

				bool IsEdgeArea = false;

				for (int32 k=0; k<4; ++k)
				{
					int32 NextRow = CurNode->Row + DR[k];
					int32 NextCol = CurNode->Col + DC[k];

					if (NextRow < 0 || NextCol < 0 || NextRow >= Height || NextCol >= Width) continue;
					if (Nodes[NextRow][NextCol].AreaID == UNDEFINED) IsEdgeArea = true;
					if (Nodes[NextRow][NextCol].AreaID != UNDEFINED) continue;
					if (Nodes[NextRow][NextCol].Type != 'A') continue;

					FMapNode* NextNode = &Nodes[NextRow][NextCol];
					NextNode->AreaID = CurNode->AreaID;

					BfsQueue.Enqueue(NextNode);
				}

				if (IsEdgeArea) AreaBorderNodes.Add(CurNode);
			}
		}
	}

	if (AreaCount >= MAX_HEIGHT) return FHBMapData();

	// 최단거리 탐색
	for (auto& Node : AreaBorderNodes)
	{
		bool IsVisit[MAX_HEIGHT][MAX_WIDTH] = { false, };

		TQueue<TPair<FMapNode*, int32>> BfsQueue;

		FMapNode* Start = Node;
		IsVisit[Start->Row][Start->Col] = true;
		BfsQueue.Enqueue({Start, 0});

		while (!BfsQueue.IsEmpty())
		{
			TPair<FMapNode*, int32> CurPair;
			bool Success = BfsQueue.Dequeue(CurPair);
			if (!Success) break;

			FMapNode* CurNode = CurPair.Key;
			int32 CurNodeDist = CurPair.Value;

			for (int32 i=0; i<4; ++i)
			{
				int32 NextRow = CurNode->Row + DR[i];
				int32 NextCol = CurNode->Col + DC[i];

				if (NextRow < 0 || NextCol < 0 || NextRow >= Height || NextCol >= Width) continue;
				if (IsVisit[NextRow][NextCol]) continue;
				if (Nodes[NextRow][NextCol].AreaID == Start->AreaID) continue;

				IsVisit[NextRow][NextCol] = true;

				FMapNode* NextNode = &Nodes[NextRow][NextCol];
				int NextNodeDist = CurNodeDist + 1;

				if (NextNode->AreaID > 0 && NextNodeDist < AreaAdj[Start->AreaID][NextNode->AreaID].Weight)
				{
					FMapEdge* UV = &AreaAdj[Start->AreaID][NextNode->AreaID];
					UV->Src = Start;
					UV->Dest = NextNode;
					UV->Weight = NextNodeDist;

					FMapEdge* VU = &AreaAdj[NextNode->AreaID][Start->AreaID];
					VU->Src = NextNode;
					VU->Dest = Start;
					VU->Weight = NextNodeDist;
				}
				else
				{
					BfsQueue.Enqueue({NextNode, NextNodeDist});
				}
			}
		}
	}

	// 영역 연결
	std::priority_queue<std::pair<int32, FMapEdge*>, std::vector<std::pair<int32, FMapEdge*>>, std::greater<std::pair<int32, FMapEdge*>>> PQ;

	for (int32 i=1; i<=AreaCount; ++i)
	{
		for (int32 j=i+1; j<=AreaCount; ++j)
		{
			FMapEdge* Edge = &AreaAdj[i][j];

			if (Edge->Weight == 0) continue;

			PQ.push({Edge->Weight, Edge});
		}
	}

	TArray<int32> ParentAreaID;
	int32 UnionCount = 0;

	for (int32 i=0; i<=AreaCount; ++i)
	{
		ParentAreaID.Add(i);
	}

	while (!PQ.empty())
	{
		FMapEdge* Edge = PQ.top().second;
		PQ.pop();

		int32 SrcAreaID = Edge->Src->AreaID;
		int32 DestAreaID = Edge->Dest->AreaID;

		if (Find(SrcAreaID, ParentAreaID) == Find(DestAreaID, ParentAreaID)) continue;

		FMapNode* Prev[MAX_HEIGHT][MAX_WIDTH] = { nullptr, };

		TQueue<FMapNode*> BfsQueue;

		FMapNode* Src = Edge->Src;
		Prev[Src->Row][Src->Col] = Src;
		BfsQueue.Enqueue(Src);

		FMapNode* Dest = nullptr;

		while (!BfsQueue.IsEmpty() && Dest == nullptr)
		{
			FMapNode* CurNode;
			bool Success = BfsQueue.Dequeue(CurNode);
			if (!Success) break;

			for (int32 i=0; i<4; ++i)
			{
				int32 NextRow = CurNode->Row + DR[i];
				int32 NextCol = CurNode->Col + DC[i];

				if (NextRow < 0 || NextCol < 0 || NextRow >= Height || NextCol >= Width) continue;
				if (Prev[NextRow][NextCol] != nullptr) continue;
				if (Nodes[NextRow][NextCol].AreaID == Src->AreaID) continue;

				Prev[NextRow][NextCol] = CurNode;

				FMapNode* NextNode = &Nodes[NextRow][NextCol];
				BfsQueue.Enqueue(NextNode);

				if (NextNode->AreaID == DestAreaID) Dest = NextNode;
			}
		}

		if (Dest == nullptr) continue;
		FMapNode* Path = Dest;

		while (Path != Src)
		{
			Nodes[Path->Row][Path->Col].Type = 'R';
			Path = Prev[Path->Row][Path->Col];
		}

		Union(SrcAreaID, DestAreaID, ParentAreaID);
		UnionCount++;

		if (UnionCount == AreaCount) break;
	}

	MapData.Resolution = { Width, Height };
	MapData.Map.SetNum(Height);
	for (int32 i=0; i<Height; ++i)
	{
		MapData.Map[i].SetNum(Width);
	}
	UpdateMap();
		
	return MapData;
}

FHBMapData UHBMapDataGenerator::GenerateHouseData(int32 HouseCount)
{
	if (MapData.Map.IsEmpty()) return FHBMapData();

	TArray<TArray<FMapNode>> CandidateNodes;
	TArray<int32> CandidateAreas;
	CandidateNodes.SetNum(1 + AreaCount);

	for (int32 Row=0; Row<MapData.Resolution.Y; ++Row)
	{
		for (int32 Col=0; Col<MapData.Resolution.X; ++Col)
		{
			int32 Perlin = Nodes[Row][Col].Perlin;
			int32 AreaID = Nodes[Row][Col].AreaID;
			
			if (Perlin > 1 && AreaID > 0)
			{
				CandidateNodes[AreaID].Add(Nodes[Row][Col]);
			}
		}
	}

	for (int i=1; i<=AreaCount; ++i)
	{
		CandidateAreas.Add(i);
		HBUtils::ShuffleWithStream(CandidateNodes[i]);
	}
	
	HBUtils::ShuffleWithStream(CandidateAreas);
	int32 AreaIndex = 0;

	for (int i=0; i<HouseCount; ++i)
	{
		bool IsValid = true;

		do
		{
			IsValid = true;
			
			int32 TargetArea = CandidateAreas[AreaIndex];
			while (CandidateNodes[TargetArea].IsEmpty())
			{
				++AreaIndex;
				if (AreaIndex == CandidateAreas.Num()) AreaIndex = 0;

				TargetArea = CandidateAreas[AreaIndex];
			}
			
			FMapNode TargetNode = CandidateNodes[TargetArea].Last();
			CandidateNodes[TargetArea].Pop();

			for (int j=0; j<4; ++j)
			{
				for (int k=0; k<4; ++k)
				{
					FMapNode AdjNode = Nodes[TargetNode.Row + j][TargetNode.Col + k];
					if (AdjNode.Perlin <= 1 || AdjNode.AreaID <= 0) IsValid = false;
					if (AdjNode.Type == 'H' || AdjNode.Type == 'h') IsValid = false;
				}
			}

			if (IsValid)
			{
				for (int j=0; j<4; ++j)
				{
					for (int k=0; k<4; ++k)
					{
						Nodes[TargetNode.Row + j][TargetNode.Col + k].Type = 'h';
					}
				}

				Nodes[TargetNode.Row][TargetNode.Col].Type = 'H';
			}
		}
		while (!IsValid);
	}

	UpdateMap();
	
	return MapData;
}

FHBMapData UHBMapDataGenerator::GenerateHouseColorData(TArray<FLinearColor> HouseColorList)
{
	for (int32 Row=0; Row<Height; ++Row)
	{
		TArray<FLinearColor> RowData;
		
		for (int32 Col=0; Col<Width; ++Col)
		{
			RowData.Add(FLinearColor(0, 0, 0, 0));			
		}

		MapData.HouseColorLayer.Add(RowData);
	}

	HBUtils::ShuffleWithStream(HouseColorList);
	int32 ColorIndex = 0;
	
	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			if (MapData.Map[Row][Col] != 'H') continue;

			MapData.HouseLocationsByColor.Add(HouseColorList[ColorIndex], FVector(Row, Col, 0));
			
			for (int32 i=0; i<4; ++i)
			{
				for (int32 j=0; j<4; ++j)
				{
					MapData.HouseColorLayer[Row + i][Col + j] = HouseColorList[ColorIndex];
				}
			}

			FVector HouseLocation = FVector((Col + 1.5f) * MapData.AreaScale * 100, (Row + 1.5f) * MapData.AreaScale * 100, 200);
			MapData.HouseLocationsByColor.Add(HouseColorList[ColorIndex], HouseLocation);
			
			++ColorIndex;
		}
	}
	
	return MapData;
}

FHBMapData UHBMapDataGenerator::GenerateForestData()
{
	ForestBorderGridIndices.Add(TArray<FVector>());
	
	int32 DR[4] = {0, 0, 1, -1};
	int32 DC[4] = {1, -1, 0, 0};
	
	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			if (Nodes[Row][Col].Type != ' ') continue;
			if (!IsForestBorder(&Nodes[Row][Col])) continue;

			++ForestCount;
			ForestBorderGridIndices.Add(TArray<FVector>());
			ForestBorderGridIndices[ForestCount].Add(FVector(Row, Col, 0));

			bool IsVisit[MAX_HEIGHT][MAX_WIDTH] = { false, };
			TArray<FMapNode*> DfsStack;
			
			FMapNode* Start = &Nodes[Row][Col];
			Start->AreaID = -ForestCount;
			Start->Type = 'F';
			IsVisit[Start->Row][Start->Col] = true;
			DfsStack.Push(Start);

			while (!DfsStack.IsEmpty())
			{
				FMapNode* CurNode = DfsStack.Last();
				DfsStack.Pop();
				
				for (int32 k=0; k<4; ++k)
				{
					int32 NextRow = CurNode->Row + DR[k];
					int32 NextCol = CurNode->Col + DC[k];

					if (NextRow < 0 || NextCol < 0 || NextRow >= Height || NextCol >= Width) continue;
					if (Nodes[NextRow][NextCol].AreaID >= 0) continue;
					if (IsVisit[NextRow][NextCol]) continue;
					
					IsVisit[NextRow][NextCol] = true;
					
					FMapNode* NextNode = &Nodes[NextRow][NextCol];

					if (IsForestBorder(NextNode))
					{
						ForestBorderGridIndices[ForestCount].Add(FVector(CurNode->Row, CurNode->Col, 0));
						DfsStack.Push(NextNode);
						NextNode->AreaID = CurNode->AreaID;
						NextNode->Type = CurNode->Type;
					}
				}
			}
		}
	}

	MapData.ForestBorderGridIndices = ForestBorderGridIndices;
	UpdateMap();

	UE_LOG(LogTemp, Log, TEXT("[ForestCount : ] %d"), ForestCount);

	return MapData;
}

FHBMapData UHBMapDataGenerator::UpdateMap()
{
	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			MapData.Map[Row][Col] = Nodes[Row][Col].Type;
		}
	}

	return MapData;
}

UTexture2D* UHBMapDataGenerator::GenerateForestTexture2D()
{
	ForestTexture2D = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
	
	TArray<FColor> Pixels;
	Pixels.SetNumUninitialized(Width * Height);

	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			FColor CurColor = (Nodes[Row][Col].Perlin >= 1 || Nodes[Row][Col].Type == 'R') ? FColor(0, 0, 0, 0) : FColor::Red;
			Pixels[Width * Row + Col] = CurColor;
		}
	}

	void* TextureData = ForestTexture2D->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
	ForestTexture2D->GetPlatformData()->Mips[0].BulkData.Unlock();
	ForestTexture2D->SRGB = false;
	ForestTexture2D->Filter = TF_Nearest;
	ForestTexture2D->AddressX = TA_Clamp;
	ForestTexture2D->AddressY = TA_Clamp;
	ForestTexture2D->NeverStream = true;
	ForestTexture2D->UpdateResource();

	MapData.ForestAsTexture2D = ForestTexture2D;
	
	return ForestTexture2D;
}

UTexture2D* UHBMapDataGenerator::GenerateMapTexture()
{
	UTexture2D* MapTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

	TArray<FColor> Pixels;
	Pixels.SetNumUninitialized(Width * Height);

	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			TCHAR Type = MapData.Map[Row][Col];

			if (Type == 'A' || Type == 'R')
			{
				Pixels[Width * Row + Col] = FColor(198, 219, 218, 240);
			}
			else if (Type == 'H' || Type == 'h')
			{
				Pixels[Width * Row + Col] = MapData.HouseColorLayer[Row][Col].ToFColor(true);
			}
			else
			{
				Pixels[Width * Row + Col] = FColor(0, 0, 0, 0);
			}
		}
	}

	void* TextureData = MapTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
	MapTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
	MapTexture->SRGB = true;
	MapTexture->Filter = TF_Nearest;
	MapTexture->AddressX = TA_Clamp;
	MapTexture->AddressY = TA_Clamp;
	MapTexture->NeverStream = true;
	MapTexture->UpdateResource();

	MapData.MapTexture = MapTexture;
	
	return MapTexture;
}

void UHBMapDataGenerator::PrintMapData()
{
	if (Width <= 0 || Height <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("PrintMapData() Resolution Error"));
		return;
	}

	FString AreaLogText;
	FString TypeLogText;

	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			if (Nodes[Row][Col].AreaID >= 0)
			{
				AreaLogText += FString::Printf(TEXT("%d"), Nodes[Row][Col].AreaID);
			}
			else
			{
				AreaLogText += TEXT(" ");
			}

			if (Nodes[Row][Col].Type != ' ')
			{
				TypeLogText += FString::Printf(TEXT("%c"), Nodes[Row][Col].Type);
			}
			else
			{
				TypeLogText += TEXT(" ");
			}
		}

		AreaLogText += TEXT("\n");
		TypeLogText += TEXT("\n");
	}

	UE_LOG(LogTemp, Log, TEXT("[Area Info] : \n%s"), *AreaLogText);
	UE_LOG(LogTemp, Log, TEXT("[Type Info] : \n%s"), *TypeLogText);

	FString MapDataTypeLogText;

	for (int32 Row=0; Row<Height; ++Row)
	{
		for (int32 Col=0; Col<Width; ++Col)
		{
			MapDataTypeLogText += FString::Printf(TEXT("%c"), MapData.Map[Row][Col]);
		}

		MapDataTypeLogText += TEXT("\n");
	}

	UE_LOG(LogTemp, Log, TEXT("[Map Type Info] : \n%s"), *MapDataTypeLogText);
}

void UHBMapDataGenerator::Union(int32 ID1, int32 ID2, TArray<int32>& Parents)
{
	int32 Root1 = Find(ID1, Parents);
	int32 Root2 = Find(ID2, Parents);

	Parents[Root2] = Root1;
}

int32 UHBMapDataGenerator::Find(int32 ID, TArray<int32>& Parents)
{
	if (Parents[ID] != ID) Parents[ID] = Find(Parents[ID], Parents);

	return Parents[ID];
}

bool UHBMapDataGenerator::IsForestBorder(FMapNode* Node)
{
	if (Node->AreaID >= 0) return false;
	if (Node->Type == 'A' || Node->Type == 'R') return false;
	
	bool IsBorder = false;
	
	int32 DR[8] = {0, 0, 1, 1, 1, -1, -1, -1};
	int32 DC[8] = {1, -1, 0, 1, -1, 0, 1, -1};
	
	for (int32 k=0; k<8; ++k)
	{
		int32 NextRow = Node->Row + DR[k];
		int32 NextCol = Node->Col + DC[k];
		
		if (NextRow < 0 || NextCol < 0 || NextRow >= Height || NextCol >= Width)
		{
			IsBorder = true;
			continue;
		}
		
		if (Nodes[NextRow][NextCol].AreaID >= 0) IsBorder = true;
		if (Nodes[NextRow][NextCol].Type == 'A' || Nodes[NextRow][NextCol].Type == 'R') IsBorder = true;
	}
	
	return IsBorder;
}
