/******************************************************************************/
/* FAME Fast and Accurate Motorola 68000 Emulation Core                       */
/* (c) 2002 Oscar Orallo Pelaez / Daniel Lancha Garcia                        */
/* Version: 2.1                                                               */
/* Date: 11-26-2006                                                           */
/* See FAME.HTML for documentation and license information                    */
/******************************************************************************/

#ifndef __FAME_H__
#define __FAME_H__

#ifdef _MSC_VER
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int8 uint8_t;
typedef __int32 int32_t;
typedef __int16 int16_t;
typedef __int8 int8_t;
#elif !defined(INT32_MAX)
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef short int16_t;
typedef signed char int8_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/************************************/
/* General library defines          */
/************************************/

#define M68K_OK 0
#define M68K_RUNNING 1
#define M68K_NO_SUP_ADDR_SPACE 2
#define M68K_INV_REG -1

/* Hardware interrupt state */

#define M68K_IRQ_LEVEL_ERROR -1
#define M68K_IRQ_INV_PARAMS -2

/* Defines to specify hardware interrupt type */

#define M68K_AUTOVECTORED_IRQ -1
#define M68K_SPURIOUS_IRQ -2

/* Defines to specify address space */

#define M68K_SUP_ADDR_SPACE 0
#define M68K_USER_ADDR_SPACE 2
#define M68K_PROG_ADDR_SPACE 0
#define M68K_DATA_ADDR_SPACE 1


/*******************/
/* Data definition */
/*******************/

/* M68K registers */
typedef enum
{
      M68K_REG_D0=0,
      M68K_REG_D1,
      M68K_REG_D2,
      M68K_REG_D3,
      M68K_REG_D4,
      M68K_REG_D5,
      M68K_REG_D6,
      M68K_REG_D7,
      M68K_REG_A0,
      M68K_REG_A1,
      M68K_REG_A2,
      M68K_REG_A3,
      M68K_REG_A4,
      M68K_REG_A5,
      M68K_REG_A6,
      M68K_REG_A7,
      M68K_REG_ASP,
      M68K_REG_PC,
      M68K_REG_SR
} m68k_register;

typedef union
{
#ifndef FAME_BIG_ENDIAN
    struct
    {
        uint8_t B,B1,B2,B3;
    } b;
    struct
    {
        int8_t SB,SB1,SB2,SB3;
    } sb;
    struct
    {
        uint16_t W,W1;
    } w;
    struct
    {
        int16_t SW,SW1;
    } sw;
#else
    struct
    {
        uint8_t B3,B2,B1,B;
    } b;
    struct
    {
        int8_t SB3,SB2,SB1,SB;
    } sb;
    struct
    {
        uint16_t W1,W;
    } w;
    struct
    {
        int16_t SW1,SW;
    } sw;
#endif
    uint32_t D;
    int32_t SD;
} m68k_reg_t;

/* Read/write handlers */
typedef uint16_t(*m68k_read16_t)(const uint32_t);
typedef void(*m68k_write16_t)(const uint32_t, const uint16_t);
typedef uint8_t(*m68k_read8_t)(const uint32_t);
typedef void(*m68k_write8_t)(const uint32_t, const uint8_t);

/* The memory blocks must be in native (Motorola) format */
typedef struct
{
	uint32_t low_addr;
	uint32_t high_addr;
	uint16_t *offset;
} M68K_PROGRAM;

/* The memory blocks must be in native (Motorola) format */
typedef struct
{
	uint32_t low_addr;
	uint32_t high_addr;
	m68k_read8_t mem_handler;
	const uint8_t *data;
} M68K_READ_BYTE_DATA;

typedef struct
{
	uint32_t low_addr;
	uint32_t high_addr;
	m68k_write8_t mem_handler;
	uint8_t *data;
} M68K_WRITE_BYTE_DATA;

typedef struct
{
	uint32_t low_addr;
	uint32_t high_addr;
	m68k_read16_t mem_handler;
	const uint16_t *data;
} M68K_READ_WORD_DATA;

