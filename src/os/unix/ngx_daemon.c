
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


// daemon为on时，终端进程fork子进程作为master进程后，退出
ngx_int_t
ngx_daemon(ngx_log_t *log)
{
    int  fd;

    switch (fork()) { // 终端作为父进程，fork子进程作为master进程
    case -1:
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "fork() failed");
        return NGX_ERROR;

    case 0: // fork返回pid为0表示当前为子进程，即master进程
        break;

    default: // fork返回子进程pid>0，表示当前为父进程，即终端进程
        exit(0); // 终端进程退出
    }

    ngx_parent = ngx_pid;
    ngx_pid = ngx_getpid();

    if (setsid() == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "setsid() failed");
        return NGX_ERROR;
    }

    umask(0);

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      "open(\"/dev/null\") failed");
        return NGX_ERROR;
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDIN) failed");
        return NGX_ERROR;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDOUT) failed");
        return NGX_ERROR;
    }

#if 0
    if (dup2(fd, STDERR_FILENO) == -1) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "dup2(STDERR) failed");
        return NGX_ERROR;
    }
#endif

    if (fd > STDERR_FILENO) {
        if (close(fd) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno, "close() failed");
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}
