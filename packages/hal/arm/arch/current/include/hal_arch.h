#ifndef CYGONCE_HAL_ARCH_H
#define CYGONCE_HAL_ARCH_H

//==========================================================================
//
//      hal_arch.h
//
//      Architecture specific abstractions
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2007, 2008 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under    
// the terms of the GNU General Public License as published by the Free     
// Software Foundation; either version 2 or (at your option) any later      
// version.                                                                 
//
// eCos is distributed in the hope that it will be useful, but WITHOUT      
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
// for more details.                                                        
//
// You should have received a copy of the GNU General Public License        
// along with eCos; if not, write to the Free Software Foundation, Inc.,    
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
//
// As a special exception, if other files instantiate templates or use      
// macros or inline functions from this file, or you compile this file      
// and link it with other works to produce a work based on this file,       
// this file does not by itself cause the resulting work to be covered by   
// the GNU General Public License. However the source code for this file    
// must still be made available in accordance with section (3) of the GNU   
// General Public License v2.                                               
//
// This exception does not invalidate any other reasons why a work based    
// on this file might be covered by the GNU General Public License.         
// -------------------------------------------                              
// ####ECOSGPLCOPYRIGHTEND####                                              
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    nickg, gthomas
// Contributors: nickg, gthomas
// Date:         1999-02-20
// Purpose:      Define architecture abstractions
// Usage:        #include <cyg/hal/hal_arch.h>

//              
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>         // To decide on stack usage
#include <cyg/infra/cyg_type.h>

#ifdef CYGBLD_HAL_ARM_PLF_ARCH_H
#include <cyg/hal/plf_arch.h>
#endif

#ifdef CYGBLD_HAL_ARM_VAR_ARCH_H
#include <cyg/hal/var_arch.h>
#endif

//
// CPSR Register defines
//

#define CPSR_IRQ_DISABLE	0x80	// IRQ disabled when =1
#define CPSR_FIQ_DISABLE	0x40	// FIQ disabled when =1
#define CPSR_THUMB_ENABLE	0x20	// Thumb mode when =1
#define CPSR_USER_MODE		0x10
#define CPSR_FIQ_MODE		0x11
#define CPSR_IRQ_MODE		0x12
#define CPSR_SUPERVISOR_MODE	0x13
#define CPSR_MONITOR_MODE	0x16
#define CPSR_ABORT_MODE		0x17
#define CPSR_UNDEF_MODE		0x1B
#define CPSR_SYSTEM_MODE	0x1F

// The following are not supported by every CPU, but if
// they are, they have the following values:
#define CPSR_ICACHE_ENABLE	0x1000
#define CPSR_BIG_ENDIAN		0x80
#define CPSR_DCACHE_ENABLE	0x4
#define CPSR_ALIGN_FAULT_ENABLE	0x2
#define CPSR_MMU_ENABLE		0x1

#define CPSR_MODE_BITS          0x1F

#define CPSR_INITIAL (CPSR_IRQ_DISABLE|CPSR_FIQ_DISABLE|CPSR_SUPERVISOR_MODE)
#define CPSR_THREAD_INITIAL (CPSR_SUPERVISOR_MODE)

//
// Vector table offsets
//

#define HAL_ARM_RESET_VECTOR			0x00
#define HAL_ARM_UNDEFINED_VECTOR		0x04
#define HAL_ARM_SWI_VECTOR			0x08
#define HAL_ARM_PREFETCH_VECTOR			0x0C
#define HAL_ARM_ABORT_VECTOR			0x10
#define HAL_ARM_RESERVED_VECTOR			0x14
#define HAL_ARM_IRQ_VECTOR			0x18
#define HAL_ARM_FIQ_VECTOR			0x1C
#define HAL_ARM_RESET_VECTOR_ADDR		0x20
#define HAL_ARM_UNDEFINED_VECTOR_ADDR		0x24
#define HAL_ARM_SWI_VECTOR_ADDR			0x28
#define HAL_ARM_PREFETCH_VECTOR_ADDR		0x2C
#define HAL_ARM_ABORT_VECTOR_ADDR		0x30
#define HAL_ARM_RESERVED_VECTOR_ADDR		0x34
#define HAL_ARM_IRQ_VECTOR_ADDR			0x38
#define HAL_ARM_FIQ_VECTOR_ADDR			0x3C

