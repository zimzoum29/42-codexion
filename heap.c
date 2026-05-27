/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 18:11:17 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/27 08:09:32 by tigondra         ###   ########.fr       */
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

void	heap_remove_at(t_heap *heap, int index, int scheduler)
{
	if (index < 0 || index >= heap->size)
		return ;
	heap->data[index] = heap->data[heap->size - 1];
	heap->size--;
	heapify_down(heap, index, scheduler);
	heapify_up(heap, index, scheduler);
}
