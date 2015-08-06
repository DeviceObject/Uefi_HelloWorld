#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	SystemTable->ConOut->OutputString(SystemTable->ConOut,L"ExitBootServices\n");
	{
		UINTN MemMapSize = 0;
		EFI_MEMORY_DESCRIPTOR *MemMap = 0;
		UINTN MapKey = 0;
		UINTN DesSize = 0;
		UINT32 DesVersion = 0;
		CHAR16* Vendor = SystemTable->FirmwareVendor;
		SystemTable->ConOut->OutputString(SystemTable->ConOut,Vendor);
		_asm int 3
		gBS->GetMemoryMap(&MemMapSize,MemMap,&MapKey,&DesSize,&DesVersion);
		gBS->ExitBootServices(ImageHandle,MapKey);
		_asm int 3
		ASSERT(SystemTable->BootServices == NULL);
		while (DesSize > 0)
		{

		}
		return (EFI_STATUS)-1;
	}
}