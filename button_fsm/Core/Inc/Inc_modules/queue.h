/*
 * queue.h
 *
 *  Created on: Oct 30, 2025
 *      Author: soaic
 */

#ifndef INC_INC_MODULES_QUEUE_H_
#define INC_INC_MODULES_QUEUE_H_

#include <stdint.h>
#include "software_timer.h"

#define QUEUE_SIZE	MAX_TIMER
#define QUEUE_NEXT(x)  (((x) + 1) % QUEUE_SIZE)

typedef struct {
    volatile uint8_t buf[QUEUE_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;
}queue_t;

static inline void queue_init(volatile queue_t *q)
{
    q->head = 0;
    q->tail = 0;
    q->count = 0;
};

static inline uint8_t queue_is_full(const volatile queue_t *q)
{
    return (q->count == QUEUE_SIZE);
};

static inline uint8_t queue_is_empty(const volatile queue_t *q)
{
    return (q->count == 0);
};

static inline uint8_t queue_enqueue(volatile queue_t *q, uint8_t data)
{
    uint8_t next = QUEUE_NEXT(q->head);
    if (next == q->tail)
    {
    	return 0;
    }
    q->buf[q->head] = data;
    q->head = next;
    return 1;
};

static inline uint8_t queue_dequeue(volatile queue_t *q, uint8_t *data)
{
    if (q->head == q->tail)
    {
    	return 0;
    }

    *data = q->buf[q->tail];
    q->tail = QUEUE_NEXT(q->tail);

    return 1;
};

static inline uint8_t queue_peek(const volatile queue_t *q, uint8_t *data)
{
    if (queue_is_empty(q))
    {
    	return 0;
    }

    *data = q->buf[q->tail];
    return 1;
};
#endif /* INC_INC_MODULES_QUEUE_H_ */
