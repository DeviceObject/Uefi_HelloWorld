#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>

INTN ShellAppMain(IN UINTN Argc,IN CHAR16 **Argv)
{
	gST->ConOut->OutputString(gST->ConOut,L"HelloWorld\n");
	return 0;
}