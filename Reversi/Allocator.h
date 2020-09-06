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
	//�g�p�ł��郁�����̐擪�A�h���X��ێ����Ă���
	Manager* m_head;
	Manager* m_right_node;
};
