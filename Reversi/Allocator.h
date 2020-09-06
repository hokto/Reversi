#pragma once

/*==== include ====*/
#include"iMemoryAllocator.h"

struct Manager
{
	bool is_used;
	size_t size;
	Manager* next_ptr;
	Manager* prev_ptr;
};
class Allocator : public iMemoryAllocator
{
public:
	Allocator(void* ptr, const size_t size);
	~Allocator() {}
	void* Alloc(const size_t size);
	void Free(void* ptr);
private:
	//使用できるメモリの先頭アドレスを保持しておく
	Manager* m_head;
	Manager* m_right_node;
};
