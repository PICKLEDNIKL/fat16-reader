//USE HEXED.IT to see what im supposed to be reading

//https:codeandlife.com/2012/04/07/simple-fat-and-sd-tutorial-part-2/
//http://www.maverick-os.dk/FileSystemFormats/FAT16_FileSystem.html


//DEFINES
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h> //uint8_t
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct __attribute__((__packed__)) 
{
	uint8_t		BS_jmpBoot[ 3 ];	// x86 jump instr. to boot code
	uint8_t		BS_OEMName[ 8 ];	// What created the filesystem
	uint16_t	BPB_BytsPerSec;		// Bytes per Sector
	uint8_t		BPB_SecPerClus;		// Sectors per Cluster
	uint16_t	BPB_RsvdSecCnt;		// Reserved Sector Count
	uint8_t		BPB_NumFATs;		// Number of copies of FAT
	uint16_t	BPB_RootEntCnt;		// FAT12+FAT16: size of root DIR
	uint16_t	BPB_TotSec16;		// Sectors, may be 0, see below
	uint8_t		BPB_Media;		    // Media type, e.g. fixed
	uint16_t	BPB_FATSz16;		// Size of FAT (FAT12 or FAT16)
	uint16_t	BPB_SecPerTrk;		// Sectors per Track
	uint16_t	BPB_NumHeads;		// Number of heads in disk
	uint32_t	BPB_HiddSec;		// Hidden Sector count
	uint32_t	BPB_TotSec32;		// Sectors if BPB_TotSec16 == 0
	uint8_t		BS_DrvNum;		    // 0 = floppy, 0x80 = hard disk
	uint8_t		BS_Reserved1;		// 
	uint8_t		BS_BootSig;		    // Should = 0x29
	uint32_t	BS_VolID;		    // 'Unique' ID for volume
	uint8_t		BS_VolLab[ 11 ];	// Non zero terminated string
	uint8_t		BS_FilSysType[ 8 ];	// = 'FAT16   ' (Not 0 term.)
} BootSector;

typedef struct __attribute__((__packed__))
{
    uint8_t     DIR_Name[ 11 ];     // Non zero terminated string
    uint8_t     DIR_Attr;           // File attributes
    uint8_t     DIR_NTRes;          // Used by Windows NT, ignore
    uint8_t     DIR_CrtTimeTenth;   // Tenths of sec. 0...199
    uint16_t    DIR_CrtTime;        // Creation Time in 2s intervals
    uint16_t    DIR_CrtDate;        // Date file created
    uint16_t    DIR_LstAccDate;     // Date of last read or write
    uint16_t    DIR_FstClusHI;      // Top 16 bits file's 1st cluster
    uint16_t    DIR_WrtTime;        // Time of last write
    uint16_t    DIR_WrtDate;        // Date of last write
    uint16_t    DIR_FstClusLO;      // Lower 16 bits file's 1st cluster
    uint32_t    DIR_FileSize;       // File size in bytes
} EntryInDirectory;

#define fatBufferSize 32

int Fileopen()
{
    int fd = open("fat16.img",O_RDONLY); //USE THIS WHEN TESTING ACTUAL WORK
    //int fd = open("skeleton.txt",O_RDONLY);
    if(fd < 0)
    {
        printf("FILE CANNOT OPEN\n");
        return 1;
    }
    else
    {
        printf("FILE WAS OPENED\n");
        return fd;
    }      
}

int Fileoffset(int fd, int offset)
{
    int fOffset = lseek(fd, offset, SEEK_SET); //SECOND NUMBER IS THE AMOUNT OF SPACES I WANT TO MOVE OUT
    if (fOffset == -1)
    {
        printf("FILE CANNOT OFFSET\n");
        return 1;
    }
    else
    {
        printf("FILE WAS OFFSET\n");
        return fOffset;
    }
}

void Fileread(int fd, void *ptr, size_t count)
{
    read(fd, ptr, count);   //count-1????
    //return ptr;
}

