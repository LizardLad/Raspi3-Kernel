// Bus Address
PERIPHERAL_BASE                = 0x3F000000 // Peripheral Base Address
BUS_ADDRESSES_l2CACHE_ENABLED  = 0x40000000 // Bus Addresses: disable_l2cache=0
BUS_ADDRESSES_l2CACHE_DISABLED = 0xC0000000 // Bus Addresses: disable_l2cache=1

// Mailbox
MAIL_BASE   = 0xB880 // Mailbox Base Address
MAIL_READ   =    0x0 // Mailbox Read Register
MAIL_CONFIG =   0x1C // Mailbox Config Register
MAIL_STATUS =   0x18 // Mailbox Status Register
MAIL_WRITE  =   0x20 // Mailbox Write Register

MAIL_EMPTY = 0x40000000 // Mailbox Status Register: Mailbox Empty (There is nothing to read from the Mailbox)
MAIL_FULL  = 0x80000000 // Mailbox Status Register: Mailbox Full  (There is no space to write into the Mailbox)

MAIL_POWER   = 0x0 // Mailbox Channel 0: Power Management Interface
MAIL_FB      = 0x1 // Mailbox Channel 1: Frame Buffer
MAIL_VUART   = 0x2 // Mailbox Channel 2: Virtual UART 
MAIL_VCHIQ   = 0x3 // Mailbox Channel 3: VCHIQ Interface
MAIL_LEDS    = 0x4 // Mailbox Channel 4: LEDs Interface
MAIL_BUTTONS = 0x5 // Mailbox Channel 5: Buttons Interface
MAIL_TOUCH   = 0x6 // Mailbox Channel 6: Touchscreen Interface
MAIL_COUNT   = 0x7 // Mailbox Channel 7: Counter
MAIL_TAGS    = 0x8 // Mailbox Channel 8: Tags (ARM to VC)

// Tags (ARM to VC)
Get_Firmware_Revision = 0x00000001 // VideoCore: Get Firmware Revision (Response: Firmware Revision)
Get_Board_Model       = 0x00010001 // Hardware: Get Board Model (Response: Board Model)
Get_Board_Revision    = 0x00010002 // Hardware: Get Board Revision (Response: Board Revision)
Get_Board_MAC_Address = 0x00010003 // Hardware: Get Board MAC Address (Response: MAC Address In Network Byte Order)
Get_Board_Serial      = 0x00010004 // Hardware: Get Board Serial (Response: Board Serial)
Get_ARM_Memory        = 0x00010005 // Hardware: Get ARM Memory (Response: Base Address In Bytes, Size In Bytes)
Get_VC_Memory         = 0x00010006 // Hardware: Get VC Memory (Response: Base Address In Bytes, Size In Bytes)
Get_Clocks            = 0x00010007 // Hardware: Get Clocks (Response: Parent Clock ID (0 For A Root Clock), Clock ID)
Get_Power_State       = 0x00020001 // Power: Get Power State (Response: Device ID, State)
Get_Timing            = 0x00020002 // Power: Get Timing (Response: Device ID, Enable Wait Time In Microseconds)
Set_Power_State       = 0x00028001 // Power: Set Power State (Response: Device ID, State)
Get_Clock_State       = 0x00030001 // Clocks: Get Clock State (Response: Clock ID, State)
Get_Clock_Rate        = 0x00030002 // Clocks: Get Clock Rate (Response: Clock ID, Rate In Hz)
Get_Voltage           = 0x00030003 // Voltage: Get Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
Get_Max_Clock_Rate    = 0x00030004 // Clocks: Get Max Clock Rate (Response: Clock ID, Rate In Hz)
Get_Max_Voltage       = 0x00030005 // Voltage: Get Max Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
Get_Temperature       = 0x00030006 // Voltage: Get Temperature (Response: Temperature ID, Value In Degrees C)
Get_Min_Clock_Rate    = 0x00030007 // Clocks: Get Min Clock Rate (Response: Clock ID, Rate In Hz)
Get_Min_Voltage       = 0x00030008 // Voltage: Get Min Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
Get_Turbo             = 0x00030009 // Clocks: Get Turbo (Response: ID, Level)
Get_Max_Temperature   = 0x0003000A // Voltage: Get Max Temperature (Response: Temperature ID, Value In Degrees C)
Allocate_Memory       = 0x0003000C // Memory: Allocates Contiguous Memory On The GPU (Response: Handle)
Lock_Memory           = 0x0003000D // Memory: Lock Buffer In Place, And Return A Bus Address (Response: Bus Address)
Unlock_Memory         = 0x0003000E // Memory: Unlock Buffer (Response: Status)
Release_Memory        = 0x0003000F // Memory: Free The Memory Buffer (Response: Status)
Execute_Code          = 0x00030010 // Memory: Calls The Function At Given (Bus) Address And With Arguments Given
Execute_QPU           = 0x00030011 // QPU: Calls The QPU Function At Given (Bus) Address And With Arguments Given (Response: Number Of QPUs, Control, No Flush, Timeout In ms)
Enable_QPU            = 0x00030012 // QPU: Enables The QPU (Response: Enable State)
Get_EDID_Block        = 0x00030020 // HDMI: Read Specificed EDID Block From Attached HDMI/DVI Device (Response: Block Number, Status, EDID Block (128 Bytes))
Set_Clock_State       = 0x00038001 // Clocks: Set Clock State (Response: Clock ID, State)
Set_Clock_Rate        = 0x00038002 // Clocks: Set Clock Rate (Response: Clock ID, Rate In Hz)
Set_Voltage           = 0x00038003 // Voltage: Set Voltage (Response: Voltage ID, Value Offset From 1.2V In Units Of 0.025V)
Set_Turbo             = 0x00038009 // Clocks: Set Turbo (Response: ID, Level)
Allocate_Buffer       = 0x00040001 // Frame Buffer: Allocate Buffer (Response: Frame Buffer Base Address In Bytes, Frame Buffer Size In Bytes)
Blank_Screen          = 0x00040002 // Frame Buffer: Blank Screen (Response: State)
Get_Physical_Display  = 0x00040003 // Frame Buffer: Get Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
Get_Virtual_Buffer    = 0x00040004 // Frame Buffer: Get Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
Get_Depth             = 0x00040005 // Frame Buffer: Get Depth (Response: Bits Per Pixel)
Get_Pixel_Order       = 0x00040006 // Frame Buffer: Get Pixel Order (Response: State)
Get_Alpha_Mode        = 0x00040007 // Frame Buffer: Get Alpha Mode (Response: State)
Get_Pitch             = 0x00040008 // Frame Buffer: Get Pitch (Response: Bytes Per Line)
Get_Virtual_Offset    = 0x00040009 // Frame Buffer: Get Virtual Offset (Response: X In Pixels, Y In Pixels)
Get_Overscan          = 0x0004000A // Frame Buffer: Get Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
Get_Palette           = 0x0004000B // Frame Buffer: Get Palette (Response: RGBA Palette Values (Index 0 To 255))
Test_Physical_Display = 0x00044003 // Frame Buffer: Test Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
Test_Virtual_Buffer   = 0x00044004 // Frame Buffer: Test Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
Test_Depth            = 0x00044005 // Frame Buffer: Test Depth (Response: Bits Per Pixel)
Test_Pixel_Order      = 0x00044006 // Frame Buffer: Test Pixel Order (Response: State)
Test_Alpha_Mode       = 0x00044007 // Frame Buffer: Test Alpha Mode (Response: State)
Test_Virtual_Offset   = 0x00044009 // Frame Buffer: Test Virtual Offset (Response: X In Pixels, Y In Pixels)
Test_Overscan         = 0x0004400A // Frame Buffer: Test Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
Test_Palette          = 0x0004400B // Frame Buffer: Test Palette (Response: RGBA Palette Values (Index 0 To 255))
Release_Buffer        = 0x00048001 // Frame Buffer: Release Buffer (Response: Releases And Disables The Frame Buffer)
Set_Physical_Display  = 0x00048003 // Frame Buffer: Set Physical (Display) Width/Height (Response: Width In Pixels, Height In Pixels)
Set_Virtual_Buffer    = 0x00048004 // Frame Buffer: Set Virtual (Buffer) Width/Height (Response: Width In Pixels, Height In Pixels)
Set_Depth             = 0x00048005 // Frame Buffer: Set Depth (Response: Bits Per Pixel)
Set_Pixel_Order       = 0x00048006 // Frame Buffer: Set Pixel Order (Response: State)
Set_Alpha_Mode        = 0x00048007 // Frame Buffer: Set Alpha Mode (Response: State)
Set_Virtual_Offset    = 0x00048009 // Frame Buffer: Set Virtual Offset (Response: X In Pixels, Y In Pixels)
Set_Overscan          = 0x0004800A // Frame Buffer: Set Overscan (Response: Top In Pixels, Bottom In Pixels, Left In Pixels, Right In Pixels)
Set_Palette           = 0x0004800B // Frame Buffer: Set Palette (Response: RGBA Palette Values (Index 0 To 255))
Get_Command_Line      = 0x00050001 // Config: Get Command Line (Response: ASCII Command Line String)
Get_DMA_Channels      = 0x00060001 // Shared Resource Management: Get DMA Channels (Response: Bits 0-15: DMA channels 0-15)

