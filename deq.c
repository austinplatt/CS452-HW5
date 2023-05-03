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

// <summary>Adds an item to the data structure</summary>
// <param name = "r">List structure</param>
// <param name = "e">End to insert onto</param>
// <param name = "d">The data to insert into the list</param>
static void put(Rep r, End e, Data d)
{
  // Allocates data for the node to be inserted.
  Node node = malloc(sizeof(struct Node));
  if(!node){
    ERROR("Node failed to malloc");
  }
  memset(node->np, 0, sizeof(node->np));
  memset(node, 0, sizeof(*node));
  node->data = d;

  // If there are no items in the data structure.
  if (r->len == 0)
  {
    // Sets head/tail to the newly added node.
    r->ht[Head] = node;
    r->ht[Tail] = node;
  }

  // If there is one or more item in the list
  else if (r->len >= 1)
  {
    // Get the end block
    Node currentEnd = r->ht[e];

    // Set head/tail to the added node
    r->ht[e] = node;

    // Get opposite of current end (If e is Head then opposite will be Tail)
    End opposite = (e == Head ? Tail : Head);
    // Set next/prev of the head/tail to the added node
    currentEnd->np[e] = node;

    // Set next/prev of the new node to the old tail/head.
    node->np[opposite] = currentEnd;
  }

  r->len++;
}

// <summary>Gets an element at i th position in the data structure from head/tail</summary>
// <param name = "r">List structure</param>
// <param name = "e">Start search from head or tail</param>
// <param name = "i">The index to return</param>
// <returns>Data at specified index i</returns>
static Data ith(Rep r, End e, int i)
{
  // Check for index out of bound
  if (i < 0 || i > r->len - 1)
  {
    return 0;
  }
  // Get opposite of current end (If e is Head then opposite will be Tail)
  End opposite = (e == Head ? Tail : Head);

  Node currentNode = r->ht[e];

  // Loops forward or backwards i indexes depending on opposites value
  for (int j = 0; j < i; j++)
  {
    currentNode = currentNode->np[opposite];
  }
  return currentNode->data;
}

// <summary>Removes specified data from the list</summary>
// <param name = "r">List structure</param>
// <param name = "e">Start search from head or tail</param>
// <param name = "d">The data to remove</param>
// <returns>The data at the removed node</returns>
static Data rem(Rep r, End e, Data d)
{
  if (r->len > 0)
  {

    // Gets start point/either head/tail
    Node currentNode = r->ht[e];

    // Get opposite of current end (If e is Head then opposite will be Tail)
    End opposite = (e == Head ? Tail : Head);

    // While node is not null
    while (currentNode)
    {
      // If the current node equals the data passed in
      if (currentNode->data == d)
      {
        // Gets reference to previous and next node pointers
        Node prevNode = currentNode->np[Head];
        Node nextNode = currentNode->np[Tail];

        // If the previous node was not null
        if (prevNode)
        {
          // Set previous node to point to the next node
          prevNode->np[Tail] = nextNode;
        }

        // If there is no previous node then update the head
        else if (!prevNode && r->len > 1)
        {
          r->ht[Head] = nextNode;
        }

        // If only one item in the list set the head to NULL
        else
        {
          r->ht[Head] = NULL;
        }

        // If there is a next node update pointer the previous node.
        if (nextNode)
        {
          nextNode->np[Head] = prevNode;
        }

        // If there is not a next node and more than one elements left then tail is now nextNode.
        else if (!nextNode && r->len > 1)
        {
          r->ht[Tail] = prevNode;
        }

        // If last element in list then tail should now be null
        else
        {
          r->ht[Tail] = NULL;
        }
        r->len--;

        // Save the data to return
        Data returnData = currentNode->data;

        // Free malloc'ed node data
        free(currentNode);

        return returnData;
      }

      // If the node was not found increment currentNode back/forwards depending on value of e
      currentNode = currentNode->np[opposite];
    }
  }

  return 0;
}

// <summary>Gets and removes item at the head/tail of the list</summary>
// <param name = "r">List structure</param>
// <param name = "e">The end to get from</param>
// <returns>The data at either end of the list</returns>
static Data get(Rep r, End e)
{
  if(r->len > 0){
    // Gets the data at specified end
    Data endData = r->ht[e]->data;
    // Uses rem function to get and remove by passing the data at the head/tail
    return rem(r, e, endData);
  }
  return 0;
 
}

// Initializes a new Deq structure
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

// Returns length of the data structure
extern int deq_len(Deq q) { return rep(q)->len; }

// Puts data at the head of the structure
extern void deq_head_put(Deq q, Data d) { put(rep(q), Head, d); }

// Gets data at head of structure
extern Data deq_head_get(Deq q) { return get(rep(q), Head); }

// Gets ith object from head of structure
extern Data deq_head_ith(Deq q, int i) { return ith(rep(q), Head, i); }

// Removes specified d value starting search from head of structure
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q), Head, d); }

// Puts data at tail of structure
extern void deq_tail_put(Deq q, Data d) { put(rep(q), Tail, d); }

// Gets data at tail of structure
extern Data deq_tail_get(Deq q) { return get(rep(q), Tail); }

// Gets data at ith index starting index 0 at the tail of the structure
extern Data deq_tail_ith(Deq q, int i) { return ith(rep(q), Tail, i); }

// Removes specified data from structure starting search from tail
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q), Tail, d); }

// <summary>Iterates through deq performing operation specified by function pointer</summary>
// <param name = "q">Data structure</param>
// <param name = "f">Pointer to function that performs operation on data</param>
extern void deq_map(Deq q, DeqMapF f)
{
  for (Node n = rep(q)->ht[Head]; n; n = n->np[Tail])
    f(n->data);
}

// <summary>Cleans up deq object and frees memory</summary>
// <param name = "q">Reference to data structure to clean</param>
// <param name = "f">Function to clear any structure that was malloced in the node's data</param>
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

// <summary>Converts the data structure to a string representation</summary>
// <param name = "q">The data structure to print</param>
// <param name = "f">Function pointer that specifies how a piece of data in a node should be converted to a string</param>
// <returns>String representation of structure</returns>
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