#ifndef REDBLACK_TREE_H_
#define REDBLACK_TREE_H_

#include <stddef.h>

/*
	Red-Black tree rules:

	1. every node is either red or black.
	2. all null nodes are considered black.
	3. a red node does not have a red child.
	4. every path from a given node to any of its leaf nodes (that is, to any 
	descendant null node) goes through the same number of black nodes.

	if a node N has exactly one child, the child must be red. 
	if the child were black, its leaves would sit at a different black depth 
	than N's null node (which is considered black by rule 2), violating 
	requirement 4.
*/

#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);						\
	((type *)(__mptr - offsetof(type, member))); })

#define rb_direction(node) (node->right ? RB_RIGHT : RB_LEFT)

typedef enum {
	RB_RED,
	RB_BLACK
} RB_Colour;

typedef struct RB_Node {
	RB_Colour colour;
	struct RB_Node *parent;
	union {
		struct {
			struct RB_Node *left;
			struct RB_Node *right;
		};
		struct RB_Node *child[2];
	};
} RB_Node;

typedef struct {
	RB_Node *root;
} RB_Tree;

typedef enum {
	RB_LEFT,
	RB_RIGHT
} RB_Direction;

typedef int (*rb_cmp_func)(const void *key, RB_Node *node);
RB_Node* rb_rotate_subtree(RB_Tree *tree, RB_Node *node, RB_Direction dir);
void rb_insert(RB_Tree *tree, RB_Node *node, RB_Node *parent, RB_Direction dir);
void rb_erase(RB_Tree *tree, RB_Node *victim);
RB_Node* rb_find(const void *key, RB_Tree *tree, rb_cmp_func compare);
RB_Node* rb_next(RB_Node *node);
RB_Node* rb_prev(RB_Node *node);

#ifdef REDBLACK_IMPLEMENTATION

RB_Node* rb_rotate_subtree(RB_Tree *tree, RB_Node *node, RB_Direction dir)
{
	RB_Node 
		*node_parent = node->parent,
		*new_root = node->child[1 - dir],
		*new_child = new_root->child[dir];

	node->child[1 - dir] = new_child;

	if (new_child) {
		new_child->parent = node;
	}

	new_root->child[dir] = node;
	node->parent = new_root;

	if (node_parent) {
		node_parent->child[node == node_parent->right] = new_root;
	} else {
		tree->root = new_root;
	}

	return new_root;
}

void rb_insert(
	RB_Tree *tree, RB_Node *node, RB_Node *parent, RB_Direction dir
) {
	node->colour = RB_RED;
	node->parent = parent;

	if (!parent) {
		tree->root = node;
		return;
	}

	parent->child[dir] = node;

	// rebalance tree
	do {
		if (parent->colour == RB_BLACK) { return; }

		RB_Node *grandparent = parent->parent;
		if (!grandparent) {
			parent->colour = RB_BLACK;
			return;
		}

		dir = rb_direction(parent);
		RB_Node *uncle = grandparent->child[1 - dir];
		if (!uncle || uncle->colour == RB_BLACK) {
			if (node == parent->child[1 - dir]) {
				RB_Node *tmp = node;
				node = parent;
				parent = tmp;
				rb_rotate_subtree(tree, parent, dir);
			}

			rb_rotate_subtree(tree, grandparent, 1 - dir);
			parent->colour = RB_BLACK;
			grandparent->colour = RB_RED;
			return;
		}

		parent->colour = RB_BLACK;
		uncle->colour = RB_BLACK;
		grandparent->colour = RB_RED;
		node = grandparent;
	} while ((parent = node->parent));
}

void rb_erase(RB_Tree *tree, RB_Node *victim)
{
	RB_Node* parent = victim->parent;

	RB_Node* sibling;
	RB_Node* close_nephew;
	RB_Node* distant_nephew;

	RB_Direction dir = rb_direction(victim);

	parent->child[dir] = NULL;
	goto start_balance;

	do {
		dir = rb_direction(victim);
start_balance:
		sibling = parent->child[1 - dir];
		distant_nephew = sibling->child[1 - dir];
		close_nephew = sibling->child[dir];
		if (sibling->colour == RB_RED) {
			// Case #3
			rb_rotate_subtree(tree, parent, dir);
			parent->colour = RB_RED;
			sibling->colour = RB_BLACK;
			sibling = close_nephew;

			distant_nephew = sibling->child[1 - dir];
			if (distant_nephew && distant_nephew->colour == RB_RED) {
				goto case_6;
            }
			close_nephew = sibling->child[dir];
			if (close_nephew && close_nephew->colour == RB_RED) {
				goto case_5;
            }

			// Case #4
			sibling->colour = RB_RED;
			parent->colour = RB_BLACK;
			return;
		}

		if (distant_nephew && distant_nephew->colour == RB_RED) {
			goto case_6;
        }

		if (close_nephew && close_nephew->colour == RB_RED) {
			goto case_5;
        }

		if (parent->colour == RB_RED) {
			// Case #4
			sibling->colour = RB_RED;
			parent->colour = RB_BLACK;
			return;
		}

		// Case #2
		sibling->colour = RB_RED;
		victim = parent;

	} while ((parent = victim->parent));
	
	// Case #1
    return;
    
case_5:

	rb_rotate_subtree(tree, sibling, 1 - dir);
	sibling->colour = RB_RED;
	close_nephew->colour = RB_BLACK;
	distant_nephew = sibling;
	sibling = close_nephew;

case_6:

	rb_rotate_subtree(tree, parent, dir);
	sibling->colour = parent->colour;
	parent->colour = RB_BLACK;
	distant_nephew->colour = RB_BLACK;
	return;
}

RB_Node* rb_find(const void *key, RB_Tree *tree, rb_cmp_func compare)
{
	RB_Node *node = tree->root;
	while (node) {
		int c = compare(key, node);

		if (c < 0) {
			node = node->left;
		} else if (c > 0) {
			node = node->right;
		} else {
			return node;
		}
	}
	return NULL;
}

RB_Node* rb_next(RB_Node *node)
{
	RB_Node *parent;

	if (node == node->parent)
		return NULL;

	if (node->right) {
		node = node->right;
		while (node->left) {
			node = node->left;
		}
		return node;
	}

	while ((parent = node->parent) && node == parent->right)
		node = parent;

	return parent;
}

RB_Node* rb_prev(RB_Node *node)
{
	RB_Node *parent;

	if (node == node->parent)
		return NULL;

	if (node->left) {
		node = node->left;
		while (node->right) {
			node = node->right;
		}
		return node;
	}

	while ((parent = node->parent) && node == parent->left)
		node = parent;

	return parent;
}

#endif // REDBLACK_IMPLEMENTATION

#endif // REDBLACK_TREE_H_