// Power: Unique Device ID's
PWR_SD_Card_ID = 0x0 // SD Card
PWR_UART0_ID   = 0x1 // UART0
PWR_UART1_ID   = 0x2 // UART1
PWR_USB_HCD_ID = 0x3 // USB HCD
PWR_I2C0_ID    = 0x4 // I2C0
PWR_I2C1_ID    = 0x5 // I2C1
PWR_I2C2_ID    = 0x6 // I2C2
PWR_SPI_ID     = 0x7 // SPI
PWR_CCP2TX_ID  = 0x8 // CCP2TX
// Clocks: Unique Clock ID's
CLK_EMMC_ID  = 0x1 // EMMC
CLK_UART_ID  = 0x2 // UART
CLK_ARM_ID   = 0x3 // ARM
CLK_CORE_ID  = 0x4 // CORE
CLK_V3D_ID   = 0x5 // V3D
CLK_H264_ID  = 0x6 // H264
CLK_ISP_ID   = 0x7 // ISP
CLK_SDRAM_ID = 0x8 // SDRAM
CLK_PIXEL_ID = 0x9 // PIXEL
CLK_PWM_ID   = 0xA // PWM

// Voltage: Unique Voltage ID's
VLT_Core_ID    = 0x1 // Core
VLT_SDRAM_C_ID = 0x2 // SDRAM_C
VLT_SDRAM_P_ID = 0x3 // SDRAM_P
VLT_SDRAM_I_ID = 0x4 // SDRAM_I
// CM / Clock Manager
CM_BASE   = 0x101000 // Clock Manager Base Address
CM_GNRICCTL =  0x000 // Clock Manager Generic Clock Control
CM_GNRICDIV =  0x004 // Clock Manager Generic Clock Divisor
CM_VPUCTL   =  0x008 // Clock Manager VPU Clock Control
CM_VPUDIV   =  0x00C // Clock Manager VPU Clock Divisor
CM_SYSCTL   =  0x010 // Clock Manager System Clock Control
CM_SYSDIV   =  0x014 // Clock Manager System Clock Divisor
CM_PERIACTL =  0x018 // Clock Manager PERIA Clock Control
CM_PERIADIV =  0x01C // Clock Manager PERIA Clock Divisor
CM_PERIICTL =  0x020 // Clock Manager PERII Clock Control
CM_PERIIDIV =  0x024 // Clock Manager PERII Clock Divisor
CM_H264CTL  =  0x028 // Clock Manager H264 Clock Control
CM_H264DIV  =  0x02C // Clock Manager H264 Clock Divisor
CM_ISPCTL   =  0x030 // Clock Manager ISP Clock Control
CM_ISPDIV   =  0x034 // Clock Manager ISP Clock Divisor
CM_V3DCTL   =  0x038 // Clock Manager V3D Clock Control
CM_V3DDIV   =  0x03C // Clock Manager V3D Clock Divisor
CM_CAM0CTL  =  0x040 // Clock Manager Camera 0 Clock Control
CM_CAM0DIV  =  0x044 // Clock Manager Camera 0 Clock Divisor
CM_CAM1CTL  =  0x048 // Clock Manager Camera 1 Clock Control
CM_CAM1DIV  =  0x04C // Clock Manager Camera 1 Clock Divisor
CM_CCP2CTL  =  0x050 // Clock Manager CCP2 Clock Control
CM_CCP2DIV  =  0x054 // Clock Manager CCP2 Clock Divisor
CM_DSI0ECTL =  0x058 // Clock Manager DSI0E Clock Control
CM_DSI0EDIV =  0x05C // Clock Manager DSI0E Clock Divisor
CM_DSI0PCTL =  0x060 // Clock Manager DSI0P Clock Control
CM_DSI0PDIV =  0x064 // Clock Manager DSI0P Clock Divisor
CM_DPICTL   =  0x068 // Clock Manager DPI Clock Control
CM_DPIDIV   =  0x06C // Clock Manager DPI Clock Divisor
CM_GP0CTL   =  0x070 // Clock Manager General Purpose 0 Clock Control
CM_GP0DIV   =  0x074 // Clock Manager General Purpose 0 Clock Divisor
CM_GP1CTL   =  0x078 // Clock Manager General Purpose 1 Clock Control
CM_GP1DIV   =  0x07C // Clock Manager General Purpose 1 Clock Divisor
CM_GP2CTL   =  0x080 // Clock Manager General Purpose 2 Clock Control
CM_GP2DIV   =  0x084 // Clock Manager General Purpose 2 Clock Divisor
CM_HSMCTL   =  0x088 // Clock Manager HSM Clock Control
CM_HSMDIV   =  0x08C // Clock Manager HSM Clock Divisor
CM_OTPCTL   =  0x090 // Clock Manager OTP Clock Control
CM_OTPDIV   =  0x094 // Clock Manager OTP Clock Divisor
CM_PCMCTL   =  0x098 // Clock Manager PCM / I2S Clock Control
CM_PCMDIV   =  0x09C // Clock Manager PCM / I2S Clock Divisor
CM_PWMCTL   =  0x0A0 // Clock Manager PWM Clock Control
CM_PWMDIV   =  0x0A4 // Clock Manager PWM Clock Divisor
CM_SLIMCTL  =  0x0A8 // Clock Manager SLIM Clock Control
CM_SLIMDIV  =  0x0AC // Clock Manager SLIM Clock Divisor
CM_SMICTL   =  0x0B0 // Clock Manager SMI Clock Control
CM_SMIDIV   =  0x0B4 // Clock Manager SMI Clock Divisor
CM_TCNTCTL  =  0x0C0 // Clock Manager TCNT Clock Control
CM_TCNTDIV  =  0x0C4 // Clock Manager TCNT Clock Divisor
CM_TECCTL   =  0x0C8 // Clock Manager TEC Clock Control
CM_TECDIV   =  0x0CC // Clock Manager TEC Clock Divisor
CM_TD0CTL   =  0x0D0 // Clock Manager TD0 Clock Control
CM_TD0DIV   =  0x0D4 // Clock Manager TD0 Clock Divisor
CM_TD1CTL   =  0x0D8 // Clock Manager TD1 Clock Control
CM_TD1DIV   =  0x0DC // Clock Manager TD1 Clock Divisor
CM_TSENSCTL =  0x0E0 // Clock Manager TSENS Clock Control
CM_TSENSDIV =  0x0E4 // Clock Manager TSENS Clock Divisor
CM_TIMERCTL =  0x0E8 // Clock Manager Timer Clock Control
CM_TIMERDIV =  0x0EC // Clock Manager Timer Clock Divisor
CM_UARTCTL  =  0x0F0 // Clock Manager UART Clock Control
CM_UARTDIV  =  0x0F4 // Clock Manager UART Clock Divisor
CM_VECCTL   =  0x0F8 // Clock Manager VEC Clock Control
CM_VECDIV   =  0x0FC // Clock Manager VEC Clock Divisor
CM_OSCCOUNT =  0x100 // Clock Manager Oscillator Count
CM_PLLA     =  0x104 // Clock Manager PLLA
CM_PLLC     =  0x108 // Clock Manager PLLC
CM_PLLD     =  0x10C // Clock Manager PLLD
CM_PLLH     =  0x110 // Clock Manager PLLH
CM_LOCK     =  0x114 // Clock Manager Lock
CM_EVENT    =  0x118 // Clock Manager Event
CM_INTEN    =  0x118 // Clock Manager INTEN
CM_DSI0HSCK =  0x120 // Clock Manager DSI0HSCK
CM_CKSM     =  0x124 // Clock Manager CKSM
CM_OSCFREQI =  0x128 // Clock Manager Oscillator Frequency Integer
CM_OSCFREQF =  0x12C // Clock Manager Oscillator Frequency Fraction
CM_PLLTCTL  =  0x130 // Clock Manager PLLT Control
CM_PLLTCNT0 =  0x134 // Clock Manager PLLT0
CM_PLLTCNT1 =  0x138 // Clock Manager PLLT1
CM_PLLTCNT2 =  0x13C // Clock Manager PLLT2
CM_PLLTCNT3 =  0x140 // Clock Manager PLLT3
CM_TDCLKEN  =  0x144 // Clock Manager TD Clock Enable
CM_BURSTCTL =  0x148 // Clock Manager Burst Control
CM_BURSTCNT =  0x14C // Clock Manager Burst
CM_DSI1ECTL =  0x158 // Clock Manager DSI1E Clock Control
CM_DSI1EDIV =  0x15C // Clock Manager DSI1E Clock Divisor
CM_DSI1PCTL =  0x160 // Clock Manager DSI1P Clock Control
CM_DSI1PDIV =  0x164 // Clock Manager DSI1P Clock Divisor
CM_DFTCTL   =  0x168 // Clock Manager DFT Clock Control
CM_DFTDIV   =  0x16C // Clock Manager DFT Clock Divisor
CM_PLLB     =  0x170 // Clock Manager PLLB
CM_PULSECTL =  0x190 // Clock Manager Pulse Clock Control
CM_PULSEDIV =  0x194 // Clock Manager Pulse Clock Divisor
CM_SDCCTL   =  0x1A8 // Clock Manager SDC Clock Control
CM_SDCDIV   =  0x1AC // Clock Manager SDC Clock Divisor
CM_ARMCTL   =  0x1B0 // Clock Manager ARM Clock Control
CM_ARMDIV   =  0x1B4 // Clock Manager ARM Clock Divisor
CM_AVEOCTL  =  0x1B8 // Clock Manager AVEO Clock Control
CM_AVEODIV  =  0x1BC // Clock Manager AVEO Clock Divisor
CM_EMMCCTL  =  0x1C0 // Clock Manager EMMC Clock Control
CM_EMMCDIV  =  0x1C4 // Clock Manager EMMC Clock Divisor
CM_SRC_OSCILLATOR =       0x01 // Clock Control: Clock Source = Oscillator
CM_SRC_TESTDEBUG0 =       0x02 // Clock Control: Clock Source = Test Debug 0
CM_SRC_TESTDEBUG1 =       0x03 // Clock Control: Clock Source = Test Debug 1
CM_SRC_PLLAPER    =       0x04 // Clock Control: Clock Source = PLLA Per
CM_SRC_PLLCPER    =       0x05 // Clock Control: Clock Source = PLLC Per
CM_SRC_PLLDPER    =       0x06 // Clock Control: Clock Source = PLLD Per
CM_SRC_HDMIAUX    =       0x07 // Clock Control: Clock Source = HDMI Auxiliary
CM_SRC_GND        =       0x08 // Clock Control: Clock Source = GND
CM_ENAB           =       0x10 // Clock Control: Enable The Clock Generator
CM_KILL           =       0x20 // Clock Control: Kill The Clock Generator
CM_BUSY           =       0x80 // Clock Control: Clock Generator Is Running
CM_FLIP           =      0x100 // Clock Control: Invert The Clock Generator Output
CM_MASH_1         =      0x200 // Clock Control: MASH Control = 1-Stage MASH (Equivalent To Non-MASH Dividers)
CM_MASH_2         =      0x400 // Clock Control: MASH Control = 2-Stage MASH
CM_MASH_3         =      0x600 // Clock Control: MASH Control = 3-Stage MASH
CM_PASSWORD       = 0x5A000000 // Clock Control: Password "5A"
// DMA Controller
DMA0_BASE  = 0x7000 // DMA Channel 0 Register Set
DMA1_BASE  = 0x7100 // DMA Channel 1 Register Set
DMA2_BASE  = 0x7200 // DMA Channel 2 Register Set
DMA3_BASE  = 0x7300 // DMA Channel 3 Register Set
DMA4_BASE  = 0x7400 // DMA Channel 4 Register Set
DMA5_BASE  = 0x7500 // DMA Channel 5 Register Set
DMA6_BASE  = 0x7600 // DMA Channel 6 Register Set
DMA7_BASE  = 0x7700 // DMA Channel 7 Register Set
DMA8_BASE  = 0x7800 // DMA Channel 8 Register Set
DMA9_BASE  = 0x7900 // DMA Channel 9 Register Set
DMA10_BASE = 0x7A00 // DMA Channel 10 Register Set
DMA11_BASE = 0x7B00 // DMA Channel 11 Register Set
DMA12_BASE = 0x7C00 // DMA Channel 12 Register Set
DMA13_BASE = 0x7D00 // DMA Channel 13 Register Set
DMA14_BASE = 0x7E00 // DMA Channel 14 Register Set
DMA_INT_STATUS = 0x7FE0 // Interrupt Status of each DMA Channel
DMA_ENABLE     = 0x7FF0 // Global Enable bits for each DMA Channel
DMA15_BASE = 0xE05000 // DMA Channel 15 Register Set
DMA_CS        =  0x0 // DMA Channel 0..14 Control & Status
DMA_CONBLK_AD =  0x4 // DMA Channel 0..14 Control Block Address
DMA_TI        =  0x8 // DMA Channel 0..14 CB Word 0 (Transfer Information)
DMA_SOURCE_AD =  0xC // DMA Channel 0..14 CB Word 1 (Source Address)
DMA_DEST_AD   = 0x10 // DMA Channel 0..14 CB Word 2 (Destination Address)
DMA_TXFR_LEN  = 0x14 // DMA Channel 0..14 CB Word 3 (Transfer Length)
DMA_STRIDE    = 0x18 // DMA Channel 0..14 CB Word 4 (2D Stride)
DMA_NEXTCONBK = 0x1C // DMA Channel 0..14 CB Word 5 (Next CB Address)
DMA_DEBUG     = 0x20 // DMA Channel 0..14 Debug
DMA_ACTIVE                         =        0x1 // DMA Control & Status: Activate the DMA
DMA_END                            =        0x2 // DMA Control & Status: DMA End Flag
DMA_INT                            =        0x4 // DMA Control & Status: Interrupt Status
DMA_DREQ                           =        0x8 // DMA Control & Status: DREQ State
DMA_PAUSED                         =       0x10 // DMA Control & Status: DMA Paused State
DMA_DREQ_STOPS_DMA                 =       0x20 // DMA Control & Status: DMA Paused by DREQ State
DMA_WAITING_FOR_OUTSTANDING_WRITES =       0x40 // DMA Control & Status: DMA is Waiting for the Last Write to be Received
DMA_ERROR                          =      0x100 // DMA Control & Status: DMA Error
DMA_PRIORITY_0                     =        0x0 // DMA Control & Status: AXI Priority Level 0
DMA_PRIORITY_1                     =    0x10000 // DMA Control & Status: AXI Priority Level 1
DMA_PRIORITY_2                     =    0x20000 // DMA Control & Status: AXI Priority Level 2
DMA_PRIORITY_3                     =    0x30000 // DMA Control & Status: AXI Priority Level 3
DMA_PRIORITY_4                     =    0x40000 // DMA Control & Status: AXI Priority Level 4
DMA_PRIORITY_5                     =    0x50000 // DMA Control & Status: AXI Priority Level 5
DMA_PRIORITY_6                     =    0x60000 // DMA Control & Status: AXI Priority Level 6
DMA_PRIORITY_7                     =    0x70000 // DMA Control & Status: AXI Priority Level 7
DMA_PRIORITY_8                     =    0x80000 // DMA Control & Status: AXI Priority Level 8
DMA_PRIORITY_9                     =    0x90000 // DMA Control & Status: AXI Priority Level 9
DMA_PRIORITY_10                    =    0xA0000 // DMA Control & Status: AXI Priority Level 10
DMA_PRIORITY_11                    =    0xB0000 // DMA Control & Status: AXI Priority Level 11
DMA_PRIORITY_12                    =    0xC0000 // DMA Control & Status: AXI Priority Level 12
DMA_PRIORITY_13                    =    0xD0000 // DMA Control & Status: AXI Priority Level 13
DMA_PRIORITY_14                    =    0xE0000 // DMA Control & Status: AXI Priority Level 14
DMA_PRIORITY_15                    =    0xF0000 // DMA Control & Status: AXI Priority Level 15
DMA_PRIORITY                       =    0xF0000 // DMA Control & Status: AXI Priority Level
DMA_PANIC_PRIORITY_0               =        0x0 // DMA Control & Status: AXI Panic Priority Level 0
DMA_PANIC_PRIORITY_1               =   0x100000 // DMA Control & Status: AXI Panic Priority Level 1
DMA_PANIC_PRIORITY_2               =   0x200000 // DMA Control & Status: AXI Panic Priority Level 2
DMA_PANIC_PRIORITY_3               =   0x300000 // DMA Control & Status: AXI Panic Priority Level 3
DMA_PANIC_PRIORITY_4               =   0x400000 // DMA Control & Status: AXI Panic Priority Level 4
DMA_PANIC_PRIORITY_5               =   0x500000 // DMA Control & Status: AXI Panic Priority Level 5
DMA_PANIC_PRIORITY_6               =   0x600000 // DMA Control & Status: AXI Panic Priority Level 6
DMA_PANIC_PRIORITY_7               =   0x700000 // DMA Control & Status: AXI Panic Priority Level 7
DMA_PANIC_PRIORITY_8               =   0x800000 // DMA Control & Status: AXI Panic Priority Level 8
DMA_PANIC_PRIORITY_9               =   0x900000 // DMA Control & Status: AXI Panic Priority Level 9
DMA_PANIC_PRIORITY_10              =   0xA00000 // DMA Control & Status: AXI Panic Priority Level 10
DMA_PANIC_PRIORITY_11              =   0xB00000 // DMA Control & Status: AXI Panic Priority Level 11
DMA_PANIC_PRIORITY_12              =   0xC00000 // DMA Control & Status: AXI Panic Priority Level 12
DMA_PANIC_PRIORITY_13              =   0xD00000 // DMA Control & Status: AXI Panic Priority Level 13
DMA_PANIC_PRIORITY_14              =   0xE00000 // DMA Control & Status: AXI Panic Priority Level 14
DMA_PANIC_PRIORITY_15              =   0xF00000 // DMA Control & Status: AXI Panic Priority Level 14
DMA_PANIC_PRIORITY                 =   0xF00000 // DMA Control & Status: AXI Panic Priority Level
DMA_WAIT_FOR_OUTSTANDING_WRITES    = 0x10000000 // DMA Control & Status: Wait for Outstanding Writes
DMA_DISDEBUG                       = 0x20000000 // DMA Control & Status: Disable Debug Pause Signal
DMA_ABORT                          = 0x40000000 // DMA Control & Status: Abort DMA
DMA_RESET                          = 0x80000000 // DMA Control & Status: DMA Channel Reset
DMA_INTEN           =       0x1 // DMA Transfer Information: Interrupt Enable
DMA_TDMODE          =       0x2 // DMA Transfer Information: 2D Mode
DMA_WAIT_RESP       =       0x8 // DMA Transfer Information: Wait for a Write Response
DMA_DEST_INC        =      0x10 // DMA Transfer Information: Destination Address Increment
DMA_DEST_WIDTH      =      0x20 // DMA Transfer Information: Destination Transfer Width
DMA_DEST_DREQ       =      0x40 // DMA Transfer Information: Control Destination Writes with DREQ
DMA_DEST_IGNORE     =      0x80 // DMA Transfer Information: Ignore Writes
DMA_SRC_INC         =     0x100 // DMA Transfer Information: Source Address Increment
DMA_SRC_WIDTH       =     0x200 // DMA Transfer Information: Source Transfer Width
DMA_SRC_DREQ        =     0x400 // DMA Transfer Information: Control Source Reads with DREQ
DMA_SRC_IGNORE      =     0x800 // DMA Transfer Information: Ignore Reads
DMA_BURST_LENGTH_1  =       0x0 // DMA Transfer Information: Burst Transfer Length 1 Word
DMA_BURST_LENGTH_2  =    0x1000 // DMA Transfer Information: Burst Transfer Length 2 Words
DMA_BURST_LENGTH_3  =    0x2000 // DMA Transfer Information: Burst Transfer Length 3 Words
DMA_BURST_LENGTH_4  =    0x3000 // DMA Transfer Information: Burst Transfer Length 4 Words
DMA_BURST_LENGTH_5  =    0x4000 // DMA Transfer Information: Burst Transfer Length 5 Words
DMA_BURST_LENGTH_6  =    0x5000 // DMA Transfer Information: Burst Transfer Length 6 Words
DMA_BURST_LENGTH_7  =    0x6000 // DMA Transfer Information: Burst Transfer Length 7 Words
DMA_BURST_LENGTH_8  =    0x7000 // DMA Transfer Information: Burst Transfer Length 8 Words
DMA_BURST_LENGTH_9  =    0x8000 // DMA Transfer Information: Burst Transfer Length 9 Words
DMA_BURST_LENGTH_10 =    0x9000 // DMA Transfer Information: Burst Transfer Length 10 Words
DMA_BURST_LENGTH_11 =    0xA000 // DMA Transfer Information: Burst Transfer Length 11 Words
DMA_BURST_LENGTH_12 =    0xB000 // DMA Transfer Information: Burst Transfer Length 12 Words
DMA_BURST_LENGTH_13 =    0xC000 // DMA Transfer Information: Burst Transfer Length 13 Words
DMA_BURST_LENGTH_14 =    0xD000 // DMA Transfer Information: Burst Transfer Length 14 Words
DMA_BURST_LENGTH_15 =    0xE000 // DMA Transfer Information: Burst Transfer Length 15 Words
DMA_BURST_LENGTH_16 =    0xF000 // DMA Transfer Information: Burst Transfer Length 16 Words
DMA_BURST_LENGTH    =    0xF000 // DMA Transfer Information: Burst Transfer Length
DMA_PERMAP_0        =       0x0 // DMA Transfer Information: Peripheral Mapping Continuous Un-paced Transfer
DMA_PERMAP_1        =   0x10000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 1
DMA_PERMAP_2        =   0x20000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 2
DMA_PERMAP_3        =   0x30000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 3
DMA_PERMAP_4        =   0x40000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 4
DMA_PERMAP_5        =   0x50000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 5
DMA_PERMAP_6        =   0x60000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 6
DMA_PERMAP_7        =   0x70000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 7
DMA_PERMAP_8        =   0x80000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 8
DMA_PERMAP_9        =   0x90000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 9
DMA_PERMAP_10       =   0xA0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 10
DMA_PERMAP_11       =   0xB0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 11
DMA_PERMAP_12       =   0xC0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 12
DMA_PERMAP_13       =   0xD0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 13
DMA_PERMAP_14       =   0xE0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 14
DMA_PERMAP_15       =   0xF0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 15
DMA_PERMAP_16       =  0x100000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 16
DMA_PERMAP_17       =  0x110000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 17
DMA_PERMAP_18       =  0x120000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 18
DMA_PERMAP_19       =  0x130000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 19
DMA_PERMAP_20       =  0x140000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 20
DMA_PERMAP_21       =  0x150000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 21
DMA_PERMAP_22       =  0x160000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 22
DMA_PERMAP_23       =  0x170000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 23
DMA_PERMAP_24       =  0x180000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 24
DMA_PERMAP_25       =  0x190000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 25
DMA_PERMAP_26       =  0x1A0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 26
DMA_PERMAP_27       =  0x1B0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 27
DMA_PERMAP_28       =  0x1C0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 28
DMA_PERMAP_29       =  0x1D0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 29
DMA_PERMAP_30       =  0x1E0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 30
DMA_PERMAP_31       =  0x1F0000 // DMA Transfer Information: Peripheral Mapping Peripheral Number 31
DMA_PERMAP          =  0x1F0000 // DMA Transfer Information: Peripheral Mapping
DMA_WAITS_0         =       0x0 // DMA Transfer Information: Add No Wait Cycles
DMA_WAITS_1         =  0x200000 // DMA Transfer Information: Add 1 Wait Cycle
DMA_WAITS_2         =  0x400000 // DMA Transfer Information: Add 2 Wait Cycles
DMA_WAITS_3         =  0x600000 // DMA Transfer Information: Add 3 Wait Cycles
DMA_WAITS_4         =  0x800000 // DMA Transfer Information: Add 4 Wait Cycles
DMA_WAITS_5         =  0xA00000 // DMA Transfer Information: Add 5 Wait Cycles
DMA_WAITS_6         =  0xC00000 // DMA Transfer Information: Add 6 Wait Cycles
DMA_WAITS_7         =  0xE00000 // DMA Transfer Information: Add 7 Wait Cycles
DMA_WAITS_8         = 0x1000000 // DMA Transfer Information: Add 8 Wait Cycles
DMA_WAITS_9         = 0x1200000 // DMA Transfer Information: Add 9 Wait Cycles
DMA_WAITS_10        = 0x1400000 // DMA Transfer Information: Add 10 Wait Cycles
DMA_WAITS_11        = 0x1600000 // DMA Transfer Information: Add 11 Wait Cycles
DMA_WAITS_12        = 0x1800000 // DMA Transfer Information: Add 12 Wait Cycles
DMA_WAITS_13        = 0x1A00000 // DMA Transfer Information: Add 13 Wait Cycles
DMA_WAITS_14        = 0x1C00000 // DMA Transfer Information: Add 14 Wait Cycles
DMA_WAITS_15        = 0x1E00000 // DMA Transfer Information: Add 15 Wait Cycles
DMA_WAITS_16        = 0x2000000 // DMA Transfer Information: Add 16 Wait Cycles
DMA_WAITS_17        = 0x2200000 // DMA Transfer Information: Add 17 Wait Cycles
DMA_WAITS_18        = 0x2400000 // DMA Transfer Information: Add 18 Wait Cycles
DMA_WAITS_19        = 0x2600000 // DMA Transfer Information: Add 19 Wait Cycles
DMA_WAITS_20        = 0x2800000 // DMA Transfer Information: Add 20 Wait Cycles
DMA_WAITS_21        = 0x2A00000 // DMA Transfer Information: Add 21 Wait Cycles
DMA_WAITS_22        = 0x2C00000 // DMA Transfer Information: Add 22 Wait Cycles
DMA_WAITS_23        = 0x2E00000 // DMA Transfer Information: Add 23 Wait Cycles
DMA_WAITS_24        = 0x3000000 // DMA Transfer Information: Add 24 Wait Cycles
DMA_WAITS_25        = 0x3200000 // DMA Transfer Information: Add 25 Wait Cycles
DMA_WAITS_26        = 0x3400000 // DMA Transfer Information: Add 26 Wait Cycles
DMA_WAITS_27        = 0x3600000 // DMA Transfer Information: Add 27 Wait Cycles
DMA_WAITS_28        = 0x3800000 // DMA Transfer Information: Add 28 Wait Cycles
DMA_WAITS_29        = 0x3A00000 // DMA Transfer Information: Add 29 Wait Cycles
DMA_WAITS_30        = 0x3C00000 // DMA Transfer Information: Add 30 Wait Cycles
DMA_WAITS_31        = 0x3E00000 // DMA Transfer Information: Add 31 Wait Cycles
DMA_WAITS           = 0x3E00000 // DMA Transfer Information: Add Wait Cycles
DMA_NO_WIDE_BURSTS  = 0x4000000 // DMA Transfer Information: Don't Do Wide Writes as a 2 Beat Burst

