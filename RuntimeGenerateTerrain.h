// Copyright Shinban.Inc

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FS_ImageLoader.h"

#include "RuntimeGenerateTerrain.generated.h"

class ALandscapeProxy;
class UMaterialInstanceDynamic;
class UMaterialInstance;
class ULandscapeComponent;

#define LANDSCAPESIZE 31

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLIGHTSIM_API URuntimeGenerateTerrain : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URuntimeGenerateTerrain();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	void InitLandscapeComponent();

	bool LoadTexture(ULandscapeComponent* C);
	bool AsyncLoadTexture(ULandscapeComponent* C);

	void MoveLandscapeComponent();
	void RemoveLandscapeComponent();
	void UpdateLeftAndRightPoint();

	void SetXYtoComponentMap(ULandscapeComponent* C);
	void RemoveXYtoComponentMap(ULandscapeComponent* C);

	void StartAddComponent();

	void StopAddComponent();

	void CalComponentIndex();

	void ResetComponentIndex();

	int GetComponentIndexX(ULandscapeComponent* C);
	int GetComponentIndexY(ULandscapeComponent* C);
	UFUNCTION()
	void LoadTextureComplete(UTexture2D* texture);



public:
	ALandscapeProxy* mLandscape = nullptr;

	UMaterialInstanceDynamic* GI;
	UMaterialInstance* SounceMaterial;
	TMap<FIntPoint, ULandscapeComponent*> mXYtoComponentMap;

private:
	bool bAddComponent:1;
	int ComponentIndexX1 = -1;
	int ComponentIndexY1 = -1;
	int ComponentIndexX2 = -1;
	int ComponentIndexY2 = -1;
	FVector ForeLocation = FVector::ZeroVector;
	FVector2D LeftBottom = FVector2D(0,0);
	FVector2D RightTop = FVector2D(LANDSCAPESIZE,LANDSCAPESIZE);

	TArray<ULandscapeComponent*> DeleteComponents;
	TArray<ULandscapeComponent*> ReadyForTextureMID;

	




};

