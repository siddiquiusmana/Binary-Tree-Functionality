#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.c"
#define MAXLEN 31
FILE *ifp;
FILE *ofp;

typedef struct objectNode {
  char name[MAXLEN];
  int count;
  struct objectNode *left, *right;
}objectNode;

typedef struct treeTitleNode {
  char treeName[MAXLEN];
  struct treeTitleNode *left, *right;
  objectNode *theTree;
}treeTitleNode;

treeTitleNode* buildMainTree(FILE *ifp, int n);
treeTitleNode* createTitleNode(char name[MAXLEN]);
treeTitleNode* insertTreeNode(treeTitleNode* root, treeTitleNode* node);
void preorderTree(treeTitleNode *current_ptr, FILE *ofp);
treeTitleNode* searchTitleNode(treeTitleNode* root, char name[MAXLEN]);
void buildSubtrees(FILE *ifp, treeTitleNode* root, int num);
objectNode* createObjectNode(char name[MAXLEN], int count);
objectNode* insertObject(objectNode* root, objectNode* item);
void inOrderItem(objectNode *current_ptr, FILE *ofp);
void traverse(treeTitleNode* root, FILE *ofp);
treeTitleNode* requestProcessing(treeTitleNode* root, FILE *ifp, FILE *ofp);
void searchRequests(treeTitleNode* root, FILE *ifp, FILE *ofp);
int item_before(objectNode* root, char name[MAXLEN], int count);
int heightOfTree(objectNode* root);
int sumOfTree(objectNode* root);
objectNode* delete(objectNode* root, char name[50]);
objectNode* parent(objectNode *root, struct objectNode *node);
objectNode* lowestItem(objectNode* root);
treeTitleNode* deleteTree(treeTitleNode* root, char name[50]);
treeTitleNode* parentTree(treeTitleNode *root, treeTitleNode *node);
treeTitleNode* lowestTreeNode(treeTitleNode* root);
void freePostItem(objectNode* root);
void freeFunction(treeTitleNode* root);

int main ()
{
  atexit(report_mem_leak);
  int n, i, q;
  ifp = fopen("in.txt", "r");
  ofp = fopen("out.txt", "w");
  fscanf(ifp, "%d %d %d", &n, &i, &q);

  treeTitleNode* treeRoot = buildMainTree(ifp, n);
  buildSubtrees(ifp, treeRoot, i);
  preorderTree(treeRoot, ofp);
  printf("\n");
  fprintf(ofp, "\n");
  traverse(treeRoot, ofp);

  for(int j=1; j<q+1; j++)
  {
    treeRoot = requestProcessing(treeRoot, ifp, ofp);
  }

  freeFunction(treeRoot);
}

treeTitleNode* buildMainTree(FILE *ifp, int n)
{
  treeTitleNode *root = NULL;

  for(int i=0; i<n; i++)
  {
    char temp[MAXLEN];
    fscanf(ifp, "%s", temp);
    treeTitleNode* node = createTitleNode(temp);
    root = insertTreeNode(root, node);
  }

  return root;
}

treeTitleNode* createTitleNode(char name[MAXLEN])
{
  treeTitleNode* node = (treeTitleNode*) malloc(sizeof(treeTitleNode));

  strcpy(node->treeName, name);

  node->left = NULL;
  node->right = NULL;
  node->theTree = NULL;

  return node;
}

treeTitleNode* insertTreeNode(treeTitleNode* root, treeTitleNode* node)
{
  if(root == NULL)
  {
    root = node;
  }
  else
  {
    if(strcmp(node->treeName, root->treeName) > 0)
    {
      if (root->right != NULL)
        root->right = insertTreeNode(root->right, node);
      else
        root->right = node;
    }
    else
    {
      if (root->left != NULL)
        root->left = insertTreeNode(root->left, node);
      else
        root->left = node;
    }
  }

  return root;
}

void preorderTree(treeTitleNode *current_ptr, FILE *ofp)
{
  if (current_ptr != NULL) {
    preorderTree(current_ptr->left, ofp);
    printf("%s ", current_ptr->treeName);
    fprintf(ofp, "%s ", current_ptr->treeName);
    preorderTree(current_ptr->right, ofp);
  }
}

