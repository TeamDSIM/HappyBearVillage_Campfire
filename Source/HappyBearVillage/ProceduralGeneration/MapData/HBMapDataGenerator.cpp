// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGeneration/MapData/HBMapDataGenerator.h"

#include <queue>

#include "ProceduralGeneration/Noise/HBNoiseSettings.h"
#include "ProceduralGeneration/Noise/HBPerlinNoise.h"

FHBMapData UHBMapDataGenerator::GenerateMapData(UHBPerlinNoise* InPerlinNoise)
{
	// 초기화
	Width = InPerlinNoise->GetNoiseSettings().Resolution.X;
	Height = InPerlinNoise->GetNoiseSettings().Resolution.Y;
	AreaCount = 0;

	for (int32 i=0; i<256; ++i)
	{
		for (int32 j=0; j<256; ++j)
		{
			Nodes[i][j].Row = i;
			Nodes[i][j].Col = j;
			Nodes[i][j].AreaID = -1;
			Nodes[i][j].Value = ' ';
			
			AreaAdj[i][j].Src = nullptr;
			AreaAdj[i][j].Dest = nullptr;
			AreaAdj[i][j].Weight = 10000;
		}
	}

	for (int32 i=0; i<Height; ++i)
	{
		for (int32 j=0; j<Width; ++j)
		{
			if (InPerlinNoise->GetNoiseElement(i, j) >= 0)
			{
				Nodes[i][j].AreaID = 0;
				Nodes[i][j].Value = 'A';
			}
		}
	}

	BorderNodes.Empty();

	// 영역 구분
	int32 DR[4] = {0, 0, 1, -1};
	int32 DC[4] = {1, -1, 0, 0};

	for (int32 i=0; i<Height; ++i)
	{
		for (int32 j=0; j<Width; ++j)
		{
			if (Nodes[i][j].AreaID != 0) continue;

			++AreaCount;

			TQueue<FMapNode*> BfsQueue;
			FMapNode* Start = &Nodes[i][j];
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
					if (Nodes[NextRow][NextCol].AreaID == -1) IsEdgeArea = true;
					if (Nodes[NextRow][NextCol].AreaID != 0) continue;

					FMapNode* NextNode = &Nodes[NextRow][NextCol];
					NextNode->AreaID = CurNode->AreaID;

					BfsQueue.Enqueue(NextNode);
				}

				if (IsEdgeArea) BorderNodes.Add(CurNode);
			}
		}
	}

	if (AreaCount >= 256) return FHBMapData();

	// 최단거리 탐색
	for (auto& Node : BorderNodes)
	{
		bool IsVisit[256][256] = { false, };

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

		FMapNode* Prev[256][256] = { nullptr, };

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
			Nodes[Path->Row][Path->Col].Value = 'R';
			Path = Prev[Path->Row][Path->Col];
		}

		Union(SrcAreaID, DestAreaID, ParentAreaID);
		UnionCount++;

		if (UnionCount == AreaCount) break;
	}

	FString MapAs1D;

	for (int32 i=0; i<Height; ++i)
	{
		for (int32 j=0; j<Width; ++j)
		{
			MapAs1D.AppendChar(Nodes[i][j].Value);
		}
	}

	Result.Resolution = { Width, Height };
	Result.Seed = InPerlinNoise->GetNoiseSettings().Seed;
	Result.MapAs1D = MapAs1D;

	return Result;
}

FHBMapData UHBMapDataGenerator::GenerateMapData(FHBNoiseSettings Settings)
{
	UHBPerlinNoise* PerlinNoise = NewObject<UHBPerlinNoise>();
	PerlinNoise->GeneratePerlinNoise(Settings);
	
	return GenerateMapData(PerlinNoise);;
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

			if (Nodes[Row][Col].Value != ' ')
			{
				TypeLogText += FString::Printf(TEXT("%c"), Nodes[Row][Col].Value);
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
