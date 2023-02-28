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

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item 
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	DLList *L;

	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// time complexity: asuume the value of int n is 'n', this function runs in O(n) time
char * s_gets( char * st ,int n )
{
	char * ret_val;
	int i = 0;
	
	ret_val = fgets(st, n, stdin);
	if (ret_val)
	{
		while (st[i] != '\n' && st[i] != '\0')
			i++;
		if (st[i] == '\n')
			st[i] = '\0';
		else
			while(getchar() != '\n')
				continue;
				
	}
	
	return ret_val;
}

// create a DLList from a text file
// Time complexity: assume the size is n, this function runs in O(n) time.
DLList *CreateDLListFromFileDlist(const char *filename)
{
	int number;						
	char data[MAX];					
	DLList * Lptr = newDLList();	
	if (strcmp(filename, "stdin") == 0)	
	{
		while (1)		
		{
			s_gets(data, MAX); //get the value as type string 
			number = atoi(data); // convert to int
			if (strcmp(data, "end") == 0) // quit condition 
			{
				//printf("\nEnd of input!\n");
				break;
			}
			else if (!number && (strcmp(data,"0")!= 0))  // check whether the input is an in integer 
			{
				printf("Invalid input!\n");
				return NULL;
			}
			
			else
			{
				DLListNode * body = newDLListNode(number);
				if (Lptr -> size == 0) // write the head node
				{
					Lptr -> first = body;
					Lptr -> last = body;
					Lptr -> size++; 
				}
				else // insert each value as a tail
				{
					Lptr -> last -> next = body;
					body -> prev = Lptr -> last;
					Lptr -> last = body;
					Lptr ->size++;
				}	
			}		
		}
	}
	else
	{
		FILE* fp = fopen(filename,"r"); //open a file in "reading" mode
		if (!fp )
		{
			printf("No such file!");
		}
		else
		{
			while (fscanf(fp, "%d", &number) == 1)
			{
				DLListNode * body = newDLListNode(number);
				if (Lptr -> size == 0) // write the head node
				{
					Lptr -> first = body;
					Lptr -> last = body;
					Lptr -> size++; 
				}
				else // insert each value as a tail
				{
					Lptr -> last -> next = body;
					body -> prev = Lptr -> last;
					Lptr -> last = body;
					Lptr ->size++;
				}	
			}	
		}			
	}	
  	return Lptr;
}

// clone a DLList
// time complexity : assume the size is n, this function runs in O(n) time.
DLList *cloneList(DLList *u)
{
	DLList * newList = newDLList();
	if (u -> size == 0)		//empty list
		return newList;
	else
	{
		DLListNode * LLptr = u -> first;
		while((newList -> size) < (u -> size))
		{
			int numberCpy = LLptr -> value; 
			DLListNode * newBody = newDLListNode(numberCpy);
			if ( newList -> size == 0)  //create a head
			{
				newList -> first = newBody;
				newList -> last = newBody;
				LLptr = LLptr -> next;
				newList -> size++;
			}
			else					   // insert the new node as a tail
			{
				newList -> last -> next = newBody;
				newBody -> prev = newList -> last;
				newList -> last = newBody;
				LLptr = LLptr -> next ;
				newList ->size++;				
			}	
		}
		return newList;						
	}
}

// time complexity: assume the smaller value is a constant, this function runs in O(1) time.
int GCD(int a, int b) 
{
	if (a == b)
	{
		return 0;
	}
	int smaller = abs(a) < abs(b) ? abs(a) : abs(b);		//find the smaller one of a and b
	int i;
	int k = 0;
	for (i = 2; i <= smaller; i++)			//find the gcd of a and b from 2 to the smaller value
	{
		if ((a % i ==0) && (b % i == 0))
		{
			k = i;
		}			
	}
	return k;	
} 

//time complexity��  assume the size is n, this function runs in O(n) time. 
DLList * dividedList(DLList * u, int m)	
{
	DLList * dvdL = cloneList(u);
	DLListNode * ptr = dvdL -> first;
	while(ptr)					// devide each value of the list by m
	{
		ptr -> value = (ptr -> value) % m;
		ptr = ptr -> next;	
	}
	return dvdL;
	
}

// time complexity: assume the size is n, this function runs in O(n) time.
int zeroCounter(DLList * u) 
{
	int count = 0;
	DLListNode * ptr = u -> first;
	while(ptr)
	{
		if (ptr -> value == 0)
		{
			count++;
			ptr = ptr -> next;
		}
		else
		{
			ptr = ptr -> next;
		}
	}
	return count;	
}

//time complexity: assume the size is n,this function runs in O(n) time.
DLList * append (DLList * u, int i) 		//if the value of a node is devisible by i, append it to a new DLList
{
	DLList * L = newDLList();
	DLListNode * ptr = u -> first;
	if (i == 0)
	{
		L -> first = ptr;
		L -> last = ptr;
		L -> size++;
	}
	else
	{
		while(ptr)
		{
			if (((ptr -> value) % i) == 0 )
			{
				if (L -> size == 0)
				{
					L -> first = ptr;
					L -> last = ptr;
					L -> size++;
				}
				else 
				{
					L -> last -> next = ptr;
					ptr -> prev = L -> last;
					L -> last = ptr;
					L ->size++;
				}
				ptr = ptr -> next;
			}
			else
			{
				ptr = ptr -> next;
			}
		
		}
	}
	return L;			
}

