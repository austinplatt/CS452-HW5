#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

// indices and size of array of node pointers
typedef enum
{
  Head,
  Tail,
  Ends
} End;

// Holds information about a node such as data and next/prev pointer
typedef struct Node
{
  struct Node *np[Ends]; // next/prev neighbors
  Data data;
} * Node;

// Holds information about head/tail of data structure and length
typedef struct
{
  Node ht[Ends]; // head/tail nodes
  int len;
} * Rep;

// Returns reference to the internal structure of Deq structure
static Rep rep(Deq q)
{
  if (!q)
    ERROR("zero pointer");
  return (Rep)q;
}

/**
 * This method serves the purpose of adding an item at the head or 
 * tail of a list and appending it to the beginning or end of the list.
 * @param r The list appending to
 * @param e head or tail types
 * @param d data that is being added
 * @return void
 * 
 */
static void put(Rep r, End e, Data d)
{
  Node node = malloc(sizeof(struct Node));
  if(!node){
    ERROR("Node failed to malloc");
  }
  memset(node->np, 0, sizeof(node->np));
  memset(node, 0, sizeof(*node));
  node->data = d;

  if (r->len == 0)
  {
    r->ht[Head] = node;
    r->ht[Tail] = node;
  }
  else if (r->len >= 1)
  {
    Node currentEnd = r->ht[e];
    r->ht[e] = node;

    End opposite = (e == Head ? Tail : Head);
    currentEnd->np[e] = node;

    node->np[opposite] = currentEnd;
  }

  r->len++;
}

/**
 * This method serves the purpose of maintaining the list's integrity 
 * while returning data from the intended index.
 * @param r list being parsed
 * @param e head or tail types
 * @param i index
 * @return data
 * 
 */
static Data ith(Rep r, End e, int i)
{
  if (i < 0 || i > r->len - 1)
  {
    return 0;
  }
  End opposite = (e == Head ? Tail : Head);

  Node currentNode = r->ht[e];

  for (int j = 0; j < i; j++)
  {
    currentNode = currentNode->np[opposite];
  }
  return currentNode->data;
}

/**
 * This method serves the purpose of removing the data if it is 
 * found in the intended list.
 * @param r intended list
 * @param e head or tail types
 * @param d data that is found
 * @return data
 * 
 */
static Data rem(Rep r, End e, Data d)
{
  if (r->len > 0)
  {
    Node currentNode = r->ht[e];
    End opposite = (e == Head ? Tail : Head);

    while (currentNode)
    {
      if (currentNode->data == d)
      {
        Node prevNode = currentNode->np[Head];
        Node nextNode = currentNode->np[Tail];

        if (prevNode)
        {
          prevNode->np[Tail] = nextNode;
        }
        else if (!prevNode && r->len > 1)
        {
          r->ht[Head] = nextNode;
        }

        else
        {
          r->ht[Head] = NULL;
        }
        if (nextNode)
        {
          nextNode->np[Head] = prevNode;
        }
        else if (!nextNode && r->len > 1)
        {
          r->ht[Tail] = prevNode;
        }

        else
        {
          r->ht[Tail] = NULL;
        }
        r->len--;

        Data returnData = currentNode->data;

        free(currentNode);

        return returnData;
      }

      currentNode = currentNode->np[opposite];
    }
  }

  return 0;
}

/**
 * This method serves the purpose of removing a node from the 
 * beginning or end of the list.
 * @param r list that is appending
 * @param e head or tail type
 * @return data
 * 
 */
static Data get(Rep r, End e)
{
  if(r->len > 0){
    Data endData = r->ht[e]->data;
    return rem(r, e, endData);
  }
  return 0;
 
}

extern Deq deq_new()
{
  Rep r = (Rep)malloc(sizeof(*r));
  if (!r)
    ERROR("malloc() failed");
  r->ht[Head] = 0;
  r->ht[Tail] = 0;
  r->len = 0;
  return r;
}

extern int deq_len(Deq q) { return rep(q)->len; }

extern void deq_head_put(Deq q, Data d) { put(rep(q), Head, d); }

extern Data deq_head_get(Deq q) { return get(rep(q), Head); }

extern Data deq_head_ith(Deq q, int i) { return ith(rep(q), Head, i); }

extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q), Head, d); }

extern void deq_tail_put(Deq q, Data d) { put(rep(q), Tail, d); }

extern Data deq_tail_get(Deq q) { return get(rep(q), Tail); }

extern Data deq_tail_ith(Deq q, int i) { return ith(rep(q), Tail, i); }

extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q), Tail, d); }

extern void deq_map(Deq q, DeqMapF f)
{
  for (Node n = rep(q)->ht[Head]; n; n = n->np[Tail])
    f(n->data);
}

extern void deq_del(Deq q, DeqMapF f)
{
  if (f)
    deq_map(q, f);
  Node curr = rep(q)->ht[Head];
  while (curr)
  {
    Node next = curr->np[Tail];
    free(curr);
    curr = next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f)
{
   char *s=strdup("");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t; asprintf(&t,"%s%s%s",s,(*s ? " " : ""),d);
    free(s); s=t;
    if (f) free(d);
  }
  return s;
}