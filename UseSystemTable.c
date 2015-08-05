#include <Uefi.h>
//build -p Nt32Pkg\Nt32Pkg.dsc
EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status;
	UINTN Index;
	EFI_INPUT_KEY Key;
	CHAR16 StrBuffer[3];
	//__asm int 3
	SystemTable->BootServices->WaitForEvent(1,&SystemTable->ConIn->WaitForKey,&Index);
	//wait input
	Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn,&Key);
	//__asm int 3
	StrBuffer[0] = Key.UnicodeChar;
	if (StrBuffer[0] == L'q')
	{
		SystemTable->ConOut->OutputString(SystemTable->ConOut,L"Input \'q\'\n");
	}
	StrBuffer[1] = '\n';

	SystemTable->ConOut->OutputString(SystemTable->ConOut,StrBuffer);
	return EFI_SUCCESS;
}