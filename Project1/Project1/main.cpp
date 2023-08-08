#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

//int count = 0; // 큐에 추가되는 개수
//int level = 1; // 현제 레벨

// 트리 연산
typedef int TElement;
typedef struct BinTNode {
	TElement data;
	struct BinTNode* left;
	struct BinTNode* right;
}TNode;
TNode* root = NULL;
void init_tree() { root = NULL; }
//void init_tree_draw() { root = NULL, level=1; }
int is_empty_tree() { return root == NULL; }
TNode* create_tree(TElement e, TNode* l, TNode* r)
{
	TNode* n = (TNode*)malloc(sizeof(TNode));
	n->data = e;
	n->left = l;
	n->right = r;
	return n;
}

TNode* search(TNode* n, int key) {
	if (n == NULL) return NULL;
	else if (key == n->data) return n;
	else if (key < n->data) return search(n->left, key);
	else return search(n->right, key);
}
void search_BST(int key) {
	TNode* n = search(root, key);
	if (n != NULL)
		printf("[탐색 연산] : Success [%d] = 0x%x\n", n->data, n);
	else
		printf("[탐색 연산] : Fail =  %d!\n", key);
}

int insert(TNode* r, TNode* n) {
	if (n->data == r->data) return 0;
	else if (n->data < r->data) {
		if (r->left == NULL) r->left = n;	// 비어있다면 해당 루트의 왼쪽 링크필드에 추가할 노드 주소값 전달
		else insert(r->left, n);			// 만약 있다면 순환을 이용하여 다시 찾기
	}
	else {
		if (r->right == NULL) r->right = n; // 위와 반대로 오른쪽에 전달
		else insert(r->right, n);
	}
	return 1;								// 루트가 아니라면 1 리턴
}
void insert_BST(int key) {
	TNode* n = create_tree(key, NULL, NULL);
	if (is_empty_tree())
		root = n;
	else if (insert(root, n) == 0)
		free(n);
}

