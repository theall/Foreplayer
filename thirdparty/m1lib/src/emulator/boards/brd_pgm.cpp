// Afega h/w (z80 + 2151 + 6295, oldest story in the book)

#include "m1snd.h"

// helper macro for merging data with the memory mask
#define COMBINE_DATA(varptr)            (*(varptr) = (*(varptr) & ~mem_mask) | (data & mem_mask))

#define ACCESSING_BITS_0_7              ((mem_mask & 0x000000ffU) != 0)
#define ACCESSING_BITS_8_15             ((mem_mask & 0x0000ff00U) != 0)
#define ACCESSING_BITS_16_23            ((mem_mask & 0x00ff0000U) != 0)
#define ACCESSING_BITS_24_31            ((mem_mask & 0xff000000U) != 0)
#define ACCESSING_BITS_32_39            ((mem_mask & 0x000000ff00000000U) != 0)
#define ACCESSING_BITS_40_47            ((mem_mask & 0x0000ff0000000000U) != 0)
#define ACCESSING_BITS_48_55            ((mem_mask & 0x00ff000000000000U) != 0)
#define ACCESSING_BITS_56_63            ((mem_mask & 0xff00000000000000U) != 0)

#define ACCESSING_BITS_0_15             ((mem_mask & 0x0000ffffU) != 0)
#define ACCESSING_BITS_16_31            ((mem_mask & 0xffff0000U) != 0)
#define ACCESSING_BITS_32_47            ((mem_mask & 0x0000ffff00000000U) != 0)
#define ACCESSING_BITS_48_63            ((mem_mask & 0xffff000000000000U) != 0)

#define ACCESSING_BITS_0_31             ((mem_mask & 0xffffffffU) != 0)
#define ACCESSING_BITS_32_63            ((mem_mask & 0xffffffff00000000U) != 0)

uint8_t *m_z80_mainram;

static READ_HANDLER( z80_ram_r )
{
    return (m_z80_mainram[offset * 2] << 8) | m_z80_mainram[offset * 2 + 1];
}

static WRITE16_HANDLER(z80_ram_w)
{
//	int pc = space.device().safe_pc();

    if (ACCESSING_BITS_8_15)
        m_z80_mainram[offset * 2] = data >> 8;
    if (ACCESSING_BITS_0_7)
        m_z80_mainram[offset * 2 + 1] = data;

//	if (pc != 0xf12 && pc != 0xde2 && pc != 0x100c50 && pc != 0x100b20)
//		if (PGMLOGERROR)
//			logerror("Z80: write %04x, %04x @ %04x (%06x)\n", offset * 2, data, mem_mask, space.device().safe_pc());
}

static WRITE16_HANDLER(z80_reset_w)
{
//	if (PGMLOGERROR)
//		logerror("Z80: reset %04x @ %04x (%06x)\n", data, mem_mask, space.device().safe_pc());

    if (data == 0x5050)
    {
        m_ics->reset();
//        m_soundcpu->set_input_line(INPUT_LINE_HALT, CLEAR_LINE);
//        m_soundcpu->set_input_line(INPUT_LINE_RESET, PULSE_LINE);
        cpu_set_irq_line(INPUT_LINE_HALT, CLEAR_LINE);
        cpu_set_irq_line(INPUT_LINE_RESET, PULSE_LINE);
    }
    else
    {
        /* this might not be 100% correct, but several of the games (ddp2, puzzli2 etc. expect the z80 to be turned
        off during data uploads, they write here before the upload */
        //m_soundcpu->set_input_line(INPUT_LINE_HALT, ASSERT_LINE);
        cpu_set_irq_line(1, IRQ_LINE_NMI, ASSERT_LINE);
    }
}

static WRITE16_HANDLER(z80_ctrl_w)
{
//	if (PGMLOGERROR)
//		logerror("Z80: ctrl %04x @ %04x (%06x)\n", data, mem_mask, space.device().safe_pc());
}

static WRITE16_HANDLER(m68k_l1_w)
{
    if(ACCESSING_BITS_0_7)
    {
//		if (PGMLOGERROR)
//			logerror("SL 1 m68.w %02x (%06x) IRQ\n", data & 0xff, space.device().safe_pc());
//		m_soundlatch->write(space, 0, data);
        //m_soundcpu->set_input_line(INPUT_LINE_NMI, PULSE_LINE );
        cpu_set_irq_line(1, IRQ_LINE_NMI, PULSE_LINE);
    }
}

static WRITE8_HANDLER(z80_l3_w)
{
//	if (PGMLOGERROR)
//		logerror("SL 3 z80.w %02x (%04x)\n", data, space.device().safe_pc());
    //m_soundlatch3->write(space, 0, data);
}


