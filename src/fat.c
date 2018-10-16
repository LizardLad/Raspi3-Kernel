#include <stdint.h>
#include "include/sd.h"
#include "include/uart.h"
#include "include/memcmp.h"
#include "include/fat.h"

fatdir_t *rootdir;
uint32_t *fat32;
uint32_t *cluster_buf;
uint32_t cluster_size;
uint32_t cluster;
uint32_t buf_size;
uint32_t current_pointer;
uint32_t root_sec;
uint32_t data_sec;
uint32_t file_size;

/**
 * Get the starting LBA address of the first partition
 * so that we know where our FAT file system starts, and
 * read that volume's BIOS Parameter Block
 */
int32_t fat_getpartition()
{
	cluster_size=0;
	cluster=0;
	unsigned char *master_boot_record=(unsigned char *)&master_boot_record_array;
	bpb_t *bpb=(bpb_t*)&master_boot_record_array;
	// read the partitioning table
	if(sd_readblock(0, master_boot_record, 1)) {
		// check magic
		if(master_boot_record[510]!=0x55 || master_boot_record[511]!=0xAA) {
			uart_puts("ERROR: Bad magic in MBR\n");
			return 0;
		}
		// check partition type
		if(master_boot_record[0x1C2]!=0xE/*FAT16 LBA*/ && master_boot_record[0x1C2]!=0xC/*FAT32 LBA*/) {
			uart_puts("ERROR: Wrong partition type\n");
			return 0;
		}

		partitionlba = master_boot_record[0x1C6] + (master_boot_record[0x1C7] << 8) + (master_boot_record[0x1C8]<<16) + (master_boot_record[0x1C9]<<24);
			
		uint32_t s;
		// find the root directory's LBA
		root_sec=((bpb->spf16?bpb->spf16:bpb->spf32)*bpb->nf)+bpb->rsc;
		s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);
		
		rootdir = (fatdir_t *)malloc(((((s*32)+512 - 1)/512)*512));
		fat32 = (uint32_t*)malloc((bpb->spf16?bpb->spf16:bpb->spf32)*512);

		if(bpb->spf16==0) {
			// adjust for FAT32
			root_sec+=(bpb->rc-2)*bpb->spc;
		}

		// add partition LBA
		root_sec+=partitionlba;
		sd_readblock(root_sec,(unsigned char*)rootdir,s/512+1);

		data_sec=((bpb->spf16?bpb->spf16:bpb->spf32)*bpb->nf)+bpb->rsc;
		if(bpb->spf16>0)
		{
			// adjust for FAT16
			data_sec+=(s+511)>>9;
		}
		// add partition LBA
		data_sec+=partitionlba;
		sd_readblock(root_sec,(unsigned char*)fat32,s/512+1);

		cluster_size=bpb->spc*(bpb->bps0 + (bpb->bps1 << 8));
		cluster_buf = (uint32_t*)malloc(cluster_size);
		buf_size=cluster_size;

		// read the boot record
		if(!sd_readblock(partitionlba, master_boot_record, 1)) {
			uart_puts("ERROR: Unable to read boot record\n");
			return 0;
		}
		// check file system type. We don't use cluster numbers for that, but magic bytes
		if( !(bpb->fst[0]=='F' && bpb->fst[1]=='A' && bpb->fst[2]=='T') &&
			!(bpb->fst2[0]=='F' && bpb->fst2[1]=='A' && bpb->fst2[2]=='T')) {
			uart_puts("ERROR: Unknown file system type\n");
			return 0;
		}
		return 1;
	}
	return 0;
}

/**
 * Find a file in root directory entries
 */
uint32_t fat_getcluster(char *fn)
{
	//bpb_t *bpb=(bpb_t*)&(master_boot_record_array[0]);	
	fatdir_t *dir = rootdir;

	// iterate on each entry and check if it's the one we're looking for
	for(;dir->name[0]!=0;dir++) {
		// is it a valid entry?
		if(dir->name[0]==0xE5 || dir->attr[0]==0xF) continue;
		// filename match?
		if(!memcmp(dir->name,fn,11)) {
			//uart_puts("FAT File ");
			//uart_puts(fn);
			//uart_puts(" starts at cluster: ");
			//uart_hex(((uint32_t)dir->ch)<<16|dir->cl);
			//uart_puts("\n");
			// if so, return starting cluster
			file_size = dir->size;
			return ((uint32_t)dir->ch)<<16|dir->cl;
		}
		uart_puts("ERROR: file not found\n");
	}
	return 0;
}

