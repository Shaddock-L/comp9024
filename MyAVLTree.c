// Author: Leyang Li
// Student ID: z5285799
// Platform: Windows
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define MAX 255

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 0; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}
// find the greater one of 2 value
// O(1)
int Max(int i, int j)
{
	return i > j ? i : j;
}

//find the height of a node 
//O(1)
int Height(AVLTreeNode * node)
{
	if (node == NULL)
		return -1;
	else
		return node->height;
}


AVLTreeNode *Search(AVLTree *T, int k, int v);
AVLTreeNode* RecursiveInsert(AVLTreeNode * node, int k, int v);
int InsertNode(AVLTree *T, int k, int v);
int Height(AVLTreeNode * node);
AVLTreeNode* SIGLRot(AVLTreeNode * node);
AVLTreeNode* SIGRRot(AVLTreeNode * node);
AVLTreeNode* DBLRot(AVLTreeNode * node);
AVLTreeNode* DBRRot(AVLTreeNode * node);
int Position(AVLTreeNode * node, int k, int v);
AVLTreeNode * RecursiveDelete(AVLTreeNode * root, int k, int v);
void PrintAVLTree(AVLTree *T);




/* Assuming the size is n. The InsertNode() function runs in O(log(n)) time. So the CreateAVLTree()function runs in O(nlog(n)) time.*/
AVLTree *CreateAVLTree(const char *filename)
{
	AVLTree * tree = newAVLTree();
	
	int key, value, i;
	
	if (strcmp(filename, "stdin") == 0)
	{
		printf("Enter the data(enter a new line to quit): \n");
		char data[MAX];
		while (fgets(data, MAX, stdin) != NULL && data[0] != '\n')
		{
			int move_forward = 0;
			int strlen = 0;
			
			while(1)
			{
				if (sscanf(data + move_forward, " %*[(]%d%*[,]%d%*[)]%n", &key, &value,&strlen) == 2)
				{
					move_forward += strlen;
					i = InsertNode(tree, key, value);
					tree->size++;
					
					//printf("key is : %d, value is : %d\n", key, value);
				}
				else
					break;
			}

		}

	}
	else
	{
		char path[MAX] = "./";
		strcat(path, filename);
		FILE * fp = fopen(path, "r");
		char lbrac, rbrac, com;
		if (!fp)
		{
			printf("No such file!\n");
			exit(0);
		}
		while ((fscanf(fp,"%c%d%c%d%c",&lbrac, &key, &com, &value,&rbrac)) != EOF)
		{
			//printf("key is : %d, value is : %d\n", key, value);
			i = InsertNode(tree, key, value);
			tree->size++;
		}
		tree->size = tree->size /2;
		fclose(fp);
	}
	
	return tree;

  
}


/* Assuming the size is n, this function runs in O(n) time.         */
AVLTreeNode * CloneNode(AVLTreeNode * iniRoot)
{
	if (iniRoot != NULL)
	{
		//height, left, right, parent
		AVLTreeNode * NewRoot = newAVLTreeNode(iniRoot->key,iniRoot->value);
		NewRoot->height = iniRoot->height;
		NewRoot->left = iniRoot->left;
		NewRoot->right = iniRoot->right;
		NewRoot->parent = iniRoot->parent;
		
		if(iniRoot->left != NULL)
			NewRoot->left = CloneNode(iniRoot->left);
		if(iniRoot->right != NULL)
			NewRoot->right = CloneNode(iniRoot->right);
			
		return NewRoot;
		
	}
	return NULL;
	
}


/* The time complexity is same as AVLTreeNode * CloneNode(), this function runs in O(n) time.*/
AVLTree *CloneAVLTree(AVLTree *T)
{
	AVLTree * cloneT = newAVLTree();
	cloneT->root = CloneNode(T->root);
	
	return cloneT;
}
 

/*  Assuming the size of T1 is m, the size of T2 is n.
	InsertNode() runs in O(log(m)) time.
	Thus, this function runs in O(nlog(m)) time.           */
