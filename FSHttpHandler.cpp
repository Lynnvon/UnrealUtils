// Fill out your copyright notice in the Description page of Project Settings.

#include "FSHttpHandler.h"
#include "SecureHash.h"
#include "Http.h"
#include "Json.h"
#include "JsonSerializer.h"
#include "Engine.h"
#include <ctime>
#include "Misc/Base64.h"

//#if PLATFORM_WINDOWS
//#include "WindowsHWrapper.h"
//#include "AllowWindowsPlatformTypes.h"
//#endif
THIRD_PARTY_INCLUDES_START
#if PLATFORM_WINDOWS

#include <ThirdParty/CryptoPP/5.6.5/include/modes.h>
#include <ThirdParty/CryptoPP/5.6.5/include/hex.h>
#include <ThirdParty/CryptoPP/5.6.5/include/aes.h>
#elif PLATFORM_MAC
#elif PLATFORM_LINUX
#endif
THIRD_PARTY_INCLUDES_END

//#if PLATFORM_WINDOWS
//#include "HideWindowsPlatformTypes.h"
//#endif


#include "Kismet/KismetStringLibrary.h"


using namespace std;
using namespace CryptoPP;



// Sets default values
AFSHttpHandler::AFSHttpHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	IsRegistry = false;
	Result = "";
	Key = "ShinBan820710";
	category = "001";
	_id = "";
	SavePath = FPlatformProcess::UserSettingsDir();
	SavePath.Append(TEXT("System32/UserSetting"));
}

// Called when the game starts or when spawned
void AFSHttpHandler::BeginPlay()
{
	Super::BeginPlay();

}

const TCHAR* AFSHttpHandler::HashStringWithSHA1(const FString &InString) const
{
	FSHAHash StringHash;
	FSHA1::HashBuffer(TCHAR_TO_ANSI(*InString), InString.Len(), StringHash.Hash);
	return *(StringHash.ToString());
}


void AFSHttpHandler::CheckIsRegitry()
{
	//Create a writer and hold it in this FString
	FString JsonStr;
	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR> > > JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR> >::Create(&JsonStr);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("id"), _id);
	JsonWriter->WriteValue(TEXT("category"), category);
	JsonWriter->WriteObjectEnd();

	// Close the writer and finalize the output such that JsonStr has what we want
	JsonWriter->Close();


	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetURL(ReqUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetContentAsString(JsonStr);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AFSHttpHandler::HttpCompleteEvent);
	HttpRequest->ProcessRequest();
}

void AFSHttpHandler::Regitry(FString serialNum)
{
	SaveFile("s", serialNum);
}

void AFSHttpHandler::SetCategory(FString _category)
{
	//Set savepath
	category = _category;
	SavePath.Append(_category);
	SavePath.Append(TEXT(".ini"));
	//SET ID 
	FString _temp = FString((TEXT("%s"), *GetUniqueId()));
	_id = FMD5::HashAnsiString(*_temp);
}

// Called every frame
void AFSHttpHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFSHttpHandler::HttpCompleteEvent(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		Result = TEXT("网络出错！");
		CheckInLocal();
		this->RegistryQueryEvent.Broadcast();
		return;
	}

	FString JsonValue = Response->GetContentAsString();

	/** 创建服务器返回的内容 */
	if (!Response.IsValid())
	{
		Result = TEXT("网络出错！");
		AFSHttpHandler::CheckInLocal();
		this->RegistryQueryEvent.Broadcast();
		return;
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef< TJsonReader<TCHAR> > JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

		bool BFlag = FJsonSerializer::Deserialize(JsonReader, JsonObject);
		/** 判断是否解析成功 */
		if (BFlag)
		{
			FString serialNum = FString();
			bool bIsRegistry = false;
			JsonObject->GetObjectField("result")->TryGetStringField("serialNum", serialNum);

			JsonObject->GetObjectField("result")->TryGetBoolField("isRegistry", bIsRegistry);

		
			if (!bIsRegistry)
			{
				Result = TEXT("未注册，请注册使用！");
				SaveFile("s", FString());
				IsRegistry = false;
			}
			else if (SaveFile("s", serialNum))
			{
				Result = TEXT("保存文件成功，已注册");
				IsRegistry = true;
			}
			else
			{
				Result = TEXT("保存文件失败，已注册");
				IsRegistry = true;

			}
		}
		else
		{
			Result = TEXT("解析失败！");
			CheckInLocal();
		}
	}
	else
	{
		Result = TEXT("网络出错，请求失败！");
		CheckInLocal();
	}

	this->RegistryQueryEvent.Broadcast();
}


