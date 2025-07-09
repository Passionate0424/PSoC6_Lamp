/*
 * Copyright (c) 2006-2025, RT-Thread Development Team
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025/02/01     Rbb666       Add license info
 * 2025/02/03     Rbb666       Unified Adaptive Interface
 * 2025/02/06     Rbb666       Add http stream support
 * 2025/02/10     CXSforHPU    Add llm history support
 */
#include "llm.h"
#include "webclient.h"
#include <cJSON.h>
#include "llm_contory.h"

#define LLM_API_KEY PKG_LLM_API_KEY
#if defined(PKG_LLM_QWEN_API_URL)
#define LLM_API_URL PKG_LLM_QWEN_API_URL
#elif defined(PKG_LLM_DOUBAO_API_URL)
#define LLM_API_URL PKG_LLM_DOUBAO_API_URL
#elif defined(PKG_LLM_DEEPSEEK_API_URL)
#define LLM_API_URL PKG_LLM_DEEPSEEK_API_URL
#endif
#define LLM_MODEL_NAME PKG_LLM_MODEL_NAME
#define WEB_SOCKET_BUF_SIZE PKG_WEB_SORKET_BUFSZ

static char authHeader[128] = {0};
static char responseBuffer[WEB_SOCKET_BUF_SIZE] = {0};
static char contentBuffer[WEB_SOCKET_BUF_SIZE] = {0};
static char allContent[WEB_SOCKET_BUF_SIZE] = {0};

/**
 * @brief: create char for request payload.
 * @messages: llm_obj.messages.
 * @return: the  char for request payload.
 * if you want to modify the request payload, you can modify the following code.
 **/
rt_weak char *create_payload(cJSON *messages)
{
    cJSON *requestRoot = cJSON_CreateObject();
    cJSON *model = cJSON_CreateString(LLM_MODEL_NAME);
    cJSON *messages_copy = cJSON_Duplicate(messages, 1);
    char *payload = NULL;
    cJSON_AddItemToObject(requestRoot, "model", model);
    cJSON_AddItemToObject(requestRoot, "messages", messages_copy);
#ifdef PKG_LLMCHAT_STREAM
    cJSON_AddBoolToObject(requestRoot, "stream", RT_TRUE);
#else
    cJSON_AddBoolToObject(requestRoot, "stream", RT_FALSE);
#endif
    payload = cJSON_PrintUnformatted(requestRoot);
    cJSON_Delete(requestRoot);

    return payload;
}

/**
 * @brief: deal the answer of the llm
 * @handle: llm_t
 * if you want to modify the dealing, you can modify the following code.
 **/
rt_weak void deal_llm_answer(llm_t handle)
{
    char *answer = RT_NULL;
    rt_mb_recv(handle->outputbuff_mb, (rt_uint32_t *)&answer, RT_WAITING_FOREVER);
    /* you can modify */

    int len = rt_strlen(answer);
    rt_kprintf("LLM :\n");
    for (int i = 0; i <= len; i++)
    {
        rt_kprintf("%c", answer[i]);
    }
    rt_kprintf("\n");

    /* end */
    rt_free(answer);
}

/**
 * @brief: add message to messages
 * @input_buffer: your input buffer or assistant output buffer.
 * @role: 'user' or 'assistant'.
 * @handle: llm_t.
 **/
void add_message2messages(const char *input_buffer, char *role, llm_t handle)
{
    if (!cJSON_IsArray(handle->messages))
    {
        handle->messages = cJSON_CreateArray();
    }

    cJSON *message = create_message(input_buffer, role);
    cJSON_AddItemToArray(handle->messages, message);
}

/**
 * @brief: creat a message
 * @input_buffer: your input buffer or assistant output buffer.
 * @role: 'user' or 'assistant'.
 * @return cJSON* The message object.
 * such as:
 * {"role": "user", "content": "Hello!"}
 * {"role": "assistant", "content": "Hi there! How can I assist you today?"}
 **/
cJSON *create_message(const char *input_buffer, char *role)
{
    cJSON *message = cJSON_CreateObject();
    cJSON_AddStringToObject(message, "role", role);
    cJSON_AddStringToObject(message, "content", input_buffer);

    return message;
}

/**
 * @brief: clear messages
 * @handle: llm_obj
 **/
void clear_messages(llm_t handle)
{
    cJSON_Delete(handle->messages);
    handle->messages = cJSON_CreateArray();
}

/**
 * @brief: create the llm_t->messages and llm_t->get_answer of llm_t
 * @return llm_t
 **/