//--------------------------------------------------------------------------
// Processor saved states:
// The layout of this structure is also defined in "arm.inc", for assembly
// code, which will be generated automatically if this file changes.

#define HAL_THREAD_CONTEXT_FIRST        0
#define HAL_THREAD_CONTEXT_R0           (0-HAL_THREAD_CONTEXT_FIRST)
#define HAL_THREAD_CONTEXT_R4           (4-HAL_THREAD_CONTEXT_FIRST)
#define HAL_THREAD_CONTEXT_R8           (8-HAL_THREAD_CONTEXT_FIRST)
#define HAL_THREAD_CONTEXT_R9           (9-HAL_THREAD_CONTEXT_FIRST)
#define HAL_THREAD_CONTEXT_R10          (10-HAL_THREAD_CONTEXT_FIRST)
#define HAL_THREAD_CONTEXT_LAST         10
#define HAL_NUM_THREAD_CONTEXT_REGS     (HAL_THREAD_CONTEXT_LAST - \
                                          HAL_THREAD_CONTEXT_FIRST+1)

// It seems that r0-r3,r12 are considered scratch by function calls

typedef struct 
{
    // These are common to all saved states
    cyg_uint32  d[HAL_NUM_THREAD_CONTEXT_REGS] ;  // Data regs (r0..r10)
    cyg_uint32  fp;                               // (r11) Frame pointer
    cyg_uint32  ip;                               // (r12)
    cyg_uint32  sp;                               // (r13) Stack pointer
    cyg_uint32  lr;                               // (r14) Link Reg
    cyg_uint32  pc;                               // (r15) PC place holder
                                                  //       (never used)
    cyg_uint32  cpsr;                             // Condition Reg
    // These are only saved for exceptions and interrupts
    cyg_uint32  vector;                           // Vector number
    cyg_uint32  svc_lr;                           // saved svc mode lr
    cyg_uint32  svc_sp;                           // saved svc mode sp

#if defined(CYGHWR_HAL_ARM_NEON) || defined(CYGHWR_HAL_ARM_FPU)
    cyg_uint64  f[32];                            // 32x64 NEON or VFP registers
    cyg_uint32  fpscr;                            // float-point status and control register
    cyg_uint32  reserved;                         // 64bit alignment
#endif

} HAL_SavedRegisters;

//-------------------------------------------------------------------------
// Exception handling function.
// This function is defined by the kernel according to this prototype. It is
// invoked from the HAL to deal with any CPU exceptions that the HAL does
// not want to deal with itself. It usually invokes the kernel's exception
// delivery mechanism.

externC void cyg_hal_deliver_exception( CYG_WORD code, CYG_ADDRWORD data );

//-------------------------------------------------------------------------
// Bit manipulation macros

externC int hal_lsbindex(int);
externC int hal_msbindex(int);

#define HAL_LSBIT_INDEX(index, mask) index = hal_lsbindex(mask)
#define HAL_MSBIT_INDEX(index, mask) index = hal_msbindex(mask)

//-------------------------------------------------------------------------
// Context Initialization
// Initialize the context of a thread.
// Arguments:
// _sparg_ name of variable containing current sp, will be changed to new sp
// _thread_ thread object address, passed as argument to entry point
// _entry_ entry point address.
// _id_ bit pattern used in initializing registers, for debugging.

// Optional FPU context initialization
#if defined(CYGHWR_HAL_ARM_NEON) || defined(CYGHWR_HAL_ARM_FPU)
#define HAL_THREAD_INIT_FPU_CONTEXT( _regs_, _id_ )                             \
{                                                                               \
    for( _i_ = 0; _i_ < 32; _i_++ ) (_regs_)->f[_i_] = (_id_)|0xFF00|_i_;       \
    (_regs_)->fpscr = 0x00000000;                                               \
}
#else
#define HAL_THREAD_INIT_FPU_CONTEXT( _regs_, _id_ )
#endif


