/** @file

  Copyright (c) 2018, Peter Kirmeier <topeterk@freenet.de>. All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "DumpHii.h"

EFI_HII_DATABASE_PROTOCOL * pHiiDbProtocol;

/**
  Main entry point of the dynamic Shell extension driver.

  @param[in]  ImageHandle   The firmware allocated handle for the present driver UEFI image.
  @param[in]  *SystemTable  A pointer to the EFI System table.

  @retval  EFI_SUCCESS           The driver was initialized.
  @retval  EFI_OUT_OF_RESOURCES  The "End of DXE" event could not be allocated or
                                 there was not enough memory in pool to install
                                 the Shell Dynamic Command protocol.
  @retval  EFI_LOAD_ERROR        Unable to add the HII package.

**/
EFI_STATUS
DumpHiiEntryPoint(
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_BOOT_SERVICES * pBS = SystemTable->BootServices;
    EFI_STATUS  Status;
	UINTN HiiPkgListSize = 0;
	EFI_HII_PACKAGE_LIST_HEADER * pHiiPkgLists = NULL;
	EFI_HII_PACKAGE_LIST_HEADER * pHiiPkgListEntry;
	UINTN PkgCount = 0;
	UINTN PkgCountOk = 0;

    DEBUG((EFI_D_INFO, "DumpHiiEntryPoint()\n"));

	Print(L"  Running Dump HII v1.0\n  by Peter Kirmeier\n  https://github.com/topeterk/DumpHii \n\n");

	if (PcdGetBool(PcdShellLibAutoInitialize) == 0)
	{
		// We are most likely compiled within shell package, but we build an external application.
		// So, if PCD is set to build shell internal tools, we have to call constructor on our own.
		// Basically this allows us to build shell applications using ShellLib together with the shell.
		ShellLibConstructorWorker(ImageHandle, SystemTable);
	}

	Status = pBS->LocateProtocol(&gEfiHiiDatabaseProtocolGuid, NULL, &pHiiDbProtocol);
	if (EFI_ERROR(Status))
	{
		DEBUG((EFI_D_WARN, "Cannot found HII database protocol - %r \n", Status));
		return EFI_DEVICE_ERROR;
	}

	// 1st get all packages
	Status = pHiiDbProtocol->ExportPackageLists(
		pHiiDbProtocol,
		NULL,
		&HiiPkgListSize,
		pHiiPkgLists);
	if (EFI_BUFFER_TOO_SMALL != Status)
	{
		DEBUG((EFI_D_ERROR, "Could not get package list size %r\n", Status));
		return EFI_PROTOCOL_ERROR;
	}

	Status = pBS->AllocatePool(EfiBootServicesData, HiiPkgListSize, &pHiiPkgLists);
	if (EFI_ERROR(Status))
	{
		DEBUG((EFI_D_ERROR, "Could not allocate package list %r\n", Status));
		return EFI_OUT_OF_RESOURCES;
	}

	Status = pHiiDbProtocol->ExportPackageLists(
		pHiiDbProtocol,
		NULL,
		&HiiPkgListSize,
		pHiiPkgLists);
	if (EFI_ERROR(Status))
	{
		DEBUG((EFI_D_ERROR, "Could not receive package list %r\n", Status));
		pBS->FreePool(pHiiPkgLists);
		return EFI_PROTOCOL_ERROR;
	}

	// 2nd iterate packages and store them in files
	for (pHiiPkgListEntry = pHiiPkgLists; (UINTN)pHiiPkgListEntry < ((UINTN)pHiiPkgLists) + HiiPkgListSize; pHiiPkgListEntry = (EFI_HII_PACKAGE_LIST_HEADER*)((UINTN)pHiiPkgListEntry + pHiiPkgListEntry->PackageLength))
	{
		SHELL_FILE_HANDLE FileHandle;
		UINTN size = pHiiPkgListEntry->PackageLength - sizeof(*pHiiPkgListEntry);
		CHAR16 FileName[64];
		PkgCount++;

		UnicodeSPrint(FileName, sizeof(FileName), L"HPK\\%g.hpk", &pHiiPkgListEntry->PackageListGuid);
		Print(L"Dumping package to \"%s\"... ", FileName);

		Status = ShellOpenFileByName(FileName, &FileHandle, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE, 0);
		if (EFI_ERROR(Status))
		{
			Print(L"Failed (Cannot open file for writing = %r)\n", Status);
			DEBUG((EFI_D_ERROR, "Cannot open file for writing \"%s\" %r\n", FileName, Status));
			continue;
		}
		Status = ShellWriteFile(FileHandle, &size, (UINTN*)(pHiiPkgListEntry+1));
		if (EFI_ERROR(Status))
		{
			Print(L"Failed (Writing error = %r)\n", Status);
			DEBUG((EFI_D_ERROR, "Cannot wrtie to file \"%s\" %r\n", FileName, Status));
		}
		ShellCloseFile(&FileHandle);
		Print(L"Done!\n", Status);
		PkgCountOk++;
	}

	pBS->FreePool(pHiiPkgLists);

	Print(L"Result: %d of %d packages sucessfully dumped!\n", PkgCountOk, PkgCount);
    return PkgCount == PkgCountOk ? EFI_SUCCESS : EFI_DEVICE_ERROR;
}
