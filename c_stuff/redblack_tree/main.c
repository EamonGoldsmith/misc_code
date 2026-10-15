#include <stdio.h>
#include <string.h>

#define REDBLACK_IMPLEMENTATION
#include "rbtree.h"

// create a tree node type
typedef struct {
	RB_Node node;
	unsigned long key;
	const char *txt;
} HashPair;

int hash_insert(RB_Tree *tree, HashPair *data)
{
	RB_Node **new = &(tree->root), *parent = NULL;

    /* figure out where to put new node */
    while (*new) {
        HashPair *this = container_of(*new, HashPair, node);
		if (!this) { parent = *new; break; }
		int result;
		if (data->txt == NULL || this->txt == NULL) {
			result = 1;
		} else {
			result = strcmp(data->txt, this->txt);
		}

        parent = *new;

        if (result < 0) {
            new = &((*new)->left);
		} else if (result > 0) {
            new = &((*new)->right);
		} else {
			return 0;
		}
    }

	/* put new node */
    rb_insert(tree, &data->node, parent,
		*new == parent->left ? RB_LEFT : RB_RIGHT
	);

    return 1;
}

int main(int argc, char **argv)
{
	// initialise
	HashPair hash_table[4] = {0};
	RB_Tree hash_tree;
	hash_tree.root = &hash_table[0].node;

	// link some values
	hash_table[1].txt = "Hello, world";
	hash_insert(&hash_tree, &hash_table[1]);

	hash_table[2].txt = "Goodbye, world";
	hash_insert(&hash_tree, &hash_table[2]);

	// print all
	for (RB_Node *node = hash_tree.root; node != NULL; node = rb_next(node)) {
		HashPair *pair = container_of(node, HashPair, node);
		printf("txt: %s\n", pair->txt);
	}

	return 0;
}
