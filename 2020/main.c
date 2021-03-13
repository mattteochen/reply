#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define M 1000

int TABLE_PEOPLE_SIZE;
int **BACKUP_PEOPLE_LIST;
int **BACKUP_PM_LIST;
int TABLE_PM_SIZE;

typedef struct mapNode{
	int val;
	struct mapNode *next;
} MN;

typedef struct productManagers{
	char name[M];
	int bonus;
	struct productManagers *next;
} C;

typedef struct developers{
	char companyName[M];
	int bonusPotential;
	int skillNumber;
	char skills[100][50];
	struct developers *next;
} P;

void addLP(char s[], P **head, P **tail);
void getXY(char s[], int *x, int *y);
void addListCompany(char s[], C **head, C **tail);
void freeListCompany(C *head);
void freeListPeople(P *head);
int calculateDiff(P *target, P *test);
int calculateWP(P *head, P *target, char *s, int *, int *);
bool isThere(char s[], P *test);
int calculateProductMBonus(C *head, C *test, int *returnPosition);
void recursiveDeletion(char **map, char goal, int i, int j, int x, int y, int*deletedSize);
void insertMapDeveloperList(MN **head, MN **tail, int size);
void createDeveloperMapList(MN **head, MN **tail, char **map, char goal, int x, int y);
void freeMapList(MN *head);
int compareOne(const void *p1, const void *p2);
int compareTwo(const void *p1, const void *p2);
int comapre(const void *p1, const void *p2);
int mainFunctionDeveloper(int, int *returnValue, int *returnLink, int spaceValue, int **table, int *first, int *usedPeople, int *usedPeopleCounter);
int calculateDeveloper(int **table, MN *head);
bool search(int value, int *a, int size);
bool checkUsed(int value, int *usedPeople);
int getFirsAvaiable(int **table, int *usedPeople, int *firstAvaiable);
int calculatePM(int **table, MN *head);
bool checkUsed2(int value, int *usedPeople);

