#include "trie.h"
#include "code.h"

#include <stdint.h>
#include <stdlib.h>

//creates a node with index as code
//sets its children to NULL
TrieNode *trie_node_create(uint16_t index) {
    TrieNode *node = calloc(1, sizeof(TrieNode));
    node->code = index;
    for (uint16_t i = 0; i < 256; i++) {
        node->children[i] = NULL;
    }
    return node;
}

//deletes a node by freeing memory and setting to null
void trie_node_delete(TrieNode *n) {
    free(n);
    n = NULL;
}

//creates a root node with EMPTY_CODE as the code
TrieNode *trie_create(void) {
    TrieNode *node = calloc(1, sizeof(TrieNode));
    node->code = EMPTY_CODE;
    for (uint16_t i = 0; i < 256; i++) {
        node->children[i] = NULL;
    }
    return node;
}

//recursively deletes the children of a node
//leaves the root node
void trie_reset(TrieNode *root) {
    for (uint16_t i = 0; i < 256; i++) {
        if (root->children[i] != NULL) {
            trie_delete(root->children[i]);
            root->children[i] = NULL;
        }
    }
}

//recursively deletes the children of a node
//then deletes the node
void trie_delete(TrieNode *n) {
    for (uint16_t i = 0; i < 256; i++) {
        //printf("%d\n", i);
        if (n->children[i] != NULL) {
            trie_delete(n->children[i]);
        }
        n->children[i] = NULL;
    }
    trie_node_delete(n);
    n = NULL;
}

//searches the children of a node for a symbol
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