#define HAL_THREAD_INIT_CONTEXT( _sparg_, _thread_, _entry_, _id_ )         \
    CYG_MACRO_START                                                         \
    register CYG_WORD _sp_ = ((CYG_WORD)_sparg_) &~15;                      \
    register HAL_SavedRegisters *_regs_;                                    \
    int _i_;                                                                \
    _regs_ = (HAL_SavedRegisters *)((_sp_) - sizeof(HAL_SavedRegisters));   \
    for( _i_ = HAL_THREAD_CONTEXT_FIRST; _i_ <= HAL_THREAD_CONTEXT_LAST;    \
           _i_++ )                                                          \
        (_regs_)->d[_i_] = (_id_)|_i_;                                      \
    (_regs_)->d[00] = (CYG_WORD)(_thread_); /* R0 = arg1 = thread ptr */    \
    (_regs_)->sp = (CYG_WORD)(_sp_);        /* SP = top of stack      */    \
    (_regs_)->lr = (CYG_WORD)(_entry_);     /* LR = entry point       */    \
    (_regs_)->pc = (CYG_WORD)(_entry_);     /* PC = [initial] entry point */\
    (_regs_)->cpsr = (CPSR_THREAD_INITIAL); /* PSR = Interrupt enabled */   \
    HAL_THREAD_INIT_FPU_CONTEXT( _regs_, _id_ )                             \
    _sparg_ = (CYG_ADDRESS)_regs_;                                          \
    CYG_MACRO_END

//--------------------------------------------------------------------------
// Context switch macros.
// The arguments are pointers to locations where the stack pointer
// of the current thread is to be stored, and from where the sp of the
// next thread is to be fetched.

externC void hal_thread_switch_context( CYG_ADDRESS to, CYG_ADDRESS from );
externC void hal_thread_load_context( CYG_ADDRESS to )
    __attribute__ ((noreturn));

#define HAL_THREAD_SWITCH_CONTEXT(_fspptr_,_tspptr_)                    \
        hal_thread_switch_context((CYG_ADDRESS)_tspptr_,                \
                                  (CYG_ADDRESS)_fspptr_);

#define HAL_THREAD_LOAD_CONTEXT(_tspptr_)                               \
        hal_thread_load_context( (CYG_ADDRESS)_tspptr_ );

//--------------------------------------------------------------------------
// Execution reorder barrier.
// When optimizing the compiler can reorder code. In multithreaded systems
// where the order of actions is vital, this can sometimes cause problems.
// This macro may be inserted into places where reordering should not happen.

#define HAL_REORDER_BARRIER() asm volatile ( "" : : : "memory" )

//--------------------------------------------------------------------------
// Breakpoint support
// HAL_BREAKPOINT() is a code sequence that will cause a breakpoint to happen
// if executed.
// HAL_BREAKINST is the value of the breakpoint instruction and 
// HAL_BREAKINST_SIZE is its size in bytes.

#define _stringify1(__arg) #__arg
#define _stringify(__arg) _stringify1(__arg)

#define HAL_BREAKINST_ARM          0xE7FFDEFE
#define HAL_BREAKINST_ARM_SIZE     4
#define HAL_BREAKINST_THUMB        0xbebe  // illegal instruction currently
#define HAL_BREAKINST_THUMB_SIZE   2

#ifdef __thumb__

# define HAL_BREAKPOINT(_label_)                         \
asm volatile (" .code 16;"                               \
              " .globl  " #_label_ ";"                   \
              #_label_":"                                \
              " .short " _stringify(HAL_BREAKINST_THUMB) \
    );

# define HAL_BREAKINST           HAL_BREAKINST_THUMB
# define HAL_BREAKINST_SIZE      HAL_BREAKINST_THUMB_SIZE
# define HAL_BREAKINST_TYPE      cyg_uint16
#else // __thumb__

#define HAL_BREAKPOINT(_label_)                   \
asm volatile (" .globl  " #_label_ ";"            \
              #_label_":"                         \
              " .word " _stringify(HAL_BREAKINST_ARM) \
    );

//#define HAL_BREAKINST           {0xFE, 0xDE, 0xFF, 0xE7}
#define HAL_BREAKINST            HAL_BREAKINST_ARM
#define HAL_BREAKINST_SIZE       HAL_BREAKINST_ARM_SIZE
#define HAL_BREAKINST_TYPE       cyg_uint32
#endif // __thumb__