/*** Memory Maps *************************************************************/

/*** Z80 (sound CPU)**********************************************************/
static MEMORY_READ_START( pgm_z80_mem_r )
//ADDRESS_MAP_START( pgm_z80_mem, AS_PROGRAM, 8, pgm_state )
//AM_RANGE(0x0000, 0xffff) AM_RAM AM_SHARE("z80_mainram")
    { 0x0000, 0xffff, MWA_RAM },
MEMORY_END

static MEMORY_WRITE_START( pgm_z80_mem_w )
    { 0x0000, 0xffff, MWA_RAM },
MEMORY_END

//ADDRESS_MAP_START( pgm_z80_io, AS_IO, 8, pgm_state )
//    AM_RANGE(0x8000, 0x8003) AM_DEVREADWRITE("ics", ics2115_device, read, write)
//    AM_RANGE(0x8100, 0x81ff) AM_DEVREAD("soundlatch3", generic_latch_8_device, read) AM_WRITE(z80_l3_w)
//    AM_RANGE(0x8200, 0x82ff) AM_DEVREADWRITE("soundlatch", generic_latch_8_device, read, write)
//    AM_RANGE(0x8400, 0x84ff) AM_DEVREADWRITE("soundlatch2", generic_latch_8_device, read, write)
//ADDRESS_MAP_END
static READ_HANDLER( ics_port_r )
{
    return 0;
}
static PORT_READ_START( pgm_z80_io_r )
    { 0x8000, 0x8003, ics_port_r },
    { 0x8100, 0x81ff, 0},
    { 0x8200, 0x82ff, 0},
    { 0x8400, 0x84ff, 0},
PORT_END
static PORT_WRITE_START( pgm_z80_io_w )
    { 0x8000, 0x8003, ics_port_r },
    { 0x8100, 0x81ff, 0},
    { 0x8200, 0x82ff, 0},
    { 0x8400, 0x84ff, 0},
PORT_END

/*** 68000 (main CPU) + variants for protection devices **********************/
ADDRESS_MAP_START( pgm_base_mem, AS_PROGRAM, 16, pgm_state )
    AM_RANGE(0x700006, 0x700007) AM_WRITENOP // Watchdog?

    AM_RANGE(0x800000, 0x81ffff) AM_RAM AM_MIRROR(0x0e0000) AM_SHARE("sram") /* Main Ram */

    AM_RANGE(0xc00002, 0xc00003) AM_DEVREAD8("soundlatch", generic_latch_8_device, read, 0x00ff) AM_WRITE(m68k_l1_w)
    AM_RANGE(0xc00004, 0xc00005) AM_DEVREADWRITE8("soundlatch2", generic_latch_8_device, read, write, 0x00ff)
    AM_RANGE(0xc00006, 0xc00007) AM_DEVREADWRITE8("rtc", v3021_device, read, write, 0x00ff)
    AM_RANGE(0xc00008, 0xc00009) AM_WRITE(z80_reset_w)
    AM_RANGE(0xc0000a, 0xc0000b) AM_WRITE(z80_ctrl_w)
    AM_RANGE(0xc0000c, 0xc0000d) AM_DEVREADWRITE8("soundlatch3", generic_latch_8_device, read, write, 0x00ff)

    AM_RANGE(0xc10000, 0xc1ffff) AM_READWRITE(z80_ram_r, z80_ram_w) /* Z80 Program */
ADDRESS_MAP_END

ADDRESS_MAP_START( pgm_mem, AS_PROGRAM, 16, pgm_state )
    AM_IMPORT_FROM(pgm_base_mem)
    AM_RANGE(0x000000, 0x0fffff) AM_ROM   /* BIOS ROM */
ADDRESS_MAP_END

ADDRESS_MAP_START( pgm_basic_mem, AS_PROGRAM, 16, pgm_state )
    AM_IMPORT_FROM(pgm_mem)
    AM_RANGE(0x100000, 0x3fffff) AM_ROMBANK("bank1") /* Game ROM */
ADDRESS_MAP_END

/*** Machine Driver **********************************************************/

/* most games require IRQ4 for inputs to work, Puzzli 2 is explicit about not wanting it tho
   what is the source? */
TIMER_DEVICE_CALLBACK_MEMBER(pgm_interrupt)
{
    int scanline = param;

// already being generated  by MCFG_CPU_VBLANK_INT_DRIVER("screen", pgm_state,  irq6_line_hold)
//  if(scanline == 224)
//      m_maincpu->set_input_line(6, HOLD_LINE);

    if(scanline == 0)
        if (!m_irq4_disabled) m_maincpu->set_input_line(4, HOLD_LINE);
}

