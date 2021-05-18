/* syscall.h -- thin wrapper around kernel syscalls */
#ifndef RENZAN_LIBOS_SYSCALL_H_
#define RENZAN_LIBOS_SYSCALL_H_

#ifdef __RENZAN_OSRT

#include <stddef.h>
#include <stdint.h>

#include <os/kernel/futex.h>
#include <os/kernel/kobject.h>
#include <os/kernel/memory.h>
#include <os/kernel/port.h>
#include <os/kernel/process.h>
#include <os/kernel/syscall.h>

kobject_t syscall_as_create(void);
kobject_t syscall_as_clone(kobject_t address_space);
int64_t   syscall_as_destroy(kobject_t address_space);
intptr_t  syscall_mmap(
     kobject_t address_space, uintptr_t vaddr, size_t size, unsigned int flag);
kobject_t syscall_mtransfer(
    kobject_t dst, kobject_t src, uintptr_t vaddr_dst, uintptr_t vaddr_src,
    size_t size, unsigned int flag);
int64_t   syscall_munmap(kobject_t address_space, uintptr_t vaddr, size_t size);
kobject_t syscall_port_create(char *name, size_t namelen);
kobject_t syscall_port_open(char *name, size_t namelen);
int64_t   syscall_port_close(kobject_t port);
int64_t   syscall_port_request(kobject_t port, port_request_t *request);
int64_t   syscall_port_receive(
      kobject_t port, port_request_t *request_buffer, void *data_buffer,
      size_t buffer_size);
int64_t syscall_port_response(
    kobject_t request, uint64_t retval, void *ret_data, size_t ret_data_size);
pid_t   syscall_process_spawn(kobject_t address_space, void *entry_point);
void    syscall_process_exit(uint64_t retval) __attribute__((noreturn));
int64_t syscall_process_wait(pid_t pid, process_state_t *state);
int64_t syscall_reincarnate(kobject_t address_space, void *entry)
    __attribute__((noreturn));
void syscall_futex_wait(void *addr, futex_val_t val);
void syscall_futex_wake(void *addr, size_t wake_count);

#endif /* __RENZAN_OSRT */

#endif /* RENZAN_LIBOS_SYSCALL_H_ */