FString AFSHttpHandler::GetUniqueId()
{

	FString tempid;
	/*tempid += FPlatformMisc::GetOperatingSystemId();
	if (tempid.IsEmpty())
	{
		TArray<uint8> MacAddr = FPlatformMisc::GetMacAddress();
		for (TArray<uint8>::TConstIterator it(MacAddr); it; ++it)
		{
			tempid += FString::Printf(TEXT("%02x"), *it);
		}
		tempid.Left(tempid.Len()-2);
	}*/
	TArray<uint8> MacAddr = FPlatformMisc::GetMacAddress();
	for (TArray<uint8>::TConstIterator it(MacAddr); it; ++it)
	{
		tempid += FString::Printf(TEXT("%02x"), *it);
	}
	//UE_LOG(LogTemp, Warning, TEXT("id----------%s"), *tempid);

	//// Check to see if we already have a valid machine ID to use
	//if (!FPlatformMisc::GetStoredValue(TEXT("Epic Games"), TEXT("Unreal Engine/Identifiers"), TEXT("now"), nowString))
	//{
	//	// No valid machine ID, generate and save a new one
	//	int64 now = FDateTime::Now().ToUnixTimestamp();
	//	nowString = FString::FromInt(now);

	//	if (!FPlatformMisc::SetStoredValue(TEXT("Epic Games"), TEXT("Unreal Engine/Identifiers"), TEXT("now"), nowString))
	//	{
	//		// Failed to persist the machine ID - reset it to zero to avoid returning a transient value
	//		nowString = "123";
	//	}
	//}

	FString nowString = GetSection("n");

	if (nowString.IsEmpty())
	{
		nowString = EncodeStr(FString::FromInt(AFSHttpHandler::GetTimeStamp()), Key);
		SaveFile("n", nowString);

	}

	bool isSuccess;

	tempid += DecodeStr(nowString, Key, isSuccess);

	tempid += category;


	return tempid;
}

bool AFSHttpHandler::GetIsRegistry()
{
	return IsRegistry;
}

bool AFSHttpHandler::CheckInLocal()
{
	try
	{
		FString serialNum = GetSection("s");
		if (verifyCode(_id, serialNum))
		{
			IsRegistry = true;

		}
		else
		{
			IsRegistry = false;
		}

		return true;
	}
	catch (const std::exception&)
	{
		IsRegistry = false;
		return false;
	}

}

bool AFSHttpHandler::SaveFile(FString sectionName, FString section)
{

	if (!GConfig) return false;
	GConfig->Flush(true, SavePath);
	//New Section to Add  
	FString RegistrySection = "Registry";
	//String  

	GConfig->SetString(
		*RegistrySection,
		(TEXT("%s"), *sectionName),
		(TEXT("%s"), *section),
		SavePath
	);
	this->SaveConfig(CPF_Config, *SavePath);
	GConfig->Flush(false, SavePath);

	return true;
}

FString AFSHttpHandler::GetSection(FString sectionName)
{
	FString val;
	if (!GConfig) return FString();
	GConfig->Flush(true, SavePath);
	FString RegistrySection = "Registry";
	bool isSuccess = GConfig->GetString(
		*RegistrySection,
		(TEXT("%s"), *sectionName),
		val,
		SavePath
	);
	GConfig->Flush(false, SavePath);
	if (isSuccess)
	{
		return val;
	}
	return FString();
}
bool AFSHttpHandler::verifyCode(FString id, FString serialNum)
{

	FString L_Serial;
	FString R_time;
	/*UE_LOG(LogTemp, Warning, TEXT("----%s"), *id);

	UE_LOG(LogTemp, Warning, TEXT("----%s"), *EncodeStr(id, Key));*/
	try
	{
		bool result = serialNum.Split("-", &L_Serial, &R_time, ESearchCase::IgnoreCase, ESearchDir::FromStart);

		if (!result)
		{
			Result.Append(TEXT("  序列号解析失败！  "));

			return false;

		}
		FString decodeStr = EncodeStr(id, Key);

		if (L_Serial.Equals(decodeStr))
		{
			if (AFSHttpHandler::VerifyTime(R_time))
			{
				Result.Append(TEXT("  使用愉快！  "));

				return true;
			}
			else
			{
				Result.Append(TEXT("  注册到期！  "));
				SaveFile("s", FString());
				return false;

			}

		}

		Result.Append(TEXT("  序列号不正确,请重试！  "));
		/*	Result.Append((TEXT("  id:---------- %s "), *id));
			Result.Append((TEXT("  L_Serial:----------%s  "), *L_Serial));
			Result.Append((TEXT("  decodeStr:---------- %s "), *decodeStr));*/

		return false;
	}
	catch (...)
	{
		Result.Append(TEXT("  序列号不正确，异常！  "));

		return false;
	}



}



