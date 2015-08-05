#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
//build -p Nt32Pkg\Nt32Pkg.dsc
EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status;
	UINTN Index;
	EFI_INPUT_KEY Key;
	CHAR16 StrBuffer[4];
	//__asm int 3
	gST->ConOut->OutputString(gST->ConOut,L"please enter any key\n");
	//wait input
	gBS->WaitForEvent(1,&gST->ConIn->WaitForKey,&Index);
	//__asm int 3
	Status = gST->ConIn->ReadKeyStroke(gST->ConIn,&Key);
	StrBuffer[0] = Key.UnicodeChar;
	if (StrBuffer[0] == L'q')
	{
		gST->ConOut->OutputString(gST->ConOut,L"Input \'q\'\n");
	}
	StrBuffer[1] = '\n';

	gST->ConOut->OutputString(gST->ConOut,StrBuffer);
	return EFI_SUCCESS;
}