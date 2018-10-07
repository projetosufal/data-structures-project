#include <stdlib.h>
#include "CUnit/Basic.h"
#include "../hufflist.h"
#include "../hufftree.h"
#include "../compress.h"

//hufflist.h
void create_huff_node_test() {
  CU_ASSERT_PTR_NULL(create_huff_node(NULL, 0, NULL, NULL, NULL)->value);
  CU_ASSERT_PTR_NULL(create_huff_node(NULL, 0, NULL, NULL, NULL)->next);
  CU_ASSERT_PTR_NULL(create_huff_node(NULL, 0, NULL, NULL, NULL)->left);
  CU_ASSERT_PTR_NULL(create_huff_node(NULL, 0, NULL, NULL, NULL)->right);
  CU_ASSERT_EQUAL(create_huff_node(NULL, 1, NULL, NULL, NULL)->freq, 1);
}

void add_to_head_test() {
  huff_node *head = create_huff_node(NULL, 0, NULL, NULL, NULL);
  add_to_head(&head, NULL, 2);
  CU_ASSERT_PTR_NULL(head->value);
  CU_ASSERT_EQUAL(head->freq, 2);
}

void add_to_tail_test() {
  huff_node *head = create_huff_node(NULL, 0, NULL, NULL, NULL);
  head = add_to_tail(head, NULL, 3);
  CU_ASSERT_PTR_NULL(head->next->value);
  CU_ASSERT_EQUAL(head->next->freq, 3);
}

void remove_from_tail_test() {
  huff_node *head = create_huff_node(NULL, 0, NULL, NULL, NULL);
  head = add_to_tail(head, NULL, 3);
  head = add_to_tail(head, NULL, 4);
  remove_from_tail(head);
  CU_ASSERT_EQUAL(head->next->freq, 3);
}

void create_list_test() {
  int *table = calloc(256, sizeof(int));
  FILE *file = fopen("sample_file", "r");
  create_table(file, table);
  huff_node *head = create_list(table);
  CU_ASSERT_EQUAL(head->freq, 1);
}

void merge_test() {
  huff_node *l = create_huff_node(NULL, 3, NULL, NULL, NULL);
  huff_node *r = create_huff_node(NULL, 2, NULL, NULL, NULL);
  huff_node *result = merge(l, r);
  CU_ASSERT_EQUAL(result->freq, 2);
  CU_ASSERT_EQUAL(result->next->freq, 3);
}

//hufftree.h
void build_huffman_tree_test() {
  huff_node *huffman_tree;
  huff_node *list = create_huff_node(NULL, 2, NULL, NULL, NULL);
  add_to_tail(list, NULL, 3);
  build_huffman_tree(&huffman_tree, list);
  CU_ASSERT_EQUAL(*((char *)huffman_tree->value), '*');
  CU_ASSERT_EQUAL(huffman_tree->left->freq, 2);
  CU_ASSERT_EQUAL(huffman_tree->right->freq, 3);
}

void search_tree_test() {
  huff_node *huffman_tree;
  byte *value0 = malloc(sizeof(char));
  *value0 = 'a';
  huff_node *list = create_huff_node(value0, 2, NULL, NULL, NULL);
  add_to_tail(list, NULL, 3);
  build_huffman_tree(&huffman_tree, list);
  char *str = malloc(sizeof(char));
  search_tree(huffman_tree, 'a', NULL, &str);
  CU_ASSERT_EQUAL(strcmp(str, "0"), 0);
}

void get_tree_size_test() {
  huff_node *huffman_tree;
  huff_node *list = create_huff_node(NULL, 2, NULL, NULL, NULL);
  add_to_tail(list, NULL, 3);
  int sz = 0;
  get_tree_size(huffman_tree, &sz);
  CU_ASSERT_EQUAL(sz, 3);
}

int main() {
   // Initialize the CUnit test registry
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   // Sets the basic run mode, CU_BRM_VERBOSE will show maximum output of run details
   // Other choices are: CU_BRM_SILENT and CU_BRM_NORMAL
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_pSuite pSuite = NULL;

    // Add a suite to the registry
    pSuite = CU_add_suite("huff_test_suite", 0, 0);

    // Always check if add was successful
    if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    // Add the tests to the suite
    if (NULL == CU_add_test(pSuite, "create_huff_node_test", create_huff_node_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "add_to_head_test", add_to_head_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }


    if (NULL == CU_add_test(pSuite, "add_to_tail_test", add_to_tail_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "remove_from_tail_test", remove_from_tail_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "create_list_test", create_list_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "add_to_head_test", add_to_head_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "merge_test", merge_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "build_huffman_tree_test", build_huffman_tree_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "get_tree_size_test", get_tree_size_test)) {
      CU_cleanup_registry();
      return CU_get_error();
    }
   // Run the tests and show the run summary
   CU_basic_run_tests();
   return CU_get_error();
}