llm_t create_llm_t()
{
    llm_t handle = (llm_t)rt_malloc(sizeof(struct llm_obj));
    rt_err_t result = init_llm(handle);

    if (result != RT_EOK)
    {
        LLM_DBG("The llm interpreter thread create failed.\n");
        return RT_NULL;
    }

    return handle;
}

/**
 * @brief: delete the llm_t
 **/
void delete_llm_t(llm_t handle)
{
    if (handle != RT_NULL)
    {
        rt_thread_detach(&handle->thread);
        cJSON_Delete(handle->messages);
        rt_mb_delete(handle->inputbuff_mb);
        rt_mb_delete(handle->outputbuff_mb);
        rt_free(handle);
    }
    else
    {
        rt_kprintf("Error: can`t free llm_t.\n");
    }
}

/**
 * @brief: display the llm_t->messages
 **/
void display_llm_messages(llm_t handle)
{
    char *jsonString = cJSON_PrintUnformatted(handle->messages);
    int len = strlen(jsonString);
    for (int i = 0; i < len; i++)
    {
        rt_kprintf("%c", jsonString[i]);
    }

    cJSON_free(jsonString);
}

/**
 * @brief Get the answer from a large language model API.
 *
 * This function sends a POST request to a large language model API with a JSON payload.
 *
 * @param messages llm_obj.messages.
 *  you need to use 'add_message2messages()' to add messages which you want to send to the model,before using this function.
 * @return char* The model's answer as a string.
 * you need to free the memory by using rt_free(),when you don`t use the answer.
 *
 * example:
 #ifdef PKG_LLMCHAT_HISTORY_PAYLOAD
            add_message2messages(input_buffer, "user", &handle);

            char *result = handle.get_answer(handle.messages);

            add_message2messages(result, "assistant", &handle);

#else

            add_message2messages(input_buffer, "user", &handle);

            char *result = handle.get_answer(handle.messages);

            rt_free(result);
            clear_messages(&handle);
 *
 **/
char *get_llm_answer(cJSON *messages)
{
    struct webclient_session *webSession = RT_NULL;
    char *payload = RT_NULL;
    char *result = RT_NULL;
    int bytesRead, responseStatus;

    allContent[0] = '\0';

    // check the messages is array
    if (!cJSON_IsArray(messages))
    {
        rt_kprintf("Error: messages must be a cJSON array.\n");
        goto cleanup;
    }

    // Create web session
    webSession = webclient_session_create(WEB_SOCKET_BUF_SIZE);
    if (webSession == NULL)
    {
        rt_kprintf("Failed to create webclient session.\n");
        goto cleanup;
    }

    // Create JSON payload
    payload = create_payload(messages);
    if (payload == NULL)
    {
        rt_kprintf("Failed to create JSON payload.\n");
        goto cleanup;
    }

#ifdef PKG_LLMCHAT_DBG

    int len = strlen(payload);
    for (size_t i = 0; i < len; i++)
    {
        rt_kprintf("%c", payload[i]);
    }

#endif
    // Prepare authorization header
    rt_snprintf(authHeader, sizeof(authHeader), "Authorization: Bearer %s\r\n", LLM_API_KEY);

    // Add headers
    webclient_header_fields_add(webSession, "Content-Type: application/json\r\n");
    webclient_header_fields_add(webSession, authHeader);
    webclient_header_fields_add(webSession, "Content-Length: %d\r\n", rt_strlen(payload));

    LLM_DBG("HTTP Header: %s\n", webSession->header->buffer);
    LLM_DBG("HTTP Payload: %s\n", payload);

    // Send POST request
    responseStatus = webclient_post(webSession, LLM_API_URL, payload, rt_strlen(payload));
    if (responseStatus != 200)
    {
        rt_kprintf("Webclient POST request failed, response status: %d\n", responseStatus);
        goto cleanup;
    }

    // Read and process response
    while ((bytesRead = webclient_read(webSession, responseBuffer, WEB_SOCKET_BUF_SIZE)) > 0)
    {
        int inContent = 0;
        for (int i = 0; i < bytesRead; i++)
        {
            if (inContent)
            {
                if (responseBuffer[i] == '"')
                {
                    inContent = 0;

                    // Append content to allContent
                    size_t newLen = rt_strlen(contentBuffer);

                    // Print content
                    for (size_t i = 0; i < newLen; i++)
                    {
                        rt_kprintf("%c", contentBuffer[i]);
                    }

                    // Append content to allContent

                    strcat(allContent, contentBuffer);

                    // Reset content buffer
                    contentBuffer[0] = '\0';
                }
                else
                {
                    strncat(contentBuffer, &responseBuffer[i], 1);
                }
            }
            else if (responseBuffer[i] == '"' && i > 8 &&
                     rt_strncmp(&responseBuffer[i - 10], "\"content\":\"", 10) == 0)
            {
                inContent = 1;
            }
        }
    }

    rt_kprintf("\n");

cleanup:
    // Cleanup resources
    if (webSession)
    {
        webclient_close(webSession);
    }
    if (payload)
    {
        cJSON_free(payload);
    }

    if (allContent[0] != '\0')
    {
        result = rt_strdup(allContent);
    }
    return result;
}

