/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 08:09:39 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/27 08:09:52 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
