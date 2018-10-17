#include <stdint.h>
#include <math.h>
#include "include/GLES.h"
#include "include/uart.h"
#include "include/lfb.h"
#include "include/malloc.h"
#include "include/console.h"
#include "include/sound.h"	//init_audio_jack(), play_audio...
#include "include/system.h"	//clocks_init();
#include "include/mmu.h"
#include "include/printf.h"
#include "include/multicore.h"
#include "include/gpu_memory.h"
#include "include/start.h"
#include "include/delays.h"
#include "include/scene.h"

static uint32_t shader[18] = {  	// Vertex Color Shader
		0x958e0dbf, 0xd1724823,	/* mov r0, vary; mov r3.8d, 1.0 */
		0x818e7176, 0x40024821,	/* fadd r0, r0, r5; mov r1, vary */
		0x818e7376, 0x10024862,	/* fadd r1, r1, r5; mov r2, vary */
		0x819e7540, 0x114248a3,	/* fadd r2, r2, r5; mov r3.8a, r0 */
		0x809e7009, 0x115049e3,	/* nop; mov r3.8b, r1 */
		0x809e7012, 0x116049e3,	/* nop; mov r3.8c, r2 */
		0x159e76c0, 0x30020ba7,	/* mov tlbc, r3; nop; thrend */
		0x009e7000, 0x100009e7,	/* nop; nop; nop */
		0x009e7000, 0x500009e7,	/* nop; nop; sbdone */
};

RENDER_STRUCT scene = { 0 };

struct VC4_Vertex {
	uint16_t  X;				// X in 12.4 fixed point
	uint16_t  Y;				// Y in 12.4 fixed point
	uint32_t  Z;				// Z in 4 byte float format
	uint32_t  W;				// 1/W in byte float format
	uint32_t  VRed;				// Varying 0 (Red)
	uint32_t  VGreen;			// Varying 1 (Green)
	uint32_t  VBlue;			// Varying 2 (Blue)
} __packed;

static float angle = 0.0;


void DoRotate(double delta) {
	double cosTheta, sinTheta;
	angle += delta;
	if (angle >= (3.1415926384 * 2)) angle -= (3.1415926384 * 2);
	cosTheta = cos(angle);
	sinTheta = sin(angle);

	uint16_t centreX = (scene.renderWth / 2) << 4;						// triangle centre x
	uint16_t centreY = (scene.renderHt / 4) << 4;						// triangle centre y
	uint16_t half_shape_size = scene.renderWth/8;						// Half size of triangle
	double x1 = 0;
	double y1 = -half_shape_size;
	double x2 = -half_shape_size;
	double y2 = half_shape_size;
	double x3 = half_shape_size;
	double y3 = half_shape_size;
	double x4 = -half_shape_size;
	double y4 = -half_shape_size;

	struct VC4_Vertex* vd = (struct VC4_Vertex*)(uintptr_t)GPU_addr_to_ARM_addr(scene.vertexVC4);
	vd[0].X = centreX + (int16_t)((cosTheta * x1 - sinTheta * y1) * 16);
	vd[0].Y = centreY + (int16_t)((cosTheta * y1 + sinTheta * x1) * 16);
	vd[1].X = centreX + (int16_t)((cosTheta * x2 - sinTheta * y2) * 16);
	vd[1].Y = centreY + (int16_t)((cosTheta * y2 + sinTheta * x2) * 16);
	vd[2].X = centreX + (int16_t)((cosTheta * x3 - sinTheta * y3) * 16);
	vd[2].Y = centreY + (int16_t)((cosTheta * y3 + sinTheta * x3) * 16);
	vd[2].X = centreX + (int16_t)((cosTheta * x4 - sinTheta * y4) * 16);
	vd[3].Y = centreY + (int16_t)((cosTheta * y4 + sinTheta * x4) * 16);
}

void main()
{
	//Setup clocks first due to firmware bug
	clocks_init();

	//Setup videocore
	init_V3D();
	
	// set up serial console
	uart_init();
	lfb_init();
	
	dynamic_memory_alloc_init();
	console_init();	
	printf_init(console_print);
	init_audio_jack();

	//gl_quad_scene_init(&scene, &(shader[0]));

	//Create mmu table on Core 0
	init_page_table();

	mmu_init(); //Now turn on MMU on Core 0

	multicore_init(); //Now core_execute is avalible to be run after this

	printf("[CORE %d] [TEST] Testing 64bit unsigned int print %u\n", get_core_id(), 0xFFFFFFFFFFFFFFFF);
	printf("[INFO] GPU memory split is: %d\n", get_gpu_memory_split());
	for(int i = 0; i < 20; i++)
	{
		printf("\n");
	}

	INCLUDE_BINARY_FILE(believer, "src/audio/believer.bin", ".rodata.believer");
	audio_start = &believer_start;
	audio_end = &believer_end;
	core_execute(2, play_audio, NULL);

	// echo everything back
	while(1) 
	{
		uart_send(uart_getc());
	}

	// Release resources
	//V3D_DestroyScene(&scene);	
}
