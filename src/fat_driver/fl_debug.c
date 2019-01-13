#include "include/access.h"
#include "include/options.h"
#include "include/filelib.h"

void fatlib_debug_print_file(FL_FILE *file)
{
	printf("[DEBUG] File parent cluster: %d\n", file->parent_cluster);
	printf("[DEBUG] File start cluster: %d\n", file->start_cluster);
	printf("[DEBUG] File byte num: %d\n", file->byte_num);
	printf("[DEBUG] File file length: %d\n", file->file_length);
	printf("[DEBUG] File file length changed: %s\n", (file->file_length_changed == 0) ? "FALSE" : "TRUE");
	printf("[DEBUG] File path: %s\n", file->path);
	printf("[DEBUG] File filename: %s\n", file->filename);
	printf("[DEBUG] File short filename: %s\n", file->short_filename);
#ifdef FAT_CLUSTER_CACHE_ENTRIES
	printf("[DEBUG] File cluster cache index: %d\n", file->cluster_cache_index[0]);
	printf("[DEBUG] File cluster cache data: %d\n", file->cluster_cache_data[0]);
#endif
	printf("[DEBUG] File struct cluster lookup last fat lookup:\n");
	printf("[DEBUG]        cluster lookup cluster index: %d\n", file->last_fat_lookup.cluster_index);
	printf("[DEBUG]        cluster lookup current cluster: %d\n", file->last_fat_lookup.current_cluster);
	for(int i = 0; i < FAT_SECTOR_SIZE; i++)
	{	
		printf("[DEBUG] File data sector itteration %d | DATA | %d\n", i, file->file_data_sector[i]);
	}
	printf("[DEBUG] File data address: %d\n", file->file_data_address);
	printf("[DEBUG] File data dirty: %s\n", (file->file_data_dirty == 0) ? "FALSE" : "TRUE");
	printf("[DEBUG] File flags: %b\n", file->flags);
	printf("[DEBUG] File fat node list node:\n");
	printf("[DEBUG]         fat node previous: %x\n", file->list_node.previous);
	printf("[DEBUG]         fat node next: %x\n", file->list_node.next);
}
