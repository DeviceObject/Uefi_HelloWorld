#include <Uefi>
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServiceTableLib.h>
#include <Library/PrintLib.h>
#include <Protocol/DiskIo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DevicePathToNext.h>
#include <Uefi/UefiGpt.h>
#include <Library/DevicePathLib.h>

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle,IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status;
	UINT HandleIndex,HandleCount;
	EFI_HANDLE *DiskControllerHandles = NULL;
	EFI_DISK_IO_PROTOCOL *DiskIo;
	//finder all EFI_DISK_IO_PROTOCOL's device
	Status = gBS->LocateHandleBuffer(ByProtocol,&gEfiDiskIoProtocolGuid,NULL,&HandleCount,&DiskControllerHandles);
	if (!EFI_ERROR(Status))
	{
		CHAR8 gptHeaderSector[512];
		EFI_PARTITION_TABLE_HEADER gptHeader = (EFI_PARTITION_TABLE_HEADER*)gptHeaderSector;
		for (HandleIndex = 0;HandleIndex < HandleCount;HandleIndex++)
		{
			//open EFI_DISK_IO_PROTOCOL
			Status = gBS->HandleProtocol(DiskControllerHandles[HandleIndex],&gEfiDiskIoProtocolGuid,(VOID**)&DiskIo);
			if (!EFI_ERROR(Status))
			{
				EFI_DEVICE_PATH_PROTOCOL *DiskDevicePath;
				EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *Device2TextProtocol = 0;
				CHAR16 *TextDevicePath = 0;
				//open EFI_DEVICE_PATH_PROTOCOL
				Status = gBS->OpenProtocol(DiskControllerHandles[HandleIndex], \
					&gEfiDevicePathProtocolGuid, \
					(VOID**)&DiskDevicePath, \
					ImageHandle, \
					NULL, \
					EFI_OPEN_PROTOCOL_GETPROTOCOL);
				if (!EFI_ERROR(Status))
				{
					if (Device2TextProtocol == 0)
					{
						Status = gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid,NULL,(VOID**)&Device2TextProtocol);
						//use EFI_DEVICE_PATH_PROTOCOL get text format DevicePath
						TextDevicePath = Device2TextProtocol->ConvertDevicePathToText(DiskDevicePath,TRUE,TRUE);
						Print(L"%s\n",TextDevicePath);
						if (TextDevicePath)
						{
							gBs->FreePool(TextDevicePath);
							Status = gBS->CloseProtocol(DiskControllerHandles[HandleIndex],&gEfiDevicePathProtocolGuid,ImageHandle,NULL);
						}
					}
				}
				EFI_BLOCK_IO_PROTOCOL *BlockIo = *(EFI_BLOCK_IO_PROTOCOL**)(DiskIo + 0x01);
				EFI_BLOCK_IO_MEDIA *Media = BlockIo->Media;
				//read first sector
				Status = DiskIo->ReadDisk(DiskIo,Media->MediaId,512,512,gptHeader);
				//check GPT Signature
				if (!EFI_ERROR(Status) && (gptHeader->Header.Signature == 0x5452415020494645))
				{
					UINT32 CRCSum,GPTHeaderCRCSum = (gptHeader->Header.CRC32);
					gptHeader->Header.CRC32 = 0;
					gBS->CalculateCrc32(gptHeader,gptHeader->Header.HeaderSize,&CRCSum);
					if (GPTHeaderCRCSum == CRCSum)
					{
						//finder right GPT Header
					}
				}
			}
		}
		gBS->FreePool(DiskControllerHandles);
	}
}