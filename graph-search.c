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
#define MAX_VERTEX_SIZE 	10 // Vertex 최대 사이즈를 10으로 설정
#define MAX_STACK_SIZE		20 // MAX_STACK_SIZE를 20으로 설정
#define MAX_QUEUE_SIZE		20 // MAX_QUEUE_SIZE를 20으로 설정

#include <stdio.h>
#include <stdlib.h>



typedef struct node { // 그래프의 노드 구조체
	int vertex;
	struct node* link;
} Node;


Node graph[MAX_VERTEX_SIZE]; // 그래프의 첫 부분을 저장하는 배열 선언
int visited[MAX_VERTEX_SIZE]; // 방문 기록을 저장하기 위한 배열 선언
int ismade = 0; // 오류 발생 방지를 위한 그래프 생성여부 저장 변수


/* for stack */

int stack[MAX_STACK_SIZE]; // MAX_STACK_SIZE 만큼 노드 구조체 포인터 배열을 할당
int top = -1; // top = -1로 초기화

int pop(); // stack의 pop()함수 선언
int push(int aNode); // stack의 push()함수 선언

/* for queue */

int queue[MAX_QUEUE_SIZE]; // MAX_QUEUE_SIZE만큼 노드 구조체 포인터 배열을 할당
int front = -1; // front = -1로 초기화
int rear = -1; // rear = -1로 초기화

int deQueue(); // queue의 deQueue() 함수 선언
int enQueue(Node* aNode); // queue의 enQueue() 함수 선언



int initializeGraph(Node* h); // 그래프 초기화 함수 선언

