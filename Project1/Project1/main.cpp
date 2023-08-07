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


// 트리 높이에 비례하여 가로축의 최대 사이즈
//int sizeX(int nodeHeight) {						
//	int x = 0;
//	if (nodeHeight == 1) return 1;
//	for (int i = 2; i <= nodeHeight; i++) {
//		if (i == 2) x += 5;
//		else {
//			x = (x * 2) + 1;
//		}
//	}
//	return x;
//}
//
//int halfSizeX(int nodeHeght) {
//	return (sizeX(nodeHeght) - 1) / 2;
//}
//
//// 트리높이 최대 노드 개수
//int numberNode(int nodeHeight) {
//	if (nodeHeight == 1) return 1;
//	else {
//		return 2 * numberNode(nodeHeight - 1);
//	}
//}
//
///*
//* 아래 함수에서의 nodeHeight는
//		a    <- 3층
//	b        <- 2층
//c            <- 1층
//maxNode -> 트리 최대 높이의 최대 노드개수
//*/
//// 레벨별 최대 노드 개수 
//int levelNumberNode(int maxNode, int nodeHeight) {
//	int x = maxNode;
//	for (int i = 1; i < nodeHeight; i++) {
//		x = x / 2;
//	}
//	return x;
//}
////해당 레벨의 노드의 최대 개수를 알때 현재층을 나타내는 함수
//int levelNumber(int maxNode) {
//	int i = 1;
//	for (int x = maxNode; x != 1; x = x / 2) {
//		i++;
//	}
//	return i;
//}
//
//int levelNumberNode_reverse(int nodeHeight) {
//	int x = 1;
//	for (int i = nodeHeight; i > 1; i--) {
//		x *= 2;
//	}
//	return x;
//}

int plusOffset = 8;
/*
				99
				
		68
		
35
							30
				 
				 26
		
							22
		
		18
				
							12
				7
							
							3

*/
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

//int offset; // 루트가 처음 벽으로부터 멀어진 거리
//
//int spaceLeft(int firstOffset, int level) {
//	if (level == 1) return firstOffset;
//	else { 
//		if (firstOffset < 0) {
//			return firstOffset + spaceLeft(firstOffset / 2, level - 1);
//		}
//		else {
//			return firstOffset + spaceLeft((-1*firstOffset) / 2, level - 1);
//		}
//	}
//}
//
//int spaceRight(int firstOffset, int level) {
//	if (level == 1) return firstOffset;
//	else return firstOffset + spaceRight(firstOffset / 2, level-1);
//}
//

//int spaceL;			// 왼쪽 공백을 나타내느 변수
//int spaceR;			// 오른쪽 공백을 나타내는 변수
//TNode* beforeNode;	// 이전노드파악용 레퍼런스 변수

//void print2D_tree(TNode* n) {
//	if (n == NULL) return;
//	int numberNodeByHeight = levelNumberNode_reverse(level);	// 각 레벨에서 가질 수 있는 최대 노드 개수 
//	int nullCount = numberNodeByHeight - count;					
//	//printf(" c = %d", count);
//	//printf(" s = %d", size_queue());
//	//printf("level = %d\n", numberNodeByHeight);
//	//printf("nullcount = %d\n", nullCount);
//
//
//	if (n == root) {
//		offset = halfSizeX(get_height(n)); //거리
//		for (int i = 0; i < offset; i++) printf(" ");
//		printf("%d", n->data);
//	}
//	else if (n->data < beforeNode->data) {
//		if (beforeNode == root) {												// 이전 노드가 루트 노드였다면
//			spaceL = spaceLeft(offset-1, level); 
//			for (int i = 0; i < spaceL; i++) printf(" ");
//		}
//		else if (n->data > root->data) {										// 이전 데이터보다는 작지만 루트 노드보다 크다면
//			spaceL = spaceLeft(offset, level);
//			for (int i = 0; i < spaceL; i++) printf(" "); 
//		}
//		else {																	// 이전 노드가 루트 노드가 아니라면
//			spaceL = spaceLeft(spaceL, level); 
//			for (int i = 0; i < spaceL; i++) printf(" ");
//		}
//		printf("%d", n->data);
//		for (int i = 0; i < spaceL; i++) printf(" ");
//		//printf("l = %d", spaceL);
//	}
//	else if (n->data > beforeNode->data) {										// 현제 데이터가 이전 데이터보다 작을 때
//		if (beforeNode == root) {												// 이전 노드가 루트 노드일때
//			spaceR = spaceRight(offset-1, level);
//			if (spaceL > 0) {													// 형제 노드가 있을 경우
//				for (int i = spaceL; i < spaceR; i++) printf(" ");
//			}
//			else {																// 형제 노드가 없을 경우
//				for (int i = 0; i < spaceR; i++) printf(" ");
//			}
//		}
//		else {																	// 이전 노드가 루트 노드가 아닐때
//			if (count - 1 !=0) {												// 형제 노드가 있을 경우[이전 노드는 나보다 작으므로 이것은 가장 오른쪽 노드]
//				spaceR = spaceRight(spaceR, level)/(count-1)-spaceL;
//				for (int i = 0; i < spaceR; i++) printf(" ");
//			}
//			else {																// 형제 노드가 없을 경우
//				spaceR = spaceRight(spaceR, level);
//				for (int i = 0; i < spaceR; i++) printf(" ");
//			}
//		}
//		printf("%d", n->data);
//		//printf("r = %d", spaceR);
//	}
//
//
//
//	if (count == numberNodeByHeight|| n==root) {	// 레벨별 노드의 개수와 현재 count가 동일 할 때 즉 포화일때
//		printf("\n\n");
//		level++;
//		count = 0;
//		beforeNode = n;
//	}
//	else if (count < numberNodeByHeight) {			 //레벨별 노드의 개수보다 count가 작을 때
//		if (size_queue()==0 || count + size_queue() +1 == numberNodeByHeight) {
//			printf("\n\n");
//			level++;
//			count = 0;
//			beforeNode = n;
//		}
//	}
//}


//int level_order(TNode* root) {
//	TNode* n;
//	if (root == NULL) return 0;
//	init_queue();
//	enqueue(root);
//	while (!is_empty_queue())
//	{
//		n = dequeue();
//		if (n != NULL) {
//			print2D_tree(n);
//			count++;
//			if (n->left != NULL) {
//				enqueue(n->left);
//			}
//			if (n->right != NULL) {
//				enqueue(n->right);
//			}
//		}
//	}
//}
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
				printf("\n엔터를 누르면 삽입됩니다. (q를 누르면 종료): ");
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
		{
			printf("\n 삭제할 값을 입력해 주세요: ");
			scanf("%d", &nodeData);
			delete_BST(nodeData);
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
