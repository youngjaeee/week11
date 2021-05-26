/*
 * Graph, DFS, BFS #1
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#define _CRT_SECURE_NO_WARNINGS
#define MAX_VERTEX_SIZE 	10
#define MAX_STACK_SIZE		20 // MAX_STACK_SIZE를 20으로 설정
#define MAX_QUEUE_SIZE		20 // MAX_QUEUE_SIZE를 20으로 설정

#include <stdio.h>
#include <stdlib.h>



typedef struct node { // 트리의 노드 구조체
	int vertex;
	struct node* link;
} Node;


Node graph[MAX_VERTEX_SIZE];
int visited[MAX_VERTEX_SIZE];
int ismade = 0;


/* for stack */

Node* stack[MAX_STACK_SIZE]; // MAX_STACK_SIZE 만큼 노드 구조체 포인터 배열을 할당
int top = -1; // top = -1로 초기화

Node* pop(); // stack의 pop()함수 선언
void push(Node* aNode); // stack의 push()함수 선언

/* for queue */

Node* queue[MAX_QUEUE_SIZE]; // MAX_QUEUE_SIZE만큼 노드 구조체 포인터 배열을 할당
int front = -1; // front = -1로 초기화
int rear = -1; // rear = -1로 초기화

Node* deQueue(); // queue의 deQueue() 함수 선언
void enQueue(Node* aNode); // queue의 enQueue() 함수 선언