extern cyg_uint32 __arm_breakinst;
extern cyg_uint16 __thumb_breakinst;
#define HAL_BREAKINST_ADDR(x) (((x)==2)? \
                              ((void*)&__thumb_breakinst) : \
                              ((void*)&__arm_breakinst))

//--------------------------------------------------------------------------
// Thread register state manipulation for GDB support.

// Register layout expected by GDB
typedef struct
{
    cyg_uint32  gpr[16];
    cyg_uint32  f0[3];
    cyg_uint32  f1[3];
    cyg_uint32  f2[3];
    cyg_uint32  f3[3];
    cyg_uint32  f4[3];
    cyg_uint32  f5[3];
    cyg_uint32  f6[3];
    cyg_uint32  f7[3];
    cyg_uint32  fps;
    cyg_uint32  ps;
} GDB_Registers;

// Translate a stack pointer as saved by the thread context macros above into
// a pointer to a HAL_SavedRegisters structure.
#define HAL_THREAD_GET_SAVED_REGISTERS( _sp_, _regs_ )  \
        (_regs_) = (HAL_SavedRegisters *)(_sp_)

// Copy a set of coprocessor registers from a HAL_SavedRegisters structure
// into a GDB_Registers structure. GDB expects placeholders for FP regs
// even for non-FP targets, so we just zero fill the fields.
#define HAL_GET_GDB_COPROCESSOR_REGISTERS( _gdb_, _regs_ )      \
    CYG_MACRO_START                                             \
    cyg_uint32 *_p_ = _gdb_->f0;                                \
    for(_i_ = 0; _i_ < (8 * 3); _i_++)                          \
        *_p_++ = 0;                                             \
    _gdb_->fps = 0;                                             \
    CYG_MACRO_END

// Copy coprocessor registers from a GDB_Registers structure into a
// HAL_SavedRegisters structure.
#define HAL_SET_GDB_COPROCESSOR_REGISTERS( _regs_, _gdb_ )

// Copy a set of registers from a HAL_SavedRegisters structure into a
// GDB_Registers structure.
#define HAL_GET_GDB_REGISTERS( _aregval_, _regs_ )              \
    CYG_MACRO_START                                             \
    GDB_Registers *_gdb_ = (GDB_Registers *)(_aregval_);        \
    int _i_;                                                    \
                                                                \
    for( _i_ = 0; _i_ <= 10; _i_++ )                            \
        _gdb_->gpr[_i_] = (_regs_)->d[_i_];                     \
                                                                \
    _gdb_->gpr[11] = (_regs_)->fp;                              \
    _gdb_->gpr[12] = (_regs_)->ip;                              \
    _gdb_->gpr[13] = (_regs_)->sp;                              \
    _gdb_->gpr[14] = (_regs_)->lr;                              \
    _gdb_->gpr[15] = (_regs_)->pc;                              \
    _gdb_->ps = (_regs_)->cpsr;                                 \
    HAL_GET_GDB_COPROCESSOR_REGISTERS(_gdb_,_regs_);            \
    CYG_MACRO_END

// Copy a set of registers from a GDB_Registers structure into a
// HAL_SavedRegisters structure.
#define HAL_SET_GDB_REGISTERS( _regs_ , _aregval_ )             \
    CYG_MACRO_START                                             \
    GDB_Registers *_gdb_ = (GDB_Registers *)(_aregval_);        \
    int _i_;                                                    \
                                                                \
    for( _i_ = 0; _i_ <= 10; _i_++ )                            \
        (_regs_)->d[_i_] = _gdb_->gpr[_i_];                     \
                                                                \
    (_regs_)->fp = _gdb_->gpr[11];                              \
    (_regs_)->ip = _gdb_->gpr[12];                              \
    (_regs_)->sp = _gdb_->gpr[13];                              \
    (_regs_)->lr = _gdb_->gpr[14];                              \
    (_regs_)->pc = _gdb_->gpr[15];                              \
    (_regs_)->cpsr = _gdb_->ps;                                 \
    HAL_SET_GDB_COPROCESSOR_REGISTERS(_regs_,_gdb_);            \
    CYG_MACRO_END

