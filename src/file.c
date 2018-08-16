#include "headers/project.h"

/*uint32_t text_file_dump(char *fn)
{
	bpb_t *bpb=(bpb_t*)(&(master_boot_record_array[0]));
	unsigned char fatdir_buffer[512];
	fatdir_t *dir=(fatdir_t*)(&(fatdir_buffer[0]));*
	uint32_t root_sec, s;
	// find the root directory's LBA
	root_sec=((bpb->spf16?bpb->spf16:bpb->spf32)*bpb->nf)+bpb->rsc;
	//WARNING gcc generates bad code for bpb->nr, causing unaligned exception
	s=*((uint32_t*)&bpb->nf);
	s>>=8;
	s&=0xFFFF;
	s<<=5;
	// now s=bpb->nr*sizeof(fatdir_t));
	if(bpb->spf16==0) {
		// adjust for FAT32
		root_sec+=(bpb->rc-2)*bpb->spc;
	}
	// add partition LBA
	root_sec+=partitionlba;
	// load the root directory
	if(sd_readblock(root_sec,(unsigned char*)dir,s/512+1)) {
		// iterate on each entry and check if it's the one we're looking for
		for(;dir->name[0]!=0;dir++) {
			// is it a valid entry?
			if(dir->name[0]==0xE5 || dir->attr[0]==0xF) continue;
			// filename match?
			if(!__builtin_memcmp(dir->name,fn,11)) {
				uart_puts("FAT File ");
				uart_puts(fn);
				uart_puts(" starts at cluster: ");
				uart_hex(((uint32_t)dir->ch)<<16|dir->cl);
				uart_puts("\n");
				// if so, return starting cluster
				if(((uint32_t) dir->ch)<<16|dir->cl)
				{
					uint32_t size = dir->size;
					void *ptr = fat_readfile(((uint32_t)dir->ch)<<16|dir->cl); 
					unsigned long a, temp;
					uint32_t x = 0, y = 0;
					unsigned char c;
					a = (unsigned long)ptr;
					for(uint32_t bytes_copied = 0; bytes_copied < size; bytes_copied++)
					{
						c=*((unsigned char *)(a));
						temp = (c<32||c>127?'.':c);
						if(c == '\n')
						{
							y++;
							x = 0;
						}
						else if(c != '\r')
						{
							lfb_print(x, y, (char *)&temp);
							x++;
						}
						else
						{
							x = 0;
						}
						a++;
					}
				}
				return 0;
			}
		}
		uart_puts("ERROR: file not found\n");
	} 
	else 
	{
		uart_puts("ERROR: Unable to load root directory\n");
	}
	return 0;
}
*/

