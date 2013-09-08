#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mylib.h"

#define MAXLENGTH 100
#define KEYLENGTH 10
#define WORDLENGTH 8

typedef struct ring {
    char string[MAXLENGTH];
    struct ring *next;
} ring_t;

typedef struct tree {
    char key[MAXLENGTH];
    ring_t *ring;
    struct tree *left;
    struct tree *right;
} tree_t;

char myget_arg(int, char **);
int create_tree(tree_t *);
int attach_tree(tree_t *, char *);
int create_ring(ring_t **);
int attach_elem(ring_t **, char *);
int walk_through_the_tree(tree_t *, char *);
int walk_through_the_ring(ring_t *, char *);
int find_word(char *, char *);
int input_and_check_word(tree_t *);
// ------------------------------------------
int main(int argc, char **argv)
{
    tree_t *root;

    if(myget_arg(argc, argv) == 'h') {
        print_manual();
        return 0;
    }
    root = (tree_t *)malloc(sizeof(*root));
    printf("Please, enter the root key: ");
    mygets(root->key, MAXLENGTH);
    create_ring(&(root->ring));
    while(create_tree(root));
    while(input_and_check_word(root));

    printf("Exiting...\n");

    return 0;
}
// ------------------------------------------
char myget_arg(int argc, char **argv)
{
    if(argc == 2) {
        if(!(strcmp(argv[1], "-h"))) {
            return 'h';
        }
    }

    return '0';
}
// ------------------------------------------
int create_ring(ring_t **ring)
{
    char buffer[MAXLENGTH];
    printf("Enter strings ('end' to stop reading):\n");
    while(1) {
        printf("----> ");
        mygets(buffer, MAXLENGTH);
        if(!strncmp(buffer, "end", 3)) {
            return 0;
        }
        attach_elem(ring, buffer);
    }

    return 0;
}
// ------------------------------------------
int attach_elem(ring_t **ring, char *string)
{
    ring_t *new_elem;

    new_elem = (ring_t *)malloc(sizeof(*new_elem));
    strncpy(new_elem->string, string, MAXLENGTH);

    if(!*ring) {
        *ring = new_elem;
        (*ring)->next = *ring;
    } else {
        new_elem->next = (*ring)->next;
        (*ring)->next = new_elem;
    }

    return 0;
}
// ------------------------------------------
int create_tree(tree_t *root)
{
    char key[MAXLENGTH];

    printf("Enter the key ('end' to stop input): ");
    mygets(key, MAXLENGTH);
    if(!strncmp(key, "end", 3)) {
        return 0;
    }
    attach_tree(root, key);

    return 1;
}
// ------------------------------------------
int attach_tree(tree_t *root, char *key)
{
    tree_t *tmp;
    tmp = root;
    while(1) {
        if(strncmp(key, tmp->key, MAXLENGTH) > 0) {
            if(tmp->left) {
                tmp = tmp->left;
            } else {
                tmp->left = (tree_t *)malloc(sizeof(*tmp->left));
                tmp = tmp->left;
                break;
            }
        } else if(tmp->right) {
            tmp = tmp->right;
        } else {
            tmp->right = (tree_t *)malloc(sizeof(*tmp->right));
            tmp = tmp->right;
            break;
        }
    }
    strncpy(tmp->key, key, MAXLENGTH);
    create_ring(&(tmp->ring));

    return 0;
}
// ------------------------------------------
int walk_through_the_ring(ring_t *ring, char *word)
{
    int counter = 0, buffer;
    ring_t *tmp;

    if(!ring) {
        return 0;
    }
    tmp = ring;

    do {
        if((buffer = find_word(tmp->string, word)) != 0) {
            printf("%s\n", tmp->string);
            counter += buffer;
        }
        tmp = tmp->next;
    }
    while(tmp != ring);

    return counter;
}
// ------------------------------------------
int find_word(char *str, char *word)
{
    int counter = 0;
    char *p;
    p = str;
    while(p) {
        p = strstr(p, word);
        if(p) {
            counter++;
        } else {
            break;
        }
        while(*p != ' ' && *p)
            p++;
    }

    return counter;
}
// ------------------------------------------
int walk_through_the_tree(tree_t *root, char *word)
{
    int counter = 0;
    if(!root) {
        return 0;
    }
    counter += walk_through_the_ring(root->ring, word);

    counter += walk_through_the_tree(root->right, word);
    counter += walk_through_the_tree(root->left, word);

    return counter;
}
// ------------------------------------------
int input_and_check_word(tree_t *root)
{
    int counter = 0;
    char buffer[WORDLENGTH];

    printf("Enter the requied word to start search (end, to exit): ");
    mygets(buffer, WORDLENGTH);
    if(!(strncmp(buffer, "end", 3))) {
        return 0;
    } else if(!(strlen(buffer))) {
        printf("You don't have typed anything, try again\n");
        return 1;
    }

    printf("The word occurs at:\n\n");
    counter = walk_through_the_tree(root, buffer);
    printf("\n%d times\n", counter);

    return 1;
}

#ifdef __cplusplus
}
#endif