void AddTreeNode(AVLTree *T1, AVLTree *T2)
{
		
	AVLTree * subL = newAVLTree();
	AVLTree * subR = newAVLTree();
	if (T2->root == NULL)
		return;
	subL->root = T2->root->left;
	subR->root = T2->root->right;
	
	AddTreeNode(T1, subL);
	InsertNode(T1, T2->root->key, T2->root->value);
	AddTreeNode(T1, subR);
	
}
/*  Assuming the size of T1 is m, the size of T2 is n.
	CloneAVLTree() runs in O(m) time.
	AddTreeNode() runs in O(nlog(m)) time.
	Thus, this function runs in O(m + nlog(m)) time as we do not know Max(m,n).*/
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	AVLTree * UnionTree = CloneAVLTree(T1);
	AddTreeNode(UnionTree, T2);
	return UnionTree;
	 
}

/*
	Assuming the size of T1 and T2 and T3 are m, n and k.
	Search() runs in O(logm).
	InsertNode() runs in O(log(k))
	Thus, this function runs in O(nlog(m)log(k)) time.

*/
void AddTreeNodeWithoutDup(AVLTree *T1, AVLTree *T2, AVLTree *T3)
{		
	AVLTree * subL = newAVLTree();
	AVLTree * subR = newAVLTree();
	if (T2->root == NULL)
		return;
	subL->root = T2->root->left;
	subR->root = T2->root->right;
	
	AddTreeNodeWithoutDup(T1, subL, T3);
	if (Search(T1, T2->root->key, T2->root->value) != NULL)
	{
		InsertNode(T3, T2->root->key, T2->root->value);
	}
		
	AddTreeNodeWithoutDup(T1, subR, T3);

}
 
 /*
 	Assuming the size of T1 and T2 and InsTree are m, n and k.
 	AddTreeNodeWithoutDup runs in O(nlog(m)log(k)) time.
 	Thus this function runs in O(nlog(m)log(k)) time.

 */
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	 AVLTree * InsTree = newAVLTree();
	 AddTreeNodeWithoutDup(T1, T2, InsTree);
	 return InsTree;
}


//O(1)
AVLTreeNode* SIGLRot(AVLTreeNode * node)
{
	AVLTreeNode * pivot = node->left;
	node->left = pivot->right;
	pivot->right = node;
	node->height = Max(Height(node->left), Height(node->right)) + 1;
	pivot->height = Max(Height(pivot->left), node->height) + 1;
	
	return pivot;
}
//O(1)
AVLTreeNode* SIGRRot(AVLTreeNode * node)
{
	AVLTreeNode * pivot = node->right;
	node->right = pivot->left;
	pivot->left = node;
	node->height = Max(Height(node->left), Height(node->right)) + 1;
	pivot->height = Max(Height(pivot->right), node->height) + 1;
	
	return pivot;
	
}
//O(1)
AVLTreeNode* DBLRot(AVLTreeNode * node)
{
	AVLTreeNode * newRoot;
	node->left = SIGRRot(node->left);
	newRoot = SIGLRot(node);
	return newRoot;
	
}
//O(1)
AVLTreeNode* DBRRot(AVLTreeNode * node)
{
	AVLTreeNode * newRoot;
	node->right = SIGLRot(node->right);
	newRoot = SIGRRot(node);
	return newRoot;
	
}


// 1 means left, 2 means right
//O(1)
int Position(AVLTreeNode * node, int k, int v)
{
	if (k < node->key)
		return 1;
	else if (k == node->key)
	{
		if (v < node->value)
			return 1;
		else if(v > node->value)
			return 2;
	}
	else if (k > node->key)
		return 2;
}

