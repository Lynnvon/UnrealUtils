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
 * ������
 */
UCLASS()
class FLIGHTSIM_API UFS_Utils : public UObject
{
	GENERATED_BODY()
public:
	//�����Ӿ�ͷ����ֵ �жϰ��µķ���
	static EVisualHelmetDirection GetAxisHelmetDirection(float Val);
	//����alphaֵ��ȡ���������е�һ��
	static FVector GetVectorBetweenTwoPointByAlpha(const FVector& Start,const FVector& End,const FVector& alpha);
	static FVector GetVectorBetweenTwoPointByAlpha(const FVector& Start,const FVector& End,float alpha);
	//�������� ��ȡ��Ŀ����
	static FVector GetVectorWithCurve(const FVector& target,const FVector& curve);
	//����alphaֵ��ȡ���Ƕ�֮��ĽǶ�
	static FRotator GetRotatorBetweenTwoAngleByAlpha(const FRotator& Start,const FRotator& End,const FVector& alpha);
	static FRotator GetRotatorBetweenTwoAngleByAlpha(const FRotator& Start,const FRotator& End,float alpha);
	//��ȡ�������ĵ�λ����
	static FVector GetUnitVector(const FVector& Start,const FVector& End);
	static FRotator FindLookAtRotation(const FVector& Start, const FVector& Target);
	static FRotator MakeRotFromX(const FVector& X);
	static float GetVectorLength(FVector _vector);
	static FVector RotateVectorAroundAxis(FVector InVector,float Angle,FVector Axis);
	static void SetRelativeLocationSmooth(USceneComponent* target,FVector newLocation,float Delta,float Speed = 1);

	static UTexture2D* LoadTexture2D_FromFile(const FString& FullFilePath,EImageFormat ImageFormat,bool& IsValid, int32& Width, int32& Height);

	static FString GetTerrainConfigSection(FString section);
};
