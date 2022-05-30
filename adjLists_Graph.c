#include <stdio.h>
#include <stdlib.h>

#define MAX_Graph_SIZE 10
#define MAX_QUEUE_SIZE 100
#define TRUE 1
#define FALSE 0

//인접리스트를 표현할 자기참조 구조체
typedef struct adjLists
{
	int key;
	struct adjLists* link;
}vertex;

int visit[MAX_Graph_SIZE] = {0}; // 방문 체크 여부 확인 배열
int checkempty = TRUE; // 그래프 비어있는지 확인
int Queue[MAX_QUEUE_SIZE] = {0}; // 큐
int front, rear = 0; // 큐의 front, rear

// 함수 프로토타입
int Initialize(vertex** p);
void freeGraph(vertex* h);
int InsertVertex(vertex* h, int key);
int InsertEdge(vertex* h, int vertex1, int vertex2);
int PrintGraph(vertex* h);
int DFS(vertex* h, int root);
int BFS(vertex* h, int root);
int enqueue(int i);
int dequeue();
void visitclear();

int main()
{
	char command;
    int key;
	int vertex1, vertex2;
	vertex* Graph = NULL; // 구조체 포인터 생성

	
	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                      Graph Searches                           \n");
		printf("-------------[-----[정하용][2018038025]-----]--------------------\n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Graph       = z                                     \n");
		printf(" Insert Vertex          = v      Insert Edge             = e \n");
		printf(" Depth First Search     = d      Breath First Search     = b \n");
		printf(" Print Graph            = p      Quit                    = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			Initialize(&Graph);
			break;
		case 'q': case 'Q':
			freeGraph(Graph);
			break;
		case 'v': case 'V':
            printf("key : ");
            scanf("%d",&key);
			InsertVertex(Graph, key);
			break;
		case 'd': case 'D':
            printf("select vertex :");
            scanf("%d",&key);
            visitclear();
			DFS(Graph,key);
			break;

		case 'e': case 'E':
			printf("choose two vertex [ex) 0 1] : ");
			scanf("%d %d",&vertex1, &vertex2);
            InsertEdge(Graph,vertex1,vertex2);
			break;
		case 'b': case 'B':
            printf("select vertex : ");
            scanf("%d",&key);
            visitclear();
			BFS(Graph,key);
			break;

		case 'p': case 'P':
			PrintGraph(Graph);
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int Initialize(vertex** p)
{
    if(*p != NULL) // p에 값이 할당되어 있으면
    {
        freeGraph(*p); // 할당 해제
    }

	*p = (vertex*)malloc(sizeof(vertex)*MAX_Graph_SIZE); // 구조체 포인터 배열 동적할당

    for(int i = 0; i<MAX_Graph_SIZE; i++) // 구조체 포인터 배열의 각 link와 key 초기화 및 방문배열 초기화
    {
        (*p+i)->link = NULL;
        (*p+i)->key = -9999;
        visit[i] = 0;
    }
    //전역변수 초기화
    rear = 0;
    front = 0;
    checkempty = TRUE;

    return 0;
}

void freeGraph(vertex* h)
{
    //인접 리스트를 탐색할 구조체 포인터 변수 생성
    vertex* n = NULL;
    vertex* p = NULL;
	
    for(int i = 0; i<MAX_Graph_SIZE; i++) 
    {
        n = h[i].link; // 구조체 포인터 배열의 i번째 인덱스 할당
        while(n != NULL)
        {
            p = n;
            n = n->link;
            free(p); // 그 위치에 연결된 모든 노드 할당 해제
        }
    }
    free(h); // 구조체 포인터 배열 할당 해제
}

int InsertVertex(vertex* h, int key)
{
    if(h[key].key != key) // 각 인덱스에 key값이 입력받은 key 값과 다를 때
    {
        h[key].key = key; // vertex 생성
        return 0;
    } 
    else // 정점이 이미 존재하면
    {
        printf("vertex has alread been created!\n"); // 오류문자 출력 
        return 0;
    }
}

int InsertEdge(vertex* h, int vertex1, int vertex2)
{
    if(h[vertex1].key != vertex1 || h[vertex2].key != vertex2) // edge를 생성할 vertex가 존재하지 않으면
    {
        printf("not exist vertex!"); // 오류문자 출력
        return 0; // 함수 종료
    }

    vertex* n = h[vertex1].link;
    vertex* p = NULL;

    while(n != NULL)
    {
        if(n->key == vertex2) //edge가 이미 생성된 경우
        {
            printf("Edge has already been created!\n");
            return 0;
        }
        n = n->link;
    }

    // edge 노드 두개 생성 및 초기화
    vertex* node1 = (vertex*)malloc(sizeof(vertex));
    node1->key = vertex1;
    node1->link = NULL;

    vertex* node2 = (vertex*)malloc(sizeof(vertex));
    node2->key = vertex2;
    node2->link = NULL;
    
    if(h[vertex1].link == NULL) // vertex에 edge가 존재하지 않을 때
    {
        h[vertex1].link = node2;
    }
    else
    {
        n = h[vertex1].link;
        p = n;

        while(n != NULL)
        {
            if(n->key > vertex2) // 오름차순 정렬
            {
                if(n == h[vertex1].link) // vertex에 edge가 한 개 존재 할 때
                {
                    h[vertex1].link = node2; 
                    node2->link = n;
                    break;
                }
                else // vertex에 edge가 여러 개 존재할 때
                {
                    node2->link = n;
                    p->link = node2;
                    break;
                }

            }
            p = n;
            n = n->link;
        }
        if(n == NULL) // 루프가 break없이 정상 종료되는 경우
            p->link = node2; // 새로 생성한 edge의 값이 가장 큰값
    }

    if(h[vertex2].link == NULL)
    {
        h[vertex2].link = node1;
    }
    else
    {
        n = h[vertex2].link;
        p = n;

        while(n != NULL)
        {
            if(n->key > vertex1)
            {
                if(n == h[vertex2].link)
                {
                    h[vertex2].link = node1;
                    node1->link = n;
                    break;
                }
                else
                {
                    node1->link = n;
                    p->link = node1;
                    break;
                }

            }
            p = n;
            n = n->link;
        }
        if(n == NULL)
        {
            p->link = node1;
        }
    }
    return 0; 
}

int PrintGraph(vertex* h)
{
    for(int i = 0; i<MAX_Graph_SIZE; i++) // 구조체 포인터 배열의 각 인덱스를 돌면서
    {
        if(h[i].key == i) // vertex가 존재하면
        {
            checkempty = FALSE; // 그래프가 비어있지 않다고 체크
            vertex* n = h[i].link;
            
            printf("[%d] ",i); // vertex 출력
             while(n != NULL)
             {
                 printf(" -> [%d]",n->key); // 연결된 각 edge 출력
                 n = n->link;
             }
             printf("\n");
             
        }

    }

    if(checkempty == TRUE) // 그래프가 비어있는경우
    {
        printf("Graph is empty!\n");
        return 0;
    }

    return 0;
}

int DFS(vertex* h, int root)
{
    if(h[root].link == NULL) // 정점이 존재하지 않으면
    {
        printf("not exist vertex!\n"); 
        return 0;
    }

    vertex *n; 
    visit[root] = 1; // 방문
    printf("[%d] ",root); // 출력
    for(n = h[root].link; n; n = n->link)
        if(!visit[n->key]) // 방문하지 않은 노드
            DFS(h,n->key); // 함수 재귀
}

int BFS(vertex* h, int root)
{

    if(h[root].link == NULL) // 그래프가 비어있는 경우
    {
        printf("not exist vertex!\n");
        return 0;
    }
    visit[root] = 1; // 방문
    printf("[%d] ",root); // 출력
    enqueue(root); // 큐에 방문한 인덱스 삽입

    int p;
    vertex* n = h[root].link; // 루트의 다음 edge
    while(rear != front)
    {
        p = dequeue(); // 큐에서 인덱스 가져옴
        n = h[p].link; // 그 큐의 정점으로 이동
        while(n != NULL) 
        {
            if(!visit[n->key]) // 방문하지 않은 노드 
            {
                visit[n->key] = 1; // 방문
                printf("[%d] ",n->key); // 출력
                enqueue(n->key); // 큐에 방문한 인덱스 삽입
            }
            n = n->link;
        }
    } 
}

int enqueue(int i) // 큐의 enqueue 부분
{
    if((rear+1)%MAX_QUEUE_SIZE == front)
    {
        printf("Queue is FULL!\n");
        return 0;
    }

    rear = (rear+1)%MAX_QUEUE_SIZE;
    Queue[rear] = i;
    return 0;
}

int dequeue() // 큐의 dequeue 부분
{
    if(front == rear)
    {
        printf("Queue is empty!\n");
        return 0;
    }

    front = (front+1)%MAX_QUEUE_SIZE;
    return Queue[front];
}

void visitclear() // 방문체크 배열 초기화
{
    for(int i = 0; i<MAX_Graph_SIZE; i++)
        visit[i] = 0;
}