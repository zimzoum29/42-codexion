/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 08:34:17 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 08:48:40 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heap_push(t_heap *heap, t_request req, int scheduler)
{
	if (heap->size >= heap->capacity)
		return ;
	heap->data[heap->size] = req;
	heapify_up(heap, heap->size, scheduler);
	heap->size++;
}

t_request	heap_top(t_heap *heap)
{
	return (heap->data[0]);
}

void	heap_pop(t_heap *heap, int scheduler)
{
	if (heap->size == 0)
		return ;
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	heapify_down(heap, 0, scheduler);
}

void	heapify_down(t_heap *heap, int index, int scheduler)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		smallest = index;
		if (left < heap->size
			&& request_is_before(heap->data[left],
				heap->data[smallest], scheduler))
			smallest = left;
		if (right < heap->size
			&& request_is_before(heap->data[right],
				heap->data[smallest], scheduler))
			smallest = right;
		if (smallest == index)
			break ;
		swap_requests(&heap->data[index], &heap->data[smallest]);
		index = smallest;
	}
}

void	heapify_up(t_heap *heap, int index, int scheduler)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_is_before(heap->data[index],
				heap->data[parent], scheduler))
			break ;
		swap_requests(&heap->data[index], &heap->data[parent]);
		index = parent;
	}
}
