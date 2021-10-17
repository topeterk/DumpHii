/** @file

  Copyright (c) 2018, Peter Kirmeier <topeterk@freenet.de>. All rights reserved.

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __SHELL_DYN_DUMP_HII_H__
#define __SHELL_DYN_DUMP_HII_H__

#include <Uefi.h>

#include <Protocol/ShellDynamicCommand.h>
#include <Protocol/HiiDatabase.h>

#include <Library/UefiLib.h> // Print()
#include <Library/HiiLib.h>
#include <Library/ShellLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>

EFI_STATUS
ShellLibConstructorWorker(
	IN EFI_HANDLE        ImageHandle,
	IN EFI_SYSTEM_TABLE  *SystemTable
	);

#endif /* __SHELL_DYN_DUMP_HII_H__ */