MACHINE_START_MEMBER(pgm_state,pgm)
{
//  machine().base_datetime(m_systime);

    m_ics = machine().device("ics");
}

MACHINE_RESET_MEMBER(pgm_state,pgm)
{
    m_soundcpu->set_input_line(INPUT_LINE_HALT, ASSERT_LINE);
}

MACHINE_CONFIG_FRAGMENT( pgmbase )
    /* basic machine hardware */
    MCFG_CPU_ADD("maincpu", M68000, 20000000) /* 20 mhz! verified on real board */
    MCFG_CPU_PROGRAM_MAP(pgm_basic_mem)

    MCFG_CPU_ADD("soundcpu", Z80, 33868800/4)
    MCFG_CPU_PROGRAM_MAP(pgm_z80_mem)
    MCFG_CPU_IO_MAP(pgm_z80_io)

    MCFG_MACHINE_START_OVERRIDE(pgm_state, pgm )
    MCFG_MACHINE_RESET_OVERRIDE(pgm_state, pgm )
    MCFG_NVRAM_ADD_0FILL("sram")

    MCFG_V3021_ADD("rtc")

    /*sound hardware */
    MCFG_SPEAKER_STANDARD_MONO("mono")

    MCFG_GENERIC_LATCH_8_ADD("soundlatch")
    MCFG_GENERIC_LATCH_8_ADD("soundlatch2")
    MCFG_GENERIC_LATCH_8_ADD("soundlatch3")

    MCFG_ICS2115_ADD("ics", 0)
    MCFG_ICS2115_IRQ_CB(INPUTLINE("soundcpu", 0))
    MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 5.0)
MACHINE_CONFIG_END

MACHINE_CONFIG_START( pgm, pgm_state )
    MCFG_FRAGMENT_ADD(pgmbase)
MACHINE_CONFIG_END

static void Pgm_SendCmd(int cmda, int cmdb);
static int cmd_latch;

static void irq_handler(int irq)
{
	cpu_set_irq_line(0, 0, irq ? ASSERT_LINE : CLEAR_LINE);
}

static struct YM2151interface afega_ym2151_intf =
{
	1,
	4000000,	/* ? */
	{ YM3012_VOL(30,MIXER_PAN_LEFT,30,MIXER_PAN_RIGHT) },
	{ irq_handler }
};

static struct OKIM6295interface afega_m6295_intf =
{
	1,
	{ 8000 },	/* ? */
	{ RGN_SAMP1 },
	{ 70 }
};

static READ_HANDLER( latch_r )
{
	cpu_set_irq_line(0, 0, CLEAR_LINE);
	return cmd_latch;
}

static MEMORY_READ_START( readmem )
	{ 0x0000, 0xefff, MRA_ROM },
	{ 0xf000, 0xf7ff, MRA_RAM },
	{ 0xf800, 0xf800, latch_r },
	{ 0xf809, 0xf809, YM2151_status_port_0_r },
	{ 0xf80a, 0xf80a, OKIM6295_status_0_r },
	{ 0xf900, 0xf900, MRA_NOP },
MEMORY_END

static MEMORY_WRITE_START( writemem )
	{ 0x0000, 0xefff, MWA_ROM },
	{ 0xf000, 0xf7ff, MWA_RAM },
	{ 0xf800, 0xf800, MWA_NOP },
	{ 0xf808, 0xf808, YM2151_register_port_0_w },
	{ 0xf809, 0xf809, YM2151_data_port_0_w },
	{ 0xf80a, 0xf80a, OKIM6295_data_0_w },
	{ 0xf8ff, 0xf8ff, MWA_NOP },
MEMORY_END

static void Pgm_SendCmd(int cmda, int cmdb);

M1_BOARD_START( pgm )
    MDRV_NAME("PGM")
    MDRV_HWDESC("Z80")
    MDRV_SEND( Pgm_SendCmd )

    MDRV_CPU_ADD(MC68000, 20000000)
    MDRV_CPUMEMHAND(&bb_readwritemem)

    MDRV_CPU_ADD(Z80C, 33868800/4)
    MDRV_CPU_MEMORY( pgm_z80_mem_r, pgm_z80_mem_w )
    MDRV_CPU_PORTS( pgm_z80_io_r, pgm_z80_io_w)

    MDRV_SOUND_ADD(ICS2115, &afega_ym2151_intf)
M1_BOARD_END

static void Pgm_SendCmd(int cmda, int cmdb)
{
	cmd_latch = cmda;
	cpu_set_irq_line(0, IRQ_LINE_NMI, ASSERT_LINE);
}
