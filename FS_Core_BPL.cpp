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


 void UFS_Core_BPL::LeftMouseButtonPressed()
{
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dx = 256; // If you need to set the position
    Input.mi.dy = 256; // If you need to set the position
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &Input, sizeof(INPUT));

}

void UFS_Core_BPL::LeftMouseButtonReleased()
{
    INPUT Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dx = 256; // If you need to set the position
    Input.mi.dy = 256; // If you need to set the position
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &Input, sizeof(INPUT));
}
void UFS_Core_BPL::UpButtonPressed()
{
    keybd_event(VK_UP, 0, 0, 0);   

}

void UFS_Core_BPL::UpButtonReleased()
{
    keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);   

}
void UFS_Core_BPL::DownButtonPressed()
{
    keybd_event(VK_DOWN, 0, 0, 0);   

}

void UFS_Core_BPL::DownButtonReleased()
{
    keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);   
}
void UFS_Core_BPL::LeftButtonPressed()
{
    keybd_event(VK_LEFT, 0, 0, 0);   

}

void UFS_Core_BPL::LeftButtonReleased()
{
    keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);   
}
void UFS_Core_BPL::RightButtonPressed()
{
    keybd_event(VK_RIGHT, 0, 0, 0);   

}

void UFS_Core_BPL::RightButtonReleased()
{
    keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);   

}

void UFS_Core_BPL::EnterButtonPressed()
{
    keybd_event(VK_RETURN, 0, 0, 0); 


}

void UFS_Core_BPL::EnterButtonReleased()
{
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);   

}

void UFS_Core_BPL::ButtonPressed(int keycode)
{
   keybd_event(keycode, 0, 0, 0); 

}
void UFS_Core_BPL::ButtonReleased(int keycode)
{
   keybd_event(keycode, 0, KEYEVENTF_KEYUP, 0);   

}