//加密字符串
FString AFSHttpHandler::EncodeStr(FString aes_content, FString aes_key)
{

	std::string sKey = TCHAR_TO_UTF8(*aes_key);
	const char *plainText = TCHAR_TO_ANSI(*aes_content);
	std::string outstr;

	//填key    
	SecByteBlock key(AES::MAX_KEYLENGTH);
	memset(key, 0x30, key.size());
	sKey.size() <= AES::MAX_KEYLENGTH ? memcpy(key, sKey.c_str(), sKey.size()) : memcpy(key, sKey.c_str(), AES::MAX_KEYLENGTH);


	AES::Encryption aesEncryption((byte *)key, AES::MAX_KEYLENGTH);

	CryptoPP::ECB_Mode_ExternalCipher::Encryption ecbEncryption(aesEncryption);
	CryptoPP::StreamTransformationFilter ecbEncryptor(
		ecbEncryption,
		new CryptoPP::HexEncoder(new StringSink(outstr)),
		BlockPaddingSchemeDef::BlockPaddingScheme::ZEROS_PADDING,
		true
	);
	ecbEncryptor.Put((byte *)plainText, strlen(plainText));
	ecbEncryptor.MessageEnd();

	return UTF8_TO_TCHAR(outstr.c_str());
}

//解密字符串
FString AFSHttpHandler::DecodeStr(FString aes_content, FString aes_key, bool& _isSuc)
{
	std::string sKey = TCHAR_TO_UTF8(*aes_key);
	const char *cipherText = TCHAR_TO_ANSI(*aes_content);
	std::string outstr;
	try
	{
		//填key    
		SecByteBlock key(AES::MAX_KEYLENGTH);
		memset(key, 0x30, key.size());
		sKey.size() <= AES::MAX_KEYLENGTH ? memcpy(key, sKey.c_str(), sKey.size()) : memcpy(key, sKey.c_str(), AES::MAX_KEYLENGTH);

		ECB_Mode<AES >::Decryption ecbDecryption((byte *)key, AES::MAX_KEYLENGTH);

		HexDecoder decryptor(new StreamTransformationFilter(ecbDecryption, new StringSink(outstr), BlockPaddingSchemeDef::BlockPaddingScheme::ZEROS_PADDING,
			true
		));
		decryptor.Put((byte *)cipherText, strlen(cipherText));
		decryptor.MessageEnd();
		_isSuc = true;
	}
	catch (...)
	{
		outstr = "error";
		_isSuc = false;
	}
	return UTF8_TO_TCHAR(outstr.c_str());
}

//检查时间
bool AFSHttpHandler::VerifyTime(FString _time) const
{

	try
	{
		bool isSuccess;
		FString time_registry = DecodeStr(_time, Key, isSuccess);



		if (isSuccess)
		{
			if (!AllisNum(time_registry))
			{
				UE_LOG(LogTemp, Log, TEXT("注册期限解析失败"));
				return false;
			}
			/*if (UKismetStringLibrary::Conv_StringToInt(time_registry) > AFSHttpHandler::GetTimeStamp())
			{
			return true;
			}*/
			string aa = TCHAR_TO_UTF8(*time_registry);
			int64 tempTime = atoi(aa.c_str());
			if (tempTime > GetTimeStamp())
			{
				UE_LOG(LogTemp, Log, TEXT("注册期限解析成功"));

				return true;
			}

		}

	}
	catch (...)
	{
		UE_LOG(LogTemp, Log, TEXT("注册期限异常！"));

		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("注册期限过期"));

	return false;

}

int64 AFSHttpHandler::GetTimeStamp() const
{
	int64 now = FDateTime::Now().ToUnixTimestamp();

	return now;
}

bool AFSHttpHandler::AllisNum(FString str) const
{
	string temp = TCHAR_TO_UTF8(*str);
	for (int i = 0; i < temp.size(); i++)
	{
		int tmp = static_cast<int>(str[i]);
		if (tmp >= 48 && tmp <= 57)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}









