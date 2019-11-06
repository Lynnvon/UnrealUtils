// Copyright Shinban.Inc
/*!
 * file RuntimeGenerateTerrain.cpp
 *
 * author Hailiang Feng
 * date 2019/11/01 9:45
 *
 *
 */
#include "RuntimeGenerateTerrain.h"
#include "Kismet/GameplayStatics.h"
#include "LandscapeComponent.h"
#include "LandscapeInfo.h"
#include "LandscapeProxy.h"
#include "EngineUtils.h"
#include "Materials/MaterialInstanceConstant.h"
#include "RenderingThread.h"
#include "FS_Utils.h"
#include "Components/TextRenderComponent.h"
#include "LandscapeInfoMap.h"
#include "Guid.h"
#include "LandscapeHeightfieldCollisionComponent.h"
#include "UnrealMathUtility.h"
#include "Async.h"


 // Sets default values for this component's properties
URuntimeGenerateTerrain::URuntimeGenerateTerrain()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAddComponent = false;
	// ...
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> _material(TEXT("MaterialInstanceConstant'/Game/GoogleMap/M_GoogleBASE_Inst.M_GoogleBASE_Inst'"));
	if (_material.Succeeded())
	{
		SounceMaterial = _material.Object;
	}


}


// Called when the game starts
void URuntimeGenerateTerrain::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UWorld* world = GetWorld();
	check(world);
	TArray<AActor*> _actor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandscapeProxy::StaticClass(), _actor);
	if (_actor.Num() > 0)
	{
		mLandscape = (ALandscapeProxy*)_actor[0];
	}
	if (mLandscape)
	{
		mLandscape->LandscapeMaterial = SounceMaterial;
		mLandscape->bUseDynamicMaterialInstance = true;
		for (ULandscapeComponent* C : mLandscape->LandscapeComponents)
		{
			LoadTexture(C);
			SetXYtoComponentMap(C);
		}
	}
}

// Called every frame
void URuntimeGenerateTerrain::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...	
	if (!mLandscape)return;
	CalComponentIndex();
	if (bAddComponent)
	{
		StopAddComponent();
		RemoveLandscapeComponent();
		MoveLandscapeComponent();

	}
}

bool URuntimeGenerateTerrain::LoadTexture(ULandscapeComponent* C)
{
	for (int i = 0; i < C->MaterialInstancesDynamic.Num(); i++)
	{
		UMaterialInstanceDynamic* MID = C->MaterialInstancesDynamic[i];
		int XIndex = GetComponentIndexX(C);
		int YIndex = GetComponentIndexY(C);

		FString _fileName = FString("Texture2D'/Game/GoogleMap/satellite_en/Terrain_1/18/");
		int rowOffset = FCString::Atoi(*UFS_Utils::GetTerrainConfigSection(FString("rowOffset")));
		int columnOffset = FCString::Atoi(*UFS_Utils::GetTerrainConfigSection(FString("columnOffset")));
		_fileName.Append(FString::FromInt(XIndex + rowOffset));
		_fileName.Append(FString("/"));
		_fileName.Append(FString::FromInt(YIndex + columnOffset));
		_fileName.Append(FString("."));
		_fileName.Append(FString::FromInt(YIndex + columnOffset));
		_fileName.Append(FString("'"));


		UTexture2D* texture = LoadObject<UTexture2D>(NULL, *_fileName);
		if (texture)
		{
			MID->SetTextureParameterValue(FName("Texture"), texture);
		}
		else
		{
			return false;
		}
	}
	return true;

}

bool URuntimeGenerateTerrain::AsyncLoadTexture(ULandscapeComponent* C)
{
	
	for (int i = 0; i < C->MaterialInstancesDynamic.Num(); i++)
	{
		UMaterialInstanceDynamic* MID = C->MaterialInstancesDynamic[i];
		int XIndex = GetComponentIndexX(C);
		int YIndex = GetComponentIndexY(C);

		FString _fileName = FString("Texture2D'/Game/GoogleMap/satellite_en/Terrain_1/18/");
		int rowOffset = FCString::Atoi(*UFS_Utils::GetTerrainConfigSection(FString("rowOffset")));
		int columnOffset = FCString::Atoi(*UFS_Utils::GetTerrainConfigSection(FString("columnOffset")));
		_fileName.Append(FString::FromInt(XIndex + rowOffset));
		_fileName.Append(FString("/"));
		_fileName.Append(FString::FromInt(YIndex + columnOffset));
		_fileName.Append(FString("."));
		_fileName.Append(FString::FromInt(YIndex + columnOffset));
		_fileName.Append(FString("'"));


		
	}
	return true;
}

