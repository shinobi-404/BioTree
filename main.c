

// Declared Liabaires 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"
#define MAXLEN 30
FILE *file;

// Struct variables 
typedef struct itemNode{
  char name[MAXLEN];
  int count;
  struct itemNode *left, *right;
}itemNode;

typedef struct treeNameNode{
  char treeName[MAXLEN];
  struct treeNameNode *left, *right;
  itemNode *theTree;
}treeNameNode;


//Function Declarations 
int count(itemNode *root);
itemNode* minValItemNode(itemNode *root);
treeNameNode* minValTreeName(treeNameNode *root);
itemNode* item_create_node(char* name, int val); 
treeNameNode* createTreeNameNode(char* name);
itemNode* insert_itemNode(itemNode* root, itemNode* element);
treeNameNode* insert_treeNameNode(treeNameNode* root, treeNameNode* element);
int height(itemNode* root);
itemNode* searchItems(itemNode *root, char itemName[MAXLEN]);
treeNameNode* searchNameNode(treeNameNode* root, char treeName[50]);
treeNameNode* buildNameTree(treeNameNode* root, int val);
void buildsubtreeitem(treeNameNode* root, int val);
void traverse_in_traverse(treeNameNode* root);
treeNameNode *itemTreeBranch(treeNameNode* root, int data);
void heightcheck(treeNameNode *root); 
void find(treeNameNode *root);
void decrease_count(treeNameNode *root);
void checkcount(treeNameNode *root);
int counter(itemNode *root, char name[MAXLEN]);
void count_before(treeNameNode *root);
itemNode* removing(itemNode *root,char name[MAXLEN]);
void removeitem(treeNameNode * root);
treeNameNode *delete_name(treeNameNode *root, char name[MAXLEN]);
void delete_tree(treeNameNode *root);
void treePrint_treeNameNode(treeNameNode* root);
void treePrint_itemNode(itemNode* root);
void free_itemNode(itemNode* root);
void free_treeNameNode(treeNameNode* root);


int main(void){
  atexit(report_mem_leak);
  
  file = fopen("out.txt","w");
  treeNameNode* branches = NULL;
  int N, I, Q;
  
  scanf("%d %d %d", &N, &I, &Q);
  
  branches = buildNameTree(branches, N);
  buildsubtreeitem(branches, I); 
  treePrint_treeNameNode(branches); 
  traverse_in_traverse(branches);
  fprintf(file, "\n");
  printf("\n");
  for(int i = 0; i < Q; i++){
   char que[MAXLEN];

    scanf("%s",que);

    if(strcmp(que, "find") == 0){
      find(branches);
    }
    else if(strcmp(que, "count_before") == 0){
      count_before(branches);
    }  
    else if(strcmp(que, "check_balance") == 0){
      heightcheck(branches);
    }
    else if(strcmp(que, "count") == 0){
      checkcount(branches);
    }
    else if(strcmp(que, "decrease") == 0){
      decrease_count(branches);
    }
    else if(strcmp(que, "remove") == 0){
      removeitem(branches);
    }
    else if(strcmp(que, "delete_name") == 0){
      delete_tree(branches);
    }
  }
  
  free_treeNameNode(branches); 

  fclose(file);
  return 0;
}

// counts number items in a tree
int count(itemNode *root) {
  if (root != NULL) return root->count + count(root->left) + count(root->right);  
  else
    return 0;
}

//finds the most minimum node in the item trees
itemNode* minValItemNode(itemNode *root){
  if(root->left == NULL) return root; //root stores the minimal value 
  else // the left subtree of the root stires the mnmal value 
    return minValItemNode(root->left);
}

// finds the minimum node in the treename tree 
treeNameNode* minValTreeName(treeNameNode *root){
  if(root->left == NULL) return root; //root stores the minimal value 
  else // the left subtree of the root stires the mnmal value 
    return minValTreeName(root->left);
}

// Create an item node and store its data
itemNode* item_create_node(char* name, int val) { 
  // Allocate space for the node, set the fields.
  itemNode* temp = malloc(sizeof(itemNode));
  strcpy(temp->name, name);
  temp->count = val;

  // makes the right and left feilds NULL
  temp->left = NULL;
  temp->right = NULL;
  
  return temp;
}

