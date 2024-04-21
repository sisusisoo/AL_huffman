#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)
#define MAX 3000
#define alph_num 300 //26 �빮�ڸ�// �� �ҹ��� 52//�ƽ�Ű�ڵ� 85


//Ʈ�� ���
typedef struct node
{
	char alphabet;  //���ĺ�
	int freq;//�󵵼�
	struct node* left; //���� �ڽ� ���
	struct node* right; //������ �ڽ� ���
}node;

node* make_Huffman_tree(char arr[]);  //������ �ڵ� Ʈ�� ����(�����ϰ��� �ϴ� ���ڿ�)
node* makenode(char alphabet, int freq, struct node* left, struct node* right); //�� ��� ����
void make_table(node* n, char str[], int len, char* table[]); //���ĺ� �� �������� �ڵ� �迭 ����
void decode(char* str, node* root); //���ڵ��Լ�
node node_arr[alph_num] = { NULL };
int ind = 0;//���� ����

//�� ��� ����(���ĺ�,�󵵼�,���� �ڽ� ���,������ �ڽ� ���) 
node* makenode(char alphabet, int freq, struct node* left, struct node* right)
{
	node* new_node = (node*)malloc(sizeof(node));
	new_node->alphabet = alphabet;
	new_node->freq = freq;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

//������ �ڵ� Ʈ�� ����(�����ϰ��� �ϴ� ���ڿ�)
node* make_Huffman_tree(char arr[])
{
	int i = 0;
	int j;
	int temp_n = 0;
	int min = 0;  //���� �󵵼��� ���� index
	int min2 = 0; //�� ��°�� �󵵼��� ���� index
	int fre[alph_num] = { 0 };  //���ĺ�(A~Z) �� ��
	int check[alph_num] = { 0 };  //���������� Ȯ��(�������� ���� �� �ʿ䰡 ������ -1)
	node* tree[alph_num] = { NULL };  //���� ��� �迭
	node* new_node; //�� ���

	while (arr[i] != NULL)
	{
		//�󵵼� ���ϱ�
		//fre[arr[i] - 'A']++;//---------------------------------
		fre[arr[i]]++;
		i++;
	}
	for (int i = 0; i < alph_num; i++)
	{
		//���ĺ��� �����ϸ�
		if (fre[i] != 0)
		{
			//node_arr[ind] = *makenode(i + 'A', fre[i], NULL, NULL);
			node_arr[ind] = *makenode(i, fre[i], NULL, NULL);

			//tree[ind++] = makenode(i + 'A', fre[i], NULL, NULL); //��� ����
			tree[ind++] = makenode(i, fre[i], NULL, NULL); //��� ����
		}
	}
	for (i = 0; i < ind - 1; i++)
	{
		//���� ���� �� ã��
		j = 0;
		while (check[j] == -1)	j++; //������ ��带 ������ �迭 �� ���� �� index
		min = j; //�켱 ���� �۴ٰ� ����

		for (j = min; j < ind - 1; j++) //��� �迭�� �˻�
			if (check[j] != -1) //�̹� ������ ��尡 �ƴϸ�(�˻��غ� �ʿ䰡 ������)
				if (tree[min]->freq > tree[j]->freq)
					//min�ε��� �󵵼� ���� �󵵼��� ���� ���
					min = j;

		//�ι�°�� ���� �� ã��
		j = 0;
		while (check[j] == -1 || j == min) j++;
		//������ ���� �ּ� ��� ������ �迭 �� ���� �� index
		min2 = j;  //�ι�°�� �۴ٰ� ����

		for (j = min2; j < ind; j++)
			if (check[j] != -1) //�̹� ������ ��尡 �ƴϸ�
				if (tree[min2]->freq > tree[j]->freq)
					//min2�ε��� �󵵼� ���� �󵵼��� ���� ���
					if (j != min) //���� ���� index�� �ƴ� ���
						min2 = j;

		tree[min] = makenode(NULL, tree[min]->freq + tree[min2]->freq, tree[min], tree[min2]);
		//min�� min2�ε����� �󵵼��� ��ģ �󵵼��� �� ��� ����
		//���� ���� ��带 min�ε��� �ڸ��� �ִ´�.

		check[min2] = -1;
		//min2�ε����� min�ε��� �ڸ��� ��忡 ���������Ƿ� check[min2]<-=1
	}
	return tree[min]; //������� Ʈ���� ��Ʈ ��� ��ȯ
}

//���ĺ� �� �������� �ڵ� �迭 ����
//(Ʈ�� ��Ʈ ���,���� ���� �ڵ� ���ڿ�,���ڿ��� �� ��ġ, ���� �� �迭)
void make_table(node* n, char str[], int len, char* table[])
{
	if (n->left == NULL && n->right == NULL) //n�� �ܳ���� ���
	{
		str[len] = '\0'; //������ ���� str���� �־��ְ�
		//�� ����� ���ĺ��� Ȯ���Ͽ� table�� ������ ��ġ�� str���ڿ��� �ִ´�.
		//strcpy(table[(n->alphabet) - 'A'], str);
		strcpy(table[(n->alphabet)], str);
	}
	else //�� ��尡 �ƴ� ���
	{
		if (n->left != NULL) //���� �ڽ��� �ִ� ���
		{
			str[len] = '0'; //���ڿ��� 0 ����
			make_table(n->left, str, len + 1, table);
			//���ȣ��(���ڿ��� �� ��ġ�� +1)
		}
		if (n->right != NULL) //������ �ڽ��� �ִ� ���
		{
			str[len] = '1'; //���ڿ��� 1 ����
			make_table(n->right, str, len + 1, table);
			//���ȣ��(���ڿ��� �� ��ġ�� +1)
		}
	}
}
void append(char* dst, char c) {
	char* p = dst;
	while (*p != '\0') p++; // ���ڿ� �� Ž��
	*p = c;
	*(p + 1) = '\0';
}
//���ڵ��Լ�(���ڵ� �ϰ� ���� ���ڿ�, Ʈ�� ��Ʈ ���)
void decode(char* str, node* root,FILE* fp)
{
	int i = 0;

	//��������� ����
	int k = 0;//arr�迭 �ʱ�ȭ ����
	//char* code[alph_num];
	char decode[MAX]=""; //���� Ǭ �������� 	
	
	//===========
	i = 0;
	char t;

	node* j = root;
	while (str[i] != '\0') //������ ���� �ƴ� ���
	{
		if (str[i] == '0') //���ڰ� 0�� ���
			j = j->left; //���� �ڽ����� �̵�
		else if (str[i] == '1') //���ڰ� 1�� ���
			j = j->right; //������ �ڽ����� �̵�
		if (j->left == NULL && j->right == NULL) //�� ����� ���
		{
			
			t = j->alphabet;
			printf("%c",t); //�� ����� ���ĺ� ���
			append(decode,t);

			j = root;
		
		}
		i++;
	}

	printf("\n");

	//���� ����°���
	
	
	fprintf(fp,decode);//���Ϸ� ���!============================
	//=========
}

//���� �Լ�
int main()
{
	//���� ����°���
	char input_file_name[100];
	char output_file_name[100];
	char decoding_file_name[100];
	printf("Enter input file name: ");
	//scanf("%s", input_file_name);
	strcpy(input_file_name, "sample1.txt");
	FILE* input_file = fopen(input_file_name, "r");
	if (input_file == NULL) {
		printf("Error opening input file!\n");
		return 1;
	}
	printf("Enter output file name: ");
	//scanf("%s", output_file_name);
	strcpy(output_file_name, "enc1.txt");
	FILE* output_file = fopen(output_file_name, "w");
	if (output_file == NULL) {
		printf("Error opening output file!\n");
		fclose(input_file);
		return 1;
	}

	printf("Enter decoding file name: ");
	strcpy(decoding_file_name, "dec1.txt");
	//scanf("%s", decoding_file_name);
	FILE* decoding_file = fopen(decoding_file_name, "w");
	if (output_file == NULL) {
		printf("Error opening decoding file!\n");
		fclose(input_file);
		fclose(output_file);
		return 1;
	}


	//--------------------------------------------------

	char arr[MAX]; //�����ϰ��� �ϴ� ���ڿ�

	char* code[alph_num]; //�� ���ĺ��� ���� �������� �ڵ� �迭
	char str[MAX]; //���ڿ� ����
	char encoding[MAX]=""; //���ڵ��ؼ� ���� ������ �迭
	int i; //�ݺ��� ����
	char answer; //���ڵ� ���ϴ°��� ���� ��� ����
	node* root;//Ʈ���� ��Ʈ


	// �Է� ���Ͽ��� �ؽ�Ʈ �б�
	fgets(arr, sizeof(arr), input_file);

	/*
		char tempChar[999] = "";
		while (feof(input_file) == 0) {
			fgets(tempChar, sizeof(arr), input_file);
			//append(arr, tempChar);
			strcat(arr, tempChar);//�̰� �ι��� ���� ������ ����??? 
			//printf("%s", str);
		}
	*/

	for (i = 0; i < alph_num; i++)
		code[i] = (char*)malloc(sizeof(char));

	//printf("�����ϰ��� �ϴ� ���ڿ�(�빮��) : ");
	//scanf("%s", arr); //�����ϰ��� �ϴ� ���ڿ� �Է�
	
	root = make_Huffman_tree(arr); //�������ڵ带 �̿��� Ʈ�� ����
	make_table(root, str, 0, code); //Ʈ���� ����� ���ĺ� �� �������� �ڵ� ����
	

	//fget�� �ٹٲޱ����� �ҷ��´�..... �ٲ���Ѵ� 

	i = 0;
	while (arr[i] != NULL) { //�Է¹��� ���ڿ��� ����������
		//strcat(encoding, code[arr[i] - 'A']); //���ں� �ڵ� ���ڵ� ���ڿ� �ڿ� �ֱ�
		strcat(encoding, code[arr[i]]); //���ں� �ڵ� ���ڵ� ���ڿ� �ڿ� �ֱ�
		//strcat(encoding, " ");

		i++;
	
	}

	for (i = 0; i < ind; i++)
		printf("%c freq :%d,huffman code : %s \n", node_arr[i].alphabet, node_arr[i].freq, code[node_arr[i].alphabet]);
		//printf("%c : %s\n", node_arr[i].alphabet, code[node_arr[i].alphabet - 'A']);
	

	printf("���� ��� : %s\n", encoding); //���ڵ� �� ������ �迭 ���
	fprintf(output_file, encoding);

	printf("���� ���� : ");
	decode(encoding, root, decoding_file); //���ڵ� �Լ� ȣ��

	//���� �ݱ� 
	fclose(input_file);
	fclose(output_file);
	fclose(decoding_file);
	return 0;
}