void delete_tree(TNode* parent, TNode* node) {
	TNode* child, * succ, * succp;
	// case1 -> 삭제할 노드가 단말 노드 일 경우
	if ((node->left == NULL && node->right == NULL)) {		// 단말 노드이므로 삭제할 노드의 링크필드는 모두 NULL이어야함
		if (parent == NULL) root = NULL;					// 
		else {
			if (parent->left == node) parent->left = NULL;
			else parent->right = NULL;
		}
	}

	// case2 -> 삭제할 노드의 자식 노드가 한 개 일때
	else if ((node->left == NULL || node->right == NULL)) {
		child = (node->left != NULL) ? node->left : node->right;
		if (node == root) root = child;
		else {
			if (parent->left == node) parent->left = child;
			else parent->right = child;
		}
	}

	// case3 -> 삭제할 노드의 자식 노드가 두 개 일때
	else {
		succp = node;											// 후계자 노드의 부모노드
		succ = node->right;										/* 후계자 노드 [우리는 오른쪽 자식으로 탐색하기 때문에 right임 왼쪽 자식으로 탐색하고 싶으면 left
																	대신 아래 while문 수정 필요]*/
		while (succ->left != NULL)							// 후계자 노드의 left가 널이 아니다-> 탐색이 더 필요 [우리는 단말노드인 후계자가 필요한 것]
		{
			succp = succ;										// 아직 끝까지 탐색한 것이 아니므로 부모노드의 위치를 변경
			succ = succ->left;									// 자식 노드는 탐색
		}
		if (succp->left == succ) succp->left = succ->right;
		else succp->right = succ->right;
		//if (succp->left == succ) succp->left = NULL;
		//else succp->right = NULL;
		node->data = succ->data;								//후계자 노드를 삭제할 노드에 복사
		node = succ;
	}
	free(node);
}
void delete_BST(int key) {
	TNode* parent = NULL;
	TNode* node = root;

	if (node == NULL) return;
	while (node != NULL && node->data != key) {
		parent = node;
		node = (key < node->data) ? node->left : node->right;
	}
	if (node == NULL) printf("\n에러: 삭제할 데이터가 없습니다.\n");
	else delete_tree(parent, node);
}
// 큐 정보
#define MAX_QUEUE_SIZE 100
typedef TNode* Element;
Element data[MAX_QUEUE_SIZE];
int front;
int  rear;
void error(const char* str) {
	fprintf(stderr, "%s\n", str);
	exit(1);
}
void init_queue() { front = rear = 0; }
int is_empty_queue() { return rear == front; }
int is_full() { return front == (rear + 1) % MAX_QUEUE_SIZE; }
int size_queue() { return (rear - front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE; }
void enqueue(Element val) {
	if (is_full())
		error("큐 포화 에러");
	rear = (rear + 1) % MAX_QUEUE_SIZE;
	data[rear] = val;
}
Element dequeue() {
	if (is_empty_queue())
		error("큐 공백 에러");
	front = (front + 1) % MAX_QUEUE_SIZE;
	return data[front];
}

TNode* get_root() {
	return root;
}
int get_count(TNode* n) {
	if (n == NULL) return 0;
	return 1 + get_count(n->left) + get_count(n->right);
}
int get_leaf_count(TNode* n) { //단말노드
	if (n == NULL) return 0;
	if (n->left == NULL && n->right == NULL) return 1; // 루트 노드만 존재
	else return get_leaf_count(n->left) + get_leaf_count(n->right); //재귀호출
}
int get_height(TNode* n) { //tree Hight
	int hLeft, hRight;
	if (n == NULL) return 0;
	hRight = get_height(n->right);
	hLeft = get_height(n->left);
	return (hLeft > hRight) ? hLeft + 1 : hRight + 1;
}

int plusOffset = 8;

void print2D_tree90(TNode* n, int offset) {
	if (n == NULL) return;						//트리가 비었으면 함수 종료

	offset += plusOffset;						// offset-> 기본 공백 plusOffset-> 추가적으로 늘어날 공백 즉, 왼쪽에서부터 떨어지는 거리
	printf("\n");
	print2D_tree90(n->right, offset);			// 오른쪽 자식부터 출력

							
	for (int i = plusOffset; i < offset; i++)	// 루트 노드로부터 떨어짐을 표현하기 위해서 plusOffset부터 offset보다 작을 동안 반복하여 공간확보
	{ 
		if (i == offset - 1) {
			printf(">");
		}
		else {
			printf(" ");
		}
	} 
	printf("%d\n", n->data);					// 함수 실행될때 자신의 노드 값 출력

	print2D_tree90(n->left, offset);					// 왼쪽 자식도 동일하게 출력
}
void print2D_UI(TNode* n) {						// 위 함수를 간단하게 쓰기 위한 함수
	print2D_tree90(n, 0);
}

int level_order_print(TNode* root) {
	TNode* n;
	if (root == NULL) return 0;
	init_queue();
	enqueue(root);
	while (!is_empty_queue())
	{
		n = dequeue();
		if (n != NULL) {
			printf("[%d] ", n->data);
			enqueue(n->left);
			enqueue(n->right);
		}
	}
}
void preorder_VLR(TNode* n) { //전위 순회 preorder traversal
	if (n != NULL) {
		printf("[%d] ", n->data);
		preorder_VLR(n->left);
		preorder_VLR(n->right);
	}
}
void inorder_LVR(TNode* n) { // 중위 순회 inorder traversal
	if (n != NULL) {
		inorder_LVR(n->left);
		printf("[%d] ", n->data);
		inorder_LVR(n->right);
	}
}
void postorder_LRV(TNode* n) { // 후위 순회 postorder traversal
	if (n != NULL) {
		postorder_LRV(n->left);
		postorder_LRV(n->right);
		printf("[%d] ", n->data);
	}
}
void main() {
	init_tree();
	char ch = 'i';
	int nodeData;
	printf("\n   트리메뉴<m>\n \n i - 노드추가\n d - 트리출력\n r - 노드삭제\n t - 트리그리기\n q - 나가기\n\n");
	while (ch != 'q')
	{
		printf("\n키를 입력해주세요 : "); scanf(" %c", &ch);

		switch (ch)
		{
		case 'i':
			// 계속해서 i를 누르지 않아도 노드를 추가할 수 있도록 반복문 설정
			while (1) {
				if (is_empty_tree()) {										// 트리가 비었을 경우
					printf("\nroot를 입력해주세요. (q를 누르면 종료): ");
				}
				else {
					printf("\n노드를 입력해주세요. (q를 누르면 종료): ");
				}
				int input_result = scanf("%d", &nodeData);

				if (input_result == 1) {								// 함수의 반환값이 1 즉, 성공적으로 숫자를 입력받았을 경우 노드를 추가
					insert_BST(nodeData);
				}
				else if (input_result == EOF) {							// 오류 발생 시 프로그램을 종료
					printf("입력 오류가 발생했습니다.\n");
					exit(1);
				}
				else {													// 숫자가 입력되지 않았을 경우 
					char next = getchar();								// 사용자 입력 버퍼로부터 하나의 문자를 받아옴
					if (next == 'q') {									// 그 값이 'q'일 경우 종료
						break; // 삽입 모드 종료
					}
					else {
						printf("잘못된 입력입니다. 숫자를 입력하거나 'q'를 눌러 종료하세요.\n");
					}
				}
			}
			break;
		case 'd':
		{
			if (is_empty_tree()) {
				printf("\n트리가 비었습니다.\n");
			}
			else {
				printf("\n");
				printf("전위 순회 방식(VLR) : ");  preorder_VLR(root);		printf("\n");
				printf("중위 순회 방식(LVR) : ");  inorder_LVR(root);			printf("\n");
				printf("후위 순회 방식(LRV) : ");  postorder_LRV(root);		printf("\n");
				printf("레벨 순회 방식	    : ");  level_order_print(root);	printf("\n");
			}
		}
		break;
		case 'r':
			// 계속해서 r를 누르지 않아도 노드를 삭제할 수 있도록 반복문 설정
			while (1) {
				if(root == NULL){}
				printf("\n삭제할 값을 입력해 주세요. (q를 누르면 종료): ");
				int delete_result = scanf("%d", &nodeData);

				if (delete_result == 1) {								// 함수의 반환값이 1 즉, 성공적으로 숫자를 입력받았을 경우 노드를 추가
					delete_BST(nodeData);
				}
				else if (delete_result == EOF) {						// 오류 발생 시 프로그램을 종료
					printf("입력 오류가 발생했습니다.\n");
					exit(1);
				}
				else {													// 숫자가 입력되지 않았을 경우 
					char next = getchar();								// 사용자 입력 버퍼로부터 하나의 문자를 받아옴
					if (next == 'q') {									// 그 값이 'q'일 경우 종료
						break; // 삽입 모드 종료
					}
					else {
						printf("잘못된 입력입니다. 숫자를 입력하거나 'q'를 눌러 종료하세요.\n");
					}
				}
			}
		break;
		case 't':
		{
			if (is_empty_tree()) {
				printf("\n트리가 비었습니다.\n");
			}
			else {
				print2D_UI(root);
			}
		}
		break;
		case 'w':
		{
			if (is_empty_tree()) {
				printf("\n트리가 비었습니다.\n");
			}
			else {
				printf("\n");
				//print2D_tree(root);
			}
		}
		break;
		case 'm':
		{
			printf("\n트리메뉴 - m\n \n 트리추가 - i \n 트리출력 - d \n 삭제 - r \n 트리그리기 - t \n 나가기 - q \n\n");
		}
		break;
		case 'q':
		{
			printf("\n종료하였습니다.\n");
			exit(0);
		}
		break;
		default:
			printf("커멘드를 잘못입력하셨습니다. 다시 입력해주세요.\n");
		}
	}
}
