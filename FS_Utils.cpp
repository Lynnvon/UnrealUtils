// Copyright Shinban.Inc
/*!
 * file FS_Utils.cpp
 *
 * author Hailiang Feng
 * date 2019/09/06 12:09
 *
 * 
 */
#include "FS_Utils.h"
#include "Kismet/KismetMathLibrary.h"
#include "FlightSim.h"
#include "Engine/Texture2D.h"


//~~~ Image Wrapper ~~~
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
//~~~ Image Wrapper ~~~





EVisualHelmetDirection UFS_Utils::GetAxisHelmetDirection(float Val)
{
	static bool bDone = false;
	EVisualHelmetDirection dir = EVisualHelmetDirection::None;
	if (Val <= 1 && Val>0)
	{
		if (bDone) return dir;
	
		bDone = true;
		if (UKismetMathLibrary::InRange_FloatFloat(Val,0.0f,0.37f,false,false))
		{
			dir = EVisualHelmetDirection::Right;
			
		}else	if (UKismetMathLibrary::InRange_FloatFloat(Val,0.37f,0.74f,false,false))
		{
			dir = EVisualHelmetDirection::Down;
		}else	if(UKismetMathLibrary::InRange_FloatFloat(Val,0.74f,0.99f,false,false))
		{
			dir = EVisualHelmetDirection::Left;
		}
		else
		{
			dir = EVisualHelmetDirection::Up;

		}
	}
	else
	{	
		if (bDone)
		{
			dir = EVisualHelmetDirection::Origin;
		}
		bDone = false;
	}

	return dir;
}

FVector UFS_Utils::GetVectorBetweenTwoPointByAlpha(const FVector& Start,const FVector& End,const FVector& alpha)
{
	float _dis = FVector::Distance(Start,End);
	FVector _unitVector = GetUnitVector(Start,End);	
	FVector returnVal  = FVector(_unitVector.X* _dis * alpha.X,_unitVector.Y * _dis * alpha.Y,_unitVector.Z * _dis * alpha.Z) + Start;	
	return returnVal;
}

FVector UFS_Utils::GetVectorBetweenTwoPointByAlpha(const FVector& Start, const FVector& End, float alpha)
{
	float _dis = FVector::Distance(Start,End);
	FVector _unitVector = GetUnitVector(Start,End);	
	FVector returnVal  = FVector(_unitVector.X* _dis * alpha,_unitVector.Y * _dis * alpha,_unitVector.Z * _dis * alpha) + Start;	
	return returnVal;
}

FVector UFS_Utils::GetVectorWithCurve(const FVector& target, const FVector& curve)
{
	return FVector(target.X*curve.X,target.Y*curve.Y,target.Z*curve.Z);
}

FRotator UFS_Utils::GetRotatorBetweenTwoAngleByAlpha(const FRotator& Start,const FRotator& End,const FVector& alpha)
{	
	FRotator returnVal = FMath::Lerp(Start,End,alpha.X);
	return returnVal;
}

FRotator UFS_Utils::GetRotatorBetweenTwoAngleByAlpha(const FRotator& Start, const FRotator& End, float alpha)
{
	FRotator returnVal = FMath::Lerp(Start,End,alpha);
	return returnVal;
}

FVector UFS_Utils::GetUnitVector(const FVector& Start,const FVector& End)
{
	return (End - Start ).GetSafeNormal();
}
FRotator UFS_Utils::FindLookAtRotation(const FVector& Start, const FVector& Target)
{
	return MakeRotFromX(Target - Start);
}

FRotator UFS_Utils::MakeRotFromX(const FVector& X)
{
	return FRotationMatrix::MakeFromX(X).Rotator();
}

float UFS_Utils::GetVectorLength(FVector _vector)
{
	return _vector.Size();
}

FVector UFS_Utils::RotateVectorAroundAxis(FVector InVector, float Angle, FVector Axis)
{
	return InVector.RotateAngleAxis(Angle,Axis);
}

void UFS_Utils::SetRelativeLocationSmooth(USceneComponent* target, FVector newLocation,float Delta, float Speed)
{

	FVector TargetLocation = FMath::VInterpTo(target->RelativeLocation,newLocation,Delta,Speed);
	target->SetRelativeLocation(TargetLocation);
}

UTexture2D* UFS_Utils::LoadTexture2D_FromFile(const FString& FullFilePath, EImageFormat ImageFormat, bool& IsValid, int32& Width, int32& Height)
{
		
	IsValid = false;
	UTexture2D* LoadedT2D = NULL;
	
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
 
	//Load From File
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath)) return NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	  
	//Create T2D!
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{ 
		const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
			
			//Valid?
			if(!LoadedT2D) return NULL;
			//~~~~~~~~~~~~~~
			
			//Out!
			Width = ImageWrapper->GetWidth();
			Height = ImageWrapper->GetHeight();
			 
			//Copy!
			void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
			LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

			//Update!
			LoadedT2D->UpdateResource();
		}
	}
	 
	// Success!
	IsValid = true;
	return LoadedT2D;
}

FString UFS_Utils::GetTerrainConfigSection(FString section)
{

	FString val;
	if (!GConfig) return FString();
	FString RegistrySection = "Terrain";
	bool isSuccess = GConfig->GetString(
		*RegistrySection,
		(TEXT("%s"), *section),
		val,
		GGameIni
	);
	if (isSuccess)
	{
		return val;
	}
	return FString();

}
