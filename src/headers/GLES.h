#ifndef _RPI_GLES_
#define _RPI_GLES_

#ifdef __cplusplus								// If we are including to a C++
extern "C" {									// Put extern C directive wrapper around
#endif

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++}
{									}			
{       Filename: rpi-GLES.h						}
{       Copyright(c): Leon de Boer(LdB) 2017				}
{       Version: 1.01							}
{									}		
{***************[ THIS CODE IS FREEWARE UNDER CC Attribution]***********}
{									}
{     This sourcecode is released for the purpose to promote programming}
{  on the Raspberry Pi. You may redistribute it and/or modify with the  }
{  following disclaimer and condition.                                  }
{									}
{      The SOURCE CODE is distributed "AS IS" WITHOUT WARRANTIES AS TO  }
{   PERFORMANCE OF MERCHANTABILITY WHETHER EXPRESSED OR IMPLIED.        }
{   Redistributions of source code must retain the copyright notices to }
{   maintain the author credit (attribution) .				}
{									}
{***********************************************************************}
{                                                                       }
{+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "stdbool.h"
#include "stdint.h"
#include "mbox.h"	// Need for mailbox


bool init_V3D (void);
uint32_t V3D_mem_alloc (uint32_t size, uint32_t align, uint32_t flags);
bool V3D_mem_free (uint32_t handle);
uint32_t V3D_mem_lock (uint32_t handle);
bool V3D_mem_unlock (uint32_t handle);
bool V3D_execute_code (uint32_t code, uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3, uint32_t r4, uint32_t r5);
bool V3D_execute_qpu (int32_t num_qpus, uint32_t control, uint32_t noflush, uint32_t timeout);

#define GL_FRAGMENT_SHADER	35632
#define GL_VERTEX_SHADER	35633

void do_rotate(float delta);
int LoadShader(int shaderType);
// Render a quad to memory
void render_quad(uint16_t render_width, uint16_t render_height, uint32_t render_buffer_addr, uint32_t bus_addr);

#ifdef __cplusplus								// If we are including to a C++ file
}												// Close the extern C directive wrapper
#endif

#endif