#if defined(CYGDBG_HAL_DEBUG_GDB_CTRLC_SUPPORT) || defined(CYGDBG_HAL_DEBUG_GDB_BREAK_SUPPORT)
#define HAL_GET_PROFILE_INFO( _thepc_, _thesp_ )                \
    CYG_MACRO_START                                             \
    extern HAL_SavedRegisters *hal_saved_interrupt_state;       \
    if ( hal_saved_interrupt_state ) {                          \
        (_thepc_) = (char *)(hal_saved_interrupt_state->pc);    \
        (_thesp_) = (char *)(hal_saved_interrupt_state->sp);    \
    }                                                           \
    CYG_MACRO_END
#endif

//--------------------------------------------------------------------------
// HAL setjmp

#define CYGARC_JMP_BUF_SIZE 16  // Actually 11, but some room left over

typedef cyg_uint32 hal_jmp_buf[CYGARC_JMP_BUF_SIZE];

externC int hal_setjmp(hal_jmp_buf env);
externC void hal_longjmp(hal_jmp_buf env, int val);


//--------------------------------------------------------------------------
// Idle thread code.
// This macro is called in the idle thread loop, and gives the HAL the
// chance to insert code. Typical idle thread behaviour might be to halt the
// processor. Here we only supply a default fallback if the variant/platform
// doesn't define anything.

#ifndef HAL_IDLE_THREAD_ACTION
#define HAL_IDLE_THREAD_ACTION(_count_) CYG_EMPTY_STATEMENT
#endif

//---------------------------------------------------------------------------

// Minimal and sensible stack sizes: the intention is that applications
// will use these to provide a stack size in the first instance prior to
// proper analysis.  Idle thread stack should be this big.

//    THESE ARE NOT INTENDED TO BE MICROMETRICALLY ACCURATE FIGURES.
//           THEY ARE HOWEVER ENOUGH TO START PROGRAMMING.
// YOU MUST MAKE YOUR STACKS LARGER IF YOU HAVE LARGE "AUTO" VARIABLES!

// This is not a config option because it should not be adjusted except
// under "enough rope" sort of disclaimers.

// A minimal, optimized stack frame, rounded up - no autos
#if defined(CYGHWR_HAL_ARM_NEON) || defined(CYGHWR_HAL_ARM_FPU)
#define CYGNUM_HAL_STACK_FRAME_SIZE ((4 * 20) + (2 * 20) + (8 * 32))
#else
#define CYGNUM_HAL_STACK_FRAME_SIZE (4 * 20)
#endif
// Stack needed for a context switch: this is implicit in the estimate for
// interrupts so not explicitly used below:
#define CYGNUM_HAL_STACK_CONTEXT_SIZE CYGNUM_HAL_STACK_FRAME_SIZE

// Interrupt + call to ISR, interrupt_end() and the DSR
#define CYGNUM_HAL_STACK_INTERRUPT_SIZE \
    (CYGNUM_HAL_STACK_FRAME_SIZE + (2 * CYGNUM_HAL_STACK_FRAME_SIZE))

// Space for the maximum number of nested interrupts, plus room to call functions
#define CYGNUM_HAL_MAX_INTERRUPT_NESTING 4

#define CYGNUM_HAL_STACK_SIZE_MINIMUM \
        ((CYGNUM_HAL_MAX_INTERRUPT_NESTING * CYGNUM_HAL_STACK_INTERRUPT_SIZE) + \
         (2 * CYGNUM_HAL_STACK_FRAME_SIZE))

#define CYGNUM_HAL_STACK_SIZE_TYPICAL \
        (CYGNUM_HAL_STACK_SIZE_MINIMUM + \
         (16 * CYGNUM_HAL_STACK_FRAME_SIZE))

//--------------------------------------------------------------------------
// Macros for switching context between two eCos instances (jump from
// code in ROM to code in RAM or vice versa).
#define CYGARC_HAL_SAVE_GP()
#define CYGARC_HAL_RESTORE_GP()

#endif // CYGONCE_HAL_ARCH_H
// End of hal_arch.h
