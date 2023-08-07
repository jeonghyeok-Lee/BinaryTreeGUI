#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)

//int count = 0; // ť�� �߰��Ǵ� ����
//int level = 1; // ���� ����

// Ʈ�� ����
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
		printf("[Ž�� ����] : Success [%d] = 0x%x\n", n->data, n);
	else
		printf("[Ž�� ����] : Fail =  %d!\n", key);
}

int insert(TNode* r, TNode* n) {
	if (n->data == r->data) return 0;
	else if (n->data < r->data) {
		if (r->left == NULL) r->left = n;	// ����ִٸ� �ش� ��Ʈ�� ���� ��ũ�ʵ忡 �߰��� ��� �ּҰ� ����
		else insert(r->left, n);			// ���� �ִٸ� ��ȯ�� �̿��Ͽ� �ٽ� ã��
	}
	else {
		if (r->right == NULL) r->right = n; // ���� �ݴ�� �����ʿ� ����
		else insert(r->right, n);
	}
	return 1;								// ��Ʈ�� �ƴ϶�� 1 ����
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
	// case1 -> ������ ��尡 �ܸ� ��� �� ���
	if ((node->left == NULL && node->right == NULL)) {		// �ܸ� ����̹Ƿ� ������ ����� ��ũ�ʵ�� ��� NULL�̾����
		if (parent == NULL) root = NULL;					// 
		else {
			if (parent->left == node) parent->left = NULL;
			else parent->right = NULL;
		}
	}

	// case2 -> ������ ����� �ڽ� ��尡 �� �� �϶�
	else if ((node->left == NULL || node->right == NULL)) {
		child = (node->left != NULL) ? node->left : node->right;
		if (node == root) root = child;
		else {
			if (parent->left == node) parent->left = child;
			else parent->right = child;
		}
	}

	// case3 -> ������ ����� �ڽ� ��尡 �� �� �϶�
	else {
		succp = node;											// �İ��� ����� �θ���
		succ = node->right;										/* �İ��� ��� [�츮�� ������ �ڽ����� Ž���ϱ� ������ right�� ���� �ڽ����� Ž���ϰ� ������ left
																	��� �Ʒ� while�� ���� �ʿ�]*/
		while (succ->left != NULL)							// �İ��� ����� left�� ���� �ƴϴ�-> Ž���� �� �ʿ� [�츮�� �ܸ������ �İ��ڰ� �ʿ��� ��]
		{
			succp = succ;										// ���� ������ Ž���� ���� �ƴϹǷ� �θ����� ��ġ�� ����
			succ = succ->left;									// �ڽ� ���� Ž��
		}
		if (succp->left == succ) succp->left = succ->right;
		else succp->right = succ->right;
		//if (succp->left == succ) succp->left = NULL;
		//else succp->right = NULL;
		node->data = succ->data;								//�İ��� ��带 ������ ��忡 ����
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
	if (node == NULL) printf("\n����: ������ �����Ͱ� �����ϴ�.\n");
	else delete_tree(parent, node);
}
// ť ����
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
		error("ť ��ȭ ����");
	rear = (rear + 1) % MAX_QUEUE_SIZE;
	data[rear] = val;
}
Element dequeue() {
	if (is_empty_queue())
		error("ť ���� ����");
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
int get_leaf_count(TNode* n) { //�ܸ����
	if (n == NULL) return 0;
	if (n->left == NULL && n->right == NULL) return 1; // ��Ʈ ��常 ����
	else return get_leaf_count(n->left) + get_leaf_count(n->right); //���ȣ��
}
int get_height(TNode* n) { //tree Hight
	int hLeft, hRight;
	if (n == NULL) return 0;
	hRight = get_height(n->right);
	hLeft = get_height(n->left);
	return (hLeft > hRight) ? hLeft + 1 : hRight + 1;
}


// Ʈ�� ���̿� ����Ͽ� �������� �ִ� ������
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
//// Ʈ������ �ִ� ��� ����
//int numberNode(int nodeHeight) {
//	if (nodeHeight == 1) return 1;
//	else {
//		return 2 * numberNode(nodeHeight - 1);
//	}
//}
//
///*
//* �Ʒ� �Լ������� nodeHeight��
//		a    <- 3��
//	b        <- 2��
//c            <- 1��
//maxNode -> Ʈ�� �ִ� ������ �ִ� ��尳��
//*/
//// ������ �ִ� ��� ���� 
//int levelNumberNode(int maxNode, int nodeHeight) {
//	int x = maxNode;
//	for (int i = 1; i < nodeHeight; i++) {
//		x = x / 2;
//	}
//	return x;
//}
////�ش� ������ ����� �ִ� ������ �˶� �������� ��Ÿ���� �Լ�
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
	if (n == NULL) return;						//Ʈ���� ������� �Լ� ����

	offset += plusOffset;						// offset-> �⺻ ���� plusOffset-> �߰������� �þ ���� ��, ���ʿ������� �������� �Ÿ�
	printf("\n");
	print2D_tree90(n->right, offset);			// ������ �ڽĺ��� ���

							
	for (int i = plusOffset; i < offset; i++)	// ��Ʈ ���κ��� �������� ǥ���ϱ� ���ؼ� plusOffset���� offset���� ���� ���� �ݺ��Ͽ� ����Ȯ��
	{ 
		if (i == offset - 1) {
			printf(">");
		}
		else {
			printf(" ");
		}
	} 
	printf("%d\n", n->data);					// �Լ� ����ɶ� �ڽ��� ��� �� ���

	print2D_tree90(n->left, offset);					// ���� �ڽĵ� �����ϰ� ���
}
void print2D_UI(TNode* n) {						// �� �Լ��� �����ϰ� ���� ���� �Լ�
	print2D_tree90(n, 0);
}

