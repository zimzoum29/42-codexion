/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 18:11:17 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 18:24:52 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_request(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	heapify_up(t_heap *heap, int index, int scheduler)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_is_before(heap->data[index],
				heap->data[parent], scheduler))
			break ;
		swap_request(&heap->data[index], &heap->data[parent]);
		index = parent;
	}
}

static void	heapify_down(t_heap *heap, int index, int scheduler)
{
	int	left;
	int	right;
	int	best;

	while (1)
	{
		left = index * 2 + 1;
		right = index * 2 + 2;
		best = index;
		if (left < heap->size
			&& request_is_before(heap->data[left],
				heap->data[best], scheduler))
			best = left;
		if (right < heap->size
			&& request_is_before(heap->data[right],
				heap->data[best], scheduler))
			best = right;
		if (best == index)
			break ;
		swap_request(&heap->data[index], &heap->data[best]);
		index = best;
	}
}

int	heap_push(t_heap *heap, t_request request, int scheduler)
{
	if (heap->size >= heap->capacity)
		return (0);
	heap->data[heap->size] = request;
	heapify_up(heap, heap->size, scheduler);
	heap->size++;
	return (1);
}

t_request	*heap_top(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (&heap->data[0]);
}

void	heap_pop(t_heap *heap, int scheduler)
{
	if (heap->size == 0)
		return ;
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	heapify_down(heap, 0, scheduler);
}