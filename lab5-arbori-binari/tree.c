/*
*	Created by Nan Mihai on 28.03.2021
*	Copyright (c) 2021 Nan Mihai. All rights reserved.
*	Laborator 5 - Structuri de date
*	Facultatea de Automatica si Calculatoare
*	Anul Universitar 2020-2021, Seria CD
*/
#include "tree.h"

/*
*	Funcție care creează un arbore cu un singur nod
*/
Tree createTree(Item value) {
	Tree root = malloc(sizeof(TreeNode));
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
void init(Tree *root, Item value) {
	
	// Allocate memory.
	*root = malloc(sizeof(TreeNode));
	// Set the value field.
	(*root)->value = value;
	// Set left and right fields.
	(*root)->left = NULL;
	(*root)->right = NULL;
}

/*
*	Funcție care inserează o valoare într-un arbore binar, respectând
* proprietățile unui arbore binar de căutare
*/
Tree insert(Tree root, Item value) {
	// Base case: the root is NULL, create a new node and return it as the new root.
	if (root == NULL) {
		root = createTree(value);
		return root;
	}

	// If the value is smaller than the root's value, recursively insert it in the left subtree.
	if (value < root->value) {
		root->left = insert(root->left, value);
	}
	// If the value is greater than the root's value, recursively insert it in the right subtree.
	else if (value > root->value) {
		root->right = insert(root->right, value);
	}

	// Return the root of the tree.
	return root;
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în postordine
*/
void printPostorder(Tree root) {
   if (root == NULL) {
        return;
    }

    // Recursively traverse the left subtree.
    printPostorder(root->left);

    // Recursively traverse the right subtree.
    printPostorder(root->right);

    // Print the root node's value.
    printf("%d ", root->value);
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în preordine
*/
void printPreorder(Tree root) {
    if (root == NULL) {
        return;
    }

	// Print the root node's value.
    printf("%d ", root->value);
	
	// Recursively traverse the left subtree.
    printPreorder(root->left);

	// Recursively traverse the right subtree.
    printPreorder(root->right);
}

/*
*	Funcție care afișează nodurile folosind parcurgerea în inordine
*/
void printInorder(Tree root) {
	if(root == NULL) {
		return;
	}
		
	// Recursively traverse the left subtree.
	printInorder(root->left);
		
	// Print the root node's value.
	printf("%d ", root->value);
		
	// Recursively traverse the right subtree.
	printInorder(root->right);
}

/*
*	Funcție care dealocă întreaga memorie alocată pentru un arbore binar
*		- root va pointa către NULL după ce se va apela funcția
*/
void freeTree(Tree *root) {
    if (*root != NULL) {
        freeTree(&((*root)->left));
        freeTree(&((*root)->right));
        free(*root);
        *root = NULL;
    }
}


/*
*	Funcție care determină numărul de noduri dintr-un arbore binar
*/
int size(Tree root) {
    if (root == NULL) {
        return 0;
    }

    int leftSize = size(root->left);
    int rightSize = size(root->right);
    return leftSize + rightSize + 1;
}

/*
*	Funcție care returnează adâncimea maximă a arborelui
*/
int maxDepth(Tree root) {
    if (root == NULL)
		return -1;

	int leftDepth, rightDepth;

	leftDepth = maxDepth(root->left);
	rightDepth = maxDepth(root->right);

	if (leftDepth > rightDepth)
		return leftDepth + 1;
	else
		return rightDepth + 1;
}

/*
*	Funcție care construiește oglinditul unui arbore binar
*/
void mirror(Tree root) {
    if (root == NULL) {
        return;
    }

    Tree temp = root->left;

    root->left = root->right;
    root->right = temp;

    mirror(root->left);
    mirror(root->right);
}

/*
*	Funcție care verifică dacă doi arbori binari sunt identici
*/
int sameTree(Tree root1, Tree root2) {
	
	// If both trees are empty, they are identical.
	if (root1 == NULL && root2 == NULL) {
		return 1;
	}

	// If one of the trees is empty and the other isn't, they are not identical.
	if (root1 == NULL || root2 == NULL) {
		return 0;
	}

	// If the values of the roots are different, the trees are not identical.
	if (root1->value != root2->value) {
		return 0;
	}

	// Recursively check if the left and right subtrees are identical.
	return sameTree(root1->left, root2->left) && sameTree(root1->right, root2->right);
}
