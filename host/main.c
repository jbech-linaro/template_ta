/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <err.h>
#include <stdio.h>
#include <string.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

/* To the the UUID (found the the TA's h-file(s)) */
#include <template_ta.h> 

#if USE_PTHREADS
#include <pthread.h>
static void *read_thread(void __unused *arg)
{
	pthread_t id = pthread_self();
	printf("NW: Thread with 0x%08lx created\n", id);
	return NULL;
}
#endif

static void call_taf_0(void)
{
        TEEC_Context ctx;
        TEEC_Operation op;
        TEEC_Result res;
        TEEC_Session sess;
        TEEC_UUID uuid = TEMPLATE_TA_UUID;
        uint32_t err_origin;
	uint32_t buf = 0xcafebabe;

        /* Initialize a context connecting us to the TEE */
        res = TEEC_InitializeContext(NULL, &ctx);
        if (res != TEEC_SUCCESS)
                errx(1, "NW: TEEC_InitializeContext failed with code 0x%x", res);

	/* Open the session with the Trusted Application */
        res = TEEC_OpenSession(&ctx, &sess, &uuid,
                               TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
        if (res != TEEC_SUCCESS)
                errx(1, "NW: TEEC_Opensession failed with code 0x%x origin 0x%x",
                        res, err_origin);

	/* Clear the TEEC_Operation struct */
        memset(&op, 0, sizeof(op));

	/*
	 * Prepare the parameters that will be sent to the Trusted Application.
	 */
        op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
                                         TEEC_NONE,
                                         TEEC_NONE,
                                         TEEC_NONE);

        op.params[0].tmpref.buffer = (void *)&buf;
        op.params[0].tmpref.size = sizeof(uint32_t);

	/* 
	 * TEMPLATE_TA_FUNCTION_ID_0 is the actual function in the TA to be
	 * called.
	 */
	printf("NW: Invoke the TA with buf val: 0x%08x\n", buf);
        res = TEEC_InvokeCommand(&sess, TEMPLATE_TA_FUNCTION_ID_0, &op,
                                 &err_origin);
        if (res != TEEC_SUCCESS)
                errx(1, "NW: TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
                        res, err_origin);

        /* We're done with the TA, close the session ... */
        TEEC_CloseSession(&sess);

        /* ... and destroy the context. */
        TEEC_FinalizeContext(&ctx);

        return;
}  

int main() {
#if USE_PTHREADS
	pthread_t pt;
	int err;
	err = pthread_create(&pt, NULL, &read_thread, NULL);
	if (err)
		errx(1, "NW: can't create thread :[%s]\n", (char *)strerror(err));
#endif
	/* Call Trusted Application with TA function ID 0 */
	call_taf_0();

	return 0;
}
