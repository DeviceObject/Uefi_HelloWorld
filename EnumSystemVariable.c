#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>

#define INIT_NAME_BUFFER_SIZE		128
#define INIT_DATA_BUFFER_SIZE		1024

EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status;
	CHAR16 *FoundVarName,*OldName;
	UINT32 Atts;
	UINT8 *DataBuffer;
	EFI_GUID FoundVarGuid;
	UINTN DataSize,NameBufferSize,NameSize,OldNameBufferSize,DataBufferSize;
	NameBufferSize = INIT_NAME_BUFFER_SIZE;
	DataBufferSize = INIT_DATA_BUFFER_SIZE;
	FoundVarName = AllocateZeroPool(NameBufferSize);
	if (NULL == FoundVarName)
	{
		return EFI_OUT_OF_RESOURCES;
	}
	DataBuffer = AllocateZeroPool(DataBufferSize);
	if (NULL == DataBuffer)
	{
		FreePool(FoundVarName);
		return EFI_OUT_OF_RESOURCES;
	}
	for (;;)
	{
		NameSize = NameBufferSize;
		Status = gRT->GetNextVariableName(&NameSize,FoundVarName,&FoundVarGuid);
		if (EFI_BUFFER_TOO_SMALL == Status)
		{
			OldName = FoundVarName;
			OldNameBufferSize = NameBufferSize;

			NameBufferSize = NameSize > NameBufferSize * 2 ? NameSize : NameBufferSize * 2;
			FoundVarName = AllocateZeroPool(NameBufferSize);
			if (NULL == FoundVarName)
			{
				Status = EFI_OUT_OF_RESOURCES;
				FreePool(OldName);
				break;
			}
			CopyMem(FoundVarName,OldName,OldNameBufferSize);
			FreePool(OldName);
			NameSize = NameBufferSize;
			Status = gRT->GetNextVariableName(&NameSize,FoundVarName,&FoundVarGuid);
		}
		if (EFI_NOT_FOUND == Status)
		{
			break;
		}
		Print(L"%s\n",FoundVarName);
		DataSize = DataBufferSize;
		Status = gRT->GetVariable(FoundVarName,&FoundVarGuid,&Atts,&DataSize,DataBuffer);
		if (EFI_BUFFER_TOO_SMALL == Status)
		{

		}
	}
	return Status;
}