int main(){
	//form here --> star reading input
	FILE *fptr = fopen ("f_glitch.txt", "r");
	if (fptr == NULL) return -1;
	else{
		printf ("file ok\n");
		//return 0;
	}
	
	//get x and y
	int x, y;
	char s[M];
	fgets(s, M, fptr);
	getXY(s, &x, &y);
	printf ("\n\n");

	//save the map
	char **MAP = (char**)calloc(y, sizeof(char*));
	if (MAP == NULL) return -1;
	for (int i=0; i<y; i++){
		MAP[i] = (char*)malloc(sizeof(char)*x);
		fgets(MAP[i], M, fptr);
		printf ("%s", MAP[i]);
	}
	printf ("\n\n");

	//get number of people
	fgets(s, M, fptr);
	int numberPeople = atoi(s);
	printf ("number people: %d\n", numberPeople);
	TABLE_PEOPLE_SIZE = numberPeople;

	//save the people
	P *headPeopleList = NULL;
	P *tailPeopleList = NULL;
	for (int i=0; i<numberPeople; i++){
		fgets(s, M, fptr);
		addLP(s, &headPeopleList, &tailPeopleList);
	}

	//print list:
	P *c = headPeopleList;
	while (c != NULL){
		printf ("%s %d %d ", c->companyName, c->bonusPotential, c->skillNumber);
		for (int i=0; i<100; i++){
			if (strlen(c->skills[i]) < 1) break;
			printf ("%s ", c->skills[i]);
		}
		printf ("\n");
		c = c->next;
	}
	printf ("\n\n");

	//save company number
	fgets(s, M, fptr);
	int numberProductManagers = atoi(s);
	//printf ("%s\n", s);
	printf ("number company: %d\n", numberProductManagers);
	TABLE_PM_SIZE = numberProductManagers;

	//save productM
	C *headProductMList;
	C *tailProductMList;
	for (int i=0; i<numberProductManagers; i++){
		fgets(s, M, fptr);
		//printf ("%s", s);
		addListCompany(s, &headProductMList, &tailProductMList);
	}
	printf ("\n\n");

	//print list:
	C *d = headProductMList;
	while (d != NULL){
		printf ("%s %d\n", d->name, d->bonus);	
		d = d->next;
	}
	printf ("End imput\n\n");
	//end reading input
	


	//search for every people which has the max WP
	//set a table to store values | number of the people | number of the people with who has the max W | WP value | same company?
	int **wpTable = calloc(numberPeople, sizeof(int*));
	BACKUP_PEOPLE_LIST = calloc(numberPeople, sizeof(int*));
	if (wpTable == NULL) return -1;
	for (int i=0; i<numberPeople; i++){
		wpTable[i] = calloc(4, sizeof(int));
		BACKUP_PEOPLE_LIST[i] = calloc(4, sizeof(int));
	}
	c = headPeopleList;	
	int k = 0, compatiblePosition = 0, sameCompanyBonus = 0;
	while(c != NULL){
		//printf ("analizyng people n: %d\n", k+1);
		wpTable[k][0] = k+1;
		wpTable[k][2] = calculateWP(headPeopleList, c, s, &compatiblePosition, &sameCompanyBonus); //s is a string to check if the company is the same -> returned by the function
		//printf ("same bonus: %d", sameCompanyBonus);	
		wpTable[k][2] += sameCompanyBonus;
		wpTable[k][1] = compatiblePosition;
		if (strcmp(c->companyName, s) == 0) wpTable[k][3] = 1;
		else wpTable[k][3] = 0;
		c = c->next;
		k++;
	}
	memcpy(BACKUP_PEOPLE_LIST, wpTable, sizeof(wpTable[0])*numberPeople);
	printf ("end analysis\nPrint analysis:\n");
	for (int i=0; i<numberPeople;i++) printf ("People: %d -->position linked: %d - WP value: %d - same company: %d\n", BACKUP_PEOPLE_LIST[i][0], BACKUP_PEOPLE_LIST[i][1], BACKUP_PEOPLE_LIST[i][2], BACKUP_PEOPLE_LIST[i][3]);
	printf ("\n\n");

	//place the product managers
	//create a table where we connect PM with the same company and save their bonus potential
	int **productBonusTable = calloc(numberProductManagers, sizeof(int*));
	BACKUP_PM_LIST = calloc(numberProductManagers, sizeof(int*));
	for (int i=0; i<numberProductManagers; i++){
		productBonusTable[i] = calloc(4, sizeof(int));
		BACKUP_PM_LIST[i] = calloc(4, sizeof(int));
	}
	//table: |number off PM 1,2,3.. | number of PM who is linked | their bonus
	d = headProductMList;
	k = 0;
	compatiblePosition = 0;
	while (d != NULL){
		productBonusTable[k][0] = k+1;
		productBonusTable[k][2] = calculateProductMBonus(headProductMList, d, &compatiblePosition);
		productBonusTable[k][1] = compatiblePosition;
		d = d->next;
		k++;
	}
	memcpy(BACKUP_PM_LIST, productBonusTable, sizeof(productBonusTable[0])*numberProductManagers);
	printf ("if the pos linked = 0 -> no linking\n");	
	for (int i=0; i<numberProductManagers;i++) printf ("Product Manager: %d -->position linked: %d - bonus value: %d\n", productBonusTable[i][0], productBonusTable[i][1], productBonusTable[i][2]);

	//create a list of linked position on the map for D and PM
	MN *headMapDeveloperList =NULL;
	MN *tailMapDeveloperList = NULL;
	MN *headMapProductManagerList = NULL;
	MN *tailMapProdcutManagerList = NULL;
	//DEV
	char **mapCopy = calloc(y, sizeof(char*));
	for (int i=0; i<y; i++){
		mapCopy[i] = calloc(x, sizeof(char));
		strcpy(mapCopy[i], MAP[i]);
	}
	createDeveloperMapList(&headMapDeveloperList, &tailMapDeveloperList, mapCopy, '_', x, y); 
	//print developer map list
	/*
	MN *pointerMN = headMapDeveloperList;
	printf ("Dev map len list:\n");
	while (pointerMN != NULL){
		printf ("len of link: %d\n", pointerMN->val);
		pointerMN = pointerMN->next;
	}
	printf ("\n\n");
	*/
	//PM
	for  (int i=0; i<y; i++){
		for (int j=0; j<x; j++) mapCopy[i][j] = MAP[i][j];
	}
	createDeveloperMapList(&headMapProductManagerList, &tailMapProdcutManagerList, mapCopy, 'M', x, y);
	//print Product Manager map list
	/*
	pointerMN = headMapProductManagerList;
	printf ("PM map len list:\n");
	while (pointerMN != NULL){
		printf ("len of link: %d\n", pointerMN->val);
		pointerMN = pointerMN->next;
	}
	printf ("\n\n");
	*/
	//solveDeveloper(headMapDeveloperList, wpTable)	
	//lets sort for WP score
	qsort(wpTable, numberPeople, sizeof(wpTable[0]), compareOne);
	qsort(productBonusTable, numberProductManagers, sizeof(productBonusTable[0]), compareTwo);

	int resDeveloper = calculateDeveloper(wpTable, headMapDeveloperList);	
	printf ("developers total sum: %d\n", resDeveloper);

	int resPM = calculatePM(productBonusTable, headMapProductManagerList);
	printf ("PM total sum: %d\n", resPM);

	printf ("total sum: %d\n", resPM + resDeveloper);

	





	freeMapList(headMapDeveloperList);
	freeMapList(headMapProductManagerList);
	for (int i=0; i<numberPeople; i++){
		free(wpTable[i]);
		//free(BACKUP_PEOPLE_LIST[i]);
	}
	free(wpTable);
	//free(BACKUP_PEOPLE_LIST);
	for (int i=0; i<numberProductManagers; i++){
		free(BACKUP_PM_LIST[i]);
		//free(productBonusTable[i]);
	}
	//free(BACKUP_PM_LIST);
	free(productBonusTable);
	freeListPeople(headPeopleList);
	freeListCompany(headProductMList);
	return 0;
}

