#ifndef IZANAMI_ERROR_H_
#define IZANAMI_ERROR_H_

enum KERN_ERRCODE {
        KERN_OK          = 0,
        KERN_ERROR_NOMEM = 1,
        KERN_ERROR_UNAUTH,
        KERN_ERROR_INVAL,
        KERN_ERROR_NOENT,
        KERN_ERROR_TIMEOUT,

        /* port */
        KERN_ERROR_PORT_CLOSED = 0x100,
        KERN_ERROR_PORT_NAME_OCCUPIED,
        KERN_ERROR_PORT_REJECTED,
        KERN_ERROR_PORT_DATA_TOO_LONG,

        KERN_ERROR_END = 0x1000
};

#endif /* YOMOTSU_ERROR_H_ */
