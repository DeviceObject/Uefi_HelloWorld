EFI_STATUS DetectBootablePartition(void *Bootable)
{
	EFI_STATUS	Status;
	EFI_HANDLE	*ControllerHandles = NULL;
	UINTN		HandleIndex,NumHandles;
	//find all setup SimpleFileSystemProtocol 's Handle and find all FAT partition table
	Status = gBS->LocateHandleBuffer(ByProtocol,&gEfiSimpleFileSystemProtocolGuid,NULL,&NumHandles,&ControllerHandles);
	if (EFI_ERROR(Status))
	{
		return Status;
	}
	//enum finder Handle
	for (HandleIndex = 0;HandleIndex < NumHandles;HandleIndex++)
	{
		EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
		//open on Handle's SimpleFileSystemProtocol
		Status = gBS->HandleProtocol(ControllerHandles[HandleIndex],&gEfiSimpleFileSystemProtocolGuid,(VOID **)&SimpleFileSystem);
		//check eif\boot\bootx64.efi file
	}
	//free ControllerHandle memory area
	if (ControllerHandles != NULL)
	{
		Status = gBS->FreePool(ControllerHandles);
	}
	return Status;
}