int calculatePM(int **table, MN *head){
	int CODE = 1;
	//the idea is to calculate a max value for for every spot
	//transform list into array
	int arraySize = 0;
	MN *pointer = head;
	while (pointer != NULL){
		arraySize++;
		pointer = pointer->next;
	}
	int *arrayMap = calloc(arraySize, sizeof(int));
	int *usedPeople = calloc(TABLE_PM_SIZE, sizeof(int));
	for (int i=0; i<TABLE_PM_SIZE; i++) usedPeople[i] = -1;

	pointer = head;
	int i = 0;
	while (pointer != NULL){
		arrayMap[i] = pointer->val;
		pointer = pointer->next;
		i++;
	}
	i = 0;
	//this sorts the values of free spaces from small to big
	qsort(arrayMap, arraySize, sizeof(arrayMap[i]), comapre);
	int t = 0, max = 0, first = 1, returnPosition = 0, usedPeopleCounter = 0; 
	for (int i=0; i<arraySize; i++){
		if (arrayMap[i] == 1) continue; //----> single spots will be filled at the end
		t = 0;
		first = 1;
		max += mainFunctionDeveloper(CODE, &t, &returnPosition, arrayMap[i], table, &first, usedPeople, &usedPeopleCounter);
		//printf ("size %d for this t: %d\n", arrayMap[i], t);		
	}
	int firstAvaiable = 0;
	for (int i=0; i<arraySize; i++){
		if (arrayMap[i] == 1)
			max += getFirsAvaiable(table, usedPeople, &firstAvaiable);
	}

	free(usedPeople);
	free(arrayMap);
	return max;
}

int comapre(const void *p1, const void *p2){
	return (*(int*)p2 - *(int*)p1);
}

int calculateDeveloper(int **table, MN *head){
	int CODE = 0;
	//the idea is to calculate a max value for for every spot
	//transform list into array
	int arraySize = 0;
	MN *pointer = head;
	while (pointer != NULL){
		arraySize++;
		pointer = pointer->next;
	}
	int *arrayMap = calloc(arraySize, sizeof(int));
	int *usedPeople = calloc(TABLE_PEOPLE_SIZE, sizeof(int));
	for (int i=0; i<TABLE_PEOPLE_SIZE; i++) usedPeople[i] = -1;

	pointer = head;
	int i = 0;
	while (pointer != NULL){
		arrayMap[i] = pointer->val;
		pointer = pointer->next;
		i++;
	}
	i = 0;
	//this sorts the values of free spaces from small to big
	qsort(arrayMap, arraySize, sizeof(arrayMap[i]), comapre);
	//for (int i=0; i<arraySize; i++) printf ("%d\n",arrayMap[i]);
	int t = 0, max = 0, first = 1, returnPosition = 0, usedPeopleCounter = 0; 
	for (int i=0; i<arraySize; i++){
		if (arrayMap[i] == 1) continue; //----> single spots will be filled at the end
		t = 0;
		first = 1;
		max += mainFunctionDeveloper(CODE, &t, &returnPosition, arrayMap[i], table, &first, usedPeople, &usedPeopleCounter);
		//printf ("size %d for this t: %d\n", arrayMap[i], t);		
	}
	int firstAvaiable = 0;
	for (int i=0; i<arraySize; i++){
		if (arrayMap[i] == 1)
			max += getFirsAvaiable(table, usedPeople, &firstAvaiable);
	}

	free(usedPeople);
	free(arrayMap);
	return max;
}

