/*==== include ====*/
#include<iostream>
#include"iMemoryAllocator.h"
#include"Allocator.h"

//ptr:�g�p�\�ȗ̈�  size:�g�p�\�ȗ̈�̃T�C�Y
Allocator::Allocator(void* ptr, const size_t size)
{
	m_head = reinterpret_cast<Manager*> (ptr);
	m_head->is_used = false;
	m_head->next_ptr = nullptr;
	m_head->prev_ptr = nullptr;
	m_head->size = (size - sizeof(Manager));
	m_right_node = m_head;
}

//size:�̈�m�ۂ������T�C�Y�̎w��
void* Allocator::Alloc(const size_t size)
{
	if (size == 0) return nullptr;
	Manager* next_node = nullptr;
	void* ret_address = nullptr;

	//�ł��E�[�ɂ���̈�ɂ����Ċm�ۂł��邩�ǂ����m���߂�
	Manager* node = m_right_node;
	if (!node->is_used && node->size >= size)
	{
		ret_address = (void*)((uintptr_t*)(node)+sizeof(Manager));
		uintptr_t* next_address = (uintptr_t*)ret_address + size;
		next_node = reinterpret_cast<Manager*>((void*)(next_address));
		node->is_used = true;
	}

	//�m�ۂł��Ȃ��Ȃ�A�擪�A�h���X�ɖ߂��āA���`���ԂŒT������
	if (ret_address == nullptr)
	{
		m_right_node = m_head;
		while (node != nullptr)
		{
			if (!node->is_used && node->size >= size)
			{
				//ret_address�Ɋm�ۉ\�ȃA�h���X��������iManager���̃A�h���X�����Z����K�v������j
				ret_address = (void*)((uintptr_t*)(node)+sizeof(Manager));
				uintptr_t* next_address = (uintptr_t*)ret_address + size;
				next_node = reinterpret_cast<Manager*>((void*)(next_address));
				node->is_used = true;
				break;
			}
			node = node->next_ptr;
		}
	}

	//�����������̗̈��Manager���̃T�C�Y���傫���̈悪�c���Ă���Ȃ�g�p�\�ȏ�Ԃɂ���
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

			//�E�[�̗̈���X�V
			m_right_node = next_node;
		}
	}
	return ret_address;
}

//ptr:�J������̈�̐擪�A�h���X
void Allocator::Free(void* ptr)
{
	if (ptr == nullptr) return;
	Manager* node = reinterpret_cast<Manager*>((void*)((uintptr_t*)ptr - sizeof(Manager)));
	if (!node->is_used) return;
	//node����̃A�h���X�ɊJ�����ꂽ�̈悪����Ȃ�A��������󂫗̈���Ȃ��Ă���
	node->is_used = false;

	//�܂��Anode�ƘA�����ꂽ�J���ς݂̗̈�̂����A�����Ƃ��A�h���X���傫�����̂�T������
	while (node->next_ptr != nullptr && !node->next_ptr->is_used)
	{
		node = node->next_ptr;
	}
	Manager* tail_node = node->next_ptr;

	//���̌�A�J�����ꂽ�̈悪��������A������1�ɂ܂Ƃ߂Ă���
	while (node->prev_ptr != nullptr && !node->prev_ptr->is_used)
	{
		node->prev_ptr->size += node->size + sizeof(Manager);
		node->prev_ptr->next_ptr = node->next_ptr;
		node = node->prev_ptr;
	}

	//�̈���܂Ƃ߂���ɁA�g�p�ς݂̗̈悪���݂���Ȃ�΁A�����֌W����������
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