int initializeGraph(Node* h); // BST 초기화 함수 선언

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
int DFS(Node* graph, int key);
int BFS(Node* graph, int key);
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* graph, int key);      /* insert a node to the tree */
int insertEdge(Node* graph, int key, int key2);
int insertList(Node* graph, int key);
int deleteNode(Node* head, int key);  /* delete the node for the key */
void freeNode(Node* ptr);
int freeGraph(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */


void printGraph(); // stack을 출력하는 함수 선언



int main()
{
	for(int i = 0; i < MAX_VERTEX_SIZE; i++)
	graph[i].vertex=-1;

	char command; // 사용자가 선택한 메뉴값 저장하는 변수
	int key; // 사용자가 탐색할 노드의 키값 저장하는 변수
	int key2; // 두개의 키값을 입력해야할 때 쓰는 변수
	Node* head = NULL; // BST의 head = NULL로 초기화

	do {
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                         Graph Searches                         \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Graph       = z                                       \n");
		printf(" Insert Vertex          = i       Insert Edge              = e \n");
		printf(" Depth First Search     = d        Breathe First Search    = b \n");
		printf(" Print Graph            = p        Quit                    = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'z': case 'Z':
			initializeGraph(graph); // BST 초기화 함수 호출
			break;
		case 'q': case 'Q':
			freeBST(head); // BST 할당 해제 함수 호출
			break;
		case 'i': case 'I':
			printf("Your Vertex = "); // Vertex 삽입을 선택하였을 경우
			scanf("%d", &key); // 삽입할 key 값을 입력받아
			insert(graph, key); // insert() 함수 호출
			break;
		case 'e': case 'E':
			printf("edge를 삽입할 Vertex A & Vertex B 값을 입력하세요.\n");
			printf("Vertex A: "); // edge 삽입을 선택하였을 경우
			scanf("%d", &key); // Vertex A의 key값 입력받음
			printf("Vertex B: ");
			scanf("%d", &key2); // Vertex A의 key값 입력받음
			insertEdge(graph, key, key2);
			break;

		case 'd': case 'D':
			printf("탐색을 시작할 정점 Vertex 값을 입력하세요.\n");
			scanf("%d", &key);
			DFS(graph, key); // 재귀를 통한 중위순회 함수 호출
			break;
		case 'b': case 'B':
			printf("탐색을 시작할 정점 Vertex 값을 입력하세요.\n");
			BFS(graph, key);
			scanf("%d", &key);
			break;

		case 'p': case 'P':
			printStack(); // stack 출력 함수 호출
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

int initializeGraph(Node* h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if (ismade != 0) // NULL 트리가 아닌 경우
		freeGraph(h); // BST에 대한 할당 해제 함수 호출

	top = -1; // 반복을 통한 중위순회 함수에서 쓰일 stack의 top = -1로 초기화

	front = rear = -1; //  레벨순회 함수에서 쓰일 queue의 front, rear = -1로 초기화

	return 1;
}

int freeGraph(Node* head) // Graph 모든 vertex 관계 삭제 함수
{


	for(int i = 0; i < MAX_VERTEX_SIZE; i++)
	{
		Node* temppointer = head + i;
		while(temppointer->link!=NULL)
		{
			Node* tofree = temppointer;
			tofree->vertex=-1;
			temppointer=temppointer->link;
			free(tofree);
		}
	}
	return 1;
}


int insert(Node* graph, int key) // BST 삽입 함수, 매개변수로 head 포인터와 삽입할 노드의 key값 받음
{
	if(graph[key].vertex == key)
	{
		printf("이미 삽입된 Vertex입니다.\n");
		return 2;
	}
	else
	{
		graph[key].vertex = key;
		return 1;
	}
}

int insertEdge(Node* graph, int key, int key2)
{
	if(key < 0 || key > 9 || key2 < 0 || key2 > 9)
		{
			printf("입력한 Vertex 값이 0~9 범위를 초과합니다.\n");
			return 2;
		}
	if(graph[key].vertex == -1)
		{
			printf("Vertex A가 사전에 insert 되지 않았습니다.\n");
			return 2;
		}
	if(graph[key2].vertex == -1)
		{
			printf("Vertex B가 사전에 insert 되지 않았습니다.\n");
			return 2;
		}
	if (key == key2)
		{
			printf("자기 간선을 입력하였습니다.\n");
			return 2;
		}

	insertList(graph+key, key2);
	insertList(graph+key2, key);
}

int insertList(Node* graph, int key)
{
	int i = 0;
	Node* temppointer = graph;
	while(temppointer->link!=NULL)
	{
		if(key>temppointer->vertex)
			{
				Node* added = (Node*)malloc(sizeof(Node*));
				added->vertex = key;
				added->link = temppointer->link->link;
				temppointer->link = added;
				return 1;
			}
		else temppointer = temppointer->link;
	}
	if(temppointer->link == NULL)
	{
		Node* added = (Node*)malloc(sizeof(Node*));
		added->vertex = key;
		added->link = NULL;
		temppointer->link = added;
		return 1;
	}

}


int DFS(Node* graph, int key)
{
	int tempvalue =-1;
	int top = -1;
	Node* nodepointer = graph;
	if(key < 0 || key > 9)
		{
			printf("입력한 Vertex 값이 0~9 범위를 초과합니다.\n");
			return 2;
		}
	if(graph[key].vertex == -1)
		{
			printf("해당 Vertex는 사전에 insert 되지 않았습니다.\n");
			return 2;
		}
	for(int i = 0; i < MAX_VERTEX_SIZE; i++)
		visited[i] = 0;

	push(graph+key);
	visited[key] = 1;
	printf("[ %d ] ", key);

	for(;;)
	{
		nodepointer = nodepointer->link;

		while(nodepointer != NULL)
		{
			if(visited[nodepointer->vertex] != 1)
				{
					push(nodepointer);
					visited[nodepointer->vertex] = 1;
					printf("[ %d ] ", nodepointer->vertex);
				}
		}

	}	
}


int BFS(Node* graph, int key)
{
	if(key < 0 || key > 9)
		{
			printf("입력한 Vertex 값이 0~9 범위를 초과합니다.\n");
			return 2;
		}
	if(graph[key].vertex == -1)
		{
			printf("해당 Vertex는 사전에 insert 되지 않았습니다.\n");
			return 2;
		}

	Node* nodepointer;
	front = -1;
	rear = -1;

	printf("[ %d ] ", key);
	visited[key] = 1;
	enQueue(graph+key);

	while(front != rear)
	{
		key = deQueue()->vertex;
		for(nodepointer = graph+key; nodepointer; nodepointer = nodepointer->link)
		{
			if(visited[nodepointer->vertex] == 0)
			{
				printf("[ %d ] ", nodepointer->vertex);
				visited[nodepointer->vertex] = 1;
				enQueue(nodepointer->vertex);
			}
		}
	}
}


/**
 * textbook: p 224s
 */


void printGraph() // stack의 요소를 출력하는 함수
{
	printf("입력된 Vertex 목록\n");
	for(int i = 0; i < MAX_VERTEX_SIZE; i++)
		{
			if(graph[i].vertex != -1)
				printf("[ %d ] ", i);
		}
	printf("\n");

	printf("입력된 Graph Adjacent List\n");
	Node* point;
	for(int i = 0; i < MAX_VERTEX_SIZE; i++)
	{
		point = graph+i;
		if(point->vertex == -1)
			continue;
		printf("adjList[%d] : ", i);
		while(1)
		{
			if(point->link == NULL)
				{
					printf("[ %d ]\n");
					break;
				}
			else
			{
				point = point->link;
			}
		}
		printf("\n");
	}

}



Node* pop() // stack에서의 pop()함수 구현
{
	if (top < 0)
		return NULL;
	else
	{
		return(stack[top--]);
	}
}

void push(Node* aNode) // stack에서의 push()함수 구현
{
	if (top >= MAX_STACK_SIZE - 1)
	{
		fprintf(stderr, "Stack is full, cannot add element\n"); // 최대 용량을 초과하였을 경우 오류메시지 출력
		exit(EXIT_FAILURE);
	}
	else
	{
		stack[++top] = aNode;
	}
}



Node* deQueue() // Queue의 delete 기능 함수
{
	if (front == rear)
		return NULL;
	return queue[++front];
}

void enQueue(Node* aNode) // Queue의 insert 기능 함수
{
	if (rear == MAX_QUEUE_SIZE - 1)
	{
		fprintf(stderr, "Queue is full, cannot add element\n");
		exit(EXIT_FAILURE);
	}
	queue[++rear] = aNode;

}