int getFirsAvaiable(int **table, int *usedPeople, int *firstAvaiable){
	for (int i=0; i<TABLE_PEOPLE_SIZE; i++){
		if (checkUsed(table[i][0], usedPeople) == false){
			(*firstAvaiable)++;
			return 0; //zero because isolated people dont have points
		}
		else (*firstAvaiable)++;
	}
}

bool search(int value, int *a, int size){
	for (int i=0; i<size; i++) if (a[i] == value) return true;
	return false;
}

int getFirstTable(int CODE, int **table, int *first, int *returnLink, int *usedPeople, int *usedPeopleCounter){	
	if (CODE == 0){
		if (*first == 1){
			*first = 0;
			int j = 0;
			usedPeople[*usedPeopleCounter] = table[0][0];
			(*usedPeopleCounter)++;
			*returnLink = table[0][1];
			//printf ("returning (1): %d Link: %d\n", table[0][2], *returnLink);
			return table[0][2];	
		}
		else{
			//this is if the direct link (with max value) is free
			if ((*returnLink) > 0 && checkUsed(BACKUP_PEOPLE_LIST[(*returnLink)-1][0], usedPeople) == false){
				//printf ("we have a free max WP link, previuos links with: %d\n", *returnLink);
				usedPeople[*usedPeopleCounter] = BACKUP_PEOPLE_LIST[(*returnLink)-1][0];
				(*usedPeopleCounter)++;
				int t = (*returnLink)-1;
				*returnLink = BACKUP_PEOPLE_LIST[t][1];
				//printf ("returning (2): %d\n", BACKUP_PEOPLE_LIST[t][2]);
				return BACKUP_PEOPLE_LIST[t][2];
			}
			//if the link is not free we are gonna put the max avaiable
			else{
				int j = 0;
				while (j < TABLE_PEOPLE_SIZE){
					if (checkUsed(table[j][0], usedPeople) == false){
						usedPeople[*usedPeopleCounter] = table[j][0];
						(*usedPeopleCounter)++;
						*returnLink = table[j][1];
						//printf ("returning (3): %d\n", table[j][2]);
						return table[j][2];
					}
					j++;
				}
			}
		}
	}
	else{
		if (*first == 1){
			*first = 0;
			int j = 0;
			usedPeople[*usedPeopleCounter] = table[0][0];
			(*usedPeopleCounter)++;
			*returnLink = table[0][1];
			//printf ("returning (1): %d Link: %d\n", table[0][2], *returnLink);
			return table[0][2];	
		}
		else{
			//this is if the direct link (with max value) is free
			if ((*returnLink) > 0 && checkUsed2(BACKUP_PM_LIST[(*returnLink)-1][0], usedPeople) == false){
				//printf ("we have a free max WP link, previuos links with: %d\n", *returnLink);
				usedPeople[*usedPeopleCounter] = BACKUP_PM_LIST[(*returnLink)-1][0];
				(*usedPeopleCounter)++;
				int t = (*returnLink)-1;
				*returnLink = BACKUP_PM_LIST[t][1];
				//printf ("returning (2): %d\n", BACKUP_PM_LIST[t][2]);
				return BACKUP_PM_LIST[t][2];
			}
			//if the link is not free we are gonna put the max avaiable
			else{
				int j = 0;
				while (j < TABLE_PM_SIZE){
					if (checkUsed2(table[j][0], usedPeople) == false){
						usedPeople[*usedPeopleCounter] = table[j][0];
						(*usedPeopleCounter)++;
						*returnLink = table[j][1];
						//printf ("returning (3): %d\n", table[j][2]);
						return table[j][2];
					}
					j++;
				}
			}
		}
	}
}

