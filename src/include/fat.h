#ifndef FAT_H
#define FAT_H

// the BIOS Parameter Block (in Volume Boot Record)
typedef struct {
    int8_t		jmp[3];
    int8_t		oem[8];
    uint8_t		bps0;
    uint8_t		bps1;
    uint8_t		spc;
    uint16_t		rsc;
    uint8_t		nf;
    uint8_t		nr0;
    uint8_t		nr1;
    uint16_t		ts16;
    uint8_t		media;
    uint16_t		spf16;
    uint16_t		spt;
    uint16_t		nh;
    uint32_t		hs;
    uint32_t		ts32;
    uint32_t		spf32;
    uint32_t		flg;
    uint32_t		rc;
    int8_t		vol[6];
    int8_t		fst[8];
    int8_t		dmy[20];
    int8_t		fst2[8];
} __attribute__((packed)) bpb_t;

// directory entry structure
typedef struct {
    int8_t	name[8];
    int8_t	ext[3];
    int8_t	attr[9];
    uint16_t	ch;
    uint32_t	attr2;
    uint16_t	cl;
    uint32_t	size;
} __attribute__((packed)) fatdir_t;

// get the end of bss segment from linker
extern uint8_t _end;
uint8_t master_boot_record_array[512] __attribute__((aligned(4)));
uint32_t partitionlba;

int32_t fat_getpartition();
uint32_t fat_getcluster(char *fn);
void fat_listdirectory();
char fat_readfile(uint32_t cluster);

#endif
