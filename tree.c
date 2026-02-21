/* tree.c */

#include "tree.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

Tree root = {
    .n =
        {
            .tag = (TagRoot | TagNode),
            .north = (Node *)&root,
            .weast = 0,
            .east = 0,
            .path = "/",
        },
};

void zero(int8 *str, int16 size) {
  int8 *p;
  int16 n;

  for (n = 0, p = str; n < size; p++, n++) {
    *p = 0;
  }
}

Leaf *find_last_linear(Node *parent) {
  assert(parent);
  Leaf *l;

  errno = NoError;
  if (!parent->east) {
    reterr(NoError);
  }
  for (l = (Leaf *)parent->east; l->east; l = l->east)
    ;
  assert(l);
  return l;
}

Leaf *create_leaf(int8 *value, int8 *key, Node *parent, int16 count) {
  assert(parent);

  Leaf *l, *new;
  int16 size;

  l = find_last(parent);
  size = sizeof(struct s_leaf);
  new = (Leaf *)malloc(size);
  assert(new);

  if (!l) {
    parent->east = (Node *)new;

  } else {
    l->east = new;
  }
  zero((int8 *)new, size);
  new->tag = TagLeaf;
  new->weast = (!l) ? (Tree *)parent : (Tree *)l;
  strncpy((char *)new->key, (char *)key, 127);
  new->value = (int8 *)malloc(count);
  zero(new->value, count);
  assert(new->value);
  strncpy((char *)new->value, (char *)value, count);
  return new;
}

Node *create_node(Node *parent, int8 *path) {
  Node *n;
  int16 size;

  assert(parent);
  size = sizeof(struct s_node);
  n = (Node *)malloc(size);
  zero((int8 *)n, size);

  parent->weast = n;
  n->tag = TagNode;
  n->north = parent;
  strcpy((char *)n->path, (char *)path);

  return n;
}

int main(int argc, char *argv[]) {

  Node *n, *n1;
  Leaf *l1, *l2;
  int8 *key, *value;
  int16 size;

  n = create_node((Node *)&root, (int8 *)"/Users");
  assert(n);
  n1 = create_node(n, (int8 *)"/Users/login");
  assert(n1);
  key = (int8 *)"hplion";
  value = (int8 *)"abc4948";
  size = (int16)sizeof((char *)value);
  l1 = create_leaf(value, key, n1, size);
  printf("%s\n", l1->value);
  key = (int8 *)"hp";
  value = (int8 *)"hp4364";
  size = (int16)sizeof((char *)value);

  l2 = create_leaf(value, key, n1, size);
  assert(l2);

  printf("%s %s\n", l2->key, l2->value);
  printf("%p %p\n", n, n1);
  free(n1);
  free(n);
  return 0;
}
