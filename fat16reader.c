//DEFINES
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h> //uint8_t
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


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

int Fileopen()
{
    //int fd = open("fat16.img",O_RDONLY); //USE THIS WHEN TESTING ACTUAL WORK
    int fd = open("skeleton.txt",O_RDONLY);
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

int Fileoffset(int fd)
{
    int fOffset = lseek(fd, 0, SEEK_SET); //SECOND NUMBER IS THE AMOUNT OF SPACES I WANT TO MOVE OUT
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

int Fileread(int fd, void *ptr, size_t count)
{
    //memcpy(text, *BootSector, sizeof(struct BootSector));
    //while (count != 30)
    //{   
        read(fd, ptr, count);
        //printf("%s", text);

        //count++;
    //}
    //read(fd, buffer, 20);
    //printf("buffer = %c\n", *buffer);
}

// int Fprint()
// {
//     //printf("%s", *text);
// }

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
    int offset = Fileoffset(fd);

    //READ

    char something[50];
    char text;
    size_t count = sizeof something;
    void *ptr;
    
    Fread(fd, ptr, count);

    Fclose(fd);
}