// compute the longest sublist
/*time complexity: asuume the size is n, the GCD() is O(1), the devidedList() is O(n),the zeroCounter() is O(n), 
  this function runs in O(n^3) time. */
DLList *longestSublist(DLList *u)
{
	DLListNode * temp1 = u -> first;
	
	int divisor = 0;		// the value of the gcd of two nodes' values
	int maxDivisor = 0;		// the gcd of two nodes' values that is devisible by most of the nodes' value
	int maxZcount = -1;		// the maximum of count of zeros by deviding the nodes' values by the gcd of two nodes' value
	while (temp1)		// the 
	{
		DLListNode * temp2 = u -> first;
		while (temp2)
		{
			if(GCD(temp1 -> value, temp2 -> value))
			{
				divisor = GCD(temp1 -> value, temp2 -> value);
				DLList * zeroL = dividedList(u, divisor);
				int count = zeroCounter(zeroL);
				if (count > maxZcount)// find the maxZcount and maxDivisor
				{
					maxZcount = count;
					maxDivisor = divisor;
					
				}
				temp2 = temp2 -> next;
			}
			else
			{
				temp2 = temp2 -> next;
			}
			
		}
		
		temp1 = temp1 -> next;
	}
	
	return append(u, maxDivisor);
	
}

//time complexity: assume the size is n, this function runs in O(n) time.
int FindNode(DLList * u, int data)
{
	DLListNode * ptr = u -> first;
	while (ptr)
	{
		if (ptr -> value == data)
		{
			return 1;
		}
		else
		{
			ptr = ptr -> next;
		}
	}
	return 0;
	
}


// compute the union of two DLLists u and v
// time complexity: assume the size is n, the FindNode() is O(n), this function runs in O(n^2) time.
DLList *setUnion(DLList *u, DLList *v)
{
	DLList * UnionL = cloneList(u);
	DLListNode * ptr = v -> first;
	while (ptr)
	{	
		int data = ptr -> value;
		DLListNode * addTail = newDLListNode(data);
		if (!(FindNode(u, data)))
		{
			
			UnionL -> last -> next = addTail;
			addTail -> prev = UnionL -> last;
			UnionL -> last = addTail;
			UnionL ->size++;
			ptr = ptr -> next;	  
		}
		else
		{
			ptr = ptr -> next;	
		}	
	}
	
	return UnionL;
	
}

// compute the insection of two DLLists u and v
// time complexity: assume the size is n, the FindNode() is O(n), this function runs in O(n^2) time.
DLList *setIntersection(DLList *u, DLList *v)
{
	DLList * InterL = newDLList();
	DLListNode * ptr = v -> first;
	while (ptr)
	{	
		int data = ptr -> value;
		if ((FindNode(u, data)))
		{
			DLListNode * addTail = newDLListNode(data);
			if (InterL -> size == 0) //creat a head
			{
				InterL -> first = addTail;
				InterL -> last = addTail;
				InterL -> size++;		
			}
			else
			{
				InterL -> last -> next = addTail;
				addTail -> prev = InterL -> last;
				InterL -> last = addTail;
				InterL -> size++;	
			}
			
		}
		ptr = ptr -> next;
		
	}
	
	return InterL;

 
}

// free up all space associated with list
// time complexity : assume the size is n, this function runs in O(n) time.
void freeDLList(DLList *L)
{
	DLListNode * freeNode = L -> first;
	while(freeNode)
	{
		freeNode = freeNode -> next;
		free(L -> first);
		L -> first = freeNode;
		
	}
	

}


// display items of a DLList
// time complexity: assume the size is n, this function runs in O(n) time.
void printDLList(DLList *u)
{
	DLListNode * ptr;
	int count = 0;
	int i = u -> size;
	ptr = u -> first;
	while (count < i)
	{
		printf("%d\n", ptr -> value);
		ptr = ptr -> next;
		count++; 
	}

}

int main()
{
 DLList *list1, *list2, *list3, *list4;

 list1=CreateDLListFromFileDlist("File1.txt");
 printDLList(list1);

 list2=CreateDLListFromFileDlist("File2.txt");
 printDLList(list2);

 list3=setUnion(list1, list2);
 printDLList(list3);

 list4=setIntersection(list1, list2);
 printDLList(list4);

 printDLList(longestSublist(list4));

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 printf("please type all the integers of list1\n");
 list1=CreateDLListFromFileDlist("stdin");

 printf("please type all the integers of list2\n");
 list2=CreateDLListFromFileDlist("stdin");

 list3=cloneList(list1);
 printDLList(list3);
 list4=cloneList(list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 return 0; 
}
