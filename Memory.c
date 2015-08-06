#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

EFI_STATUS TestGetMemoryMap()
{
	EFI_STATUS Status = 0;
	UINTN MemoryMapSize = 0;
	EFI_MEMORY_DESCRIPTOR *MemoryMap = 0;
	UINTN MapKey = 0;
	UINTN DescriptorSize = 0;
	UINTN i;
	UINT32 DescriptorVersion = 0;
	EFI_MEMORY_DESCRIPTOR *MmMap = 0;

	Status = gBS->GetMemoryMap(&MemoryMapSize, \
		MemoryMap, \
		&MapKey, \
		&DescriptorSize, \
		&DescriptorVersion);
	if (Status != EFI_BUFFER_TOO_SMALL)
	{
		return Status;
	}
	Status = gBS->AllocatePool(EfiBootServicesData, \
		MemoryMapSize, \
		&MemoryMap);
	Status = gBS->GetMemoryMap(&MemoryMapSize, \
		MemoryMap, \
		&MapKey, \
		&DescriptorSize, \
		&DescriptorVersion);
	for (i = 0;i < MemoryMapSize / DescriptorSize;i++)
	{
		MmMap = (EFI_MEMORY_DESCRIPTOR *)(((CHAR8*)MemoryMap) + i * DescriptorSize);
		Print(L"MemoryMap %4d %10d :", MmMap[0].Type, MmMap[0].NumberOfPages);
		Print(L"%10lx<->",MmMap[0].PhysicalStart);
		Print(L"%10lx\n",MmMap[0].VirtualStart);
	}
	Status = gBS->FreePool(MemoryMap);
	return Status;
}
EFI_STATUS TestAllocateAddress()
{
	EFI_STATUS Status = 0;
	EFI_PHYSICAL_ADDRESS Pages = 1024 * 1024 * 10;

	Status = gBS->AllocatePages(AllocateAddress, \
		EfiBootServicesData, \
		3, \
		&Pages);
	Print(L"AllocateAddress:%r %x\n",Status,Pages);
	if(Status == 0)
	{
		CHAR16* str;
		str = (CHAR16*)Pages;
		str[0] = 'H';
		str[1] = 0;
		Print(L"Test Str:%s\n",str);
		Status = gBS->FreePages(Pages,3);
	}
	else
	{
	} 
	return Status;
}
EFI_STATUS TestAllocateAnyPages()
{
	EFI_STATUS Status;
	EFI_PHYSICAL_ADDRESS Pages;

	Status = gBS->AllocatePages(AllocateAnyPages, \
		EfiBootServicesData, \
		3, \
		&Pages);
	if(Status == 0)
	{
		CHAR16* str;
		str = (CHAR16*)Pages;
		str[0] = 'H';
		str[1] = 0;
		Print(L"Test Str:%s\n",str);
		Status = gBS->FreePages(Pages,3);
	}
	else
	{
	} 
	return Status;
}
EFI_STATUS UefiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status = 0;
	Status = TestAllocateAddress();
	Status = TestAllocateAnyPages();
	Status = TestGetMemoryMap();
	return Status;
}