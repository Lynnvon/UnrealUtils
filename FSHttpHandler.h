// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpRequestAdapter.h"
#include "FSHttpHandler.generated.h"

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRegistryCompleteEvent);

	UCLASS()
	class FLIGHTSIM_API AFSHttpHandler : public AActor
	{
		GENERATED_BODY()

	public:
		// Sets default values for this actor's properties
		AFSHttpHandler();

		UFUNCTION(BlueprintCallable, Category = "Registry")
		void CheckIsRegitry();

		UFUNCTION(BlueprintCallable, Category = "Registry")
		void Regitry(FString serialNum);

		UFUNCTION(BlueprintCallable, Category = "Registry")
		void SetCategory(FString _category);

		UFUNCTION(BlueprintCallable, Category = "Registry")
		FString GetUniqueId();

		UFUNCTION(BlueprintPure,Category = "Registry")
		bool GetIsRegistry();

		UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Registry")
		FString Result;
		UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Registry")
		FString _id;
		UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Registry")
		FString Key;
		UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Registry")
		FString category;
		UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Registry")
		FString SavePath;
		UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Registry")
		FString ReqUrl;

		UPROPERTY(BlueprintAssignable, Category = "Registry")
		FRegistryCompleteEvent RegistryQueryEvent;

		UFUNCTION(BlueprintCallable, Category = "Registry")
		static FString EncodeStr(FString RawData, FString Keys);
		UFUNCTION(BlueprintCallable, Category = "Registry")
		static FString DecodeStr(FString Des, FString Keys, bool& _succ);
	private:
		bool IsRegistry;
		bool CheckInLocal();
		bool SaveFile(FString sectionName, FString section);
		FString GetSection(FString sectionName);	
		bool VerifyTime(FString serialNum) const;
		int64 GetTimeStamp() const;
	    bool AllisNum(FString str) const;
		bool verifyCode(FString id, FString serialNum);
	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
		const TCHAR* HashStringWithSHA1(const FString& InString) const;

		void HttpCompleteEvent(FHttpRequestPtr request, FHttpResponsePtr response, bool succees);
	public:
		// Called every frame
		virtual void Tick(float DeltaTime) override;



	};