DMA_XLENGTH =     0xFFFF // DMA Transfer Length: Transfer Length in Bytes
DMA_YLENGTH = 0x3FFF0000 // DMA Transfer Length: When in 2D Mode, This is the Y Transfer Length

DMA_S_STRIDE =     0xFFFF // DMA 2D Stride: Source Stride (2D Mode)
DMA_D_STRIDE = 0xFFFF0000 // DMA 2D Stride: Destination Stride (2D Mode)

DMA_READ_LAST_NOT_SET_ERROR =        0x1 // DMA Debug: Read Last Not Set Error
DMA_FIFO_ERROR              =        0x2 // DMA Debug: Fifo Error
DMA_READ_ERROR              =        0x4 // DMA Debug: Slave Read Response Error
DMA_OUTSTANDING_WRITES      =       0xF0 // DMA Debug: DMA Outstanding Writes Counter
DMA_ID                      =     0xFF00 // DMA Debug: DMA ID
DMA_STATE                   =  0x1FF0000 // DMA Debug: DMA State Machine State
DMA_VERSION                 =  0xE000000 // DMA Debug: DMA Version
DMA_LITE                    = 0x10000000 // DMA Debug: DMA Lite

DMA_INT0  =    0x1 // DMA Interrupt Status: Interrupt Status of DMA Engine 0
DMA_INT1  =    0x2 // DMA Interrupt Status: Interrupt Status of DMA Engine 1
DMA_INT2  =    0x4 // DMA Interrupt Status: Interrupt Status of DMA Engine 2
DMA_INT3  =    0x8 // DMA Interrupt Status: Interrupt Status of DMA Engine 3
DMA_INT4  =   0x10 // DMA Interrupt Status: Interrupt Status of DMA Engine 4
DMA_INT5  =   0x20 // DMA Interrupt Status: Interrupt Status of DMA Engine 5
DMA_INT6  =   0x40 // DMA Interrupt Status: Interrupt Status of DMA Engine 6
DMA_INT7  =   0x80 // DMA Interrupt Status: Interrupt Status of DMA Engine 7
DMA_INT8  =  0x100 // DMA Interrupt Status: Interrupt Status of DMA Engine 8
DMA_INT9  =  0x200 // DMA Interrupt Status: Interrupt Status of DMA Engine 9
DMA_INT10 =  0x400 // DMA Interrupt Status: Interrupt Status of DMA Engine 10
DMA_INT11 =  0x800 // DMA Interrupt Status: Interrupt Status of DMA Engine 11
DMA_INT12 = 0x1000 // DMA Interrupt Status: Interrupt Status of DMA Engine 12
DMA_INT13 = 0x2000 // DMA Interrupt Status: Interrupt Status of DMA Engine 13
DMA_INT14 = 0x4000 // DMA Interrupt Status: Interrupt Status of DMA Engine 14
DMA_INT15 = 0x8000 // DMA Interrupt Status: Interrupt Status of DMA Engine 15

