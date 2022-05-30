#include <stdio.h>
#include <stdlib.h>

#define MAX_Graph_SIZE 10
#define MAX_QUEUE_SIZE 100
#define TRUE 1
#define FALSE 0

//��������Ʈ�� ǥ���� �ڱ����� ����ü
typedef struct adjLists
{
	int key;
	struct adjLists* link;
}vertex;

int visit[MAX_Graph_SIZE] = {0}; // �湮 üũ ���� Ȯ�� �迭
int checkempty = TRUE; // �׷��� ����ִ��� Ȯ��
int Queue[MAX_QUEUE_SIZE] = {0}; // ť
int front, rear = 0; // ť�� front, rear

// �Լ� ������Ÿ��
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
	vertex* Graph = NULL; // ����ü ������ ����

	
	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                      Graph Searches                           \n");
		printf("-------------[-----[���Ͽ�][2018038025]-----]--------------------\n");
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
    if(*p != NULL) // p�� ���� �Ҵ�Ǿ� ������
    {
        freeGraph(*p); // �Ҵ� ����
    }

	*p = (vertex*)malloc(sizeof(vertex)*MAX_Graph_SIZE); // ����ü ������ �迭 �����Ҵ�

    for(int i = 0; i<MAX_Graph_SIZE; i++) // ����ü ������ �迭�� �� link�� key �ʱ�ȭ �� �湮�迭 �ʱ�ȭ
    {
        (*p+i)->link = NULL;
        (*p+i)->key = -9999;
        visit[i] = 0;
    }
    //�������� �ʱ�ȭ
    rear = 0;
    front = 0;
    checkempty = TRUE;

    return 0;
}

void freeGraph(vertex* h)
{
    //���� ����Ʈ�� Ž���� ����ü ������ ���� ����
    vertex* n = NULL;
    vertex* p = NULL;
	
    for(int i = 0; i<MAX_Graph_SIZE; i++) 
    {
        n = h[i].link; // ����ü ������ �迭�� i��° �ε��� �Ҵ�
        while(n != NULL)
        {
            p = n;
            n = n->link;
            free(p); // �� ��ġ�� ����� ��� ��� �Ҵ� ����
        }
    }
    free(h); // ����ü ������ �迭 �Ҵ� ����
}

int InsertVertex(vertex* h, int key)
{
    if(h[key].key != key) // �� �ε����� key���� �Է¹��� key ���� �ٸ� ��
    {
        h[key].key = key; // vertex ����
        return 0;
    } 
    else // ������ �̹� �����ϸ�
    {
        printf("vertex has alread been created!\n"); // �������� ��� 
        return 0;
    }
}

int InsertEdge(vertex* h, int vertex1, int vertex2)
{
    if(h[vertex1].key != vertex1 || h[vertex2].key != vertex2) // edge�� ������ vertex�� �������� ������
    {
        printf("not exist vertex!"); // �������� ���
        return 0; // �Լ� ����
    }

    vertex* n = h[vertex1].link;
    vertex* p = NULL;

    while(n != NULL)
    {
        if(n->key == vertex2) //edge�� �̹� ������ ���
        {
            printf("Edge has already been created!\n");
            return 0;
        }
        n = n->link;
    }

    // edge ��� �ΰ� ���� �� �ʱ�ȭ
    vertex* node1 = (vertex*)malloc(sizeof(vertex));
    node1->key = vertex1;
    node1->link = NULL;

    vertex* node2 = (vertex*)malloc(sizeof(vertex));
    node2->key = vertex2;
    node2->link = NULL;
    
    if(h[vertex1].link == NULL) // vertex�� edge�� �������� ���� ��
    {
        h[vertex1].link = node2;
    }
    else
    {
        n = h[vertex1].link;
        p = n;

        while(n != NULL)
        {
            if(n->key > vertex2) // �������� ����
            {
                if(n == h[vertex1].link) // vertex�� edge�� �� �� ���� �� ��
                {
                    h[vertex1].link = node2; 
                    node2->link = n;
                    break;
                }
                else // vertex�� edge�� ���� �� ������ ��
                {
                    node2->link = n;
                    p->link = node2;
                    break;
                }

            }
            p = n;
            n = n->link;
        }
        if(n == NULL) // ������ break���� ���� ����Ǵ� ���
            p->link = node2; // ���� ������ edge�� ���� ���� ū��
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
    for(int i = 0; i<MAX_Graph_SIZE; i++) // ����ü ������ �迭�� �� �ε����� ���鼭
    {
        if(h[i].key == i) // vertex�� �����ϸ�
        {
            checkempty = FALSE; // �׷����� ������� �ʴٰ� üũ
            vertex* n = h[i].link;
            
            printf("[%d] ",i); // vertex ���
             while(n != NULL)
             {
                 printf(" -> [%d]",n->key); // ����� �� edge ���
                 n = n->link;
             }
             printf("\n");
             
        }

    }

    if(checkempty == TRUE) // �׷����� ����ִ°��
    {
        printf("Graph is empty!\n");
        return 0;
    }

    return 0;
}

int DFS(vertex* h, int root)
{
    if(h[root].link == NULL) // ������ �������� ������
    {
        printf("not exist vertex!\n"); 
        return 0;
    }

    vertex *n; 
    visit[root] = 1; // �湮
    printf("[%d] ",root); // ���
    for(n = h[root].link; n; n = n->link)
        if(!visit[n->key]) // �湮���� ���� ���
            DFS(h,n->key); // �Լ� ���
}

int BFS(vertex* h, int root)
{

    if(h[root].link == NULL) // �׷����� ����ִ� ���
    {
        printf("not exist vertex!\n");
        return 0;
    }
    visit[root] = 1; // �湮
    printf("[%d] ",root); // ���
    enqueue(root); // ť�� �湮�� �ε��� ����

    int p;
    vertex* n = h[root].link; // ��Ʈ�� ���� edge
    while(rear != front)
    {
        p = dequeue(); // ť���� �ε��� ������
        n = h[p].link; // �� ť�� �������� �̵�
        while(n != NULL) 
        {
            if(!visit[n->key]) // �湮���� ���� ��� 
            {
                visit[n->key] = 1; // �湮
                printf("[%d] ",n->key); // ���
                enqueue(n->key); // ť�� �湮�� �ε��� ����
            }
            n = n->link;
        }
    } 
}

int enqueue(int i) // ť�� enqueue �κ�
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

int dequeue() // ť�� dequeue �κ�
{
    if(front == rear)
    {
        printf("Queue is empty!\n");
        return 0;
    }

    front = (front+1)%MAX_QUEUE_SIZE;
    return Queue[front];
}

void visitclear() // �湮üũ �迭 �ʱ�ȭ
{
    for(int i = 0; i<MAX_Graph_SIZE; i++)
        visit[i] = 0;
}