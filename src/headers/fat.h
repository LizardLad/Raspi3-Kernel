#ifndef FAT_H
#define FAT_H

// the BIOS Parameter Block (in Volume Boot Record)
typedef struct {
    char            jmp[3];
    char            oem[8];
    unsigned short  bps;
    unsigned char   spc;
    unsigned short  rsc;
    unsigned char   nf;
    unsigned short  nr;
    unsigned short  ts16;
    unsigned char   media;
    unsigned short  spf16;
    unsigned short  spt;
    unsigned short  nh;
    unsigned int    hs;
    unsigned int    ts32;
    unsigned int    spf32;
    unsigned int    flg;
    unsigned int    rc;
    char            vol[6];
    char            fst[8];
    char            dmy[20];
    char            fst2[8];
} __attribute__((packed)) bpb_t;

// directory entry structure
typedef struct {
    char            name[8];
    char            ext[3];
    char            attr[9];
    unsigned short  ch;
    unsigned int    attr2;
    unsigned short  cl;
    unsigned int    size;
} __attribute__((packed)) fatdir_t;

// get the end of bss segment from linker
extern unsigned char _end;
unsigned char master_boot_record_array[512] __attribute__((aligned(4)));
unsigned int partitionlba;

int fat_getpartition();
unsigned int fat_getcluster(char *fn);
void fat_listdirectory();
char *fat_readfile(unsigned int cluster);

#endif
