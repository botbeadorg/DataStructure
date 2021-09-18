#ifndef WHY__MEMORY__POOL__VERY__SIMPLE__HEADER
#define WHY__MEMORY__POOL__VERY__SIMPLE__HEADER

#include <vector>
#include <deque>
#include <stdint.h>
#include <windows.h>
#include <process.h>

#ifndef NUM_CHUNK_RESERVE
#define NUM_CHUNK_RESERVE 128
#endif

#ifndef NUM_ORIG_BLOCK
#define NUM_ORIG_BLOCK 4096
#endif

#ifndef NUM_PLUS_BLOCK
#define NUM_PLUS_BLOCK NUM_ORIG_BLOCK / 2
#endif

unsigned __stdcall plus(void *prm);

struct WHYmempool_s {
	bool stop;
	size_t size_block;
	uintptr_t t;

	std::vector<unsigned char *>chunks;
	std::deque<unsigned char *>block_stack;

	CRITICAL_SECTION cs_top;
	CRITICAL_SECTION cs_app_ready;
	CONDITION_VARIABLE cv_ready, cv_app;

	WHYmempool_s(size_t sz = 1024) : size_block(sz) {
		unsigned char * p =
			new(std::nothrow) unsigned char[size_block * NUM_ORIG_BLOCK];
		if (p) {
			stop = false;
			unsigned t_id;
			chunks.reserve(NUM_CHUNK_RESERVE);
			chunks.push_back(p);
			for (size_t i = 0; i < NUM_ORIG_BLOCK; ++i) {
				block_stack.push_back(p);
				p += size_block;
			}
			InitializeCriticalSectionAndSpinCount(&cs_top, 0x200);
			InitializeCriticalSectionAndSpinCount(&cs_app_ready, 0x400);
			InitializeConditionVariable(&cv_ready);
			InitializeConditionVariable(&cv_app);
			t = _beginthreadex(0, 0, plus, this, 0, &t_id);
		}
	}

	~WHYmempool_s() {
		stop = true;
		WakeAllConditionVariable(&cv_app);
		WaitForSingleObject((void *)t, INFINITE);
		CloseHandle((void *)t);
		DeleteCriticalSection(&cs_top);
		DeleteCriticalSection(&cs_app_ready);
		block_stack.clear();
		for (std::vector<unsigned char *>::iterator it = chunks.begin();
		chunks.end() != it; ++it) {
			unsigned char *p = *it;
			delete[]p;
		}
	}

	void get(unsigned char* *p) {
		EnterCriticalSection(&cs_app_ready);
		if (block_stack.size() <= NUM_PLUS_BLOCK) {
			WakeConditionVariable(&cv_app);
			SleepConditionVariableCS(&cv_ready, &cs_app_ready, INFINITE);
		}
		if (stop) {
			*p = 0;
			LeaveCriticalSection(&cs_app_ready);
			return;
		}
		LeaveCriticalSection(&cs_app_ready);
		EnterCriticalSection(&cs_top);
		*p = block_stack.front();
		block_stack.pop_front();
		LeaveCriticalSection(&cs_top);
	}

	void put_back(unsigned char *p) {
		EnterCriticalSection(&cs_app_ready);
		if (block_stack.size() <= NUM_PLUS_BLOCK) {
			WakeConditionVariable(&cv_app);
			SleepConditionVariableCS(&cv_ready, &cs_app_ready, INFINITE);
		}
		if (stop) {
			LeaveCriticalSection(&cs_app_ready);
			return;
		}
		LeaveCriticalSection(&cs_app_ready);
		EnterCriticalSection(&cs_top);
		block_stack.push_front(p);
		LeaveCriticalSection(&cs_top);
	}
};

unsigned __stdcall plus(void *prm) {
	WHYmempool_s *mp = (WHYmempool_s*)prm;
	while (1) {
		std::deque<unsigned char *>block_stack_b;
		unsigned char * p =
			new(std::nothrow) unsigned char[mp->size_block * NUM_PLUS_BLOCK];
		if (!p)
			break;
		mp->chunks.push_back(p);
		for (size_t i = 0; i < NUM_PLUS_BLOCK; ++i) {
			block_stack_b.push_back(p);
			p += mp->size_block;
		}

		EnterCriticalSection(&(mp->cs_app_ready));
		while (!mp->stop && mp->block_stack.size() > NUM_PLUS_BLOCK) {
			SleepConditionVariableCS(&mp->cv_app, &mp->cs_app_ready, INFINITE);
		}
		if (mp->stop) {
			LeaveCriticalSection(&mp->cs_app_ready);
			break;
		}
		mp->block_stack.insert(mp->block_stack.end(), block_stack_b.begin(),
			block_stack_b.end());
		LeaveCriticalSection(&(mp->cs_app_ready));
		WakeAllConditionVariable(&(mp->cv_ready));
	}
	return 0;
}
#endif
