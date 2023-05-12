/*
*	Created by Nan Mihai on 05.05.2021
*	Copyright (c) 2020 Nan Mihai. All rights reserved.
*	Laborator 6 - Structuri de date
*	Grupa 314CD
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
Tree createTree(Tree parent, Item value) {
	Tree root = malloc(sizeof(TreeNode));
	root->parent = parent;
	root->value = value;
	root->left = NULL;
	root->right = NULL;
	return root;
}

/*
*	Funcție care inițializează un nod de arbore
*		- îi alocă memorie
*		- îi setează câmpul valoare
*		- setează left și right să pointeze către NULL
*/
void init(Tree *root, Tree parent, Item value) {

    // Allocate memory.
    *root = malloc(sizeof(TreeNode));

    // Set the value field.
    (*root)->value = value;

    // Set left and right fields.
    (*root)->left = NULL;
    (*root)->right = NULL;
	
    // Set parent field.
    (*root)->parent = parent;
}

/*
*	Funcție care inserează o valoare într-un arbore binar, respectând
* proprietățile unui arbore binar de căutare
*/
Tree insertHelper(Tree root, Tree parent, Item value) {
	if (root == NULL)
		return createTree(parent, value);

	if (root->value == value) {
		return root;
	}

	if (value < root->value)
		root->left = insertHelper(root->left, root, value);
	else
		root->right = insertHelper(root->right, root, value);

	return root;
}

Tree insert(Tree root, Item value) {
	return insertHelper(root, NULL, value);
}


/*
*	Funcție care verifică dacă un arbore este vid sau nu
*		- întoarce 1 dacă arborele este vid
*		- întoarce 0 dacă arborele nu este vid
*/
int isEmpty(Tree root) {
    return (root == NULL);
}

/*
*	Funcție care verifică dacă o valoare există sau nu în arbore
*		- 1 dacă value există în arbore
*		- 0 dacă value nu există în arbore
*/
int contains(Tree tree, Item value) {
	if (tree == NULL)
		return 0;

	if (tree->value == value)
		return 1;

	if (value < tree->value)
		return contains(tree->left, value);
	else
		return contains(tree->right, value);
}

/*
*	Funcție care determină elementul minim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree minimum(Tree tree) {
    if (tree == NULL) {
        return NULL;
    }

    while (tree->left != NULL) {
        tree = tree->left;
    }

    return tree;
}

/*
*	Funcție care determină elementul maxim dintr-un arbore binar de căutare
*		- NULL pentru arbore vid
*/
Tree maximum(Tree tree) {
	if (tree == NULL) {
        return NULL;
    }

    while (tree->right != NULL) {
        tree = tree->right;
    }

    return tree;
}

/*
*	Funcție care determină succesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree successor(Tree root, Item value) {

    // Find the node containing the given value.
    Tree current = root;

    while (current != NULL && current->value != value) {
        if (value < current->value)
            current = current->left;
        else
            current = current->right;
    }

    if (current == NULL)
        return NULL;

    // If the right subtree is not NULL, then the successor is the
    // leftmost node in the right subtree.
    if (current->right != NULL) {
        current = current->right;
        while (current->left != NULL) {
            current = current->left;
		}

        return current;
    }

    // Otherwise, the successor is the deepest ancestor of the node
    // for which the node is in the left subtree.
    Tree parent = current->parent;
    while (parent != NULL && current == parent->right) {
        current = parent;
        parent = parent->parent;
    }
	
    return parent;
}

/*
*	Funcție care determină predecesorul în inordine pentru nodul care conține
* valoarea value.
*/
Tree predecessor(Tree root, Item value) {

    // Find the node with the given value.
    Tree node = root;
    while (node != NULL && node->value != value) {
        if (value < node->value) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (node == NULL) {
        // The value is not in the tree.
        return NULL;
    }

    // If the node has a left subtree, the predecessor is the maximum
    // value in that subtree.
    if (node->left != NULL) {
        return maximum(node->left);
    }

    // Otherwise, the predecessor is the first ancestor that is a
    // right child of its parent (or NULL if there is no such ancestor)
    Tree parent = node->parent;
    while (parent != NULL && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void destroyTree(Tree *root) {
    if (*root == NULL) {
        return;
    }

    destroyTree(&(*root)->left);
    destroyTree(&(*root)->right);

    free(*root);
    *root = NULL;
}

/*
*	Funcție care șterge un nod cu o anumită valoare din arbore
*/
Tree delete(Tree root, Item value) {
    if (root == NULL) {
        return root;
    }

    if (value < root->value) {
        root->left = delete(root->left, value);
    }
	else if (value > root->value) {
        root->right = delete(root->right, value);
    }
	else {

        // Case 1: No child or only one child
        if (root->left == NULL) {
            Tree temp = root->right;
            free(root);
            return temp;
        }
		else if (root->right == NULL) {
            Tree temp = root->left;
            free(root);
            return temp;
        }

        // Case 2: Two children
        Tree temp = minimum(root->right);
        root->value = temp->value;
        root->right = delete(root->right, temp->value);
    }
    return root;
}

/*
*	Funcție care determină cel mai apropiat strămoș comun pentru
* două noduri având cheile value1 și value2
*/
Tree lowestCommonAncestor(Tree root, Item value1, Item value2) {
    if (root == NULL) {
        return NULL;
    }

    if (root->value == value1 || root->value == value2) {
        return root;
    }

    Tree leftLCA = lowestCommonAncestor(root->left, value1, value2);
    Tree rightLCA = lowestCommonAncestor(root->right, value1, value2);

    if (leftLCA != NULL && rightLCA != NULL) {
        return root;
    }

    if (leftLCA != NULL) {
        return leftLCA;
    }

    if (rightLCA != NULL) {
        return rightLCA;
    }
	
    return NULL;
}