/*
	Assuming the number of the posterity of the node is n.
	This function runs in O(logn) time.
*/
AVLTreeNode* RecursiveInsert(AVLTreeNode * node, int k, int v)
{
	AVLTreeNode * istNode = newAVLTreeNode(k, v);
	//create a root if the root does not exist
	if (node == NULL)		
	{
		node = istNode;
	}
	if (k < node->key)		//insert into the left subtree
	{
		node->left = RecursiveInsert(node->left, k, v);
		if (Height(node->left) - Height(node->right) == 2)
		{
			if (Position(node->left, k, v) == 1)
				node = SIGLRot(node);
			else if (Position(node->left, k, v) == 2)
				node = DBLRot(node);
		}
	}
	else if (k == node->key)
	{
		if (v < node->value)		//insert into the left subtree
		{
			node->left = RecursiveInsert(node->left, k, v);
			if (Height(node->left) - Height(node->right) == 2)
			{
				if (Position(node->left, k, v) == 1)
					node = SIGLRot(node);
				else if (Position(node->left, k, v) == 2)
					node = DBLRot(node);
			}
		}
		else if (v > node->value)		//insert into the right subtree
		{
			node->right = RecursiveInsert(node->right, k, v);
			if (Height(node->right) - Height(node->left) == 2)
			{
				if (Position(node->right, k, v) == 2)
					node = SIGRRot(node);
				else if (Position(node->right, k, v) == 1)
					node = DBRRot(node);
				
			}
		}
		
	}
	else if(k > node->key)		//insert into the right subtree
	{
		node->right = RecursiveInsert(node->right, k, v);
		if (Height(node->right) - Height(node->left) == 2)
		{
			if (Position(node->right, k, v) == 2)
				node = SIGRRot(node);
			else if (Position(node->right, k, v) == 1)
				node = DBRRot(node);
			
		}
	}
	node->height = Max(Height(node->left), Height(node->right)) + 1;
	return node;
}


/*
	Assuming the size of the tree is n.
	Search() runs in O(logn) time.
	RecursiveInsert() runs in O(logn) time.
	Thus, this function runs in O(logn) time.
*/
int InsertNode(AVLTree *T, int k, int v)
{
	if (T->root == NULL)
	{
		T->root = newAVLTreeNode(k,v); 
	}
	if (Search(T, k, v) != NULL)
		return 0;
	
	T->root = RecursiveInsert(T->root, k, v);
	
	return 1;
}

/*
	Assuming the number of the posterity of the root is n.
	This function runs in O(logn) time.
*/
AVLTreeNode * RecursiveDelete(AVLTreeNode * root, int k, int v)
{
	if (root == NULL)
		return NULL;
	if (k < root->key)
		root->left = RecursiveDelete(root->left, k, v);
	else if (k > root->key)
		root->right = RecursiveDelete(root->right, k, v);
	else if(k == root->key && v < root->value)
		root->left = RecursiveDelete(root->left, k, v);
	else if(k == root->key && v > root->value)
		root->right = RecursiveDelete(root->right, k, v);
	
	else if(k == root->key && v == root->value)
	{
		
		if (root->left == NULL && root->right ==NULL)
		{
			free(root);
			root = NULL;
		}
		//
		else if(root->left == NULL)
		{
			root = root->right;
			free(root->right);
		}
		else if (root->right == NULL)
		{
			root = root->left;
			free(root->left);
		}

	}
	if (root == NULL)
		return NULL;
	
	else
	
	{
		//left subtree is longer
		if (Height(root->left) - Height(root->right) == 2)
		{
			if (Height(root->left->left) >= Height(root->left->right))
				root = SIGLRot(root);
			else if (Height(root->left->left) <= Height(root->left->right))
				root = DBLRot(root);
			
		}
		else if (Height(root->right) - Height(root->left) == 2)
		{
			if (Height(root->right->left) <= Height(root->right->right))
				root = SIGRRot(root);
			else if (Height(root->right->left) >= Height(root->right->right))
				root = DBRRot(root);

		}
	}
	root->height = Max(Height(root->left), Height(root->right)) + 1;
	return root;
	
	
}



