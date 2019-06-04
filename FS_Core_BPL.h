/*!
 * file FS_Core_BPL.h
 *
 * author Hailiang Feng
 * date 2019/01/08 15:51
 *
 * 
 */
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FS_Core_BPL.generated.h"

class AFS_Pawn;
class AFSPlayerController;
class AFSGameMode;
class UFSGameInstance;

/**
 *
 */
UCLASS()
class FLIGHTSIM_API UFS_Core_BPL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:	
	/** Returns the FSPlayerPawn... */
	UFUNCTION(BlueprintPure, Category = "FS", meta = (WorldContext = "WorldContextObject"))
		static AFS_Pawn* FSGetPlayerPawn(UObject* WorldContextObject,int index = 0);
	/** Returns the FSGameController. */
	UFUNCTION(BlueprintPure, Category = "FS", meta = (WorldContext = "WorldContextObject"))
		static AFSPlayerController*	FSGetGameController(UObject* WorldContextObject,int index = 0);

	/** Returns the FSGameInstance. */
	UFUNCTION(BlueprintPure, Category = "FS", meta = (WorldContext = "WorldContextObject"))
		static UFSGameInstance*	FSGetGameInstance(UObject* WorldContextObject);

	/** Returns the FSGameMode. */
	UFUNCTION(BlueprintPure, Category = "FS", meta = (WorldContext = "WorldContextObject"))
		static AFSGameMode* FSGetGameMode(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void LeftMouseButtonDown();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void LeftMouseButtonUp();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void UpButtonDown();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void UpButtonUp();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void DownButtonDown();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void DownButtonUp();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void LeftButtonDown();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void LeftButtonUp();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void RightButtonDown();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void RightButtonUp();
	
};
