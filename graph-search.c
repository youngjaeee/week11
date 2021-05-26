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
void iterativeInorder(Node* node) // 반복을 통한 중외순회 함수
{
	int top = -1; // stack의 top = -1로 초기화
	for (;;) // 모든 노드의 탐색을 완료할 때까지 반복
	{
		for (; node; node = node->left) // root부터 반복하여 좌측 자식 노드까지
			push(node); // 노드를 반복하여 push함
		node = pop(); // for문을 돌아 단말 노드까지 push를 마친 경우 node = pop();

		if (!node) break; // node가 NULL일 경우 for문 벗어남

		printf("[%d] ", node->key); // node가 NULL이 아닐 경우 key값 출력
		node = node->right; // node가 우측 자식 노드 가리키게 하여 다시 이중 for문 실행
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{
	int front = -1;
	int rear = -1; // 레벨 순서 순회함수 동작을 위해 쓰이는 queue 초기화

	if (!ptr) return; // NULL BST일 경우 함수 종료
	enQueue(ptr); //queue에 ptr, 즉 root을 enqueue함

	for (;;) // 모든 노드에 대한 탐색을 마칠 때까지
	{
		ptr = deQueue(); // dequeue한 값을 ptr에 대입
		if (ptr) // ptr이 NULL이 아닐 경우
		{
			printf("[%d] ", ptr->key); // ptr의 key값 출력
			if (ptr->left)
				enQueue(ptr->left); // ptr에 좌측 자식 노드가 있을 경우 enqueue
			if (ptr->right)
				enQueue(ptr->right); // ptr에 우측 자식 노드가 있을 경우 enqueue
		}
		else break;
	}
}





int deleteNode(Node* head, int key) // 입력한 key값과 동일한 값을 가지는 노드를 지우는 함수
{
	Node* ptr = head->left; // 노드 포인터 ptr 선언, root 가리킴
	Node* preptr = head; // ptr의 부모 포인터 preptr 선언 및 초기화

	while (ptr) // ptr != NULL일 동안
	{
		if (key == ptr->key) // 동일한 key값을 가지는 ptr 탐색을 성공했을 경우
			break; // while문 벗어남
		if (key < ptr->key) // 사용자가 입력한 key값이 ptr->key 보다 작은 경우
		{
			preptr = ptr; // preptr을 ptr로 설정하고
			ptr = ptr->left; // ptr을 ptr의 좌측 자식 노드를 가리키게 설정
		}
		else
		{
			preptr = ptr; // 사용자가 입력한 key값이 ptr->key 보다 큰 경우 preptr을 ptr로 설정하고
			ptr = ptr->right; // ptr을 ptr의 우측 자식 노드를 가리키게 설정
		}
	}
	if (ptr == NULL) // ptr이 NULL일 경우, 즉 탐색을 실패한 경우 오류 메시지 출력
	{
		printf("BST에서 입력한 값 탐색을 실패했습니다.\n");
		return 0;
	}

	if (ptr->left == NULL && ptr->right == NULL) // 탐색하여 찾은 ptr이 단말 노드일 경우
	{
		if (preptr->key > ptr->key) // ptr의 key값이 preptr의 key값보다 작은 경우
		{
			preptr->left = NULL; // ptr은 preptr의 좌측 자식노드 이므로 preptr->left = NULL
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
		else if (preptr->key < ptr->key) // ptr의 key값이 preptr의 key값보다 큰 경우
		{
			preptr->right = NULL; // ptr은 preptr의 우측 자식노드 이므로 preptr->right = NULL
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
	}
	else if (ptr->left != NULL && ptr->right == NULL) // ptr이 좌측 자식 노드만 가지는 경우
	{
		if (preptr->key > ptr->key)  // ptr의 key값이 preptr의 key값보다 작은 경우
		{
			preptr->left = ptr->left; // ptr은 preptr의 좌측 자식노드이므로 preptr->left = ptr->left
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
		else if (preptr->key < ptr->key) // ptr의 key값이 preptr의 key값보다 큰 경우
		{
			preptr->right = ptr->left; // ptr은 preptr의 우측 자식노드이므로 preptr->right = ptr->left
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
	}
	else if (ptr->left == NULL && ptr->right != NULL) // ptr이 우측 자식 노드만 가지는 경우
	{
		if (preptr->key > ptr->key)  // ptr의 key값이 preptr의 key값보다 작은 경우
		{
			preptr->left = ptr->right; // ptr은 preptr의 좌측 자식노드이므로 preptr->left = ptr->right
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
		else if (preptr->key < ptr->key) // ptr의 key값이 preptr의 key값보다 큰 경우
		{
			preptr->right = ptr->right; // ptr은 preptr의 우측 자식노드이므로 preptr->right = ptr->right
			ptr->left = NULL;
			ptr->right = NULL;
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
	}

	else if (ptr->left != NULL && ptr->right != NULL) // ptr이 좌측, 우측 모두 자식 노드를 가질 경우
	{
		Node* minrightptr = ptr->right; // ptr의 우측 subtree에서 가장 작은 노드를 탐색하기 위한 노드 포인터 선언
		Node* preminrightptr = ptr; // 우측 subtree 중 최소값 가지는 노드의 부모 노드 포인터 선언

		while (1) // minrightptr이 NULL이 아닐 때까지
		{
			if (minrightptr->left != NULL) // 최소값을 찾기 위해 좌측 자식 노드 탐색 진행
			{
				preminrightptr = minrightptr; // minrightptr 값보다 작은 노드가 존재하는 경우 preminrightptr = minrightptr
				minrightptr = minrightptr->left; // minrightptr이 좌측 자식 노드 가리키게 함
			}
			else break; // 최소값을 찾은 경우 while문 벗어남
		}

		preminrightptr->left = minrightptr->left;
		minrightptr->left = ptr->left; // minrightptr의 좌측, 우측 자식노드를 기존 ptr의 자식노드로 재설정
		minrightptr->right = ptr->right;
		ptr->left = NULL;
		ptr->right = NULL;

		if (preptr->key > ptr->key)  // ptr의 key값이 preptr의 key값보다 작은 경우
		{
			preptr->left = minrightptr; // ptr은 preptr의 좌측 자식노드이므로 preptr->left = minrightptr
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}
		else if (preptr->key < ptr->key) // ptr의 key값이 preptr의 key값보다 큰 경우
		{
			preptr->right = minrightptr; // ptr은 preptr의 우측 자식노드이므로 preptr->right = minrightptr
			freeNode(ptr); // ptr에 대한 할당 해제 함수 호출
		}

	}


}


void printStack() // stack의 요소를 출력하는 함수
{
	if (top != -1)
	{
		for (int i = 0; i < top; i++)
			printf("[%d] ", stack[i]->key);
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