typedef struct
{
	uint32_t low_addr;
	uint32_t high_addr;
	m68k_write16_t mem_handler;
	uint16_t *data;
} M68K_WRITE_WORD_DATA;

typedef struct
{
	uint32_t low_addr;
	uint32_t high_addr;
	void    *mem_handler;
	void    *data;
} M68K_DATA;

/* Custom function handler */
typedef void (*m68k_except_t)(uint32_t vector);

/* M68K CPU CONTEXT */
typedef struct
{
	M68K_PROGRAM         *fetch;
	//M68K_READ_BYTE_DATA  *read_byte;
	//M68K_READ_WORD_DATA  *read_word;
	//M68K_WRITE_BYTE_DATA *write_byte;
	//M68K_WRITE_WORD_DATA *write_word;
	M68K_DATA *read_byte;
	M68K_DATA *read_word;
	M68K_DATA *write_byte;
	M68K_DATA *write_word;
	M68K_PROGRAM         *sv_fetch;
	//M68K_READ_BYTE_DATA  *sv_read_byte;
	//M68K_READ_WORD_DATA  *sv_read_word;
	//M68K_WRITE_BYTE_DATA *sv_write_byte;
	//M68K_WRITE_WORD_DATA *sv_write_word;
	M68K_DATA *sv_read_byte;
	M68K_DATA *sv_read_word;
	M68K_DATA *sv_write_byte;
	M68K_DATA *sv_write_word;
	M68K_PROGRAM         *user_fetch;
	//M68K_READ_BYTE_DATA  *user_read_byte;
	//M68K_READ_WORD_DATA  *user_read_word;
	//M68K_WRITE_BYTE_DATA *user_write_byte;
	//M68K_WRITE_WORD_DATA *user_write_word;
	M68K_DATA *user_read_byte;
	M68K_DATA *user_read_word;
	M68K_DATA *user_write_byte;
	M68K_DATA *user_write_word;
	void           (*reset_handler)(void);
	void           (*iack_handler)(unsigned level);
	m68k_except_t *icust_handler;
	m68k_reg_t  dreg[8];
	m68k_reg_t  areg[8];
	uint32_t    asp;
	uint32_t    pc;
	uint32_t    cycles_counter;
	uint8_t     interrupts[8];
	uint16_t    sr;
	uint16_t    execinfo;
} M68K_CONTEXT;


/************************/
/* Function definition  */
/************************/

/* General purpose functions */
void     famem68k_init(void);
uint32_t famem68k_reset(void);
void     famem68k_emulate(uint32_t clocks);
uint32_t famem68k_get_pc(void);
uint32_t famem68k_get_cpu_state(void);
uint32_t famem68k_fetch(uint32_t address, uint32_t memory_space);

/* Interrupt handling functions */
int32_t  famem68k_raise_irq(uint32_t level, int32_t vector);
int32_t  famem68k_lower_irq(uint32_t level);
int32_t  famem68k_get_irq_vector(uint32_t level);
int32_t  famem68k_change_irq_vector(uint32_t level, int32_t vector);

/* CPU context handling functions */
uint32_t famem68k_get_context_size(void);
void     famem68k_get_context(M68K_CONTEXT *context);
void     famem68k_set_context(const M68K_CONTEXT *context);
int32_t  famem68k_get_register(m68k_register reg);
int32_t  famem68k_set_register(m68k_register reg, uint32_t value);

/* Timing functions */
uint32_t famem68k_get_cycles_counter(void);
uint32_t famem68k_trip_cycles_counter(void);
uint32_t famem68k_control_cycles_counter(uint32_t clocks);
void     famem68k_release_timeslice(void);
void     famem68k_stop_emulating(void);
void     famem68k_add_cycles(uint32_t clocks);
void     famem68k_release_cycles(uint32_t clocks);

#ifdef __cplusplus
}
#endif

#endif