treeTitleNode* searchTitleNode(treeTitleNode* root, char name[MAXLEN])
{
  if(root == NULL)
  {
    return NULL;
  }
  if(strcmp(name, root->treeName)==0)
  {
    return root;
  }
  else
  {
    if(strcmp(name, root->treeName) < 0)
      return searchTitleNode(root->left, name);
    else
      return searchTitleNode(root->right, name);
  }
}

void buildSubtrees(FILE *ifp, treeTitleNode* root, int num)
{
  char treeName[MAXLEN];
  char itemName[MAXLEN];
  int count;
  for(int i=0; i<num; i++)
  {
    fscanf(ifp, "%s %s %d", treeName, itemName, &count);
    objectNode* item = createObjectNode(itemName, count);
    treeTitleNode* search = searchTitleNode(root, treeName);
    search->theTree = insertObject(search->theTree, item);
  }
}

objectNode* createObjectNode(char name[MAXLEN], int count)
{
  objectNode* item = (objectNode*) malloc(sizeof(objectNode));

  strcpy(item->name, name);
  item->count = count;
  item->left = NULL;
  item->right = NULL;

  return item;
}

objectNode* insertObject(objectNode* root, objectNode* item)
{
  if(root == NULL)
  {
    root = item;
    //printf("%s is root\n", root->name);
  }
  else
  {
    if(strcmp(item->name, root->name)<0)
    {
      if(root->left == NULL)
      {
        root->left = item;
      }
      else
        insertObject(root->left, item);
    }
    else
    {
      if(root->right == NULL)
      {
        root->right = item;
      }
      else
        insertObject(root->right, item);
    }
  }
  return root;
}

void inOrderItem(objectNode *current_ptr, FILE *ofp)
{
  if (current_ptr != NULL) {
    inOrderItem(current_ptr->left, ofp);
    printf("%s ", current_ptr->name);
    fprintf(ofp, "%s ", current_ptr->name);
    inOrderItem(current_ptr->right, ofp);
  }
}

void traverse(treeTitleNode* root, FILE *ofp)
{
  if (root != NULL) {
    traverse(root->left, ofp);
    printf("===%s===\n", root->treeName);
    fprintf(ofp, "===%s===\n", root->treeName);
    inOrderItem(root->theTree, ofp);
    printf("\n");
    fprintf(ofp, "\n");
    traverse(root->right, ofp);
  }
}

objectNode* searchItemTree(objectNode* root, char name[MAXLEN])
{
  if(root == NULL)
  {
    return NULL;
  }
  if(strcmp(name, root->name)==0)
  {
    return root;
  }
  else
  {
    if(strcmp(name, root->name) < 0)
      return searchItemTree(root->left, name);
    else
      return searchItemTree(root->right, name);
  }
}

treeTitleNode* requestProcessing(treeTitleNode* root, FILE *ifp, FILE *ofp)
{
  char request[50];
  fscanf(ifp, "%s", request);

  if(strcmp(request, "search")==0)
    searchRequests(root, ifp, ofp);

  else if(strcmp(request, "item_before")==0)
  {
    fscanf(ifp, "%s", request);
    treeTitleNode* treeRequest = searchTitleNode(root, request);
    fscanf(ifp, "%s", request);
    int ib = item_before(treeRequest->theTree, request, 0);
    printf("item before %s: %d\n", request, ib);
    fprintf(ofp, "item before %s: %d\n", request, ib);
  }
  else if(strcmp(request, "height_balance")==0)
  {
    fscanf(ifp, "%s", request);
    treeTitleNode* search = searchTitleNode(root, request);
    int left = heightOfTree(search->theTree->left) - 1;
    int right = heightOfTree(search->theTree->right) - 1;
    int diff = abs(left - right);
    if(diff != 0){
      printf("%s: left height %d, right height %d, difference %d, not balanced\n", search->treeName, left, right, diff);
      fprintf(ofp, "%s: left height %d, right height %d, difference %d, not balanced\n", search->treeName, left, right, diff);
    }
    else{
      printf("%s: left height %d, right height %d, difference %d, balanced\n", search->treeName, left, right, diff);
      fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced\n", search->treeName, left, right, diff);
    }
  }
  else if(strcmp(request, "count")==0)
  {
    fscanf(ifp, "%s", request);
    treeTitleNode* search = searchTitleNode(root, request);
    int total = sumOfTree(search->theTree);
    printf("%s count %d\n", search->treeName, total);
    fprintf(ofp, "%s count %d\n", search->treeName, total);
  }
  else if(strcmp(request, "delete")==0)
  {
    fscanf(ifp, "%s", request);
    treeTitleNode* search = searchTitleNode(root, request);
    fscanf(ifp, "%s", request);
    search->theTree = delete(search->theTree, request);
    printf("%s deleted from %s\n", request, search->treeName);
    fprintf(ofp, "%s deleted from %s\n", request, search->treeName);
  }
  else if(strcmp(request, "delete_name")==0)
  {
    fscanf(ifp, "%s", request);
    deleteTree(root, request);
    printf("%s deleted\n", request);
    fprintf(ofp, "%s deleted\n", request);
  }
  else if(strcmp(request, "reduce")==0)
  {
    fscanf(ifp, "%s", request);
    treeTitleNode* search = searchTitleNode(root, request);
    fscanf(ifp, "%s", request);
    objectNode* item = searchItemTree(search->theTree, request);
    int reduce;
    fscanf(ifp, "%d", &reduce);
    //printf("REDUCE: %d - %d\n", item->count, reduce);
    item->count = item->count - reduce;
    printf("%s reduced\n", request);
    fprintf(ofp, "%s reduced\n", request);
    if(item->count < 0)
    {
      search->theTree = delete(search->theTree, item->name);
    }
  }
  return root;
}