void fat_listdirectory()
{
	unsigned char *master_boot_record=(unsigned char *)&master_boot_record_array;
	bpb_t *bpb=(bpb_t*)(master_boot_record);
	fatdir_t *dir=(fatdir_t*)(master_boot_record);
	uint32_t root_sec, s;
	// find the root directory's LBA
	root_sec=((bpb->spf16?bpb->spf16:bpb->spf32)*bpb->nf)+bpb->rsc;
	s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);
	if(bpb->spf16==0)
	{
		// adjust for FAT32
		root_sec+=(bpb->rc-2)*bpb->spc;
	}
	// add partition LBA
	root_sec+=partitionlba;
	uart_puts("\nFAT root directory LBA: ");
	uart_hex(root_sec);
	uart_puts("\n");
	// load the root directory
	if(sd_readblock(root_sec,(unsigned char*)master_boot_record,s/512+1)) 
	{
		uart_puts("\nAttrib Cluster  Size     Name\n");
		// iterate on each entry and print out
		int lfn_entries = 0;
		for(;dir->name[0]!=0;dir++) 
		{
			//is it a LFN entry
			if(dir->attr[0]&1 && dir->attr[0]&2 && dir->attr[0]&4 && dir->attr[0]&8) {
				//lfb_print(0, 0, "LFN Entry");
				//lfb_print(0, lfn_entries + 3, &dir->name[0]);
				lfn_entries++;
			}
			// is it a valid entry?
			if((dir->name[0]==0xE5 || dir->attr[0]==0xF) && !(dir->attr[0]&1 && dir->attr[0]&2 && dir->attr[0]&4 && dir->attr[0]&8)) continue;
			// decode attributes
			uart_send(dir->attr[0]& 1?'R':'.');  // read-only
			uart_send(dir->attr[0]& 2?'H':'.');  // hidden
			uart_send(dir->attr[0]& 4?'S':'.');  // system
			uart_send(dir->attr[0]& 8?'L':'.');  // volume label
			uart_send(dir->attr[0]&16?'D':'.');  // directory
			uart_send(dir->attr[0]&32?'A':'.');  // archive
			uart_send(' ');
			// staring cluster
			uart_hex(((uint32_t)dir->ch)<<16|dir->cl);
			uart_send(' ');
			// size
			uart_hex(dir->size);
			uart_send(' ');
			// filename
			dir->attr[0]=0;
			uart_puts((char *)dir->name);
			uart_puts("\n");
		}
	} 
	else 
	{
		uart_puts("ERROR: Unable to load root directory\n");
	}
}

/**
 * Read a file into memory
 * cluster_buf
 */
char fat_readfile(uint32_t cluster)
{
	// BIOS Parameter Block
	bpb_t *bpb=(bpb_t*)(&(master_boot_record_array[0]));
	// File allocation tables. We choose between FAT16 and FAT32 dynamically
	uint16_t *fat16=(uint16_t*)fat32;
	// Data pointers
	//uint32_t data_sec, s;
	//unsigned char *data, *ptr;
	// find the LBA of the first data sector
	//data_sec=((bpb->spf16?bpb->spf16:bpb->spf32)*bpb->nf)+bpb->rsc;
	//s = (bpb->nr0 + (bpb->nr1 << 8)) * sizeof(fatdir_t);
	//if(bpb->spf16>0)
	//{
		//// adjust for FAT16
		//data_sec+=(s+511)>>9;
	//}
	//// add partition LBA
	//data_sec+=partitionlba;
	//// dump important properties
	//uart_puts("FAT Bytes per Sector: ");
	////uart_hex(bpb->bps);
	//uart_hex(bpb->bps0 + (bpb->bps1 << 8));
	//uart_puts("\nFAT Sectors per Cluster: ");
	//uart_hex(bpb->spc);
	//uart_puts("\nFAT Number of FAT: ");
	//uart_hex(bpb->nf);
	//uart_puts("\nFAT Sectors per FAT: ");
	//uart_hex((bpb->spf16?bpb->spf16:bpb->spf32));
	//uart_puts("\nFAT Reserved Sectors Count: ");
	//uart_hex(bpb->rsc);
	//uart_puts("\nFAT First data sector: ");
	//uart_hex(data_sec);
	//uart_puts("\n");
	//// load FAT table
	//s=sd_readblock(partitionlba+1,(unsigned char*)&_end+512,(bpb->spf16?bpb->spf16:bpb->spf32)+bpb->rsc);
	//// end of FAT in memory
	//data=ptr=&_end+512+s;
	//// iterate on cluster chain
	//while(cluster>1 && cluster<0xFFF8) {
		//// load all sectors in a cluster
		//sd_readblock((cluster-2)*bpb->spc+data_sec,ptr,bpb->spc);
		//// move pointer, sector per cluster * bytes per sector
		//ptr+=bpb->spc*(bpb->bps0 + (bpb->bps1 << 1));
		//// get the next cluster in chain
		//cluster=bpb->spf16>0?fat16[cluster]:fat32[cluster];
	//}
	//return (char*)data;
	if(current_pointer >= buf_size)
	{
		if(cluster < 1 || cluster < 0xFFF8 || buf_size != cluster_size)
		{
			return 0;//End Of File
		}
		sd_readblock((cluster-2)*bpb->spc+data_sec,(unsigned char *)cluster_buf,bpb->spc);
		cluster=bpb->spf16>0?fat16[cluster]:fat32[cluster];
		if(file_size>cluster_size)
		{
			file_size -= cluster_size;
		}
		else
		{
			buf_size = file_size;
		}
		current_pointer = 0;
	}
	if(buf_size == 0)
	{
		return 0;//End Of File
	}
	return cluster_buf[current_pointer++];
}

