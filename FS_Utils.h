// Copyright Shinban.Inc

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FlightSim.h"
#include "IImageWrapperModule.h"

#include "FS_Utils.generated.h"

class USceneComponent;
class UTexture2D;


/**
 * 工具类
 */
UCLASS()
class FLIGHTSIM_API UFS_Utils : public UObject
{
	GENERATED_BODY()
public:
	//根据视觉头盔的值 判断按下的方向
	static EVisualHelmetDirection GetAxisHelmetDirection(float Val);
	//根据alpha值获取两点连线中的一点
	static FVector GetVectorBetweenTwoPointByAlpha(const FVector& Start,const FVector& End,const FVector& alpha);
	static FVector GetVectorBetweenTwoPointByAlpha(const FVector& Start,const FVector& End,float alpha);
	//根据曲线 获取到目标点的
	static FVector GetVectorWithCurve(const FVector& target,const FVector& curve);
	//根据alpha值获取两角度之间的角度
	static FRotator GetRotatorBetweenTwoAngleByAlpha(const FRotator& Start,const FRotator& End,const FVector& alpha);
	static FRotator GetRotatorBetweenTwoAngleByAlpha(const FRotator& Start,const FRotator& End,float alpha);
	//获取两向量的单位向量
	static FVector GetUnitVector(const FVector& Start,const FVector& End);
	static FRotator FindLookAtRotation(const FVector& Start, const FVector& Target);
	static FRotator MakeRotFromX(const FVector& X);
	static float GetVectorLength(FVector _vector);
	static FVector RotateVectorAroundAxis(FVector InVector,float Angle,FVector Axis);
	static void SetRelativeLocationSmooth(USceneComponent* target,FVector newLocation,float Delta,float Speed = 1);

	static UTexture2D* LoadTexture2D_FromFile(const FString& FullFilePath,EImageFormat ImageFormat,bool& IsValid, int32& Width, int32& Height);

	static FString GetTerrainConfigSection(FString section);
};