void searchRequests(treeTitleNode* root, FILE *ifp, FILE *ofp)
{
  char request[50];
  fscanf(ifp, "%s", request);
  treeTitleNode* tree = searchTitleNode(root, request);
  objectNode* result = NULL;
  if(tree == NULL)
  {
    printf("%s does not exist\n", request);
    fprintf(ofp, "%s does not exist\n", request);
    char temp[50];
    fscanf(ifp, "%s", temp);
  }
  else
  {
    fscanf(ifp, "%s", request);
    result = searchItemTree(tree->theTree, request);
    if(result == NULL)
    {
      printf("%s not found in %s\n", request, tree->treeName);
      fprintf(ofp, "%s not found in %s\n", request, tree->treeName);
    }
    else
    {
      printf("%d %s found in %s\n", result->count, result->name, tree->treeName);
      fprintf(ofp, "%d %s found in %s\n", result->count, result->name, tree->treeName);
    }
  }
}

int item_before(objectNode* root, char name[MAXLEN], int count)
{
  count++;
  if(root == NULL)
    return (count -1);
  if(strcmp(name, root->name)==0)
  {
    return (count+1);
  }
  else
  {
    if(strcmp(name, root->name) < 0)
      return item_before(root->left, name, count);
    else
      return item_before(root->right, name, count);
  }
}

int heightOfTree(objectNode* root)
{
  if(root == NULL)
    return 0;
  else
  {
    int l;
    int r;
    l = heightOfTree(root->left);
    r = heightOfTree(root->right);
    if(l > r)
      return l+1;
    else
      return r+1;
  }
}

int sumOfTree(objectNode* root)
{
  if (root != NULL)
    return root->count + sumOfTree(root->left)+ sumOfTree(root->right);
  else
    return 0;
}

objectNode* delete(objectNode* root, char name[50])
{
  objectNode* delnode;
  objectNode *new_del_node;
  objectNode *save_node;
  objectNode *par;
  char sname[50];
  int scount;

  delnode = searchItemTree(root, name);
  par = parent(root, delnode);

  if (delnode->left == NULL && delnode->right == NULL)
  {
    if (par == NULL)
    {
      free(root);
      return NULL;
    }

    if (strcmp(name, par->name)<0)
    {
      free(par->left);
      par->left = NULL;
    }

    else {
      free(par->right);
      par->right = NULL;
    }

    return root;
  }

  if (delnode->left!= NULL && delnode->right == NULL)
  {
    if (par == NULL) {
      save_node = delnode->left;
      free(delnode);
      return save_node;
    }

    if (strcmp(name, par->name)<0)
    {
      save_node = par->left;
      par->left = par->left->left;
      free(save_node);
    }
    else
    {
      save_node = par->right;
      par->right = par->right->left;
      free(save_node);
    }

    return root;
  }

  if (delnode->left== NULL && delnode->right != NULL)
  {
    if (par == NULL)
    {
      save_node = delnode->right;
      free(delnode);
      return save_node;
    }

    if (strcmp(name, par->name)<0)
    {
      save_node = par->left;
      par->left = par->left->right;
      free(save_node);
    }
    else
    {
      save_node = par->right;
      par->right = par->right->right;
      free(save_node);
    }
    return root;
  }

  new_del_node = lowestItem(delnode->right);
  strcpy(sname, new_del_node->name);
  scount = new_del_node->count;

  delete(root, sname);

  strcpy(delnode->name, sname);
  delnode->count = scount;

  return root;
}

