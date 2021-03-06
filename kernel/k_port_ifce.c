/* XXX to be removed */

#include <k_asm.h>
#include <k_atomic.h>
#include <k_cdefs.h>
#include <k_console.h>
#include <k_port.h>
#include <k_port_ifce.h>
#include <k_string.h>

#include <osrt/error.h>

/* interface declarations */
void port_ifce_debug_print_init(void);
void port_ifce_setfs_init(void);

#define PORT_IFCE_HASH_SIZE 512
static atomic_ptr port_ifce_hash[PORT_IFCE_HASH_SIZE];

static void
_port_ifce_hash_insert(port_ifce_t *ifce)
{
        size_t hash =
            kstr_hash(ifce->name, kstrlen(PORT_NAME_MAX_LEN, ifce->name));

        size_t i = hash % PORT_IFCE_HASH_SIZE;
        do {
                void *expected = NULL;
                if (atomic_cmpxchg_strong_ptr(
                        port_ifce_hash[i], expected, ifce, __ATOMIC_ACQ_REL,
                        __ATOMIC_ACQUIRE)) {
                        return;
                }
                kprintf("expected = 0x%x\n", expected);
                i = (i + 1) % PORT_IFCE_HASH_SIZE;
        } while (i != hash % PORT_IFCE_HASH_SIZE);

        PANIC("failed to create port-like interface");
}

static port_ifce_t *
_port_ifce_hash_locate(const char *name)
{
        size_t hash = kstr_hash(name, kstrlen(PORT_NAME_MAX_LEN, name));

        size_t i = hash % PORT_IFCE_HASH_SIZE;
        do {
                port_ifce_t *ret =
                    atomic_load_ptr(port_ifce_hash[i], __ATOMIC_ACQUIRE);

                if (!ret) { return NULL; }

                if (kstrequ(PORT_NAME_MAX_LEN, ret->name, name)) { return ret; }

                i = (i + 1) % PORT_IFCE_HASH_SIZE;
        } while (i != hash % PORT_IFCE_HASH_SIZE);

        return NULL;
}

void
port_ifce_init(void)
{
        kmemset(port_ifce_hash, 0, sizeof(port_ifce_hash));

        port_ifce_debug_print_init();
        port_ifce_setfs_init();
}

void
port_ifce_create(port_ifce_t *ifce)
{
        _port_ifce_hash_insert(ifce);
}

port_ifce_t *
port_ifce_lookup(const char *name)
{
        return _port_ifce_hash_locate(name);
}

/** debug print interface **/

sint
port_ifce_debug_print_open(void)
{
        /* TODO: privilege check */
        return OK;
}

u64
port_ifce_debug_print_request(
    u64 val_small, void *data, size_t data_size, void *reply, size_t reply_size)
{
        DONTCARE(val_small);
        DONTCARE(reply);
        DONTCARE(reply_size);

        kprintf("%sn", data, data_size);

        return OK;
}

void
port_ifce_debug_print_close(void)
{
        return;
}

void
port_ifce_debug_print_init(void)
{
        port_ifce_t *ifce = kmem_alloc(sizeof(port_ifce_t));
        VERIFY(ifce, "failed to allocate debug interface");

        ifce->name       = "kern.debug_print";
        ifce->open_func  = port_ifce_debug_print_open;
        ifce->req_func   = port_ifce_debug_print_request;
        ifce->close_func = port_ifce_debug_print_close;

        port_ifce_create(ifce);
}

/** FSBase setting interface **/

sint
port_ifce_setfs_open(void)
{
        return OK;
}

u64
port_ifce_setfs_request(
    u64 val_small, void *data, size_t data_size, void *reply, size_t reply_size)
{
        DONTCARE(data);
        DONTCARE(data_size);
        DONTCARE(reply);
        DONTCARE(reply_size);
        /* write FSBase */
        wrmsr(0xc0000100, val_small);
        return OK;
}

void
port_ifce_setfs_close(void)
{
        return;
}

void
port_ifce_setfs_init(void)
{
        port_ifce_t *ifce = kmem_alloc(sizeof(port_ifce_t));
        VERIFY(ifce, "failed to allocate debug interface");

        ifce->name       = "kern.setfsbase";
        ifce->open_func  = port_ifce_setfs_open;
        ifce->req_func   = port_ifce_setfs_request;
        ifce->close_func = port_ifce_setfs_close;

        port_ifce_create(ifce);
}
