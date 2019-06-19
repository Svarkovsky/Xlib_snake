// http://zonakoda.ru/dek-na-osnove-massiva.html
// Автор: Сергей Валентинович Фролов
// zonakoda@gmail.com

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef int el_type; //тип элементов дека  -  это просто псевдоним int

typedef struct
{
	int head;			//индекс головы
	int tail;			//индекс хвоста
	int size;			//размер массива elements
	int count;			//количество элементов в деке
	el_type elements[]; //массив для хранения элементов
} deq;

typedef struct
{
	deq *d;
	int index;
} iterator;

deq *create_deq(int size);				  //создать дек
deq *push_back(deq *d, el_type elem);	 //вставить в конец
el_type pop_back(deq *d);				  //удалить из конца
el_type peek_back(deq *d);				  //получить из конца не удаляя
deq *push_front(deq *d, el_type elem);	//вставить в начало
el_type pop_front(deq *d);				  //удалить из начала
el_type peek_front(deq *d);				  //получить из начала не удаляя
bool is_empty(deq *d);					  //true, если дек пустой
bool is_full(deq *d);					  //true, если дек полный
iterator get_iterator(deq *d, bool head); //получить итератор
el_type next(iterator *it);				  //получить следующий элемент
el_type prev(iterator *it);				  //получить предыдущий элемент
deq *resize(deq *d, int new_size);		  //изменить размер дека

void print_deq_ht(deq *d); // вывести все элементы дека от головы к хвосту
void print_deq_th(deq *d); // вывести все элементы дека от хвоста к голове
void dell_deq(deq *d);	 // извлечение всех элементов из дека и оказ от использования free()

int in_count_deq(deq *d); // вернуть число элементов



