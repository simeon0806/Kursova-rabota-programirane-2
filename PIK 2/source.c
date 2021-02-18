#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>

struct Drug {
	char id[15];
	char name[30];
	double price;
	char date[11];
};

struct Node {
	struct Drug drug;
	struct Node* next;
};
struct Node* first = NULL;

struct Drug input();
struct Drug insertItem();
struct Node* new_item(struct Drug* p);
struct Node* createList();
struct Node* findDrugID(struct Node* first, char* id);
void ShowProduct(struct Drug* assist, char* expirationDate);
void rewriteFile(struct Node* first);
void freeList(struct Node* first);
int checkID(char* id);
int correctDate(char* date);

void AddNewDrug(struct Node* first);
void ChangeDrugPrice(struct Node* head, char* id);
void chackDate(struct Node* first, char* presentDate, int* shelfLife);
void FindbyID(struct Node* first, char* id);

int main()
{
	first = createList();

	int num, flag = 1;
	char id[15];
	char presentDate[11];
	int shelfLife;

	while (flag)
	{
		printf("...MENI...\n");
		printf("1) Add new drug\n");
		printf("2) Change drug's price\n");
		printf("3) Show products older then expiration date\n");
		printf("4) Find by ID\n");
		printf("0) Exit\n");
		scanf("%d", &num);
		printf("\n");
		switch (num)
		{
		case 1:
		{
			AddNewDrug(first);
			flag = 1;
			break;
		}
		case 2:
		{
			printf("Input drug's ID - ");
			scanf("%s", id);
			ChangeDrugPrice(first, id);
			flag = 1;
			break;
		}
		case 3:
		{
			while (1)
			{
				printf("Input present date :");
				scanf("%s", presentDate);
				if (correctDate(presentDate))
					break;
				else
					printf("Wrong form of DATE, try again...\n");;
			}

			printf("Shelf life in years:");
			scanf("%d", &shelfLife);
			printf("\n");
			chackDate(first, presentDate, &shelfLife);
			flag = 1;
			break;
		}
		case 4:
		{
			printf("Input drug's ID - ");
			scanf("%s", id);
			FindbyID(first, id);
			flag = 1;
			break;
		}
		case 0:
		{
			freeList(first);
			flag = 0;
			break;
		}
		default:
		{
			printf("Wrong option...\n");
			flag = 1;
			break;
		}
		}
		if (flag)
			printf("\n");
	}



	system("pause>nul");
	return 0;
}

// Creating List

/*Create new element*/
struct Node* new_item(struct Drug* p)
{
	struct Node* result = NULL;
	result = (struct Node*)malloc(sizeof(struct Node));

	result->drug = *p;
	result->next = NULL;

	return result;
}

//Създаване на опашъчен списък
struct Node* createList()
{
	struct Drug newDrug;
	struct Node* curr = NULL;
	int f = 0;
	//struct Node *first = NULL;

	FILE* fp;

	if ((fp = fopen("file.bin", "rb")) == NULL)
	{
		f = 1;
		if ((fp = fopen("file.bin", "ab")) == NULL)
		{
			perror(NULL);
			exit(1);
		}
	}

	while (1)
	{
		if (fread(&newDrug, sizeof(newDrug), 1, fp) != 1)
			break;

		if (first == NULL)
		{
			if (f == 1)
				newDrug = input();//
			first = new_item(&newDrug);
			curr = first;
		}
		else
		{
			curr->next = new_item(&newDrug);
			curr = curr->next;
		}
	}

	fclose(fp);

	return first;
}

// 1)

void AddNewDrug(struct Node* first)
{
	struct Node* curr = first;
	struct Node* pre = curr;
	struct Drug new;

	while (1)
	{
		if (curr == NULL)
		{
			curr = pre;
			new = insertItem();
			curr->next = new_item(&new);
			break;
		}
		pre = curr;
		curr = curr->next;
	}
}

// Chek did ID is 1 leter and 1 or more digits
int checkID(char* id)
{
	int flag = 0;
	if (isalpha(id[0]))
		flag = 1;
	if ((id[1]) == '\0')
		flag = 0;

	int i;
	for (i = 1; (id[i]) != '\0'; i++)
	{
		if (!isdigit(id[i]))
			flag = 0;
	}

	return flag;
}

// Check if tis ID is exsist
int sameID(char* id)
{
	struct Node* curr = first;

	while (curr != NULL)
	{
		if (strcmp(curr->drug.id, id) == 0)
			return 1;
		curr = curr->next;
	}
	return 0;
}

// Checking Date form. (Needed dd.mm.yyyy)
int correctDate(char* date)
{
	int flag = 1;
	int i, day, mount;

	day = (date[0] - '0') * 10 + (date[1] - '0');
	mount = (date[3] - '0') * 10 + (date[4] - '0');

	if (((day < 0) || (day > 31)) || ((mount < 0) || (mount > 12)))
		flag = 0;

	for (i = 1; (date[i]) != '\0'; i++)
	{
		if ((i == 2) || (i == 5))
		{
			if (date[i] != '.')
				flag = 0;
		}
		else
		{
			if (!isdigit(date[i]))
				flag = 0;
		}
	}
	if (i != 10)
		flag = 0;

	return flag;
}

// Input the elements
struct Drug input()
{

	struct Drug drug;

	while (1)
	{
		printf("ID(1 letter and number): ");
		scanf("%s", drug.id);
		if (sameID(&drug.id))
			printf("Wrong form of ID, try again...\n");
		else if (checkID(&drug.id))
			break;
		else
			printf("Wrong form of ID, try again...\n");
	}

