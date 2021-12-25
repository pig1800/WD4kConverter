#define  _CRT_SECURE_NO_WARNINGS
// WD4kConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <ntddscsi.h>

#define CMD_IDENTIFY 0xec
#define CMD_READLOGEXT 0x2f
#define CMD_SECTORCONFEXT 0xb2
#define LOGADDRESS_SECTORCONF 0x2f
#define SECTORCONF_BUFSIZE 512

#pragma pack(1)

typedef struct _SECTORCONF {
    uint8_t reserved : 7;
    uint8_t validBit : 1;
    uint8_t relationShip;
    uint16_t checkField;
    uint32_t sectorSize;
    uint64_t reserved2;
} SECTORCONF, *LPSECTORCONF;

typedef struct _IDEREGS48 {
	UCHAR bFeaturesReg_Previous;
	UCHAR bSectorCountReg_Previous;
	UCHAR bLba_24_31;
	UCHAR bLba_32_39;
	UCHAR bLba_40_47;
	UCHAR bDriveHeadReg_Previous;
	UCHAR bCommandReg_Previous;
	UCHAR bReserved_Previous;
	UCHAR bFeaturesReg;
	UCHAR bSectorCountReg;
	UCHAR bLba_00_07;
	UCHAR bLba_08_15;
	UCHAR bLba_16_23;
	UCHAR bDriveHeadReg;
	UCHAR bCommandReg;
	UCHAR bReserved;
} IDEREGS48, * PIDEREGS48, * LPIDEREGS48;

typedef struct _IDEREGSDBL {
	UCHAR bFeaturesReg_Previous;
	UCHAR bSectorCountReg_Previous;
	UCHAR bSectorNumberReg_Previous;
	UCHAR bCylLowReg_Previous;
	UCHAR bCylHighReg_Previous;
	UCHAR bDriveHeadReg_Previous;
	UCHAR bCommandReg_Previous;
	UCHAR bReserved_Previous;
	UCHAR bFeaturesReg;
	UCHAR bSectorCountReg;
	UCHAR bSectorNumberReg;
	UCHAR bCylLowReg;
	UCHAR bCylHighReg;
	UCHAR bDriveHeadReg;
	UCHAR bCommandReg;
	UCHAR bReserved;
} IDEREGSDBL, *PIDEREGSDBL, *LPIDEREGSDBL;

