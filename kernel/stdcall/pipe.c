#include <stdint.h>
#include "../mm.h"
#include "../semaphore.h"
#include "../task.h"
#include "../util.h"

int gen_pipe(thread *t)
{
    t->pstruct->pipe_ptr = kmalloc(NPIPEQUEUE);
    if (t->pstruct->pipe_ptr) {
        t->pstruct->pstate = EMPTY;
        t->pstruct->datalen = 0;
        *t->sid_ptr = create_sem(0);
        return 1;
    } else {
        return 0;
    }
}

int pread(void *buf, uint64_t buflen)
{
    static int index = 0;
    thread *t = get_thread_ptr(get_cur_thread_tid());
    if (t->pstruct->pstate == EXIST) {
        t->pstruct->pstate = RECEIVING;
        if (t->pstruct->datalen > buflen) {
            memcpy(buf, &t->pstruct->pipe_ptr[index], buflen);
            index += buflen;
            t->pstruct->pstate = EXIST;
        } else {
            memcpy(buf, &t->pstruct->pipe_ptr[index], t->pstruct->datalen);
            index = 0;
            t->pstruct->pstate = EMPTY;
        }
    }
    if (index != 0) {
        return 0;
    } else {
        return 0;
    }
}

int pwrite(void *buf, uint64_t n)
{
    thread *t = get_thread_ptr(get_cur_thread_tid());
    if (t->pstruct->pstate != FULL) {
        t->pstruct->pstate = SENDING;
        if (n > NPIPEQUEUE) {
            memcpy(t->pstruct->pipe_ptr, buf, NPIPEQUEUE);
            t->pstruct->pstate = FULL;
            t->pstruct->datalen = NPIPEQUEUE;
            return 0;
        } else {
            memcpy(t->pstruct->pipe_ptr, buf, n);
            t->pstruct->pstate = EXIST;
            t->pstruct->datalen = n;
            return 1;
        }
    }
    return 0;
}

int pwait(sid_t sem)
{
    //thread *t = get_thread_ptr(get_cur_thread_tid());
    //if (t->pstruct->pstate == EMPTY || t->pstruct->pstate == SENDING) {
        return wait(sem);
    //} else {
    //    return 1;
    //}
}

int pwait_consumer(sid_t sem)
{
    thread *t = get_thread_ptr(get_cur_thread_tid());
    if (t->pstruct->pstate == EMPTY || t->pstruct->pstate == SENDING) {
        return wait(sem);
    } else {
        return 1;
    }
}

int pwait_producer(sid_t sem)
{
    thread *t = get_thread_ptr(get_cur_thread_tid());
    if (t->pstruct->pstate == EXIST || t->pstruct->pstate == RECEIVING) {
        return wait(sem);
    } else {
        return 1;
    }
}

int psignal(sid_t sem)
{
    //thread *t = get_thread_ptr(get_cur_thread_tid());
    //if (t->pstruct->pstate == EXIST) {
        return signal(sem);
    //} else {
    //    return 0;
    //}
}

int psignal_consumer(sid_t sem)
{
    thread *t = get_thread_ptr(get_cur_thread_tid());
    if (t->pstruct->pstate == EMPTY) {
        return signal(sem);
    } else {
        return 0;
    }
}

int psignal_producer(sid_t sem)
{
    thread *t = get_thread_ptr(get_cur_thread_tid());
    if (t->pstruct->pstate == EXIST) {
        return signal(sem);
    } else {
        return 0;
    }
}

int pread_await(void *buf, uint64_t buflen)
{
    thread *t = get_thread_ptr(get_cur_thread_tid());
    int s = pwait(*t->sid_ptr);
    if (!s) {
        return -1;
    } else {
        int r = pread(buf, buflen);
        psignal(*t->sid_ptr);
        return r;
    }
}

int pwrite_await(void *buf, uint64_t n)
{
    thread *t = get_thread_ptr(get_cur_thread_tid());
    int s = pwait(*t->sid_ptr);
    if (!s) {
        return -1;
    } else {
        int w = pwrite(buf, n);
        psignal(*t->sid_ptr);
        return w;
    }
}