//int offset; // ��Ʈ�� ó�� �����κ��� �־��� �Ÿ�
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

//int spaceL;			// ���� ������ ��Ÿ���� ����
//int spaceR;			// ������ ������ ��Ÿ���� ����
//TNode* beforeNode;	// ��������ľǿ� ���۷��� ����

//void print2D_tree(TNode* n) {
//	if (n == NULL) return;
//	int numberNodeByHeight = levelNumberNode_reverse(level);	// �� �������� ���� �� �ִ� �ִ� ��� ���� 
//	int nullCount = numberNodeByHeight - count;					
//	//printf(" c = %d", count);
//	//printf(" s = %d", size_queue());
//	//printf("level = %d\n", numberNodeByHeight);
//	//printf("nullcount = %d\n", nullCount);
//
//
//	if (n == root) {
//		offset = halfSizeX(get_height(n)); //�Ÿ�
//		for (int i = 0; i < offset; i++) printf(" ");
//		printf("%d", n->data);
//	}
//	else if (n->data < beforeNode->data) {
//		if (beforeNode == root) {												// ���� ��尡 ��Ʈ ��忴�ٸ�
//			spaceL = spaceLeft(offset-1, level); 
//			for (int i = 0; i < spaceL; i++) printf(" ");
//		}
//		else if (n->data > root->data) {										// ���� �����ͺ��ٴ� ������ ��Ʈ ��庸�� ũ�ٸ�
//			spaceL = spaceLeft(offset, level);
//			for (int i = 0; i < spaceL; i++) printf(" "); 
//		}
//		else {																	// ���� ��尡 ��Ʈ ��尡 �ƴ϶��
//			spaceL = spaceLeft(spaceL, level); 
//			for (int i = 0; i < spaceL; i++) printf(" ");
//		}
//		printf("%d", n->data);
//		for (int i = 0; i < spaceL; i++) printf(" ");
//		//printf("l = %d", spaceL);
//	}
//	else if (n->data > beforeNode->data) {										// ���� �����Ͱ� ���� �����ͺ��� ���� ��
//		if (beforeNode == root) {												// ���� ��尡 ��Ʈ ����϶�
//			spaceR = spaceRight(offset-1, level);
//			if (spaceL > 0) {													// ���� ��尡 ���� ���
//				for (int i = spaceL; i < spaceR; i++) printf(" ");
//			}
//			else {																// ���� ��尡 ���� ���
//				for (int i = 0; i < spaceR; i++) printf(" ");
//			}
//		}
//		else {																	// ���� ��尡 ��Ʈ ��尡 �ƴҶ�
//			if (count - 1 !=0) {												// ���� ��尡 ���� ���[���� ���� ������ �����Ƿ� �̰��� ���� ������ ���]
//				spaceR = spaceRight(spaceR, level)/(count-1)-spaceL;
//				for (int i = 0; i < spaceR; i++) printf(" ");
//			}
//			else {																// ���� ��尡 ���� ���
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
//	if (count == numberNodeByHeight|| n==root) {	// ������ ����� ������ ���� count�� ���� �� �� �� ��ȭ�϶�
//		printf("\n\n");
//		level++;
//		count = 0;
//		beforeNode = n;
//	}
//	else if (count < numberNodeByHeight) {			 //������ ����� �������� count�� ���� ��
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
void preorder_VLR(TNode* n) { //���� ��ȸ preorder traversal
	if (n != NULL) {
		printf("[%d] ", n->data);
		preorder_VLR(n->left);
		preorder_VLR(n->right);
	}
}
void inorder_LVR(TNode* n) { // ���� ��ȸ inorder traversal
	if (n != NULL) {
		inorder_LVR(n->left);
		printf("[%d] ", n->data);
		inorder_LVR(n->right);
	}
}
void postorder_LRV(TNode* n) { // ���� ��ȸ postorder traversal
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
	printf("\n   Ʈ���޴�<m>\n \n i - ����߰�\n d - Ʈ�����\n r - ������\n t - Ʈ���׸���\n q - ������\n\n");
	while (ch != 'q')
	{
		printf("\nŰ�� �Է����ּ��� : "); scanf(" %c", &ch);

		switch (ch)
		{
		case 'i':
			// ����ؼ� i�� ������ �ʾƵ� ��带 �߰��� �� �ֵ��� �ݺ��� ����
			while (1) {
				printf("\n���͸� ������ ���Ե˴ϴ�. (q�� ������ ����): ");
				int input_result = scanf("%d", &nodeData);

				if (input_result == 1) {								// �Լ��� ��ȯ���� 1 ��, ���������� ���ڸ� �Է¹޾��� ��� ��带 �߰�
					insert_BST(nodeData);
				}
				else if (input_result == EOF) {							// ���� �߻� �� ���α׷��� ����
					printf("�Է� ������ �߻��߽��ϴ�.\n");
					exit(1);
				}
				else {													// ���ڰ� �Էµ��� �ʾ��� ��� 
					char next = getchar();								// ����� �Է� ���۷κ��� �ϳ��� ���ڸ� �޾ƿ�
					if (next == 'q') {									// �� ���� 'q'�� ��� ����
						break; // ���� ��� ����
					}
					else {
						printf("�߸��� �Է��Դϴ�. ���ڸ� �Է��ϰų� 'q'�� ���� �����ϼ���.\n");
					}
				}
			}
			break;
		case 'd':
		{
			if (is_empty_tree()) {
				printf("\nƮ���� ������ϴ�.\n");
			}
			else {
				printf("\n");
				printf("���� ��ȸ ���(VLR) : ");  preorder_VLR(root);		printf("\n");
				printf("���� ��ȸ ���(LVR) : ");  inorder_LVR(root);			printf("\n");
				printf("���� ��ȸ ���(LRV) : ");  postorder_LRV(root);		printf("\n");
				printf("���� ��ȸ ���	    : ");  level_order_print(root);	printf("\n");
			}
		}
		break;
		case 'r':
		{
			printf("\n ������ ���� �Է��� �ּ���: ");
			scanf("%d", &nodeData);
			delete_BST(nodeData);
		}
		break;
		case 't':
		{
			if (is_empty_tree()) {
				printf("\nƮ���� ������ϴ�.\n");
			}
			else {
				print2D_UI(root);
			}
		}
		break;
		case 'w':
		{
			if (is_empty_tree()) {
				printf("\nƮ���� ������ϴ�.\n");
			}
			else {
				printf("\n");
				//print2D_tree(root);
			}
		}
		break;
		case 'm':
		{
			printf("\nƮ���޴� - m\n \n Ʈ���߰� - i \n Ʈ����� - d \n ���� - r \n Ʈ���׸��� - t \n ������ - q \n\n");
		}
		break;
		case 'q':
		{
			printf("\n�����Ͽ����ϴ�.\n");
			exit(0);
		}
		break;
		default:
			printf("Ŀ��带 �߸��Է��ϼ̽��ϴ�. �ٽ� �Է����ּ���.\n");
		}
	}
}
