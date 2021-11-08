/*
 * callback_scheduler.c
 *
 *  Created on: 2018年1月26日
 *      Author: gene
 */

#include "callback_scheduler.h"
#include "common/systick.h"
#include "base/types.h"

#ifndef MAX_SCHEDULER_CALLBACK_COUNT
#define MAX_SCHEDULER_CALLBACK_COUNT 30
#endif

static callback_void_void LOCKED_SYSTICK_CALLBACK;
static bool_t POST_DELAY_LOCKED;

typedef struct callback_record {
	int_16 next:16;
	uint_16 type:16;
	void* pv;
	uint_32 interval;
	callback_void_type_pointer callback;
} callback_record_t;

static int_16 _workheadpos = -1;
static int_16 _freeheadpos = -1;
static callback_record_t _recordspool[MAX_SCHEDULER_CALLBACK_COUNT];

static void _initialze_wf_stack() {
	_freeheadpos = MAX_SCHEDULER_CALLBACK_COUNT - 1;
	for (int_16 i = MAX_SCHEDULER_CALLBACK_COUNT - 1; i > -1; i--) {
		_recordspool[i].next = i - 1;
	}
}

static inline void _insert_element(uint_16 element, uint_32 interval) {
	int_16 next, current = _workheadpos;
	uint_32 nextInterval;

	while(true) {
		next = _recordspool[current].next;

		if (next == -1) {
			break;
		}

		nextInterval = _recordspool[next].interval;

		if (nextInterval > interval) {
			_recordspool[next].interval = nextInterval - interval;
			break;
		}

		interval = interval - nextInterval;
		current = next;
	}

	_recordspool[current].next = element;
	_recordspool[element].next = next;
	_recordspool[element].interval = interval;
}

void scheduler_systick_callback() {
	if (POST_DELAY_LOCKED) {
		LOCKED_SYSTICK_CALLBACK = scheduler_systick_callback;
		return;
	}

	if (_workheadpos == -1) return;

	int_16 element = _workheadpos;

	// Point to next callback element and post.
	_workheadpos = _recordspool[_workheadpos].next;

	if (_workheadpos != -1) {
		systick_postdelay(_recordspool[_workheadpos].interval);
	}

	// Push free
	_recordspool[element].next = _freeheadpos;
	_freeheadpos = element;

	_recordspool[element].callback(_recordspool[element].type, _recordspool[element].pv);
}

bool_t initialze_scheduler() {
	LOCKED_SYSTICK_CALLBACK = null;
	POST_DELAY_LOCKED = false;
	_initialze_wf_stack();
	systick_initialize(scheduler_systick_callback);
	return true;
}

bool_t callback_scheduler_postdely_s(callback_void_type_pointer callback, void* pv, uint_16 type, uint_32 delay) {
	return callback_scheduler_postdely_us(callback, pv, type, delay * 1000000);
}

bool_t callback_scheduler_postdely_ms(callback_void_type_pointer callback, void* pv, uint_16 type, uint_32 delay) {
	return callback_scheduler_postdely_us(callback, pv, type, delay * 1000);
}

bool_t callback_scheduler_postdely_us(callback_void_type_pointer callback, void* pv, uint_16 type, uint_32 delay) {
	POST_DELAY_LOCKED = true;

	int_16 element = _freeheadpos;

	// Can not get free callback element.
	if (element == -1) return false;

	// Pop free element.
	_freeheadpos = _recordspool[element].next;

	_recordspool[element].callback = callback;
	_recordspool[element].type = type;
	_recordspool[element].pv = pv;

	uint_32 timeleft = systick_get_timeleft();

	if (_workheadpos == -1) {
		goto ADD_AND_POST;
	}

	if (delay < timeleft) {
		systick_clear();
		_recordspool[_workheadpos].interval = timeleft - delay;
		goto ADD_AND_POST;
	} else {
		_insert_element(element, delay - timeleft);
		goto DO_NOTHING;
	}

ADD_AND_POST:
	_recordspool[element].next = _workheadpos;
	_recordspool[element].interval = delay;
	_workheadpos = element;
	systick_postdelay(delay);
DO_NOTHING:

	POST_DELAY_LOCKED = false;
	if (null != LOCKED_SYSTICK_CALLBACK) {
		LOCKED_SYSTICK_CALLBACK();
		LOCKED_SYSTICK_CALLBACK = null;
	}
	return true;
}