// Creates TreeNameNode and stores its data 
treeNameNode* createTreeNameNode(char* name){ 
  // Allocate space for the node, set the fields.
  treeNameNode* temp = malloc(sizeof(treeNameNode));
  strcpy(temp->treeName, name);
  temp->theTree = NULL; // creates the other one NULL

  //makes the right and left feilds NULL
  temp->left = NULL;
  temp->right = NULL;
  
  return temp;
}

// places the item node in the proper place in the tree 
itemNode* insert_itemNode(itemNode* root, itemNode* element){  
  if(root == NULL) return element; // returns element if root is NULL
  else{
    // if the name is not the same than 
    if(strcmp(root->name, element->name)>0){
      // checks for the left nodes till NULL
      if(root->left!= NULL){
        root->left = insert_itemNode(root->left, element);
      }
      // if the leaf it is NULL than it inserts the element 
      else{
        root->left = element;
      }
    }
    // does hte right side 
    else{
      // checks if the right children are NULL
      if(root->right != NULL){
        // moves trough the right till it finds its child 
        root->right = insert_itemNode(root->right, element);
      }
      // if the child is NULL inserts the elemet 
      else{
        root->right = element;
      }
    }
  }
  return root;
}

// inserts the treenamenode in the proper position 
treeNameNode* insert_treeNameNode(treeNameNode* root, treeNameNode* element) {  

  if (root == NULL) return element; // checks if the element is NULL 
  else {
    // compares the root to see if the strings are the same 
    if (strcmp(root->treeName, element->treeName) > 0) {
      // traverses trough the left side of the tree till it reaches the bottom 
      if (root->left != NULL) {
        root->left = insert_treeNameNode(root->left, element);
      }
      // inserts the element once the bottom of the tree is found 
      else {
        root->left = element;
      }
    }
    // checks if the right side is NULL and than places it in the proper positon 
    else{
      if (root->right != NULL) {
        root->right = insert_treeNameNode(root->right, element);
      }
      // places the element in the proper positon 
      else {
        root->right = element;
      }
    }
  }
  return root;
}

// checks the height of the tree 
int height(itemNode* root) {
  // declares variables 
  int leftH = 0, rightH = 0;
  // checks if the root is NULL
  if(root == NULL) return -1;
  // recursively checks the height
  leftH = height(root->left);
  rightH = height(root->right);
  // adds which part of the hieght is greater 
  if (leftH > rightH){
    return leftH + 1;
  }
  else{
    return rightH + 1;
  }
}

// searches for the Item in the itemNOdetree  
itemNode* searchItems(itemNode *root, char itemName[MAXLEN]){
  if(root != NULL){
    if(strcmp(root->name, itemName) == 0) return root; // once found it is returned 
    if(strcmp(root->name, itemName) > 0) return searchItems(root->left, itemName); // keeps searching left side
    else{
      return searchItems(root->right, itemName); // searches right side 
    }
  }
  else{
    return NULL; 
  }
}

// searches for name of the tree in TreeName tree
treeNameNode* searchNameNode(treeNameNode* root, char treeName[50]){  
  if(root != NULL){
    if(strcmp(root->treeName, treeName) == 0) return root; // once found it is returned 
    if(strcmp(root->treeName, treeName) > 0) return searchNameNode(root->left, treeName); // searches left side 
    else{
      return searchNameNode(root->right, treeName); // searches right side 
    }
  }
  else{
    return NULL;
  }
}

// Builds the tree Nametree 
treeNameNode* buildNameTree(treeNameNode* root, int val){   
  char name[MAXLEN]; // variable declaration 
  for(int i = 0 ; i<val; i++){
    scanf("%s", name);

    treeNameNode* branch = createTreeNameNode(name);
    root = insert_treeNameNode(root, branch);
  }
  return root;
}

// builds the item tree
void buildsubtreeitem(treeNameNode* root, int val){
  // variable declarations 
  char name[MAXLEN], nameitem[MAXLEN];
  int c; // count variable 
  // loops trough to scan and search items and build and insert them in the proper positon 
  for(int i = 0; i < val; i++){
    scanf("%s %s %d", name, nameitem, &c);
    
    itemNode* branchitem = item_create_node(nameitem, c);
    treeNameNode* branch = searchNameNode(root, name);
    
    branch->theTree = insert_itemNode(branch->theTree, branchitem);
  }
}

// used to traverse the tree and prints them 
void traverse_in_traverse(treeNameNode* root){  
  if(root != NULL){
    traverse_in_traverse(root->left);
    
    printf("\n###%s###\n", root->treeName);
    fprintf(file,"\n###%s###\n", root->treeName);
    
    treePrint_itemNode(root->theTree);
    traverse_in_traverse(root->right);
  }
}

