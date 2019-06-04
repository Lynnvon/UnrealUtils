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
	static void LeftMouseButtonPressed();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void LeftMouseButtonReleased();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void UpButtonPressed();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void UpButtonReleased();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void DownButtonPressed();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void DownButtonReleased();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void LeftButtonPressed();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void LeftButtonReleased();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void RightButtonPressed();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void RightButtonReleased();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void EnterButtonPressed();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void EnterButtonReleased();
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void ButtonPressed(int keycode);
	UFUNCTION(BlueprintCallable, Category = "Flight|UI")
	static void ButtonReleased(int keycode);


	
};