// I have copied the struct declaration from 
// "IDENTIFY_DEVICE_DATA structure" http://msdn.microsoft.com/en-us/library/windows/hardware/ff559006(v=vs.85).aspx
// I think it is better to include the suitable header (MSDN says the header is Ata.h and suggests to include Irb.h)
typedef struct _IDENTIFY_DEVICE_DATA {
	struct {
		USHORT Reserved1 : 1;
		USHORT Retired3 : 1;
		USHORT ResponseIncomplete : 1;
		USHORT Retired2 : 3;
		USHORT FixedDevice : 1;
		USHORT RemovableMedia : 1;
		USHORT Retired1 : 7;
		USHORT DeviceType : 1;
	} GeneralConfiguration;
	USHORT NumCylinders;
	USHORT ReservedWord2;
	USHORT NumHeads;
	USHORT Retired1[2];
	USHORT NumSectorsPerTrack;
	USHORT VendorUnique1[3];
	UCHAR  SerialNumber[20];
	USHORT Retired2[2];
	USHORT Obsolete1;
	UCHAR  FirmwareRevision[8];
	UCHAR  ModelNumber[40];
	UCHAR  MaximumBlockTransfer;
	UCHAR  VendorUnique2;
	USHORT ReservedWord48;
	struct {
		UCHAR  ReservedByte49;
		UCHAR  DmaSupported : 1;
		UCHAR  LbaSupported : 1;
		UCHAR  IordyDisable : 1;
		UCHAR  IordySupported : 1;
		UCHAR  Reserved1 : 1;
		UCHAR  StandybyTimerSupport : 1;
		UCHAR  Reserved2 : 2;
		USHORT ReservedWord50;
	} Capabilities;
	USHORT ObsoleteWords51[2];
	USHORT TranslationFieldsValid : 3;
	USHORT Reserved3 : 13;
	USHORT NumberOfCurrentCylinders;
	USHORT NumberOfCurrentHeads;
	USHORT CurrentSectorsPerTrack;
	ULONG  CurrentSectorCapacity;
	UCHAR  CurrentMultiSectorSetting;
	UCHAR  MultiSectorSettingValid : 1;
	UCHAR  ReservedByte59 : 7;
	ULONG  UserAddressableSectors;
	USHORT ObsoleteWord62;
	USHORT MultiWordDMASupport : 8;
	USHORT MultiWordDMAActive : 8;
	USHORT AdvancedPIOModes : 8;
	USHORT ReservedByte64 : 8;
	USHORT MinimumMWXferCycleTime;
	USHORT RecommendedMWXferCycleTime;
	USHORT MinimumPIOCycleTime;
	USHORT MinimumPIOCycleTimeIORDY;
	USHORT ReservedWords69[6];
	USHORT QueueDepth : 5;
	USHORT ReservedWord75 : 11;
	USHORT ReservedWords76[4];
	USHORT MajorRevision;
	USHORT MinorRevision;
	struct {
		USHORT SmartCommands : 1;
		USHORT SecurityMode : 1;
		USHORT RemovableMediaFeature : 1;
		USHORT PowerManagement : 1;
		USHORT Reserved1 : 1;
		USHORT WriteCache : 1;
		USHORT LookAhead : 1;
		USHORT ReleaseInterrupt : 1;
		USHORT ServiceInterrupt : 1;
		USHORT DeviceReset : 1;
		USHORT HostProtectedArea : 1;
		USHORT Obsolete1 : 1;
		USHORT WriteBuffer : 1;
		USHORT ReadBuffer : 1;
		USHORT Nop : 1;
		USHORT Obsolete2 : 1;
		USHORT DownloadMicrocode : 1;
		USHORT DmaQueued : 1;
		USHORT Cfa : 1;
		USHORT AdvancedPm : 1;
		USHORT Msn : 1;
		USHORT PowerUpInStandby : 1;
		USHORT ManualPowerUp : 1;
		USHORT Reserved2 : 1;
		USHORT SetMax : 1;
		USHORT Acoustics : 1;
		USHORT BigLba : 1;
		USHORT DeviceConfigOverlay : 1;
		USHORT FlushCache : 1;
		USHORT FlushCacheExt : 1;
		USHORT Resrved3 : 2;
		USHORT SmartErrorLog : 1;
		USHORT SmartSelfTest : 1;
		USHORT MediaSerialNumber : 1;
		USHORT MediaCardPassThrough : 1;
		USHORT StreamingFeature : 1;
		USHORT GpLogging : 1;
		USHORT WriteFua : 1;
		USHORT WriteQueuedFua : 1;
		USHORT WWN64Bit : 1;
		USHORT URGReadStream : 1;
		USHORT URGWriteStream : 1;
		USHORT ReservedForTechReport : 2;
		USHORT IdleWithUnloadFeature : 1;
		USHORT Reserved4 : 2;
	} CommandSetSupport;
	struct {
		USHORT SmartCommands : 1;
		USHORT SecurityMode : 1;
		USHORT RemovableMediaFeature : 1;
		USHORT PowerManagement : 1;
		USHORT Reserved1 : 1;
		USHORT WriteCache : 1;
		USHORT LookAhead : 1;
		USHORT ReleaseInterrupt : 1;
		USHORT ServiceInterrupt : 1;
		USHORT DeviceReset : 1;
		USHORT HostProtectedArea : 1;
		USHORT Obsolete1 : 1;
		USHORT WriteBuffer : 1;
		USHORT ReadBuffer : 1;
		USHORT Nop : 1;
		USHORT Obsolete2 : 1;
		USHORT DownloadMicrocode : 1;
		USHORT DmaQueued : 1;
		USHORT Cfa : 1;
		USHORT AdvancedPm : 1;
		USHORT Msn : 1;
		USHORT PowerUpInStandby : 1;
		USHORT ManualPowerUp : 1;
		USHORT Reserved2 : 1;
		USHORT SetMax : 1;
		USHORT Acoustics : 1;
		USHORT BigLba : 1;
		USHORT DeviceConfigOverlay : 1;
		USHORT FlushCache : 1;
		USHORT FlushCacheExt : 1;
		USHORT Resrved3 : 2;
		USHORT SmartErrorLog : 1;
		USHORT SmartSelfTest : 1;
		USHORT MediaSerialNumber : 1;
		USHORT MediaCardPassThrough : 1;
		USHORT StreamingFeature : 1;
		USHORT GpLogging : 1;
		USHORT WriteFua : 1;
		USHORT WriteQueuedFua : 1;
		USHORT WWN64Bit : 1;
		USHORT URGReadStream : 1;
		USHORT URGWriteStream : 1;
		USHORT ReservedForTechReport : 2;
		USHORT IdleWithUnloadFeature : 1;
		USHORT Reserved4 : 2;
	} CommandSetActive;
	USHORT UltraDMASupport : 8;
	USHORT UltraDMAActive : 8;
	USHORT ReservedWord89[4];
	USHORT HardwareResetResult;
	USHORT CurrentAcousticValue : 8;
	USHORT RecommendedAcousticValue : 8;
	USHORT ReservedWord95[5];
	ULONG  Max48BitLBA[2];
	USHORT StreamingTransferTime;
	USHORT ReservedWord105;
	struct {
		USHORT LogicalSectorsPerPhysicalSector : 4;
		USHORT Reserved0 : 8;
		USHORT LogicalSectorLongerThan256Words : 1;
		USHORT MultipleLogicalSectorsPerPhysicalSector : 1;
		USHORT Reserved1 : 2;
	} PhysicalLogicalSectorSize;
	USHORT InterSeekDelay;
	USHORT WorldWideName[4];
	USHORT ReservedForWorldWideName128[4];
	USHORT ReservedForTlcTechnicalReport;
	USHORT WordsPerLogicalSector[2];
	struct {
		USHORT ReservedForDrqTechnicalReport : 1;
		USHORT WriteReadVerifySupported : 1;
		USHORT Reserved01 : 11;
		USHORT Reserved1 : 2;
	} CommandSetSupportExt;
	struct {
		USHORT ReservedForDrqTechnicalReport : 1;
		USHORT WriteReadVerifyEnabled : 1;
		USHORT Reserved01 : 11;
		USHORT Reserved1 : 2;
	} CommandSetActiveExt;
	USHORT ReservedForExpandedSupportandActive[6];
	USHORT MsnSupport : 2;
	USHORT ReservedWord1274 : 14;
	struct {
		USHORT SecuritySupported : 1;
		USHORT SecurityEnabled : 1;
		USHORT SecurityLocked : 1;
		USHORT SecurityFrozen : 1;
		USHORT SecurityCountExpired : 1;
		USHORT EnhancedSecurityEraseSupported : 1;
		USHORT Reserved0 : 2;
		USHORT SecurityLevel : 1;
		USHORT Reserved1 : 7;
	} SecurityStatus;
	USHORT ReservedWord129[31];
	struct {
		USHORT MaximumCurrentInMA2 : 12;
		USHORT CfaPowerMode1Disabled : 1;
		USHORT CfaPowerMode1Required : 1;
		USHORT Reserved0 : 1;
		USHORT Word160Supported : 1;
	} CfaPowerModel;
	USHORT ReservedForCfaWord161[8];
	struct {
		USHORT SupportsTrim : 1;
		USHORT Reserved0 : 15;
	} DataSetManagementFeature;
	USHORT ReservedForCfaWord170[6];
	USHORT CurrentMediaSerialNumber[30];
	USHORT ReservedWord206;
	USHORT ReservedWord207[2];
	struct {
		USHORT AlignmentOfLogicalWithinPhysical : 14;
		USHORT Word209Supported : 1;
		USHORT Reserved0 : 1;
	} BlockAlignment;
	USHORT WriteReadVerifySectorCountMode3Only[2];
	USHORT WriteReadVerifySectorCountMode2Only[2];
	struct {
		USHORT NVCachePowerModeEnabled : 1;
		USHORT Reserved0 : 3;
		USHORT NVCacheFeatureSetEnabled : 1;
		USHORT Reserved1 : 3;
		USHORT NVCachePowerModeVersion : 4;
		USHORT NVCacheFeatureSetVersion : 4;
	} NVCacheCapabilities;
	USHORT NVCacheSizeLSW;
	USHORT NVCacheSizeMSW;
	USHORT NominalMediaRotationRate;
	USHORT ReservedWord218;
	struct {
		UCHAR NVCacheEstimatedTimeToSpinUpInSeconds;
		UCHAR Reserved;
	} NVCacheOptions;
	USHORT ReservedWord220[35];
	USHORT Signature : 8;
	USHORT CheckSum : 8;
} IDENTIFY_DEVICE_DATA, * PIDENTIFY_DEVICE_DATA;