DMA_EN0  =    0x1 // DMA Enable: Enable DMA Engine 0
DMA_EN1  =    0x2 // DMA Enable: Enable DMA Engine 1
DMA_EN2  =    0x4 // DMA Enable: Enable DMA Engine 2
DMA_EN3  =    0x8 // DMA Enable: Enable DMA Engine 3
DMA_EN4  =   0x10 // DMA Enable: Enable DMA Engine 4
DMA_EN5  =   0x20 // DMA Enable: Enable DMA Engine 5
DMA_EN6  =   0x40 // DMA Enable: Enable DMA Engine 6
DMA_EN7  =   0x80 // DMA Enable: Enable DMA Engine 7
DMA_EN8  =  0x100 // DMA Enable: Enable DMA Engine 8
DMA_EN9  =  0x200 // DMA Enable: Enable DMA Engine 9
DMA_EN10 =  0x400 // DMA Enable: Enable DMA Engine 10
DMA_EN11 =  0x800 // DMA Enable: Enable DMA Engine 11
DMA_EN12 = 0x1000 // DMA Enable: Enable DMA Engine 12
DMA_EN13 = 0x2000 // DMA Enable: Enable DMA Engine 13
DMA_EN14 = 0x4000 // DMA Enable: Enable DMA Engine 14

