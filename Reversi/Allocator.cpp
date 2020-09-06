/*==== include ====*/
#include<iostream>
#include"iMemoryAllocator.h"
#include"Allocator.h"

//ptr:使用可能な領域  size:使用可能な領域のサイズ
Allocator::Allocator(void* ptr, const size_t size)
{
	m_head = reinterpret_cast<Manager*> (ptr);
	m_head->is_used = false;
	m_head->next_ptr = nullptr;
	m_head->prev_ptr = nullptr;
	m_head->size = (size - sizeof(Manager));
	m_right_node = m_head;
}

//size:領域確保したいサイズの指定
void* Allocator::Alloc(const size_t size)
{
	if (size == 0) return nullptr;
	Manager* next_node = nullptr;
	void* ret_address = nullptr;

	//最も右端にある領域において確保できるかどうか確かめる
	Manager* node = m_right_node;
	if (!node->is_used && node->size >= size)
	{
		ret_address = (void*)((uintptr_t*)(node)+sizeof(Manager));
		uintptr_t* next_address = (uintptr_t*)ret_address + size;
		next_node = reinterpret_cast<Manager*>((void*)(next_address));
		node->is_used = true;
	}

	//確保できないなら、先頭アドレスに戻して、線形時間で探索する
	if (ret_address == nullptr)
	{
		m_right_node = m_head;
		while (node != nullptr)
		{
			if (!node->is_used && node->size >= size)
			{
				//ret_addressに確保可能なアドレスを代入する（Manager分のアドレスを加算する必要がある）
				ret_address = (void*)((uintptr_t*)(node)+sizeof(Manager));
				uintptr_t* next_address = (uintptr_t*)ret_address + size;
				next_node = reinterpret_cast<Manager*>((void*)(next_address));
				node->is_used = true;
				break;
			}
			node = node->next_ptr;
		}
	}

	//分割した次の領域にManager分のサイズより大きい領域が残っているなら使用可能な状態にする
	if (ret_address != nullptr)
	{
		size_t usable_size = node->size - size;
		if (usable_size > sizeof(Manager))
		{
			next_node->is_used = false;
			next_node->size = usable_size - sizeof(Manager);
			next_node->prev_ptr = node;
			next_node->next_ptr = node->next_ptr;
			node->size = size;
			if (node->next_ptr != nullptr)
			{
				node->next_ptr->prev_ptr = next_node;
			}
			node->next_ptr = next_node;

			//右端の領域を更新
			m_right_node = next_node;
		}
	}
	return ret_address;
}

//ptr:開放する領域の先頭アドレス
void Allocator::Free(void* ptr)
{
	if (ptr == nullptr) return;
	Manager* node = reinterpret_cast<Manager*>((void*)((uintptr_t*)ptr - sizeof(Manager)));
	if (!node->is_used) return;
	//nodeより後のアドレスに開放された領域があるなら、そこから空き領域をつなげていく
	node->is_used = false;

	//まず、nodeと連結された開放済みの領域のうち、もっともアドレスが大きいものを探索する
	while (node->next_ptr != nullptr && !node->next_ptr->is_used)
	{
		node = node->next_ptr;
	}
	Manager* tail_node = node->next_ptr;

	//その後、開放された領域が続く限り、これらを1つにまとめていく
	while (node->prev_ptr != nullptr && !node->prev_ptr->is_used)
	{
		node->prev_ptr->size += node->size + sizeof(Manager);
		node->prev_ptr->next_ptr = node->next_ptr;
		node = node->prev_ptr;
	}

	//領域をまとめた後に、使用済みの領域が存在するならば、順序関係を持たせる
	if (tail_node != nullptr)
	{
		tail_node->prev_ptr = node;
		node->next_ptr = tail_node;
		m_right_node = tail_node->next_ptr;
	}
	else
	{
		m_right_node = node->prev_ptr;
	}
}