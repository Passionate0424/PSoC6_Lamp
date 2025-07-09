#ifndef __LLM_CONTORY_H__
#define __LLM_CONTORY_H__

#include <rtthread.h>
#include "llm.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern rt_mutex_t llm_mutex;

    void llm_send_env(void);
    void llm_send_char(char *buffer);

#ifdef __cplusplus
}
#endif

#endif // __LLM_CONTORY_H__
