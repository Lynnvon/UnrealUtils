/*!
 * file FS_Core_BPL.cpp
 *
 * author Hailiang Feng
 * date 2019/01/08 16:29
 *
 * 
 */

#include "FS_Core_BPL.h"
#include "FSGameInstance.h"
#include "FSGameMode.h"
#include "FSPlayerController.h"
#include "FS_Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "AllowWindowsPlatformTypes.h"
#include "WinUser.h"


AFS_Pawn * UFS_Core_BPL::FSGetPlayerPawn(UObject * WorldContextObject, int index)
{
	return Cast<AFS_Pawn>(UGameplayStatics::GetPlayerPawn(WorldContextObject,index));
}
AFSPlayerController * UFS_Core_BPL::FSGetGameController(UObject * WorldContextObject, int index)
{
	return Cast<AFSPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject,index));
}

UFSGameInstance * UFS_Core_BPL::FSGetGameInstance(UObject * WorldContextObject)
 {
	 return Cast<UFSGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
 }

AFSGameMode * UFS_Core_BPL::FSGetGameMode(UObject * WorldContextObject)
 {
	 return Cast<AFSGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
 }


 void UFS_Core_BPL::LeftMouseButtonDown()
{
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dx = 256; // If you need to set the position
    Input.mi.dy = 256; // If you need to set the position
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &Input, sizeof(INPUT));

}

void UFS_Core_BPL::LeftMouseButtonUp()
{
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dx = 256; // If you need to set the position
    Input.mi.dy = 256; // If you need to set the position
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &Input, sizeof(INPUT));
}
void UFS_Core_BPL::UpButtonDown()
{
    INPUT Input = { 0 };
    Input.type       = INPUT_KEYBOARD;
    Input.mi.dwFlags = KEYEVENTF_EXTENDEDKEY;
    Input.ki.wVk   = VK_UP; 
    SendInput( 1, &Input, sizeof( INPUT ) );

}

void UFS_Core_BPL::UpButtonUp()
{
     INPUT Input = { 0 };
    Input.type       = INPUT_KEYBOARD;
    Input.mi.dwFlags = KEYEVENTF_KEYUP;
    Input.ki.wVk   = VK_UP; 
    SendInput( 1, &Input, sizeof( INPUT ) );
}
void UFS_Core_BPL::DownButtonDown()
{
    INPUT Input = { 0 };
    Input.type       = INPUT_KEYBOARD;
    Input.mi.dwFlags = KEYEVENTF_EXTENDEDKEY;
    Input.ki.wVk   = VK_DOWN; 
    SendInput( 1, &Input, sizeof( INPUT ) );

}

void UFS_Core_BPL::DownButtonUp()
{
     INPUT Input = { 0 };
    Input.type       = INPUT_KEYBOARD;
    Input.mi.dwFlags = KEYEVENTF_KEYUP;
    Input.ki.wVk   = VK_DOWN; 
    SendInput( 1, &Input, sizeof( INPUT ) );
}
void UFS_Core_BPL::LeftButtonDown()
{
    INPUT Input = { 0 };
    Input.type       = INPUT_KEYBOARD;
    Input.mi.dwFlags = KEYEVENTF_EXTENDEDKEY;
    Input.ki.wVk   = VK_LEFT; 
    SendInput( 1, &Input, sizeof( INPUT ) );

}

void UFS_Core_BPL::LeftButtonUp()
{
     INPUT Input = { 0 };
    Input.type       = INPUT_KEYBOARD;
    Input.mi.dwFlags = KEYEVENTF_KEYUP;
    Input.ki.wVk   = VK_LEFT; 
    SendInput( 1, &Input, sizeof( INPUT ) );
}
void UFS_Core_BPL::RightButtonDown()
{
    INPUT Input = { 0 };
    Input.type       = INPUT_KEYBOARD;
    Input.mi.dwFlags = KEYEVENTF_EXTENDEDKEY;
    Input.ki.wVk   = VK_RIGHT; 
    SendInput( 1, &Input, sizeof( INPUT ) );

}

void UFS_Core_BPL::RightButtonUp()
{
     INPUT Input = { 0 };
    Input.type       = INPUT_KEYBOARD;
    Input.mi.dwFlags = KEYEVENTF_KEYUP;
    Input.ki.wVk   = VK_RIGHT; 
    SendInput( 1, &Input, sizeof( INPUT ) );
}