void URuntimeGenerateTerrain::MoveLandscapeComponent()
{
	for (int32 ComponentIndexY = ComponentIndexY1; ComponentIndexY <= ComponentIndexY2; ComponentIndexY++)
	{
		for (int32 ComponentIndexX = ComponentIndexX1; ComponentIndexX <= ComponentIndexX2; ComponentIndexX++)
		{
			ULandscapeComponent* LandscapeComponent = mXYtoComponentMap.FindRef(FIntPoint(ComponentIndexX, ComponentIndexY));
			if (!LandscapeComponent)
			{
				// Add New component...
				FIntPoint ComponentBase = FIntPoint(ComponentIndexX, ComponentIndexY)*mLandscape->ComponentSizeQuads;

				if (DeleteComponents.Num() > 0)
				{
					LandscapeComponent = DeleteComponents.Top();
					LandscapeComponent->SetMobility(EComponentMobility::Movable);
					LandscapeComponent->SetRelativeLocation(FVector(ComponentBase.X, ComponentBase.Y, 0));
					LandscapeComponent->SetMobility(EComponentMobility::Static);
					SetXYtoComponentMap(LandscapeComponent);
					LoadTexture(LandscapeComponent);
					DeleteComponents.RemoveSingleSwap(LandscapeComponent);
				}
			}
		}
	}
	UpdateLeftAndRightPoint();

	UE_LOG(LogTemp, Warning, TEXT("LeftBottom = %s,RightTop = %s"), *LeftBottom.ToString(), *RightTop.ToString());
	UE_LOG(LogTemp, Warning, TEXT("X1 = %s,X2 = %s"), *FString::FromInt(ComponentIndexX1), *FString::FromInt(ComponentIndexX2));
	UE_LOG(LogTemp, Warning, TEXT("Y1 = %s,Y2 = %s"), *FString::FromInt(ComponentIndexY1), *FString::FromInt(ComponentIndexY2));
	//结束本次增加
	ResetComponentIndex();
}

void URuntimeGenerateTerrain::RemoveLandscapeComponent()
{
	DeleteComponents.Empty();
	//删除远处的component
	//X正向 加

	int Xnum = ComponentIndexX2 - ComponentIndexX1 + 1;
	if (ComponentIndexX1 > RightTop.X)
	{

		for (int32 y = LeftBottom.Y; y <= RightTop.Y; y++)
		{
			for (int32 x = LeftBottom.X; x < LeftBottom.X + Xnum; x++)
			{
				ULandscapeComponent* LandscapeComponent = mXYtoComponentMap.FindRef(FIntPoint(x, y));
				if (LandscapeComponent)
				{
					DeleteComponents.Add(LandscapeComponent);
				}

			}
		}

	}
	//x 负方向 加
	if (ComponentIndexX2 < LeftBottom.X)
	{
		for (int32 y = LeftBottom.Y; y <= RightTop.Y; y++)
		{
			for (int32 x = RightTop.X; x > RightTop.X - Xnum; x--)
			{
				ULandscapeComponent* LandscapeComponent = mXYtoComponentMap.FindRef(FIntPoint(x, y));
				if (LandscapeComponent)
				{
					DeleteComponents.Add(LandscapeComponent);
				}

			}
		}

	}
	//Y方向 
	int Ynum = ComponentIndexY2 - ComponentIndexY1 + 1;
	if (ComponentIndexY1 > RightTop.Y)
	{
		for (int32 x = LeftBottom.X; x < (RightTop.X + 1); x++)
		{
			for (int32 y = LeftBottom.Y; y < LeftBottom.Y + Ynum; y++)
			{
				ULandscapeComponent* LandscapeComponent = mXYtoComponentMap.FindRef(FIntPoint(x, y));
				if (LandscapeComponent)
				{
					DeleteComponents.Add(LandscapeComponent);
				}

			}
		}

	}
	if (ComponentIndexY2 < LeftBottom.Y)
	{
		for (int32 x = LeftBottom.X; x < (RightTop.X + 1); x++)
		{
			for (int32 y = RightTop.Y; y > RightTop.Y - Ynum; y--)
			{
				ULandscapeComponent* LandscapeComponent = mXYtoComponentMap.FindRef(FIntPoint(x, y));
				if (LandscapeComponent)
				{
					DeleteComponents.Add(LandscapeComponent);
				}

			}
		}
	}
	for (ULandscapeComponent* DeleteComponent : DeleteComponents)
	{
		if (!DeleteComponent)continue;

		RemoveXYtoComponentMap(DeleteComponent);
	}
}