// builds the tree 
treeNameNode *itemTreeBranch(treeNameNode* root, int data){
  char tname[MAXLEN], nameitem[MAXLEN];
  int c;
  
  for(int i = 0; i<data; i++){
    
    scanf("%s %s %d", tname, nameitem, &c);
    
    itemNode* item = item_create_node(nameitem, c);
    treeNameNode* temp = searchNameNode(root, tname);
    temp->theTree = insert_itemNode(temp->theTree, item);
  }
  
  return root;
}


// checks the height to make sure the tree is balanced 
void heightcheck(treeNameNode *root){
  char name[MAXLEN];
  scanf("%s",name);
  treeNameNode *stem = searchNameNode(root, name);
  /// recursively checks the hieght 
  int leftH = height(stem->theTree->left);
  int rightH = height(stem->theTree->right);
  int hvalue = abs(rightH - leftH); // calcauates the absolute value 

  // prints the data accordingly 
  if(hvalue > 1){
    fprintf(file,"%s: left height %d, right height %d, difference %d, not balanced\n",stem->treeName,leftH, rightH, hvalue);
    printf("%s: left height %d, right height %d, difference %d, not balanced\n",stem->treeName,leftH, rightH, hvalue);
  }

  else{
    fprintf(file,"%s: left height %d, right height %d, difference %d, balanced\n",stem->treeName,leftH, rightH, hvalue);
    printf("%s: left height %d, right height %d, difference %d, balanced\n",stem->treeName,leftH, rightH, hvalue);
  }
  
}

// looks for the name in the tree 
void find(treeNameNode *root){
  // variable declaration 
  char name[MAXLEN], nameitem[MAXLEN];
  treeNameNode *branchname = NULL;
  // takes input from the file 
  scanf("%s %s", name, nameitem);
  branchname = searchNameNode(root, name); // searches the name 

  // if the name doesnt exist 
  if(branchname == NULL){
    fprintf(file,"%s does not exist\n",name);
    printf("%s does not exist\n",name);
  }
  // looks for the name in the item side
  else{
    itemNode *branchitem = searchItems(branchname->theTree,nameitem);
    if(branchitem == NULL){
      fprintf(file,"%s not found in %s\n", nameitem,branchname->treeName);
      printf("%s not found in %s\n", nameitem,branchname->treeName);
    }
      else{
        fprintf(file,"%d %s found in %s\n",branchitem->count,branchitem->name,branchname->treeName);
        printf("%d %s found in %s\n",branchitem->count,branchitem->name,branchname->treeName);
    }
  }
}

// decreases the number of items in the tree
void decrease_count(treeNameNode *root){
  // variable declarion 
  char iname[MAXLEN], nameitem[MAXLEN];
  int count;
  // takes input 
  scanf("%s %s %d", iname, nameitem, &count);

  treeNameNode *branchname =  searchNameNode(root,iname);
  itemNode *branchitem = searchItems(branchname->theTree, nameitem);

  branchitem->count = branchitem->count - count;

  if(branchitem->count <= 0){
    fprintf(file,"%s reduced\n", branchitem->name);
    printf("%s reduced\n", branchitem->name);
    branchname->theTree =  removing(branchname->theTree,nameitem);
  }

  else{
    fprintf(file,"%s reduced\n",branchitem->name);
    printf("%s reduced\n", branchitem->name);
  }
}

// scans and determines the cout using hte other count functions 
void checkcount(treeNameNode *root){
  // variable declaratio 
  char name[MAXLEN];

  scanf("%s",name);
  // searches nad counts based of data scanned 
  treeNameNode *branch = searchNameNode(root,name);
  int countBr = count(branch->theTree);

  fprintf(file,"%s count %d\n",branch->treeName,countBr);
  printf("%s count %d\n",branch->treeName,countBr);
}

// Counts everything alphabetically  and adds em 
int counter(itemNode *root, char name[MAXLEN]){
  int c = 0;

  if(root == NULL){
    return 0;
  }
  else if (strcmp(root->name, name) < 0){
    c++;
    c += counter(root->left, name);
    c += counter(root->right, name);
  }
  else{
    c += counter(root->left, name);
  }
  return c;
}