// GPIO
GPIO_BASE      = 0x200000 // GPIO Base Address
GPIO_GPFSEL0   =      0x0 // GPIO Function Select 0
GPIO_GPFSEL1   =      0x4 // GPIO Function Select 1
GPIO_GPFSEL2   =      0x8 // GPIO Function Select 2
GPIO_GPFSEL3   =      0xC // GPIO Function Select 3
GPIO_GPFSEL4   =     0x10 // GPIO Function Select 4
GPIO_GPFSEL5   =     0x14 // GPIO Function Select 5
GPIO_GPSET0    =     0x1C // GPIO Pin Output Set 0
GPIO_GPSET1    =     0x20 // GPIO Pin Output Set 1
GPIO_GPCLR0    =     0x28 // GPIO Pin Output Clear 0
GPIO_GPCLR1    =     0x2C // GPIO Pin Output Clear 1
GPIO_GPLEV0    =     0x34 // GPIO Pin Level 0
GPIO_GPLEV1    =     0x38 // GPIO Pin Level 1
GPIO_GPEDS0    =     0x40 // GPIO Pin Event Detect Status 0
GPIO_GPEDS1    =     0x44 // GPIO Pin Event Detect Status 1
GPIO_GPREN0    =     0x4C // GPIO Pin Rising Edge Detect Enable 0
GPIO_GPREN1    =     0x50 // GPIO Pin Rising Edge Detect Enable 1
GPIO_GPFEN0    =     0x58 // GPIO Pin Falling Edge Detect Enable 0
GPIO_GPFEN1    =     0x5C // GPIO Pin Falling Edge Detect Enable 1
GPIO_GPHEN0    =     0x64 // GPIO Pin High Detect Enable 0
GPIO_GPHEN1    =     0x68 // GPIO Pin High Detect Enable 1
GPIO_GPLEN0    =     0x70 // GPIO Pin Low Detect Enable 0
GPIO_GPLEN1    =     0x74 // GPIO Pin Low Detect Enable 1
GPIO_GPAREN0   =     0x7C // GPIO Pin Async. Rising Edge Detect 0
GPIO_GPAREN1   =     0x80 // GPIO Pin Async. Rising Edge Detect 1
GPIO_GPAFEN0   =     0x88 // GPIO Pin Async. Falling Edge Detect 0
GPIO_GPAFEN1   =     0x8C // GPIO Pin Async. Falling Edge Detect 1
GPIO_GPPUD     =     0x94 // GPIO Pin Pull-up/down Enable
GPIO_GPPUDCLK0 =     0x98 // GPIO Pin Pull-up/down Enable Clock 0
GPIO_GPPUDCLK1 =     0x9C // GPIO Pin Pull-up/down Enable Clock 1
GPIO_TEST      =     0xB0 // GPIO Test

GPIO_FSEL0_IN   = 0x0 // GPIO Function Select: GPIO Pin X0 Is An Input
GPIO_FSEL0_OUT  = 0x1 // GPIO Function Select: GPIO Pin X0 Is An Output
GPIO_FSEL0_ALT0 = 0x4 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 0
GPIO_FSEL0_ALT1 = 0x5 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 1
GPIO_FSEL0_ALT2 = 0x6 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 2
GPIO_FSEL0_ALT3 = 0x7 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 3
GPIO_FSEL0_ALT4 = 0x3 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 4
GPIO_FSEL0_ALT5 = 0x2 // GPIO Function Select: GPIO Pin X0 Takes Alternate Function 5
GPIO_FSEL0_CLR  = 0x7 // GPIO Function Select: GPIO Pin X0 Clear Bits

GPIO_FSEL1_IN   =  0x0 // GPIO Function Select: GPIO Pin X1 Is An Input
GPIO_FSEL1_OUT  =  0x8 // GPIO Function Select: GPIO Pin X1 Is An Output
GPIO_FSEL1_ALT0 = 0x20 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 0
GPIO_FSEL1_ALT1 = 0x28 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 1
GPIO_FSEL1_ALT2 = 0x30 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 2
GPIO_FSEL1_ALT3 = 0x38 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 3
GPIO_FSEL1_ALT4 = 0x18 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 4
GPIO_FSEL1_ALT5 = 0x10 // GPIO Function Select: GPIO Pin X1 Takes Alternate Function 5
GPIO_FSEL1_CLR  = 0x38 // GPIO Function Select: GPIO Pin X1 Clear Bits

GPIO_FSEL2_IN   =   0x0 // GPIO Function Select: GPIO Pin X2 Is An Input
GPIO_FSEL2_OUT  =  0x40 // GPIO Function Select: GPIO Pin X2 Is An Output
GPIO_FSEL2_ALT0 = 0x100 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 0
GPIO_FSEL2_ALT1 = 0x140 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 1
GPIO_FSEL2_ALT2 = 0x180 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 2
GPIO_FSEL2_ALT3 = 0x1C0 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 3
GPIO_FSEL2_ALT4 =  0xC0 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 4
GPIO_FSEL2_ALT5 =  0x80 // GPIO Function Select: GPIO Pin X2 Takes Alternate Function 5
GPIO_FSEL2_CLR  = 0x1C0 // GPIO Function Select: GPIO Pin X2 Clear Bits

GPIO_FSEL3_IN   =   0x0 // GPIO Function Select: GPIO Pin X3 Is An Input
GPIO_FSEL3_OUT  = 0x200 // GPIO Function Select: GPIO Pin X3 Is An Output
GPIO_FSEL3_ALT0 = 0x800 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 0
GPIO_FSEL3_ALT1 = 0xA00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 1
GPIO_FSEL3_ALT2 = 0xC00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 2
GPIO_FSEL3_ALT3 = 0xE00 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 3
GPIO_FSEL3_ALT4 = 0x600 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 4
GPIO_FSEL3_ALT5 = 0x400 // GPIO Function Select: GPIO Pin X3 Takes Alternate Function 5
GPIO_FSEL3_CLR  = 0xE00 // GPIO Function Select: GPIO Pin X3 Clear Bits

GPIO_FSEL4_IN   =    0x0 // GPIO Function Select: GPIO Pin X4 Is An Input
GPIO_FSEL4_OUT  = 0x1000 // GPIO Function Select: GPIO Pin X4 Is An Output
GPIO_FSEL4_ALT0 = 0x4000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 0
GPIO_FSEL4_ALT1 = 0x5000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 1
GPIO_FSEL4_ALT2 = 0x6000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 2
GPIO_FSEL4_ALT3 = 0x7000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 3
GPIO_FSEL4_ALT4 = 0x3000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 4
GPIO_FSEL4_ALT5 = 0x2000 // GPIO Function Select: GPIO Pin X4 Takes Alternate Function 5
GPIO_FSEL4_CLR  = 0x7000 // GPIO Function Select: GPIO Pin X4 Clear Bits