// Taken from smartmontools
// Copies n bytes (or n-1 if n is odd) from in to out, but swaps adjacents
// bytes.
static void swapbytes(char* out, const char* in, size_t n)
{
	for (size_t i = 0; i < n; i += 2) {
		out[i] = in[i + 1];
		out[i + 1] = in[i];
	}
}

// Taken from smartmontools
// Copies in to out, but removes leading and trailing whitespace.
static void trim(char* out, const char* in)
{
	// Find the first non-space character (maybe none).
	int first = -1;
	int i;
	for (i = 0; in[i]; i++)
		if (!isspace((int)in[i])) {
			first = i;
			break;
		}

	if (first == -1) {
		// There are no non-space characters.
		out[0] = '\0';
		return;
	}

	// Find the last non-space character.
	for (i = (int)strlen(in) - 1; i >= first && isspace((int)in[i]); i--)
		;
	int last = i;

	strncpy(out, in + first, last - first + 1);
	out[last - first + 1] = '\0';
}

// Taken from smartmontools
// Convenience function for formatting strings from ata_identify_device
void ata_format_id_string(char* out, const unsigned char* in, int n)
{
	bool must_swap = true;
#ifdef __NetBSD__
	/* NetBSD kernel delivers IDENTIFY data in host byte order (but all else is LE) */
	// TODO: Handle NetBSD case in os_netbsd.cpp
	if (isbigendian())
		must_swap = !must_swap;
#endif

	char tmp[65];
	n = n > 64 ? 64 : n;
	if (!must_swap)
		strncpy(tmp, (const char*)in, n);
	else
		swapbytes(tmp, (const char*)in, n);
	tmp[n] = '\0';
	trim(out, tmp);
}

