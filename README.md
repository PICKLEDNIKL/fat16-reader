# FAT16 Reader

This project consists of a FAT16 reader implemented in C. It reads and displays information from a FAT16 file system image.

## Features

- Reads and displays the Boot Sector information
- Reads and displays the Root Directory entries
- Reads and displays file contents from specified clusters

## File Structure

### Boot Sector

The boot sector structure is defined by the `BootSector` struct, which contains information like bytes per sector, sectors per cluster, number of FATs, and more.

### Directory Entry

The directory entry structure is defined by the `EntryInDirectory` struct, which contains information like file name, attributes, creation time, and more.

### Long Directory Entry

The long directory entry structure is defined by the `LongDirectoryEntry` struct, which is used for long file names.

## Functions

### `int Fileopen()`

Opens the FAT16 image file.

### `int Fileoffset(int fd, int offset)`

Sets the file offset.

### `void Fileread(int fd, void *ptr, size_t count)`

Reads data from the file.

### `void BSprint(BootSector *ptr)`

Prints the Boot Sector information.

### `void RDprint(EntryInDirectory *ptr)`

Prints a Root Directory entry.

### `void Fileprint(char *ptr)`

Prints file content.

### `int Fileclose(int fd)`

Closes the FAT16 image file.

## Usage

1. Compile the code using `gcc`:
    ```sh
    gcc -o fat16reader fat16reader.c
    ```

2. Run the executable:
    ```sh
    ./fat16reader
    ```

3. Follow the prompts to interact with the FAT16 reader.

~ Year 2 Operating Systems coursework
