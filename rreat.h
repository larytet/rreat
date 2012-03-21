#ifndef __RREAT__
#define __RREAT__

#include "config.h"

typedef unsigned long addr_t;

struct _rreat;

typedef struct _rreat_simulate_t {
	addr_t start;
	addr_t end;

	// internal
	struct _rreat_t *_rr;
	addr_t _mem;
	void *_backup;
} rreat_simulate_t;

typedef struct _rreat_thread_t {
    int thread_id;
    HANDLE handle;
} rreat_thread_t;

typedef struct _rreat_t {
	int process_id;
    HANDLE handle;
	int thread_count;
	rreat_thread_t *threads;
} rreat_t;

#define RREAT_SUCCESS 0
#define RREAT_WAIT    1

#define RREAT_READ   1
#define RREAT_WRITE  2
#define RREAT_EXEC   4
#define RREAT_RWX    (RREAT_READ | RREAT_WRITE | RREAT_EXEC)

#ifndef sizeofarray
#define sizeofarray(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

//
// RREAT Memory API
//

// allocate memory in process
addr_t rreat_alloc(rreat_t *rr, unsigned long size, unsigned long flags);
// free memory in process
void rreat_free(rreat_t *rr, addr_t addr);
// write to memory in process
void rreat_write(rreat_t *rr, addr_t addr, const void *src, unsigned long size);
// read from memory in process
void rreat_read(rreat_t *rr, addr_t addr, void *dest, unsigned long size);

//
// RREAT Debugger API
//

// create a new process object
rreat_t *rreat_process_init(const char *filename);

// create a new thread object
rreat_thread_t *rreat_thread_init(rreat_t *rr, HANDLE handle);

// resume a thread
void rreat_thread_resume(rreat_t *rr, int thread_id);

// get a thread object by its id
rreat_thread_t *rreat_thread_by_id(rreat_t *rr, int thread_id);

// dump a series of pages
void rreat_dump_module(rreat_t *rr, addr_t base_addr, const char *filename);

// attach JIT Debugger to Process
void rreat_jitdbg_attach(rreat_t *rr);

// create a dummy thread
int rreat_thread_dummy(rreat_t *rr);

// places the thread in a while(1) loop
// with a jmp behind it that will point to the original address
void rreat_thread_while1(rreat_t *rr, int thread_id);

//
// RREAT Simulate API
//

// init new object
rreat_simulate_t *rreat_simulate_init(rreat_t *rr);

// assign start and end address, `wait' will run until `end' is hit.
void rreat_simulate_address(rreat_simulate_t *rr, addr_t start, addr_t end);

// apply in the process
void rreat_simulate_apply(rreat_simulate_t *sim);

// wait for a certain thread to finish this `simulation'
int rreat_simulate_wait(rreat_simulate_t *sim, int thread_id, int milliseconds);

// restore the thread to the real address
void rreat_simulate_restore(rreat_simulate_t *sim, int thread_id);

// free simulate api object
void rreat_simulate_free(rreat_simulate_t *sim);

// single-threaded blocking `simulate' event.
void rreat_simulate_single(rreat_t *rr, addr_t start, addr_t end,
        int milliseconds, int thread_id);

#endif