// using the other fucntions it prints and compares the counts 
void count_before(treeNameNode *root){
  // variable declaration 
  char name[MAXLEN], nameitem[MAXLEN];
  int c = 0;

  scanf("%s %s",name, nameitem);
  // searches the tree 
  treeNameNode *branch =  searchNameNode(root,name);
  itemNode *branchitem = searchItems(branch->theTree,nameitem);
  // calls to count 
  c = counter(branch->theTree, nameitem);
  // prints 
  fprintf(file,"item before %s: %d\n",branchitem->name,c);
  printf("item before %s: %d\n",branchitem->name,c);
}

// removes the item from item node
itemNode* removing(itemNode *root,char name[MAXLEN]){
  if(root!=NULL){
    if(strcmp(name,root->name) < 0){
      root->left = removing(root->left,name);
    }
    else if(strcmp(name, root->name) > 0){
      root->right = removing(root->right,name);
    }
    else{
      if(root->left == NULL){
        itemNode *branchitem = root->right;
        free(root);
        return branchitem;
      }
      else if(root->right == NULL){
        itemNode *stemitem = root->left;
        free(root);
        return stemitem;
      }
      else{
      itemNode *subitem = minValItemNode(root->right);
      strcpy(root->name,subitem->name);
      root->count = subitem->count;
      root->right = removing(root->right, subitem->name);
      }
    }
    return root;
  }
  else{
    return root;
  }
}

// removes the item from the treeNameNode. 
void removeitem(treeNameNode * root){
  // declaring and storing variables
  char rname[MAXLEN], iname[MAXLEN], del[MAXLEN];
  scanf("%s %s",rname, iname);
  // searches for the tree and items 
  treeNameNode *tree = searchNameNode(root, rname);
  itemNode *item =  searchItems(tree->theTree, iname);
  // removes and prints the info 
  strcpy(del, item->name);
  tree->theTree = removing(tree->theTree, iname);

  fprintf(file,"%s deleted from %s\n",del, tree->treeName);
  printf("%s deleted from %s\n",del, tree->treeName);
}


// deletes the name 
treeNameNode *delete_name(treeNameNode *root, char name[MAXLEN]){
  if(root == NULL) return root; // returns root if NULL
  // compares the words and to search and delete the proper word 
  if(strcmp(name, root->treeName) < 0){
    root->left = delete_name(root->left, name);
  }
  else if (strcmp(name,root->treeName) > 0){
    root->right = delete_name(root->right,name);
  }
  else{
    if(root->left == NULL){
      treeNameNode *branchr = root->right;
      free(root);
      return branchr;
    }
    else if(root->right == NULL){
      treeNameNode *branchl = root->left;
      free(root);
      return branchl;
    }
    else{
      treeNameNode *branch = minValTreeName(root->right);
      strcpy(root->treeName,branch->treeName);
      root->right = delete_name(root->right, branch->treeName);
    }
  }
  return root;
}

// deletes the tree 
void delete_tree(treeNameNode *root){
  char treeN[MAXLEN], name[MAXLEN];

  scanf("%s",name);
  treeNameNode* branch = searchNameNode(root,name);

  strcpy(treeN, branch->treeName);
  free_itemNode(branch->theTree);
  root = delete_name(root,name);

  fprintf(file,"%s deleted\n",treeN);
  printf("%s deleted\n",treeN);
  
}


// prints in order  
void treePrint_treeNameNode(treeNameNode* root){ //Printtreenamenode
  if(root!= NULL){
    treePrint_treeNameNode(root->left);
    
    fprintf(file,"%s ", root->treeName);// prints on file 
    printf("%s ", root->treeName);// prints on console 
    
    treePrint_treeNameNode(root->right);
  }
}
// prints in order 
void treePrint_itemNode(itemNode* root){ //ptintitemtree
  if(root!=NULL){
    treePrint_itemNode(root->left);
    
    fprintf(file,"%s ", root->name);
    printf("%s ", root->name);
    
    treePrint_itemNode(root->right);
  }
}

// free the item node 
void free_itemNode(itemNode* root){
  if(root != NULL){
    free_itemNode(root->left);
    free_itemNode(root->right);
    
    free(root);
  }
}

// frees the memory for the treeNameNode
void free_treeNameNode(treeNameNode* root){
  if(root != NULL){ 
    free_treeNameNode(root->left);
    free_treeNameNode(root->right);
    
    free_itemNode(root->theTree);
    
    free(root);
  }
}