/* functions that you have to implement */
int DFS(Node* graph, int key);
int BFS(Node* graph, int key);
int insert(Node* graph, int key);      /* insert a node to the tree */
int insertEdge(Node* graph, int key, int key2);
int insertList(Node* graph, int key);
void freeNode(Node* ptr);
int freeGraph(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */


void printGraph(); // stack을 출력하는 함수 선언



int main()
{
	for (int i = 0; i < MAX_VERTEX_SIZE; i++)
		graph[i].vertex = -1;

	char command; // 사용자가 선택한 메뉴값 저장하는 변수
	int key; // 사용자가 탐색할 노드의 키값 저장하는 변수
	int key2; // 두개의 키값을 입력해야할 때 쓰는 변수
	Node* head = NULL; // 그래프의 head = NULL로 초기화

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
			initializeGraph(graph); // 그래프 초기화 함수 호출
			break;
		case 'q': case 'Q':
			freeGraph(graph); // 그래프 할당 해제 함수 호출
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
			scanf("%d", &key2); // Vertex B의 key값 입력받음
			insertEdge(graph, key, key2);
			break;

		case 'd': case 'D':
			printf("탐색을 시작할 정점 Vertex 값을 입력하세요.\n");
			scanf("%d", &key);
			DFS(graph, key); // DFS 호출 함수
			break;
		case 'b': case 'B':
			printf("탐색을 시작할 정점 Vertex 값을 입력하세요.\n");
			scanf("%d", &key);
			BFS(graph, key); // BFS 호출 함수
			break;

		case 'p': case 'P':
			printGraph(); // stack 출력 함수 호출
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
	if (ismade != 0) // NULL 그래프가 아닌 경우
		freeGraph(h); // 그래프에 대한 할당 해제 함수 호출

	top = -1; // 깊이 우선 탐색 함수에서 쓰일 stack의 top = -1로 초기화

	front = rear = -1; //  너비 우선 탐색 함수에서 쓰일 queue의 front, rear = -1로 초기화
	ismade = 0;
	return 1;
}

int freeGraph(Node* head) // Graph 모든 vertex 관계 삭제 함수
{
	ismade = 0; // 그래프 생성 여부 변수 초기화

	for (int i = 0; i < MAX_VERTEX_SIZE; i++)
	{
		Node* temppointer = head + i; // 각 Vertex의 리스트 첫 부분을 지정하여
		while (temppointer->link != NULL) // 모든 부분을 처리할 때까지
		{
			Node* tofree = temppointer; // 임시 포인터 변수
			tofree->vertex = -1; // vertex값 초기화
			temppointer = temppointer->link; // 다음 리스트 노드 처리를 위한 대상 변경
			free(tofree); // 기존 리스트 노드 할당 해제
		}
	}
	return 1;
}


int insert(Node* graph, int key) // 그래프 삽입 함수, 매개변수로 head 포인터와 삽입할 노드의 key값 받음
{
	if (graph[key].vertex == key) //  Vertex를 중복하여 삽입하려는 경우
	{
		printf("이미 삽입된 Vertex입니다.\n"); // 오류 메시지 출력
		return 2;
	}
	else
	{
		graph[key].vertex = key; // vertex에 입력한 key값 대입
		graph[key].link = NULL; // 새로 추가하면서 edge가 없으므로 link를 NULL로 설정
		ismade++;
		return 1;
	}
}

int insertEdge(Node* graph, int key, int key2)
{

	if (ismade <= 1) // 발생할 수 있는 오류 방지를 위한 조건 생성
	{
		printf("2개 이상의 Vertex를 먼저 입력하세요.\n");
		return 2;
	}
	if (key < 0 || key > 9 || key2 < 0 || key2 > 9)
	{
		printf("입력한 Vertex 값이 0~9 범위를 초과합니다.\n");
		return 2;
	}
	if (graph[key].vertex == -1)
	{
		printf("Vertex A가 사전에 insert 되지 않았습니다.\n");
		return 2;
	}
	if (graph[key2].vertex == -1)
	{
		printf("Vertex B가 사전에 insert 되지 않았습니다.\n");
		return 2;
	}
	if (key == key2)
	{
		printf("자기 간선을 입력하였습니다.\n");
		return 2;
	}

	insertList(graph + key, key2); // 조건을 모두 만족할 경우 리스트 삽입 함수 호출
	insertList(graph + key2, key); // 상대 vertex에서도 리스트 삽입 함수 호출
}

int insertList(Node* graph, int key)
{
	int i = 0;
	Node* temppointer = graph; // 노드 포인터를 graph로 지정
	while (temppointer->link != NULL) // 모든 리스트 노드 값과 비교하여
	{
		if (key >= temppointer->vertex && key <= temppointer->link->vertex) // 오름차순 배치를 위해 알맞은 위치를 찾아
		{
			Node* added = (Node*)malloc(sizeof(Node*)); // 해당 위치에 삽입하기 위한
			added->vertex = key; // vertex 및 edge 값 재설정
			added->link = temppointer->link;
			temppointer->link = added;
			return 1;
		}
		else temppointer = temppointer->link; // 조건에 맞지 않을 경우 다음 리스트 노드와 비교
	}
	if (temppointer->link == NULL) // 모든 노드와 비교했을 때도 조건을 만족하지 않을 경우
	{
		Node* added = (Node*)malloc(sizeof(Node*)); // 가장 큰 Vertex값에 해당하므로
		added->vertex = key;
		added->link = NULL;
		temppointer->link = added; // 알맞게 vertex값 대입 및 edge 관계 설정
		return 1;
	}

}



int DFS(Node* graph, int key)
{
	int tempvalue = -1;
	Node* nodepointer = graph;
	if (key < 0 || key > 9)
	{
		printf("입력한 Vertex 값이 0~9 범위를 초과합니다.\n"); // 오류 방지를 위한 조건 설정
		return 2;
	}
	if (graph[key].vertex == -1)
	{
		printf("해당 Vertex는 사전에 insert 되지 않았습니다.\n");
		return 2;
	}
	for (int i = 0; i < MAX_VERTEX_SIZE; i++)
		visited[i] = 0; // 방문 여부 저장하는 배열 초기화

	push(key); // 시작 정점 key값 대입 후
	visited[key] = 1; // 방분 여부 저장 배열에도 1 대입
	printf("[ %d ] ", key); // 해당 key값 출력

	while(1) // while문을 통한 iterative DFS
	{
		if (top == -1) // 탐색을 완료해 stack에 남은 원소가 없을 경우 반복문 종료
			break;

		key = pop(); // 다음 link에 해당하는 vertex 삽입했을 경우 pop()

			for (nodepointer = graph + key; nodepointer;nodepointer=nodepointer->link) // 모든 노드에 대해 확인할 때까지
			{
					if (visited[nodepointer->vertex] == 0) // 방문하지 않은 vertex의 경우
					{
						printf("[ %d ] ", nodepointer->vertex); // 해당 vertex값 출력
						visited[nodepointer->vertex] = 1; // 방문 여부 확인
						push(nodepointer->vertex); // 다음 vertex 확인 위해 push함
					}
				
			}

		}
	}


int BFS(Node* graph, int key)
{
	if (key < 0 || key > 9)
	{
		printf("입력한 Vertex 값이 0~9 범위를 초과합니다.\n");
		return 2;
	}
	if (graph[key].vertex == -1)
	{
		printf("해당 Vertex는 사전에 insert 되지 않았습니다.\n"); // 오류 방지를 위한 조건 추가
		return 2;
	}

	for (int i = 0; i < MAX_VERTEX_SIZE; i++) // 방문 여부 저장 배열 초기화
		visited[i] = 0;

	Node* nodepointer; // vertex를 가리킬 포인터 선언 후
	front = -1; // queue 초기화
	rear = -1;

	printf("[ %d ] ", key); // 시작 정점값 출력후
	visited[key] = 1; // 해당 정점 방문 여부 확인
	enQueue(key); // queue에 insert함

	while (1)
	{
		if (front == rear) // 모든 탐색을 완료하여 queue에 남은 원소가 없을 경우 종료
			break;
		key = deQueue(); // 이전에 enqueue했던 값을 dequeue 하여 key에 대입
		for (nodepointer = graph + key; nodepointer; nodepointer = nodepointer->link) // 너비 우선 탐색 실현을 위하여
		{
			if (visited[nodepointer->vertex] == 0) // 방문한 이력이 없는 vertex에 방문하였을 경우
			{
				printf("[ %d ] ", nodepointer->vertex); // 해당 vertex 값 출력하고
				visited[nodepointer->vertex] = 1; // 방문 이력 확인
				enQueue(nodepointer->vertex); // level order 구현하기 위해 enqueue
			}
		}
	}
}




void printGraph() // graph의 요소를 출력하는 함수
{
	printf("입력된 Vertex 목록\n");
	for (int i = 0; i < MAX_VERTEX_SIZE; i++)
	{
		if (graph[i].vertex != -1)
			printf("[ %d ] ", i); // insert된 vertex만 출력하는 for문이다.
	}
	printf("\n");

	printf("입력된 Graph Adjacent List\n");
	Node* point;
	for (int i = 0; i < MAX_VERTEX_SIZE; i++) // 각 vertex의 adjacent list를 출력하는 반복문이다.
	{
		point = graph + i;
		if (point->vertex == -1)
			continue;
		printf("adjList[%d] : ", i);

		while (1)
		{
			if (point->link == NULL) // point->link == NULL로 출력을 완료하였을 경우 반복문 벗어남
			{
				printf("[ %d ]\n", point->vertex);
				break;
			}
			else
			{
				printf("[ %d ] ", point->vertex);
				point = point->link;
			}
		}
		printf("\n");
	}

}



int pop() // stack에서의 pop()함수 구현
{
	if (top < 0)
		return NULL;
	else
	{
		return(stack[top--]);
	}
}

int push(int aNode) // stack에서의 push()함수 구현
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



int deQueue() // Queue의 delete 기능 함수
{
	if (front == rear)
		return NULL;
	return queue[++front];
}

int enQueue(int aNode) // Queue의 insert 기능 함수
{
	if (rear == MAX_QUEUE_SIZE - 1)
	{
		fprintf(stderr, "Queue is full, cannot add element\n");
		exit(EXIT_FAILURE);
	}
	queue[++rear] = aNode;

}