void URuntimeGenerateTerrain::UpdateLeftAndRightPoint()
{
	//更新左下及右上的坐标
	if (ComponentIndexX1 > RightTop.X)
	{
		LeftBottom.X = ComponentIndexX1 - LANDSCAPESIZE;
		RightTop.X = ComponentIndexX2;
	}
	if (ComponentIndexX2 < LeftBottom.X)
	{

		LeftBottom.X = ComponentIndexX1;
		RightTop.X = LANDSCAPESIZE + ComponentIndexX1;
	}
	//Y方向 
	if (ComponentIndexY1 > RightTop.Y)
	{
		RightTop.Y = ComponentIndexY2;
		LeftBottom.Y = ComponentIndexY1 - LANDSCAPESIZE;
	}
	if (ComponentIndexY2 < LeftBottom.Y)
	{

		LeftBottom.Y = ComponentIndexY1;
		RightTop.Y = LANDSCAPESIZE + ComponentIndexY1;

	}
}

void URuntimeGenerateTerrain::SetXYtoComponentMap(ULandscapeComponent* C)
{

	int XIndex = GetComponentIndexX(C);
	int YIndex = GetComponentIndexY(C);
	mXYtoComponentMap.Add(FIntPoint(XIndex, YIndex), C);
}

void URuntimeGenerateTerrain::RemoveXYtoComponentMap(ULandscapeComponent* C)
{

	int XIndex = GetComponentIndexX(C);
	int YIndex = GetComponentIndexY(C);
	mXYtoComponentMap.Remove(FIntPoint(XIndex, YIndex));
}

void URuntimeGenerateTerrain::StartAddComponent()
{
	bAddComponent = true;
}

void URuntimeGenerateTerrain::StopAddComponent()
{
	bAddComponent = false;
}

void URuntimeGenerateTerrain::CalComponentIndex()
{

	int XOffset = GetOwner()->GetActorLocation().X - ForeLocation.X;
	int YOffset = GetOwner()->GetActorLocation().Y - ForeLocation.Y;
	bool bXChange = false;
	bool bYChange = false;
	if (FMath::Abs(XOffset) > mLandscape->ComponentSizeQuads * mLandscape->GetActorScale().X)
	{
		if (XOffset > 0)
		{
			int Xnum = XOffset / (mLandscape->ComponentSizeQuads * mLandscape->GetActorScale().X);
			ComponentIndexX1 = RightTop.X + 1;
			ComponentIndexX2 = RightTop.X + Xnum;

		}
		else
		{
			int Xnum = FMath::Abs(XOffset) / (mLandscape->ComponentSizeQuads * mLandscape->GetActorScale().X);
			ComponentIndexX2 = LeftBottom.X - 1;
			ComponentIndexX1 = LeftBottom.X - Xnum;

		}
		bXChange = true;
	}
	else
	{
		ComponentIndexX1 = LeftBottom.X;
		ComponentIndexX2 = RightTop.X;
	}

	if (FMath::Abs(YOffset) > mLandscape->ComponentSizeQuads * mLandscape->GetActorScale().Y)
	{
		if (YOffset > 0)
		{
			int Ynum = YOffset / (mLandscape->ComponentSizeQuads * mLandscape->GetActorScale().Y);
			ComponentIndexY1 = RightTop.Y + 1;
			ComponentIndexY2 = RightTop.Y + Ynum;
		}
		else
		{
			int Ynum = FMath::Abs(YOffset) / (mLandscape->ComponentSizeQuads * mLandscape->GetActorScale().Y);
			ComponentIndexY2 = LeftBottom.Y - 1;
			ComponentIndexY1 = LeftBottom.Y - Ynum;
		}
		bYChange = true;
	}
	else
	{
		ComponentIndexY1 = LeftBottom.Y;
		ComponentIndexY2 = RightTop.Y;
	}

	if (bXChange || bYChange)
	{
		StartAddComponent();
		ForeLocation = GetOwner()->GetActorLocation();

	}


}

void URuntimeGenerateTerrain::ResetComponentIndex()
{
	ComponentIndexX1 = -1;
	ComponentIndexX2 = -1;
	ComponentIndexY1 = -1;
	ComponentIndexY2 = -1;
}

int URuntimeGenerateTerrain::GetComponentIndexX(ULandscapeComponent* C)
{
	return  C->GetRelativeTransform().GetLocation().X / mLandscape->ComponentSizeQuads;
}

int URuntimeGenerateTerrain::GetComponentIndexY(ULandscapeComponent* C)
{
	return C->GetRelativeTransform().GetLocation().Y / mLandscape->ComponentSizeQuads;
}



