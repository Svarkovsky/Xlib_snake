// http://zonakoda.ru/dek-na-osnove-massiva.html
// Автор: Сергей Валентинович Фролов
// zonakoda@gmail.com
#include <stdio.h> // для printf
#include "m_deq.h"

//создать дек
deq *create_deq(int size)
{
	if (size <= 0)
		return 0;
	deq *d = malloc(sizeof(deq) + size * sizeof(el_type));
	if (d)
	{
		d->size = size;
		d->head = d->tail = -1;
		d->count = 0;
	}
	return d;
}

//вставить в конец
deq *push_back(deq *d, el_type elem)
{
	if (d->count)
	{
		if (++(d->tail) == d->size)
			d->tail = 0;
	}
	else
		d->tail = d->head = 0;
	d->elements[d->tail] = elem;
	d->count++;
	return d;
}

//удалить из конца
el_type pop_back(deq *d)
{
	int index = d->tail;
	if (d->count == 1)
		d->tail = d->head = -1;
	else if (--(d->tail) < 0)
		d->tail = d->size - 1;
	d->count--;
	return d->elements[index];
}

//получить из конца не удаляя
el_type peek_back(deq *d)
{
	return d->elements[d->tail];
}

//вставить в начало
deq *push_front(deq *d, el_type elem)
{
	if (d->count)
	{
		if (--(d->head) < 0)
			d->head = d->size - 1;
	}
	else
		d->tail = d->head = 0;
	d->elements[d->head] = elem;
	d->count++;
	return d;
}

//удалить из начала
el_type pop_front(deq *d)
{
	int index = d->head;
	if (d->count == 1)
		d->tail = d->head = -1;
	else if (++(d->head) == d->size)
		d->head = 0;
	d->count--;
	return d->elements[index];
}

//получить из начала не удаляя
el_type peek_front(deq *d)
{
	return d->elements[d->head];
}

//true, если дек пустой
bool is_empty(deq *d)
{
	return !d->count;
}

//true, если дек полный
bool is_full(deq *d)
{
	return d->count == d->size;
}

//получить итератор
iterator get_iterator(deq *d, bool head)
{
	iterator it;
	it.d = d;
	it.index = head ? d->head : d->tail;
	return it;
}

//получить текущий элемент и сделать текущим следующий
el_type next(iterator *it)
{
	int index = it->index;
	if (index == it->d->tail)
		it->index = -1;
	else if (++(it->index) == it->d->size)
		it->index = 0;
	return it->d->elements[index];
}

//получить текущий элемент и сделать текущим предыдущий
el_type prev(iterator *it)
{
	int index = it->index;
	if (index == it->d->head)
		it->index = -1;
	else if (--(it->index) < 0)
		it->index = it->d->size - 1;
	return it->d->elements[index];
}

//изменить размер дека
deq *resize(deq *d, int new_size)
{
	if (new_size <= 0)
		return 0;
	if (new_size == d->size)
		return d;
	deq *nd = malloc(sizeof(deq) + new_size * sizeof(el_type));
	if (!nd)
		return 0;
	nd->size = new_size;
	nd->count = new_size > d->count ? d->count : new_size;
	nd->head = 0;
	nd->tail = nd->count - 1;
	int r = d->size - d->head;
	if (nd->count <= r)
		memcpy(nd->elements, d->elements + d->head, nd->count * sizeof(el_type));
	else
	{
		memcpy(nd->elements, d->elements + d->head, r * sizeof(el_type));
		memcpy(nd->elements + d->size - d->head, d->elements, (nd->count - r) * sizeof(el_type));
	}
	free(d);
	return nd;
}

// -- // вывести все элементы дека от головы к хвосту
void print_deq_ht(deq *d)
{
	if (!is_empty(d))
	{
		iterator it = get_iterator(d, true);
		while (it.index != -1)
		{
			printf("%d ", next(&it));
		}
		printf("\n");
	}
}
// -- // вывести все элементы дека от хвоста к голове
void print_deq_th(deq *d)
{
	if (!is_empty(d))
	{
		iterator it = get_iterator(d, false);
		while (it.index != -1)
		{
			printf("%d ", prev(&it));
		}
		printf("\n");
	}
}
// -- // извлечение всех элементов из дека и оказ от использования free()
void dell_deq(deq *d)
{ // "артистично" извлекаем элементы
	while (!is_empty(d))
	{
		if (rand() % 2)
			pop_back(d);
		else
			pop_front(d);
	}
	free(d);
	d = NULL;
}

// -- // вернуть число элементов
int in_count_deq(deq *d)
{ int i;
	if (!is_empty(d))
	{
		iterator it = get_iterator(d, true);
		for (i = 0; it.index != -1; ++i)
		{
			next(&it);
		}
	}
	return i;
}

/*
    if (!is_empty(deq_x) && !is_empty(deq_y))
    {
        iterator it_x = get_iterator(deq_x, false);
        iterator it_y = get_iterator(deq_y, false);
        while (it_x.index != -1 && it_y.index != -1)
        {
            printf("[x: %3d y: %3d]", prev(&it_x), prev(&it_y));
        }
        printf("\n");
    }


    if (!is_empty(deq_x) && !is_empty(deq_y))
    {
        iterator it_x = get_iterator(deq_x, true);
        iterator it_y = get_iterator(deq_y, true);
        while (it_x.index != -1 && it_y.index != -1)
        {
            printf("[x: %3d y: %3d] \n", next(&it_x), next(&it_y));
        }
        printf("\n");
    }


*/ 