	printf("Drug name: ");
	getchar();
	fgets(drug.name, 30, stdin);
	drug.name[strlen(drug.name) - 1] = '\0';

	printf("Drug Price: ");
	scanf("%lf", &drug.price);

	while (1)
	{
		printf("Date of prodaction (dd.mm.yyyy): ");
		scanf("%s", drug.date);
		if (correctDate(drug.date))
			break;
		else
			printf("Wrong form of DATE, try again...\n");
	}
	return drug;
}

struct Drug insertItem()
{
	struct Drug newDrug;

	FILE* fp;

	if ((fp = fopen("file.bin", "ab")) == NULL)
	{
		perror(NULL);
		exit(1);
	}

	newDrug = input();

	if (fwrite(&newDrug, sizeof(newDrug), 1, fp) != 1)
	{
		fclose(fp);
		perror(NULL);
		exit(2);
	}

	fclose(fp);

	return newDrug;
}

// 2)

/*Finding location on needed drug*/
struct Node* findDrugID(struct Node* first, char* id)
{
	struct Node* curr = first;
	while (curr != NULL)
	{
		if (!(strcmp(curr->drug.id, id)))
			return curr;
		curr = curr->next;
	}
	return NULL;
}

void ChangeDrugPrice(struct Node* first, char* id)
{
	struct Node* check = findDrugID(first, id);
	if (check != NULL)
	{
		printf("Input new price: ");
		scanf("%lf", &check->drug.price);
		rewriteFile(first);
	}
	else
		printf("Don't have element with that ID.\n");
}

void rewriteFile(struct Node* first)
{
	struct Node* curr = first;

	FILE* fp;

	if ((fp = fopen("file.bin", "wb")) == NULL)
	{
		perror(NULL);
		exit(1);
	}

	while (curr != NULL)
	{
		if (fwrite(&curr->drug, sizeof(curr->drug), 1, fp) != 1)
		{
			fclose(fp);
			perror(NULL);
			exit(2);
		}
		curr = curr->next;
	}

	fclose(fp);
}

// 3)

void ShowProduct(struct Drug* assist, char* expirationDate)
{
	printf("DRUG WITH Expired Shelf life...\n");
	printf("ID: %s\tDrug name: %s\tDrug Price: %.2lf\tDate of prodaction: %s\t Expiration Date:%s\n", assist->id, assist->name, assist->price, assist->date, expirationDate);
}

void chackDate(struct Node* first, char* presentDate, int* shelfLife)
{
	struct Node* curr = first;
	char assistMount[3], assistDay[3], assistYear[5];
	int year;
	char assistDate[11];

	char  pMount[3], pDay[3], pYear[5];

	pDay[0] = presentDate[0];
	pDay[1] = presentDate[1];
	pMount[0] = presentDate[3];
	pMount[1] = presentDate[4];
	pYear[0] = presentDate[6];
	pYear[1] = presentDate[7];
	pYear[2] = presentDate[8];
	pYear[3] = presentDate[9];

	pDay[2] = pMount[2] = pYear[4] = '\0';

	while (curr != NULL)
	{
		assistDay[0] = curr->drug.date[0];
		assistDay[1] = curr->drug.date[1];
		assistMount[0] = curr->drug.date[3];
		assistMount[1] = curr->drug.date[4];
		assistYear[0] = curr->drug.date[6];
		assistYear[1] = curr->drug.date[7];
		assistYear[2] = curr->drug.date[8];
		assistYear[3] = curr->drug.date[9];

		assistDay[2] = assistMount[2] = assistYear[4] = '\0';

		year = atoi(assistYear) + *shelfLife;

		assistYear[0] = year / 1000 + '0';
		assistYear[1] = year / 100 % 10 + '0';
		assistYear[2] = year / 10 % 10 + '0';
		assistYear[3] = year % 10 + '0';
		//itoa(year, assistYear, 10);

		int checkYear, checkMount, checkDay;
		checkYear = strcmp(pYear, assistYear);
		checkMount = strcmp(pMount, assistMount);
		checkDay = strcmp(pDay, assistDay);

		// Grouping expiration date
		strcpy(assistDate, assistDay);
		assistDate[2] = '.';
		assistDate[3] = '\0';
		strncat(assistDate, assistMount, 2);
		assistDate[5] = '.';
		assistDate[6] = '\0';
		strncat(assistDate, assistYear, 4);
		assistDate[10] = '\0';

		if (checkYear > 0)
			ShowProduct(&curr->drug, assistDate);
		else if ((checkMount > 0) && (checkYear == 0))
			ShowProduct(&curr->drug, assistDate);
		else if ((checkDay > 0) && (checkYear == 0) && (checkMount == 0))
			ShowProduct(&curr->drug, assistDate);

		curr = curr->next;
	}
}

// 4)

void FindbyID(struct Node* first, char* id)
{
	struct Node* node = NULL;

	node = findDrugID(first, id);

	if (node != NULL)
	{
		printf("DRUG INFO...\n");
		printf("ID: %s\tDrug name: %s\tDrug Price: %.2lf\tDate of prodaction: %s\n", node->drug.id, node->drug.name, node->drug.price, node->drug.date);
	}
	else
		printf("There is no element with this ID\n");
}

// Clear list

void freeList(struct Node* first)
{
	struct Node* curr = first;
	while (first != NULL)
	{
		first = first->next;
		free(curr);
		curr = first;
	}
}