static void recv_inputBuff_mb(void *handle)
{
    llm_t llm = (llm_t)handle;
    char *input_buffer = RT_NULL;
    while (1)
    {
        rt_mb_recv(llm->inputbuff_mb, (rt_uint32_t *)&input_buffer, RT_WAITING_FOREVER);

        /* to show the input */
        int len = rt_strlen(input_buffer);
        rt_kprintf("USER :\n");
        for (int i = 0; i <= len; i++)
        {
            rt_kprintf("%c", input_buffer[i]);
        }
        rt_kprintf("\n");

#ifdef PKG_LLMCHAT_HISTORY_PAYLOAD
        add_message2messages(input_buffer, "user", &llm);

        char *result = llm.get_answer(llm.messages);

        add_message2messages(result, "assistant", &llm);

#else
        add_message2messages(input_buffer, "user", llm);

        char *result = llm->get_answer(llm->messages);
#if defined(LLM_DBG)
        display_llm_messages(llm);
#endif
        clear_messages(llm);

#endif

        rt_mb_send(llm->outputbuff_mb, (rt_uint32_t)result);

        deal_llm_answer(llm);

        rt_free(input_buffer);
        rt_mutex_release(llm_mutex);
        rt_kprintf("llm_mutex released.\n");
    }
}

/**
 * @brief send the inputBuffer to llm_chat
 * @param handle the  llm_t
 * @param inputBuffer the inputBuffer
 */
void send_llm_mb(llm_t handle, char *inputBuffer)
{
    char *buffer = (char *)rt_malloc(strlen(inputBuffer) + 1);
    if (buffer == RT_NULL)
    {
        rt_kprintf("Failed to allocate memory for input buffer\n");
        return;
    }

    rt_memset(buffer, 0, strlen(inputBuffer) + 1);
    rt_strncpy(buffer, inputBuffer, strlen(inputBuffer));

    rt_mb_send(handle->inputbuff_mb, (rt_uint32_t)buffer);
}

/**
 * @brief: init the llm_t handle->messages ,llm_obj handle->get_answer
 * @handle: llm_t
 * @return: rt_err_t
 **/
rt_err_t init_llm(llm_t handle)
{
    handle->get_answer = get_llm_answer;
    handle->messages = cJSON_CreateArray();

    handle->inputbuff_mb = rt_mb_create("llm_inputbuff_mb", sizeof(char *) * LLM_CHAT_NUM, RT_IPC_FLAG_FIFO);
    if (handle->inputbuff_mb == RT_NULL)
    {
        rt_kprintf("can`t create inputbuff_mb");
    }
    handle->outputbuff_mb = rt_mb_create("llm_outputbuff_mb", sizeof(char *) * LLM_CHAT_NUM, RT_IPC_FLAG_FIFO);
    if (handle->outputbuff_mb == RT_NULL)
    {
        rt_kprintf("can`t create outputbuff_mb");
    }

#if defined(RT_VERSION_CHECK) && (RTTHREAD_VERSION >= RT_VERSION_CHECK(5, 1, 0))
    rt_uint8_t prio = RT_SCHED_PRIV(rt_thread_self()).current_priority + 1;
#else
    rt_uint8_t prio = rt_thread_self()->current_priority + 1;
#endif

    rt_err_t result = rt_thread_init(&handle->thread,
                                     "llm_thread",
                                     recv_inputBuff_mb,
                                     (void *)handle,
                                     &handle->thread_stack[0],
                                     sizeof(handle->thread_stack),
                                     prio,
                                     10);

    if (result != RT_EOK)
    {
        LLM_DBG("The llm interpreter thread create failed.\n");
        return RT_ERROR;
    }
    rt_thread_startup(&handle->thread);

    return RT_EOK;
}