GPIO_FSEL5_IN   =     0x0 // GPIO Function Select: GPIO Pin X5 Is An Input
GPIO_FSEL5_OUT  =  0x8000 // GPIO Function Select: GPIO Pin X5 Is An Output
GPIO_FSEL5_ALT0 = 0x20000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 0
GPIO_FSEL5_ALT1 = 0x28000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 1
GPIO_FSEL5_ALT2 = 0x30000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 2
GPIO_FSEL5_ALT3 = 0x38000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 3
GPIO_FSEL5_ALT4 = 0x18000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 4
GPIO_FSEL5_ALT5 = 0x10000 // GPIO Function Select: GPIO Pin X5 Takes Alternate Function 5
GPIO_FSEL5_CLR  = 0x38000 // GPIO Function Select: GPIO Pin X5 Clear Bits

GPIO_FSEL6_IN   =      0x0 // GPIO Function Select: GPIO Pin X6 Is An Input
GPIO_FSEL6_OUT  =  0x40000 // GPIO Function Select: GPIO Pin X6 Is An Output
GPIO_FSEL6_ALT0 = 0x100000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 0
GPIO_FSEL6_ALT1 = 0x140000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 1
GPIO_FSEL6_ALT2 = 0x180000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 2
GPIO_FSEL6_ALT3 = 0x1C0000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 3
GPIO_FSEL6_ALT4 =  0xC0000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 4
GPIO_FSEL6_ALT5 =  0x80000 // GPIO Function Select: GPIO Pin X6 Takes Alternate Function 5
GPIO_FSEL6_CLR  = 0x1C0000 // GPIO Function Select: GPIO Pin X6 Clear Bits

GPIO_FSEL7_IN   =      0x0 // GPIO Function Select: GPIO Pin X7 Is An Input
GPIO_FSEL7_OUT  = 0x200000 // GPIO Function Select: GPIO Pin X7 Is An Output
GPIO_FSEL7_ALT0 = 0x800000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 0
GPIO_FSEL7_ALT1 = 0xA00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 1
GPIO_FSEL7_ALT2 = 0xC00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 2
GPIO_FSEL7_ALT3 = 0xE00000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 3
GPIO_FSEL7_ALT4 = 0x600000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 4
GPIO_FSEL7_ALT5 = 0x400000 // GPIO Function Select: GPIO Pin X7 Takes Alternate Function 5
GPIO_FSEL7_CLR  = 0xE00000 // GPIO Function Select: GPIO Pin X7 Clear Bits

GPIO_FSEL8_IN   =       0x0 // GPIO Function Select: GPIO Pin X8 Is An Input
GPIO_FSEL8_OUT  = 0x1000000 // GPIO Function Select: GPIO Pin X8 Is An Output
GPIO_FSEL8_ALT0 = 0x4000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 0
GPIO_FSEL8_ALT1 = 0x5000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 1
GPIO_FSEL8_ALT2 = 0x6000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 2
GPIO_FSEL8_ALT3 = 0x7000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 3
GPIO_FSEL8_ALT4 = 0x3000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 4
GPIO_FSEL8_ALT5 = 0x2000000 // GPIO Function Select: GPIO Pin X8 Takes Alternate Function 5
GPIO_FSEL8_CLR  = 0x7000000 // GPIO Function Select: GPIO Pin X8 Clear Bits

GPIO_FSEL9_IN   =        0x0 // GPIO Function Select: GPIO Pin X9 Is An Input
GPIO_FSEL9_OUT  =  0x8000000 // GPIO Function Select: GPIO Pin X9 Is An Output
GPIO_FSEL9_ALT0 = 0x20000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 0
GPIO_FSEL9_ALT1 = 0x28000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 1
GPIO_FSEL9_ALT2 = 0x30000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 2
GPIO_FSEL9_ALT3 = 0x38000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 3
GPIO_FSEL9_ALT4 = 0x18000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 4
GPIO_FSEL9_ALT5 = 0x10000000 // GPIO Function Select: GPIO Pin X9 Takes Alternate Function 5
GPIO_FSEL9_CLR  = 0x38000000 // GPIO Function Select: GPIO Pin X9 Clear Bits

GPIO_0  =        0x1 // GPIO Pin 0: 0
GPIO_1  =        0x2 // GPIO Pin 0: 1
GPIO_2  =        0x4 // GPIO Pin 0: 2
GPIO_3  =        0x8 // GPIO Pin 0: 3
GPIO_4  =       0x10 // GPIO Pin 0: 4
GPIO_5  =       0x20 // GPIO Pin 0: 5
GPIO_6  =       0x40 // GPIO Pin 0: 6
GPIO_7  =       0x80 // GPIO Pin 0: 7
GPIO_8  =      0x100 // GPIO Pin 0: 8
GPIO_9  =      0x200 // GPIO Pin 0: 9
GPIO_10 =      0x400 // GPIO Pin 0: 10
GPIO_11 =      0x800 // GPIO Pin 0: 11
GPIO_12 =     0x1000 // GPIO Pin 0: 12
GPIO_13 =     0x2000 // GPIO Pin 0: 13
GPIO_14 =     0x4000 // GPIO Pin 0: 14
GPIO_15 =     0x8000 // GPIO Pin 0: 15
GPIO_16 =    0x10000 // GPIO Pin 0: 16
GPIO_17 =    0x20000 // GPIO Pin 0: 17
GPIO_18 =    0x40000 // GPIO Pin 0: 18
GPIO_19 =    0x80000 // GPIO Pin 0: 19
GPIO_20 =   0x100000 // GPIO Pin 0: 20
GPIO_21 =   0x200000 // GPIO Pin 0: 21
GPIO_22 =   0x400000 // GPIO Pin 0: 22
GPIO_23 =   0x800000 // GPIO Pin 0: 23
GPIO_24 =  0x1000000 // GPIO Pin 0: 24
GPIO_25 =  0x2000000 // GPIO Pin 0: 25
GPIO_26 =  0x4000000 // GPIO Pin 0: 26
GPIO_27 =  0x8000000 // GPIO Pin 0: 27
GPIO_28 = 0x10000000 // GPIO Pin 0: 28
GPIO_29 = 0x20000000 // GPIO Pin 0: 29
GPIO_30 = 0x40000000 // GPIO Pin 0: 30
GPIO_31 = 0x80000000 // GPIO Pin 0: 31

GPIO_32 =        0x1 // GPIO Pin 1: 32
GPIO_33 =        0x2 // GPIO Pin 1: 33
GPIO_34 =        0x4 // GPIO Pin 1: 34
GPIO_35 =        0x8 // GPIO Pin 1: 35
GPIO_36 =       0x10 // GPIO Pin 1: 36
GPIO_37 =       0x20 // GPIO Pin 1: 37
GPIO_38 =       0x40 // GPIO Pin 1: 38
GPIO_39 =       0x80 // GPIO Pin 1: 39
GPIO_40 =      0x100 // GPIO Pin 1: 40
GPIO_41 =      0x200 // GPIO Pin 1: 41
GPIO_42 =      0x400 // GPIO Pin 1: 42
GPIO_43 =      0x800 // GPIO Pin 1: 43
GPIO_44 =     0x1000 // GPIO Pin 1: 44
GPIO_45 =     0x2000 // GPIO Pin 1: 45
GPIO_46 =     0x4000 // GPIO Pin 1: 46
GPIO_47 =     0x8000 // GPIO Pin 1: 47
GPIO_48 =    0x10000 // GPIO Pin 1: 48
GPIO_49 =    0x20000 // GPIO Pin 1: 49
GPIO_50 =    0x40000 // GPIO Pin 1: 50
GPIO_51 =    0x80000 // GPIO Pin 1: 51
GPIO_52 =   0x100000 // GPIO Pin 1: 52
GPIO_53 =   0x200000 // GPIO Pin 1: 53

// PCM / I2S Audio Interface
PCM_BASE     = 0x203000 // PCM Base Address
PCM_CS_A     =      0x0 // PCM Control & Status
PCM_FIFO_A   =      0x4 // PCM FIFO Data
PCM_MODE_A   =      0x8 // PCM Mode
PCM_RXC_A    =      0xC // PCM Receive Configuration
PCM_TXC_A    =     0x10 // PCM Transmit Configuration
PCM_DREQ_A   =     0x14 // PCM DMA Request Level
PCM_INTEN_A  =     0x18 // PCM Interrupt Enables
PCM_INTSTC_A =     0x1C // PCM Interrupt Status & Clear
PCM_GRAY     =     0x20 // PCM Gray Mode Control

