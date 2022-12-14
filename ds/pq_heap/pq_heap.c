/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Name: Daniel Jenudi
Reviewer: Ran
Date: 25/05/2022
File: Data Structures - Priority Queue heap
Description: Implementing Priority Queue data structure with heap
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include <assert.h>/*assert*/
#include <stdlib.h>/*malloc, free*/

#include "pq_heap.h"/* pq.h */
#include "heap.h"/* heap.h */

enum Status
{
	SUCCESS = 0,
	FAIL
};

struct pq
{
	heap_t *queue;
	pq_cmp_func_t cmp;
	void *cmp_param;
};

typedef struct param_ismatch
{
	pq_is_match_t *is_match;
	void *param;
} param_ismatch_t;

static int IsMatchRapper(const void *data, const void *heap_data, 
						 void *param_ismatch);

pq_t *PQCreate(pq_cmp_func_t cmp_func, void *param)
{
	pq_t *pq = NULL;
	
	assert(NULL != cmp_func);
	
	pq = (pq_t *)malloc(sizeof(pq_t));
	if(NULL == pq)
	{	
		return NULL;	
	}
	
	pq->queue = HeapCreate(cmp_func, param);
	if(NULL == pq->queue)
	{
		free(pq);
		pq = NULL;
		return NULL;
	} 
	
	/* init PQ */
	pq->cmp = cmp_func;
	pq->cmp_param = param;

	return pq;
}

void PQDestroy(pq_t *pq)
{
	assert(NULL != pq);
	
	HeapDestroy(pq->queue);
	pq->queue = NULL;
	free(pq);
	pq = NULL;
}

int PQEnqueue(pq_t *pq, void *data)
{
	assert(NULL != pq);
	assert(NULL != data);

	if(FAIL == HeapPush(pq->queue, data))
	{
		HeapRemove(pq->queue, data, pq->cmp, pq->cmp_param);
		return FAIL;
	}

	return SUCCESS;
}

void *PQDequeue(pq_t *pq)
{
	assert(NULL != pq);
	return HeapPop(pq->queue);
}

int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);
	return HeapIsEmpty(pq->queue);
}

void *PQPeek(const pq_t *pq)
{
	assert(NULL != pq);	
	return HeapPeek(pq->queue);
}

size_t PQSize(const pq_t *pq)
{
	assert(NULL != pq);	
	return HeapSize(pq->queue);
}

void PQClear(pq_t *pq)
{
	assert(NULL != pq);	

	while(!HeapIsEmpty(pq->queue))
	{
		HeapPop(pq->queue);
	}
}

void *PQErase(pq_t *pq, void *param, pq_is_match_t is_match)
{
	param_ismatch_t param_ismatch = {0}; 
	
	assert(NULL != pq);		
	assert(NULL != is_match);

	param_ismatch.is_match = &is_match; 
	param_ismatch.param = param; 
	
	return HeapRemove(pq->queue, NULL, &IsMatchRapper, &param_ismatch);;
}

static int IsMatchRapper(const void *data, const void *heap_data, 
						 void *param_ismatch)
{
	param_ismatch_t param_ismatch_stract = {0};
	pq_is_match_t *is_match = NULL;
	void *param = NULL;

	assert(NULL != heap_data);		
	assert(NULL != param_ismatch);

	param_ismatch_stract = *(param_ismatch_t *)param_ismatch;
	is_match = param_ismatch_stract.is_match;
	param = param_ismatch_stract.param;

	(void)data;

	return (*is_match)(heap_data, param);
}