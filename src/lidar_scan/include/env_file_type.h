#ifndef ENV_FILE_TYPE_H
#define ENV_FILE_TYPE_H

#define FILE_VERSION 1 //Version of file this program runs on

struct save_file_info_t
{
        char file_type[10]; //Holds "VOXELWORLD" for the VOXELWORLD file type   //byte 0x0  - 0xA
        uint16_t  file_type_version;                                            //byte 0xA  - 0xC

        uint16_t superchunk_x_dimention;                                        //byte 0xC  - 0xE
        uint16_t superchunk_y_dimention;                                        //byte 0xE  - 0x10
        uint16_t superchunk_z_dimention;                                        //byte 0x10 - 0x12

        uint16_t chunk_x_dimention;                                             //byte 0x12 - 0x14
        uint16_t chunk_y_dimention;                                             //byte 0x14 - 0x16
        uint16_t chunk_z_dimention;                                             //byte 0x16 - 0x18

        uint64_t unused;                                                        //byte 0x18 - 0x20
};

struct chunk_save_data_t
{
        char name[4]; //Should be "CHNK"        //byte 0x0 - 0x4
        int32_t ax;                             //byte 0x4 - 0x8
        int32_t ay;                             //byte 0x8 - 0xC
        int32_t az;                             //byte 0xC - 0x10
};

struct block_save_data_t
{
        char name[5]; //Should be "BLOCK"               //byte 0x0 - 0x5
        uint8_t type; //type matches a type index       //byte 0x5 - 0x6
        uint16_t unused;                                //byte 0x6 - 0x8
};

#endif