/*
	Assuming the size of T is n.
	Search() runs in O(logn) time.
	RecursiveDelete() runs in O(logn) time.
	Thus, this function runs in O(logn) time.
*/
int DeleteNode(AVLTree *T, int k, int v)
{
	if (Search(T, k, v) == NULL)
		return 0;
	T->size -= 1;
	T->root = RecursiveDelete(T->root, k, v);
	return 1;
		
}

/*	Assuming the Tree size is n.
	This function runs in O(logn) time	
*/
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
	AVLTreeNode * temp = T->root;
 	while(temp != NULL)
  	{
  		if (temp->key == k)
  		{
  			if (temp->value < v)
  			{
  				temp = temp->right;
			}
			else if (temp->value > v)
			{
				temp = temp->left;
			}
			else if (temp->value == v)
			{
				return temp;
			}
		}
		else if (temp->key > k)
		{
			temp = temp->left;
		}
  		else
  		{
  			temp = temp->right;	
		}
  	
  	}
  
  	return NULL;
  
  
}
/*
	Assuming the number of the posterity of the node is n.
	This function runs in O(n) time.
*/
void FreeNode(AVLTreeNode * node)
{
	if (node == NULL)
		return;
	FreeNode(node->left);
	FreeNode(node->right);
	free(node);	
	
}

/*
	Assuming the sizeof T is n.
	FreeNode() runs in O(n) time.
	Thus this function runs in O(n) time.
*/
void FreeAVLTree(AVLTree *T)
{
	//free all leaves
	FreeNode(T->root);
	//free the tree
	free(T);
}

/*
	Assuming the size of T is n.
	This function runs in O(n) time.
*/
void PrintAVLTree(AVLTree *T)
{
	AVLTree * subL = newAVLTree();
	AVLTree * subR = newAVLTree();
	if (T->root == NULL)
		return;
	
	subL->root = T->root->left;
	subR->root = T->root->right;
	PrintAVLTree(subL);
	printf("(%d %d),height: %d \n", T->root->key, T->root->value, T->root->height);
	PrintAVLTree(subR);


}

int main() //sample main for testing 
{ int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("stdin");
 PrintAVLTree(tree1);
 FreeAVLTree(tree1);
 //you need to create the text file file1.txt
 // to store a set of items without duplicate items
 tree2=CreateAVLTree("File1.txt"); 
 PrintAVLTree(tree2);
 tree3=CloneAVLTree(tree2);
 PrintAVLTree(tree3);
 FreeAVLTree(tree2);
 FreeAVLTree(tree3);
 //Create tree4 
 tree4=newAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
  PrintAVLTree(tree4);
  node1=Search(tree4,20,20);
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else 
    printf("Key 20 does not exist\n");
  
  for (i=17; i>0; i--)
  {
    j=DeleteNode(tree4, i, i);
	if (j==0) 
	  printf("Key %d does not exist\n",i);  
    PrintAVLTree(tree4);
  }
 FreeAVLTree(tree4);
 //Create tree5
 tree5=newAVLTree();
 j=InsertNode(tree5, 6, 25);
 j=InsertNode(tree5, 6, 10);
 j=InsertNode(tree5, 6, 12);
 j=InsertNode(tree5, 6, 20);
 j=InsertNode(tree5, 9, 25);
 j=InsertNode(tree5, 10, 25);
 PrintAVLTree(tree5);
 //Create tree6
 tree6=newAVLTree();
 j=InsertNode(tree6, 6, 25);
 j=InsertNode(tree6, 5, 10);
 j=InsertNode(tree6, 6, 12);
 j=InsertNode(tree6, 6, 20);
 j=InsertNode(tree6, 8, 35);
 j=InsertNode(tree6, 10, 25);
 PrintAVLTree(tree6);
 tree7=AVLTreesIntersection(tree5, tree6);
 tree8=AVLTreesUnion(tree5,tree6);
 PrintAVLTree(tree7);
 PrintAVLTree(tree8);
 return 0; 
}