PCM_EN      =       0x1 // PCM Control & Status: Enable the PCM Audio Interface
PCM_RXON    =       0x2 // PCM Control & Status: Enable Reception
PCM_TXON    =       0x4 // PCM Control & Status: Enable Transmission
PCM_TXCLR   =       0x8 // PCM Control & Status: Clear the TX FIFO
PCM_RXCLR   =      0x10 // PCM Control & Status: Clear the RX FIFO
PCM_TXTHR_0 =       0x0 // PCM Control & Status: Sets the TX FIFO Threshold at which point the TXW flag is Set when the TX FIFO is Empty
PCM_TXTHR_1 =      0x20 // PCM Control & Status: Sets the TX FIFO Threshold at which point the TXW flag is Set when the TX FIFO is less than Full
PCM_TXTHR_2 =      0x40 // PCM Control & Status: Sets the TX FIFO Threshold at which point the TXW flag is Set when the TX FIFO is less than Full
PCM_TXTHR_3 =      0x60 // PCM Control & Status: Sets the TX FIFO Threshold at which point the TXW flag is Set when the TX FIFO is Full but for one Sample
PCM_TXTHR   =      0x60 // PCM Control & Status: Sets the TX FIFO Threshold at which point the TXW flag is Set
PCM_RXTHR_0 =       0x0 // PCM Control & Status: Sets the RX FIFO Threshold at which point the RXR flag is Set when we have a single Sample in the RX FIFO
PCM_RXTHR_1 =      0x80 // PCM Control & Status: Sets the RX FIFO Threshold at which point the RXR flag is Set when the RX FIFO is at least Full
PCM_RXTHR_2 =     0x100 // PCM Control & Status: Sets the RX FIFO Threshold at which point the RXR flag is Set when the RX FIFO is at least Full
PCM_RXTHR_3 =     0x180 // PCM Control & Status: Sets the RX FIFO Threshold at which point the RXR flag is Set when the RX FIFO is Full
PCM_RXTHR   =     0x180 // PCM Control & Status: Sets the RX FIFO Threshold at which point the RXR flag is Set
PCM_DMAEN   =     0x200 // PCM Control & Status: DMA DREQ Enable
PCM_TXSYNC  =    0x2000 // PCM Control & Status: TX FIFO Sync
PCM_RXSYNC  =    0x4000 // PCM Control & Status: RX FIFO Sync
PCM_TXERR   =    0x8000 // PCM Control & Status: TX FIFO Error
PCM_RXERR   =   0x10000 // PCM Control & Status: RX FIFO Error
PCM_TXW     =   0x20000 // PCM Control & Status: Indicates that the TX FIFO needs Writing
PCM_RXR     =   0x40000 // PCM Control & Status: Indicates that the RX FIFO needs Reading
PCM_TXD     =   0x80000 // PCM Control & Status: Indicates that the TX FIFO can accept Data
PCM_RXD     =  0x100000 // PCM Control & Status: Indicates that the RX FIFO contains Data
PCM_TXE     =  0x200000 // PCM Control & Status: TX FIFO is Empty
PCM_RXF     =  0x400000 // PCM Control & Status: RX FIFO is Full
PCM_RXSEX   =  0x800000 // PCM Control & Status: RX Sign Extend
PCM_SYNC    = 0x1000000 // PCM Control & Status: PCM Clock Sync helper
PCM_STBY    = 0x2000000 // PCM Control & Status: RAM Standby

PSM_FSLEN   =      0x3FF // PCM Mode: Frame Sync Length
PSM_FLEN    =    0xFFC00 // PCM Mode: Frame Length
PCM_FSI     =   0x100000 // PCM Mode: Frame Sync Invert this logically inverts the Frame Sync Signal
PCM_FSM     =   0x200000 // PCM Mode: Frame Sync Mode
PCM_CLKI    =   0x400000 // PCM Mode: Clock Invert this logically inverts the PCM_CLK Signal
PCM_CLKM    =   0x800000 // PCM Mode: PCM Clock Mode
PCM_FTXP    =  0x1000000 // PCM Mode: Transmit Frame Packed Mode
PCM_FRXP    =  0x2000000 // PCM Mode: Receive Frame Packed Mode
PCM_PDME    =  0x4000000 // PCM Mode: PDM Input Mode Enable
PCM_PDMN    =  0x8000000 // PCM Mode: PDM Decimation Factor (N)
PCM_CLK_DIS = 0x10000000 // PCM Mode: PCM Clock Disable

PCM_CH2WID_8  =        0x0 // PCM Receive & Transmit Configuration: Channel 2 Width 8 bits Wide
PCM_CH2WID_9  =        0x1 // PCM Receive & Transmit Configuration: Channel 2 Width 9 bits Wide
PCM_CH2WID_10 =        0x2 // PCM Receive & Transmit Configuration: Channel 2 Width 10 bits Wide
PCM_CH2WID_11 =        0x3 // PCM Receive & Transmit Configuration: Channel 2 Width 11 bits Wide
PCM_CH2WID_12 =        0x4 // PCM Receive & Transmit Configuration: Channel 2 Width 12 bits Wide
PCM_CH2WID_13 =        0x5 // PCM Receive & Transmit Configuration: Channel 2 Width 13 bits Wide
PCM_CH2WID_14 =        0x6 // PCM Receive & Transmit Configuration: Channel 2 Width 14 bits Wide
PCM_CH2WID_15 =        0x7 // PCM Receive & Transmit Configuration: Channel 2 Width 15 bits Wide
PCM_CH2WID_16 =        0x8 // PCM Receive & Transmit Configuration: Channel 2 Width 16 bits Wide
PCM_CH2WID_17 =        0x9 // PCM Receive & Transmit Configuration: Channel 2 Width 17 bits Wide
PCM_CH2WID_18 =        0xA // PCM Receive & Transmit Configuration: Channel 2 Width 18 bits Wide
PCM_CH2WID_19 =        0xB // PCM Receive & Transmit Configuration: Channel 2 Width 19 bits Wide
PCM_CH2WID_20 =        0xC // PCM Receive & Transmit Configuration: Channel 2 Width 20 bits Wide
PCM_CH2WID_21 =        0xD // PCM Receive & Transmit Configuration: Channel 2 Width 21 bits Wide
PCM_CH2WID_22 =        0xE // PCM Receive & Transmit Configuration: Channel 2 Width 22 bits Wide
PCM_CH2WID_23 =        0xF // PCM Receive & Transmit Configuration: Channel 2 Width 23 bits Wide
PCM_CH2WID    =        0xF // PCM Receive & Transmit Configuration: Channel 2 Width
PCM_CH2WID_24 =     0x8000 // PCM Receive & Transmit Configuration: Channel 2 Width 24 bits wide
PCM_CH2WID_25 =     0x8001 // PCM Receive & Transmit Configuration: Channel 2 Width 25 bits wide
PCM_CH2WID_26 =     0x8002 // PCM Receive & Transmit Configuration: Channel 2 Width 26 bits wide
PCM_CH2WID_27 =     0x8003 // PCM Receive & Transmit Configuration: Channel 2 Width 27 bits wide
PCM_CH2WID_28 =     0x8004 // PCM Receive & Transmit Configuration: Channel 2 Width 28 bits wide
PCM_CH2WID_29 =     0x8005 // PCM Receive & Transmit Configuration: Channel 2 Width 29 bits wide
PCM_CH2WID_30 =     0x8006 // PCM Receive & Transmit Configuration: Channel 2 Width 30 bits wide
PCM_CH2WID_31 =     0x8007 // PCM Receive & Transmit Configuration: Channel 2 Width 31 bits wide
PCM_CH2WID_32 =     0x8008 // PCM Receive & Transmit Configuration: Channel 2 Width 32 bits wide
PCM_CH2POS    =     0x3FF0 // PCM Receive & Transmit Configuration: Channel 2 Position
PCM_CH2EN     =     0x4000 // PCM Receive & Transmit Configuration: Channel 2 Enable
PCM_CH2WEX    =     0x8000 // PCM Receive & Transmit Configuration: Channel 2 Width Extension Bit
PCM_CH1WID_8  =        0x0 // PCM Receive & Transmit Configuration: Channel 1 Width 8 bits Wide
PCM_CH1WID_9  =    0x10000 // PCM Receive & Transmit Configuration: Channel 1 Width 9 bits Wide
PCM_CH1WID_10 =    0x20000 // PCM Receive & Transmit Configuration: Channel 1 Width 10 bits Wide
PCM_CH1WID_11 =    0x30000 // PCM Receive & Transmit Configuration: Channel 1 Width 11 bits Wide
PCM_CH1WID_12 =    0x40000 // PCM Receive & Transmit Configuration: Channel 1 Width 12 bits Wide
PCM_CH1WID_13 =    0x50000 // PCM Receive & Transmit Configuration: Channel 1 Width 13 bits Wide
PCM_CH1WID_14 =    0x60000 // PCM Receive & Transmit Configuration: Channel 1 Width 14 bits Wide
PCM_CH1WID_15 =    0x70000 // PCM Receive & Transmit Configuration: Channel 1 Width 15 bits Wide
PCM_CH1WID_16 =    0x80000 // PCM Receive & Transmit Configuration: Channel 1 Width 16 bits Wide
PCM_CH1WID_17 =    0x90000 // PCM Receive & Transmit Configuration: Channel 1 Width 17 bits Wide
PCM_CH1WID_18 =    0xA0000 // PCM Receive & Transmit Configuration: Channel 1 Width 18 bits Wide
PCM_CH1WID_19 =    0xB0000 // PCM Receive & Transmit Configuration: Channel 1 Width 19 bits Wide
PCM_CH1WID_20 =    0xC0000 // PCM Receive & Transmit Configuration: Channel 1 Width 20 bits Wide
PCM_CH1WID_21 =    0xD0000 // PCM Receive & Transmit Configuration: Channel 1 Width 21 bits Wide
PCM_CH1WID_22 =    0xE0000 // PCM Receive & Transmit Configuration: Channel 1 Width 22 bits Wide
PCM_CH1WID_23 =    0xF0000 // PCM Receive & Transmit Configuration: Channel 1 Width 23 bits Wide
PCM_CH1WID    =    0xF0000 // PCM Receive & Transmit Configuration: Channel 1 Width
PCM_CH1WID_24 = 0x80000000 // PCM Receive & Transmit Configuration: Channel 1 Width 24 bits wide
PCM_CH1WID_25 = 0x80010000 // PCM Receive & Transmit Configuration: Channel 1 Width 25 bits wide
PCM_CH1WID_26 = 0x80020000 // PCM Receive & Transmit Configuration: Channel 1 Width 26 bits wide
PCM_CH1WID_27 = 0x80030000 // PCM Receive & Transmit Configuration: Channel 1 Width 27 bits wide
PCM_CH1WID_28 = 0x80040000 // PCM Receive & Transmit Configuration: Channel 1 Width 28 bits wide
PCM_CH1WID_29 = 0x80050000 // PCM Receive & Transmit Configuration: Channel 1 Width 29 bits wide
PCM_CH1WID_30 = 0x80060000 // PCM Receive & Transmit Configuration: Channel 1 Width 30 bits wide
PCM_CH1WID_31 = 0x80070000 // PCM Receive & Transmit Configuration: Channel 1 Width 31 bits wide
PCM_CH1WID_32 = 0x80080000 // PCM Receive & Transmit Configuration: Channel 1 Width 32 bits wide
PCM_CH1POS    = 0x3FF00000 // PCM Receive & Transmit Configuration: Channel 1 Position
PCM_CH1EN     = 0x40000000 // PCM Receive & Transmit Configuration: Channel 1 Enable
PCM_CH1WEX    = 0x80000000 // PCM Receive & Transmit Configuration: Channel 1 Width Extension Bit