bool checkUsed2(int value, int *usedPeople){
	for (int i=0; i<TABLE_PM_SIZE; i++){
		if (usedPeople[i] == -1) return false;
		if (usedPeople[i] == value) return true;
	}
	return false;
}

bool checkUsed(int value, int *usedPeople){
	for (int i=0; i<TABLE_PEOPLE_SIZE; i++){
		if (usedPeople[i] == -1) return false;
		if (usedPeople[i] == value) return true;
	}
	return false;
}

//space value -> position on the map to fill
int mainFunctionDeveloper(int CODE, int *returnValue, int *returnLink, int spaceValue, int **table, int *first, int *usedPeople, int *usedPeopleCounter){
	int ans = 0;	
	if (spaceValue == 0) return ans;
	else{
		//printf ("entering recursion...\n");
		ans += getFirstTable(CODE, table, first, returnLink, usedPeople, usedPeopleCounter) + mainFunctionDeveloper(CODE, returnValue, returnLink, spaceValue-1, table, first, usedPeople, usedPeopleCounter);	
	}
	//printf ("end\n");
	return ans;
}

int compareTwo(const void *p1, const void *p2){
	const int *pOne = *(int**)p1;
	const int *pTwo = *(int**)p2;
	return(pTwo[1] - pOne[1]);
}

int compareOne(const void *p1, const void *p2){
	const int *pOne = *(int**)p1;
	const int *pTwo = *(int**)p2;
	return(pTwo[2] - pOne[2]);
}

void recursiveDeletion(char **map, char goal, int i, int j, int x, int y, int*deletedSize){
	if (map[i][j] == goal){
		map[i][j] = 'A'; //assigned
		(*deletedSize)++;
		if (i-1 >= 0) recursiveDeletion(map, goal, i-1, j, x, y, deletedSize);
		if (j-1 >= 0) recursiveDeletion(map, goal, i, j-1, x, y, deletedSize);
		if (i+1 < y) recursiveDeletion(map, goal, i+1, j, x, y, deletedSize);
		if (j+1 < x) recursiveDeletion(map, goal, i, j+1, x, y, deletedSize);
	}
	else return;
}

void insertMapDeveloperList(MN **head, MN **tail, int size){
	MN *new = malloc(sizeof(MN));
	new->val = size;
	new->next = NULL;

	if (*head == NULL){
		*head = new;
		*tail = new;
	}
	else{
		(*tail)->next = new;
		*tail = new;
	}
}

void createDeveloperMapList(MN **head, MN **tail, char **map, char goal, int x, int y){
	int deletedSize = 0;

	for (int i=0; i<y; i++){
		for (int j=0; j<x; j++){
			deletedSize = 0;
			if (map[i][j] != goal) continue;
			else (recursiveDeletion(map, goal, i, j, x, y, &deletedSize));
			insertMapDeveloperList(head, tail, deletedSize);
		}
	}
}

int calculateProductMBonus(C *head, C *test, int *returnPosition){
	C *c = head;
	int t = 0;
	int max = 0;
	int compatiblePosition = 1;
	while (c != NULL){
		if (c != test){
			if (strcmp(test->name, c->name) == 0) t = test->bonus * c->bonus;
			if (t > max){
				max = t;
				*returnPosition = compatiblePosition;
			}
		}
		compatiblePosition++;
		c = c->next;
	}
	return max;
}

bool isThere(char s[], P *test){
	for (int i=0; i<100; i++){
		if (strlen(test->skills[i]) < 1) break;
		if (strcmp(s, test->skills[i]) == 0) return true;
	}
	return false;
}

int calculateDiff(P *target, P *test){
	int same = 0;
	for (int i=0; i<100; i++){
		if (strlen(target->skills[i]) < 1) break;
		if (isThere(target->skills[i], test) == true) same++;
	}
	
	return (same * ((target->skillNumber + test->skillNumber) - same));
}

