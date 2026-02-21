/* tree.h */

#define _GNU_SOURCE

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TagRoot 1 /*00 01*/
#define TagNode 2 /*00 10*/
#define TagLeaf 4 /*01 00*/
#define NoError 0

#define nullptr ((void *)0)

#define find_last(x) find_last_linear(x)
#define reterr(x)                                                              \
  errno = (x);                                                                 \
  return nullptr

typedef unsigned int int32;
typedef unsigned short int16;
typedef unsigned char int8;
typedef unsigned char Tag;

struct s_node {
  struct s_node *north;
  struct s_node *weast;
  Tag tag;
  struct s_node *east;
  int8 path[256];
};

typedef struct s_node Node;

struct s_leaf {
  union u_tree *weast;
  Tag tag;
  struct s_leaf *east;
  int8 key[128];
  int8 *value;
  int16 size;
};

typedef struct s_leaf Leaf;

union u_tree {
  Node n;
  Leaf l;
};

typedef union u_tree Tree;