PCM_RX       =       0x7F // PCM DMA Request Level: RX Request Level
PCM_TX       =     0x7F00 // PCM DMA Request Level: TX Request Level
PCM_RX_PANIC =   0x7F0000 // PCM DMA Request Level: RX Panic Level
PCM_TX_PANIC = 0x7F000000 // PCM DMA Request Level: TX Panic Level

PCM_TXW   = 0x1 // PCM Interrupt Enables & Interrupt Status & Clear: TX Write Interrupt Enable
PCM_RXR   = 0x2 // PCM Interrupt Enables & Interrupt Status & Clear: RX Read Interrupt Enable
PCM_TXERR = 0x4 // PCM Interrupt Enables & Interrupt Status & Clear: TX Error Interrupt
PCM_RXERR = 0x8 // PCM Interrupt Enables & Interrupt Status & Clear: RX Error Interrupt

PCM_GRAY_EN     =      0x1 // PCM Gray Mode Control: Enable GRAY Mode
PCM_GRAY_CLR    =      0x2 // PCM Gray Mode Control: Clear the GRAY Mode Logic
PCM_GRAY_FLUSH  =      0x4 // PCM Gray Mode Control: Flush the RX Buffer into the RX FIFO
PCM_RXLEVEL     =    0x3F0 // PCM Gray Mode Control: The Current Fill Level of the RX Buffer
PCM_FLUSHED     =   0xFC00 // PCM Gray Mode Control: The Number of Bits that were Flushed into the RX FIFO
PCM_RXFIFOLEVEL = 0x3F0000 // PCM Gray Mode Control: The Current Level of the RX FIFO

// PWM / Pulse Width Modulator Interface
PWM_BASE = 0x20C000 // PWM Base Address
PWM_CTL  =      0x0 // PWM Control
PWM_STA  =      0x4 // PWM Status
PWM_DMAC =      0x8 // PWM DMA Configuration
PWM_RNG1 =     0x10 // PWM Channel 1 Range
PWM_DAT1 =     0x14 // PWM Channel 1 Data
PWM_FIF1 =     0x18 // PWM FIFO Input
PWM_RNG2 =     0x20 // PWM Channel 2 Range
PWM_DAT2 =     0x24 // PWM Channel 2 Data

PWM_PWEN1 =    0x1 // PWM Control: Channel 1 Enable
PWM_MODE1 =    0x2 // PWM Control: Channel 1 Mode
PWM_RPTL1 =    0x4 // PWM Control: Channel 1 Repeat Last Data
PWM_SBIT1 =    0x8 // PWM Control: Channel 1 Silence Bit
PWM_POLA1 =   0x10 // PWM Control: Channel 1 Polarity
PWM_USEF1 =   0x20 // PWM Control: Channel 1 Use Fifo
PWM_CLRF1 =   0x40 // PWM Control: Clear Fifo
PWM_MSEN1 =   0x80 // PWM Control: Channel 1 M/S Enable
PWM_PWEN2 =  0x100 // PWM Control: Channel 2 Enable
PWM_MODE2 =  0x200 // PWM Control: Channel 2 Mode
PWM_RPTL2 =  0x400 // PWM Control: Channel 2 Repeat Last Data
PWM_SBIT2 =  0x800 // PWM Control: Channel 2 Silence Bit
PWM_POLA2 = 0x1000 // PWM Control: Channel 2 Polarity
PWM_USEF2 = 0x2000 // PWM Control: Channel 2 Use Fifo
PWM_MSEN2 = 0x8000 // PWM Control: Channel 2 M/S Enable

PWM_FULL1 =    0x1 // PWM Status: Fifo Full Flag
PWM_EMPT1 =    0x2 // PWM Status: Fifo Empty Flag
PWM_WERR1 =    0x4 // PWM Status: Fifo Write Error Flag
PWM_RERR1 =    0x8 // PWM Status: Fifo Read Error Flag
PWM_GAPO1 =   0x10 // PWM Status: Channel 1 Gap Occurred Flag
PWM_GAPO2 =   0x20 // PWM Status: Channel 2 Gap Occurred Flag
PWM_GAPO3 =   0x40 // PWM Status: Channel 3 Gap Occurred Flag
PWM_GAPO4 =   0x80 // PWM Status: Channel 4 Gap Occurred Flag
PWM_BERR  =  0x100 // PWM Status: Bus Error Flag
PWM_STA1  =  0x200 // PWM Status: Channel 1 State
PWM_STA2  =  0x400 // PWM Status: Channel 2 State
PWM_STA3  =  0x800 // PWM Status: Channel 3 State
PWM_STA4  = 0x1000 // PWM Status: Channel 4 State

PWM_ENAB = 0x80000000 // PWM DMA Configuration: DMA Enable

.section .text.init_audio_jack, "ax", %progbits
.balign	4
.globl init_audio_jack;
.type init_audio_jack, %function
init_audio_jack:
	mov x0,PERIPHERAL_BASE + GPIO_BASE
	mov w1,GPIO_FSEL0_ALT0
	orr w1,w1,GPIO_FSEL5_ALT0
	str w1,[x0,GPIO_GPFSEL4]

	// Set Clock
	mov w0,(PERIPHERAL_BASE + CM_BASE) & 0x0000FFFF
	mov w1,(PERIPHERAL_BASE + CM_BASE) & 0xFFFF0000
	orr w0,w0,w1
	mov w1,CM_PASSWORD
	orr w1,w1,0x2000 // Bits 0..11 Fractional Part Of Divisor = 0, Bits 12..23 Integer Part Of Divisor = 2
	str w1,[x0,CM_PWMDIV]

	mov w1,CM_PASSWORD
	orr w1,w1,CM_ENAB
	orr w1,w1,CM_SRC_OSCILLATOR + CM_SRC_PLLCPER // Use 650MHz PLLC Clock
	str w1,[x0,CM_PWMCTL]

	// Set PWM
	mov w0,(PERIPHERAL_BASE + PWM_BASE) & 0x0000FFFF
	mov w1,(PERIPHERAL_BASE + PWM_BASE) & 0xFFFF0000
	orr w0,w0,w1
	mov w1,0x2C48 // Range = 13bit 44100Hz Mono
	str w1,[x0,PWM_RNG1]
	str w1,[x0,PWM_RNG2]

	mov w1,PWM_USEF2 + PWM_PWEN2 + PWM_USEF1 + PWM_PWEN1 + PWM_CLRF1
	str w1,[x0,PWM_CTL]
	ret

.section .text.play_audio, "ax", %progbits
.balign	4
.globl play_audio;
.type play_audio, %function
play_audio:
	mov x0,PERIPHERAL_BASE + GPIO_BASE
	mov w0,(PERIPHERAL_BASE + PWM_BASE) & 0x0000FFFF
	mov w1,(PERIPHERAL_BASE + PWM_BASE) & 0xFFFF0000
	orr w0,w0,w1

	Loop:
		adr x1, _binary_src_audio_Interlude_bin_start // X1 = Sound Sample
		ldr w2, =_binary_src_audio_Interlude_bin_end
		and w2, w2, 0x0000FFFF // W2 = End Of Sound Sample
		ldr w3, =_binary_src_audio_Interlude_bin_end
		and w3, w3, 0xFFFF0000
		orr w2,w2,w3
		FIFO_Write:
			ldrh w3,[x1],2 // Write 2 Bytes To FIFO
			lsr w3,w3,3 // Convert 16bit To 13bit
			str w3,[x0,PWM_FIF1] // FIFO Address
			
			ldrh w3, [x1], 2
			lsr w3, w3, 3
			str w3, [x0, PWM_FIF1]
		FIFO_Wait:
			ldr w3,[x0,PWM_STA]
			tst w3,PWM_FULL1 // Test Bit 1 FIFO Full
			b.ne FIFO_Wait
		cmp w1,w2 // Check End Of Sound Sample
		b.ne FIFO_Write
	b Loop // Play Sample Again