objectNode* parent(objectNode *root, struct objectNode *node)
{
  if (root == NULL || root == node)
    return NULL;

  if (root->left == node || root->right == node)
    return root;

  if (strcmp(node->name, root->name)<0)
    return parent(root->left, node);

  else if (strcmp(node->name, root->name)>0)
    return parent(root->right, node);

  return NULL;
}

objectNode* lowestItem(objectNode* root)
{
  if (root->left == NULL)
    return root;
  else
    return lowestItem(root->left);
}

treeTitleNode* deleteTree(treeTitleNode* root, char name[50])
{
  treeTitleNode* delnode;
  treeTitleNode *new_del_node;
  treeTitleNode *save_node;
  treeTitleNode *par;
  char sname[50];
  objectNode* stree;

  delnode = searchTitleNode(root, name);
  par = parentTree(root, delnode);

  if (delnode->left == NULL && delnode->right == NULL)
  {
    if (par == NULL)
    {
      freePostItem(root->theTree);
      free(root);
      return NULL;
    }

    if (strcmp(name, par->treeName)<0)
    {
      freePostItem(par->left->theTree);
      free(par->left);
      par->left = NULL;
    }

    else {
      freePostItem(par->right->theTree);
      free(par->right);
      par->right = NULL;
    }

    return root;
  }

  if (delnode->left!= NULL && delnode->right == NULL)
  {
    if (par == NULL) {
      save_node = delnode->left;
      freePostItem(delnode->theTree);
      free(delnode);
      return save_node;
    }

    if (strcmp(name, par->treeName)<0)
    {
      save_node = par->left;
      par->left = par->left->left;
      freePostItem(save_node->theTree);
      free(save_node);
    }
    else
    {
      save_node = par->right;
      par->right = par->right->left;
      freePostItem(save_node->theTree);
      free(save_node);
    }

    return root;
  }

  if (delnode->left== NULL && delnode->right != NULL)
  {
    if (par == NULL)
    {
      save_node = delnode->right;
      freePostItem(delnode->theTree);
      free(delnode);
      return save_node;
    }

    if (strcmp(name, par->treeName)<0)
    {
      save_node = par->left;
      par->left = par->left->right;
      freePostItem(save_node->theTree);
      free(save_node);
    }
    else
    {
      save_node = par->right;
      par->right = par->right->right;
      freePostItem(save_node->theTree);
      free(save_node);
    }
    return root;
  }

  new_del_node = lowestTreeNode(delnode->right);
  strcpy(sname, new_del_node->treeName);
  stree = new_del_node->theTree;

  deleteTree(root, sname);

  strcpy(delnode->treeName, sname);
  delnode->theTree = stree;

  return root;
}

treeTitleNode* parentTree(treeTitleNode *root, treeTitleNode *node)
{
  if (root == NULL || root == node)
    return NULL;

  if (root->left == node || root->right == node)
    return root;

  if (strcmp(node->treeName, root->treeName)<0)
    return parentTree(root->left, node);

  else if (strcmp(node->treeName, root->treeName)>0)
    return parentTree(root->right, node);

  return NULL;
}

treeTitleNode* lowestTreeNode(treeTitleNode* root)
{
  if (root->left == NULL)
    return root;
  else
    return lowestTreeNode(root->left);
}

void freeFunction(treeTitleNode* root)
{
  if(root!=NULL)
  {
    freeFunction(root->left);
    freeFunction(root->right);
    freePostItem(root->theTree);
    free(root);
  }
}

void freePostItem(objectNode* root)
{
  if(root != NULL)
  {
    freePostItem(root->left);
    freePostItem(root->right);
    free(root);
  }
}
