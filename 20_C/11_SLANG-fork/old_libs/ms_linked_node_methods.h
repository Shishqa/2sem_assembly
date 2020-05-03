/*
 * ms_linked_node_methods.h
 *
 *  Created on: 29 окт. 2019 г.
 *      Author: shishqa
 */

#ifndef MS_LINKED_NODE_METHODS_H_
#define MS_LINKED_NODE_METHODS_H_

template <typename elem_t>
bool Node<elem_t>::init(const elem_t val, const int next, const int prev)
{
	this->val = val;
	this->next = next;
	this->prev = prev;

	return (true);
}

template <typename elem_t>
bool Node<elem_t>::clear(const elem_t poison_val, const int empty)
{
	this->val = poison_val;
	this->next = empty;
	this->prev = EMPTY_MARKER;

	return (true);
}

#endif /* MS_LINKED_NODE_METHODS_H_ */