int calculateWP(P *head, P *target, char *company, int *returnPosition, int *sameCompanyBonus){
	P *pointer = head;
	int sum = 0, max = 0, positionInList = 1;
	//printf("head: %s\n", pointer->companyName);
	
	while (pointer != NULL){
		//printf ("comparing with: %s\n", pointer->companyName);
		if (pointer != target){
			sum = calculateDiff(target, pointer);
			if (sum > max){
				*returnPosition = positionInList;
				max = sum;
				strcpy(company, pointer->companyName);
				if (strcmp(company, target->companyName) == 0) *sameCompanyBonus = target->bonusPotential * pointer->bonusPotential;
				else *sameCompanyBonus = 0;
			}
		}
		pointer = pointer->next;
		positionInList++;
	}
	return max;
}

void freeMapList(MN *head){
	MN *c = head;
	MN *p = NULL;

	while (c != NULL){
		if (p != NULL) free(p);
		p = c;
		c = c->next;
	}
	free(p);
}

void freeListPeople(P *head){
	P *c = head;
	P *p = NULL;

	while (c != NULL){
		if (p != NULL) free(p);
		p = c;
		c = c->next;
	}
	free(p);
}

void freeListCompany(C *head){
	C *c = head;
	C *p = NULL;

	while (c != NULL){
		if (p != NULL) free(p);
		p = c;
		c = c->next;
	}
	free(p);
}

void addListCompany(char s[], C **head, C **tail){
	int i = 0;
	char name[M];
	C *new = (C*)malloc(sizeof(C));
		
	if (new == NULL) return;
	new->next = NULL;
	
	while (s[i] != ' '){
		name[i] = s[i];
		i++;
	}
	name[i] = '\0';
	i++;

	//save name
	strcpy(new->name, name);

	int j = 0;
	while (i < strlen(s) && s[i] != '\n'){
		name[j] = s[i];
		j++;
		i++;
	}
	name[j] = '\0';

	//save value
	new->bonus = atoi(name);

	//add list
	if (*head == NULL){
		*head = new;
		*tail = new;
	}
	else{
		(*tail)->next = new;
		*tail = new;
	}
}

void addLP(char s[], P **head, P **tail){
	char company[M];
	int bonusPotential;
	int skillNumber;
	P *new = (P*)malloc(sizeof(P));

	if (new == NULL) return;
	new->next = NULL;
	
	int i = 0, j = 0;
	while (i < strlen(s) && s[i] != ' '){
		company[j] = s[i];
		i++;
		j++;
	}
	company[j] = '\0';
	//printf ("comapny red:%s\n", company);

	//save company inside people
	strcpy(new->companyName, company);

	i++;
	int flagNumber = 0;
	
	//save the bonusPotential & skillNumber
	j = 0;
	int spaceCounter = 0;
	while (spaceCounter < 2){
		if (s[i] != ' '){
			company[j] = s[i];
			i++;
			j++;
		}
		if (s[i] == ' '){
			spaceCounter++;
			company[j] = '\0';
			j = 0;
			if (flagNumber == 0){
				new->bonusPotential = atoi(company);
				flagNumber++;
			}
			else{
				new->skillNumber = atoi(company);
			}
		}
		i++;
	}
	//i is on the first letter of the first skill
	//printf ("b & s: %d %d\n", new->bonusPotential, new->skillNumber);
	
	//save skills
	j = 0;
	int skillscounter = 0;
	while (i < strlen(s)){
		if (s[i] != ' ' && s[i] != '\n'){
			company[j] = s[i];
			j++;
		}
		else{
			company[j] = '\0';
			j = 0;
			strcpy(new->skills[skillscounter], company);
			skillscounter++;
		}
		i++;
	}

	//add list
	if (*head == NULL){
		*head = new;
		*tail = new;
	}
	else{
		(*tail)->next = new;
		*tail = new;
	}
}

void getXY(char s[], int *x, int *y){
	int i = 0, j = 0;
	char X[M];
	char Y[M];
	
	while (i <  strlen(s) && s[i] != ' '){
		X[j] = s[i];
		i++;
		j++;
	}
	X[j] = '\0';
	i++;
	j = 0;
	while (i <  strlen(s) && s[i] != ' '){
		Y[j] = s[i];
		i++;
		j++;
	}
	Y[j] = '\0';
	(*x) = atoi(X);
	(*y) = atoi(Y);
	printf ("Map size: %d x %d\n", *x, *y);
}