void initSectorConfLogCommand(void* target) {
    LPIDEREGS48 reg = (LPIDEREGS48)target;

	memset(target, 0, sizeof(IDEREGS48));

	reg->bCommandReg = CMD_READLOGEXT;
    reg->bLba_00_07 = LOGADDRESS_SECTORCONF;
	reg->bSectorCountReg = 1;
}

void initSectorConfExt(void* target, uint16_t checkField, uint8_t index) {
	LPIDEREGSDBL reg = (LPIDEREGSDBL)target;

	memset(target, 0, sizeof(IDEREGS48));

	reg->bCommandReg = CMD_SECTORCONFEXT;
	reg->bDriveHeadReg = 0x40;
	reg->bSectorCountReg = index;
	reg->bFeaturesReg = checkField & 0xff;
	reg->bFeaturesReg_Previous = checkField >> 8;
}

int main(int argc, char** argv)
{
	bool stdOutSet = true;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hStdOut == INVALID_HANDLE_VALUE) {
		stdOutSet = false;
	}
	else {
		DWORD mode;
		if (!GetConsoleMode(hStdOut, &mode)) {
			stdOutSet = false;
		}
		else {
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			if (!SetConsoleMode(hStdOut, mode)) {
				stdOutSet = false;
			}
		}
	}

	const char* bgColorRed = stdOutSet ? "\x1b[41m" : "";
	const char* bgColorDef = stdOutSet ? "\x1b[49m" : "";
	const char* textColorYellow = stdOutSet ? "\x1b[33m" : "";
	const char* textColorDef = stdOutSet ? "\x1b[39m" : "";
	
	if (argc != 2) {
		printf("Usage :\n%s \\\\.\\PhysicalDrive[number]", argv[0]);
	}

	HANDLE handle = ::CreateFileA(
		argv[1],
		GENERIC_READ | GENERIC_WRITE, //IOCTL_ATA_PASS_THROUGH requires read-write
		FILE_SHARE_READ,
		0,            //no security attributes
		OPEN_EXISTING,
		0,              //flags and attributes
		0             //no template file
	);

	if (handle == INVALID_HANDLE_VALUE) {
		std::cout << "Invalid drive or no administrator privilege!\n";
		return 1;
	}

	// IDENTIFY command requires a 512 byte buffer for data:
	unsigned char Buffer[SECTORCONF_BUFSIZE + sizeof(ATA_PASS_THROUGH_EX)] = { 0 };
	ATA_PASS_THROUGH_EX& PTE = *(ATA_PASS_THROUGH_EX*)Buffer;
	PTE.Length = sizeof(PTE);
	PTE.TimeOutValue = 10;
	PTE.DataTransferLength = 512;
	PTE.DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX);

	// Set up the IDE registers as specified in ATA spec.
	
	IDEREGS* ir = (IDEREGS*)PTE.CurrentTaskFile;
	ir->bCommandReg = CMD_IDENTIFY;
	ir->bSectorCountReg = 1;

	// IDENTIFY is neither 48-bit nor DMA, it reads from the device:
	PTE.AtaFlags = ATA_FLAGS_DATA_IN | ATA_FLAGS_DRDY_REQUIRED | ATA_FLAGS_48BIT_COMMAND;

	DWORD BR = 0;
	BOOL b = ::DeviceIoControl(handle, IOCTL_ATA_PASS_THROUGH, &PTE, sizeof(Buffer), &PTE, sizeof(Buffer), &BR, 0);
	if (b == 0) {
		std::cout << "Identify device call failed!\n";

		return 1;
	}

	IDENTIFY_DEVICE_DATA* data = (IDENTIFY_DEVICE_DATA*)(Buffer + sizeof(ATA_PASS_THROUGH_EX));

	// Nota Bene: I think some endianness control is needed
	char model[40 + 1];
	ata_format_id_string(model, data->ModelNumber, sizeof(model) - 1);

	char serial[20 + 1];
	ata_format_id_string(serial, data->SerialNumber, sizeof(serial) - 1);

	char firmware[8 + 1];
	ata_format_id_string(firmware, data->FirmwareRevision, sizeof(firmware) - 1);

	std::cout << "ModelNumber:      " << model << "\n";
	std::cout << "SerialNumber:     " << serial << "\n";
	std::cout << "FirmwareRevision: " << firmware << "\n\n";

	initSectorConfLogCommand(&PTE.PreviousTaskFile);

	b = ::DeviceIoControl(handle, IOCTL_ATA_PASS_THROUGH, &PTE, sizeof(Buffer), &PTE, sizeof(Buffer), &BR, 0);
	if (b == 0) {
		std::cout << "Query sector configuration log call failed!\n";
		return 1;
	}

	LPIDEREGS reg = (LPIDEREGS)PTE.CurrentTaskFile;

	printf("Query sector configuration log:\n");
	if (reg->bCommandReg & 1) {
		printf("Error!\n");

		if (reg->bFeaturesReg & 4) {
			printf("Command aborted!\n");
		}
		else {
			printf("Reason Unknown\n");
		}

		printf("Your drive might not support this feature!\n");

		return 1;
	}
	else {
		printf("Succeeded!\n\n");
	}


	LPSECTORCONF confData = (LPSECTORCONF)(Buffer + sizeof(ATA_PASS_THROUGH_EX));

	for (int i = 0; i < 8; i++) {
		if (confData[i].validBit) {
			printf("Index:                %d\n", i);
			printf("Relationship Setting: %d\n", confData[i].relationShip);
			printf("Descriptor Check:     %d\n", confData[i].checkField);
			printf("Sector Size setting:  %d %s\n\n", confData[i].sectorSize, confData[i].sectorSize == 2048 ? "(Probably 4096 in actual)" : confData[i].sectorSize == 256 ? "(Probably 512 in actual)" : "");
		}
	}

	int input;

	printf("%s--- Please double check if your drive serial is correct!                ---%s\n", bgColorRed, bgColorDef);
	printf("%s--- Despite the function itself doesn't affect existing data            ---\n", textColorYellow);
	printf(  "--- Partition table and file system is directly relying on sector size! ---\n");
	printf(  "--- Selecting wrong drive will render files inaccessible!               ---\n");
	printf("%sEnter Index: ", textColorDef);

	if (scanf("%d", &input) != 1) {
		printf("Invalid input!\n");
		return 1;
	}
	
	if (input < 0 || input > 7 || !confData[input].validBit) {
		printf("Invalid index!\n");
		return 1;
	}

	initSectorConfExt(&PTE.PreviousTaskFile, confData[input].checkField, input);

	b = ::DeviceIoControl(handle, IOCTL_ATA_PASS_THROUGH, &PTE, sizeof(Buffer), &PTE, sizeof(Buffer), &BR, 0);
	if (b == 0) {
		std::cout << "Set sector configuration ext call failed!\n";
		return 1;
	}

	reg = (LPIDEREGS)PTE.CurrentTaskFile;

	printf("Set sector configuration ext:\n");
	if (reg->bCommandReg & 1) {
		printf("Error!\n");
		
		if (reg->bFeaturesReg & 4) {
			printf("Command aborted!\n");
		}
		else {
			printf("Reason Unknown\n");
		}
	}
	else {
		printf("Succeeded!\n\n");
	}

	printf("Please completely unplug and replug the drive or restart the computer.\n");
	printf("All drive access will be invalid until OS re-identify the drive.\n");

	return 0;
}
