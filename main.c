/* COP 3502C Assignment 5
* This program is written by: Ruba Ismail
* UCF ID: 5186181
* Date: 3/29/2025
* Class: COP 3205C, Professor Tanvir
* Purpose: implementing a BST that handles strings to track tickets and their owners
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 31

struct tree_node* insert(struct tree_node *root, char *name, int tickets, int *depth);
struct tree_node* search(struct tree_node *root, char *name, int *depth);
struct tree_node* delete(struct tree_node *root, char *name);
struct tree_node* createNode(char *name, int tickets);
struct tree_node* findNode(struct tree_node *root, char *name);
int isLeaf(struct tree_node *root);
int hasOnlyLeftChild(struct tree_node *root);
int hasOnlyRightChild(struct tree_node *root);
struct tree_node *parent(struct tree_node *root, struct tree_node *node);
struct tree_node* minValNode(struct tree_node *root);
struct tree_node* maxValNode(struct tree_node *root);
int sumTickets(struct tree_node *root);
int numNodes(struct tree_node *root);
double meanTickets(struct tree_node *root);
int countBefore(struct tree_node *root, char *name);
void freeTree(struct tree_node *root);


struct tree_node {
    int numTickets;
    char name[MAX_LENGTH];
    struct tree_node *left;
    struct tree_node *right;
};

struct tree_node* createNode(char *name, int tickets)
{
    struct tree_node *temp = malloc(sizeof(struct tree_node));
    temp->numTickets = tickets;
    strcpy(temp->name, name);
    temp->left = NULL;
    temp->right = NULL;

    return temp;
}

// inserts a fan into to the BST based on alphabetical order of name
struct tree_node* insert(struct tree_node *root, char *name, int tickets, int *depth)
{
    if (root == NULL) {
        *depth = 0;
        return createNode(name, tickets);
    }

    else
    {
        int cmp = strcmp(name, root->name);

        // fan already exists, we updaye ticket count
        if (cmp == 0) {
            root->numTickets += tickets;
            return root;
        }

        (*depth)++;

        if (cmp > 0)
        {
            if(root->right != NULL)
                root->right =  insert(root->right, name, tickets, depth);
            else
                root->right = createNode(name, tickets);
        }

        else   // (new node's name is smaller alphabetically)
        {
            if(root->left != NULL)
                root->left =  insert(root->left, name, tickets, depth);
            else
                root->left = createNode(name, tickets);
        }

        return root;
    }
}

// searches the BST for a fan by name, retuns the node if found
struct tree_node* search(struct tree_node *root, char *name, int *depth)
{
    if (root == NULL)
        return NULL;

    int cmp = strcmp(name, root->name);
    if (cmp == 0)
        return root;

    (*depth)++;
    if (cmp > 0)
        return search(root->right, name, depth);
    else
        return search(root->left, name, depth);
}


// deletes a node by name from the BST
struct tree_node* delete(struct tree_node *root, char *name)
{
    struct tree_node *delNode, *par, *tempNode;

    delNode = findNode(root, name);
    if (delNode == NULL)
        return root;

    par = parent(root, delNode);

    // case 1: node deleting is a leaf node
    if (isLeaf(delNode))
    {
        if(par == NULL) {  // the leaf node is the root, no parent
            free(root);
            return NULL;
        }

        if(strcmp(name, par->name) < 0) {
            free(par->left);
            par->left = NULL;
        }

        else {
            free(par->right);
            par->right = NULL;
        }

        return root;
    }

    // Case 2: Node has only one child.
    else if (hasOnlyLeftChild(delNode))   // case 2.1: node has a left child
    {
        if (par == NULL) {
            tempNode = delNode->left;
            free(delNode);
            return tempNode;
        }
        if (strcmp(name, par->name) < 0) {  // delNode is left child.
            tempNode = par->left;
            par->left = par->left->left;
            free(tempNode);
        }
        else {  // delNode is right child.
            tempNode = par->right;
            par->right = par->right->left;
            free(tempNode);
        }

        return root;
    }

    else if (hasOnlyRightChild(delNode))   // case 2.2: node has a right child
    {
        if (par == NULL) {
            tempNode = delNode->right;
            free(delNode);
            return tempNode;
        }

        if (strcmp(name, par->name) < 0) {  // delNode is left child
            tempNode = par->left;
            par->left = par->left->right;
            free(tempNode);
        }

        else {                              // delNode is right child
            tempNode = par->right;
            par->right = par->right->right;
            free(tempNode);
        }

        return root;
    }

    // Case 3: node has 2 children
    struct tree_node *newDelNode = maxValNode(delNode->left);

    int newTickets = newDelNode->numTickets;
    char newName[MAX_LENGTH];
    strcpy(newName, newDelNode->name);
    root = delete(root, newName);   // delete the node so we can move its data to the position of the old node

    // update the node we intend to delete with the data of the replacdement node
    strcpy(delNode->name, newName);
    delNode->numTickets = newTickets;

    return root;
}

// finds the height of the BST, empty tree has height -1
int height(struct tree_node *root) {
    if (root == NULL)
        return -1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    if (leftHeight > rightHeight)
        return leftHeight + 1;
    else
        return rightHeight + 1;
}

// sums the tickets in all nodes
int sumTickets(struct tree_node *root) {
    if (root == NULL)
        return 0;
    return root->numTickets + sumTickets(root->left) + sumTickets(root->right);
}

// calculate the mean tickets per fan
double meanTickets(struct tree_node *root)
{
    int totalTickets = sumTickets(root);
    int totalNodes = numNodes(root);

    if (totalNodes == 0)
        return 0;

    double  mean = (double) totalTickets / totalNodes;
    return mean;
}

// sums the tickets for all fans whose names are alphabetically less than or equal to the name passed in command
int countBefore(struct tree_node *root, char *name)
{
    if (root == NULL)
        return 0;

    int total = 0;
    if (strcmp(root->name, name) <= 0){
        total += root->numTickets + countBefore(root->left, name) + countBefore(root->right, name);
    }
    else {
        total += countBefore(root->left, name);
    }

    return total;
}


int main()
{
    int numCommands;
    char command[20];
    struct tree_node *root = NULL;

    scanf("%d", &numCommands);

    for (int i = 0; i < numCommands; i++) {
        scanf("%s", command);

        if (strcmp(command, "buy") == 0)
        {
            char name[MAX_LENGTH];
            int count, depth = 0;
            scanf("%s %d", name, &count);

            // insert/update fan
            root = insert(root, name, count, &depth);
            // depth already updated in insertion, passing a dummy depth var for the search
            int dummyDepth = 0;
            struct tree_node *node = search(root, name, &dummyDepth);
            printf("%s %d %d\n", name, node->numTickets, depth);
        }

        else if (strcmp(command, "use") == 0)
        {
            char name[MAX_LENGTH];
            int count, depth = 0;
            scanf("%s %d", name, &count);
            struct tree_node *node = search(root, name, &depth);

            if (node == NULL)
                printf("%s not found\n", name);

            else {
                // deducting tickets, if resulting count is <= 0, deletes the fan
                node->numTickets -= count;

                if (node->numTickets <= 0) {
                    root = delete(root, name);
                    printf("%s deleted\n", name);
                }
                else
                    printf("%s %d %d\n", name, node->numTickets, depth);
            }
        }

        else if (strcmp(command, "find") == 0)
        {
            char name[MAX_LENGTH];
            int depth = 0;
            scanf("%s", name);
            struct tree_node *node = search(root, name, &depth);

            if (node == NULL)
                printf("%s not found\n", name);
            else
                printf("%s %d %d\n", name, node->numTickets, depth);

        }

        else if (strcmp(command, "mean_tickets") == 0)
        {
            double mean = meanTickets(root);
            printf("%.2lf\n", mean);
        }

        else if (strcmp(command, "height_balance") == 0)
        {
            struct tree_node *leftSub = NULL;
            struct tree_node *rightSub = NULL;

            if (root != NULL) {
                leftSub = root->left;
                rightSub = root->right;
            }

            int leftHeight = height(leftSub);
            int rightHeight = height(rightSub);
            char *status;

            if (leftHeight == rightHeight)
                status = "balanced";
            else
                status = "not balanced";

            printf("left height = %d right height = %d %s\n", leftHeight, rightHeight, status);
        }

        else if (strcmp(command, "count_before") == 0)
        {
            char name[MAX_LENGTH];
            scanf("%s", name);
            int total = countBefore(root, name);
            printf("%d\n", total);
        }
    }

    freeTree(root);

    return 0;
}

// some of my helper functions:

// finds a node by name
struct tree_node* findNode(struct tree_node *root, char *name)
{
    if (root == NULL)
        return NULL;

    int cmp = strcmp(name, root->name);

    if (cmp == 0)
        return root;

    else if (cmp < 0)
        return findNode(root->left, name);

    else
        return findNode(root->right, name);
}

// returns 1 if node is a leaf, 0 otherwise
int isLeaf(struct tree_node *root)
{
    if (root->left == NULL && root->right == NULL)
        return 1;
    else
        return 0;
}
// returns 1 if node only has a left child, 0 otherwise
int hasOnlyLeftChild(struct tree_node *root)
{
    if (root->left != NULL && root->right == NULL)
        return 1;
    else
        return 0;
}
// returns 1 if node only has a right child, 0 otherwise
int hasOnlyRightChild(struct tree_node *root)
{
    if (root->right != NULL && root->left == NULL)
        return 1;
    else
        return 0;
}
// finds the parent node of a given node
struct tree_node *parent(struct tree_node *root, struct tree_node *node)
{
    if (root == NULL || root == node)
        return NULL;

    // the root is a direct parent of the node
    if (root->left == node || root->right == node)
        return root;

    // looking for parent in left sub-tree
    if (strcmp(root->name, node->name) > 0)
        return parent(root->left, node);

    // look in left or right based on the value of the node compared to root

    // looking for parent in right sub-tree
    else if (strcmp(root->name, node->name) < 0)
        return parent(root->right, node);

    else
        return NULL;
}

// returns the node with the smallest vlaue in tree
struct tree_node* minValNode(struct tree_node *root)
{
    if (root == NULL)
        return NULL;

    else if (root->left == NULL)
        return root;

    else
        return minValNode(root->left);
}

// returns the node with the largest value in tree
struct tree_node* maxValNode(struct tree_node *root)
{
    if (root == NULL)
        return NULL;

    else if (root->right == NULL)
        return root;

    else
        return maxValNode(root->right);
}

// returns num of nodes in the BST
int numNodes(struct tree_node *root)
{
    if (root != NULL)
        return 1 + numNodes(root->left) + numNodes(root->right);

    else
        return 0;
}

void freeTree(struct tree_node *root)
{
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