void BSprint(BootSector *ptr)
{
    // for (int i = 0; i<sizeof(BootSector); i++)
    // {
    //     printf("%s", ptr[i]);
    // }

    printf("BytsPerSec: %i\n", ptr ->BPB_BytsPerSec);
    printf("SecPerClus: %i\n", ptr ->BPB_SecPerClus);
    printf("RsvdSecCnt: %i\n", ptr ->BPB_RsvdSecCnt); //number of reserved sectors
    printf("NumFATs: %i\n", ptr ->BPB_NumFATs);
    printf("RootEntCnt: %i\n", ptr ->BPB_RootEntCnt);
    printf("TotSec16: %i\n", ptr ->BPB_TotSec16);
    printf("FATSz16: %i\n", ptr ->BPB_FATSz16);
    printf("TotSec32: %i\n", ptr ->BPB_TotSec32);
    
    printf("VolLab: ");
    for (int i = 0; i<11; i++)
    {
        printf("%c", ptr ->BS_VolLab[i]);
    }
    printf("\n");
}

void RDprint(EntryInDirectory *ptr)
{
    if (ptr->DIR_Name[0] == 0)
    {
        printf("No further valid entries");
    }
    else if (ptr->DIR_Name[0] == 0xE5)
    {
        printf("Specific entry is currently unused and should be ignored, perhaps due to deleted file");
    }
    else if(ptr->DIR_Attr)
    {
        printf("boopy");
    }
    else 
    {
        printf("%i\n", ptr ->DIR_FileSize);
        printf("splooge");
    }
}

int Fileclose(int fd)
{
    close(fd);
    printf("FILE WAS CLOSED\n");
    return 0;
}

//MAIN
int main()
{
    //OPEN
    int fd = Fileopen();

    //OFFSET
    //int offset = 3; //change value to change offset
    //int foffset = Fileoffset(fd, offset);

    //READ
    BootSector* bp = malloc(sizeof(BootSector));
    size_t bsSize = sizeof(BootSector);
    Fileread(fd, bp, bsSize);

    //BOOTSECTOR PRINT
    BSprint(bp);

    //RESERVED SECTORS
    uint16_t fatReserveOffset = (bp->BPB_RsvdSecCnt * bp->BPB_BytsPerSec);
    Fileoffset(fd, fatReserveOffset);

    //FAT[N]
    int startsize;
    int ess = 0;
    while (ess == 0)
    {
        printf("Enter an integer: ");
        scanf("%i", &startsize);
        if (startsize >= 0xFFF8)
        {
            printf("Number must be less than 65528\n");
        }
        else
        {
            ess = 1;
        }
    }

    uint16_t fatsize = (bp->BPB_BytsPerSec * bp->BPB_FATSz16);
    uint16_t fat[fatsize];
    Fileread(fd, fat, fatsize);

    uint16_t ent = 0;

    for (int i=0; i<fatsize; i++)
    {
        if (fat[i] == startsize)
        {
            ent = i;
            //printf("%i\n", ent);
            break;
        }
    }

    uint16_t sz = startsize;

    while(fat[sz] < 0xFFF8)
    {
        printf("%i\n", fat[sz]);
        sz = fat[sz];
    }

    //ROOT DIRECTORY
    uint16_t firstSectorRD = bp->BPB_RsvdSecCnt + bp->BPB_NumFATs * bp->BPB_FATSz16;

    EntryInDirectory* eid = malloc(sizeof(EntryInDirectory));
    size_t eidSize = sizeof(EntryInDirectory);
    Fileread(fd, eid, eidSize);
    Fileoffset(fd, firstSectorRD);
    
    RDprint(eid);












    // printf("%li\n", firstSector);

    // //uint16_t directory[malloc(sizeof(Directory))];
    // EntryInDirectory dir[12];
    // Fileread(fd, &dir, 12);
    // for (int i=0; i<12; i++)
    // {
    //     printf("%i\n", dir->DIR_Attr);
    // }
    // int padding = 0x20;
    
    //CLOSE
    Fileclose(fd);
}

