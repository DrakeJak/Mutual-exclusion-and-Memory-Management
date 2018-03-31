/*
	Arvids Jakobsons
	Student Number: 0953757
	Assignment 3 Pat_2: Holes
	****LINKEDLIST TAKEN FROM PREVIOUS CLASS WORK****
*/

#include "LinkedList.h"
#include "Fits.h"



#define MEMORY_SIZE 128

//including the parser function of type linked list
LinkedList * parse(LinkedList *processHead, char* fname);
Boolean isNumber(char *number);

int main(int argc, char **argv)
{
	LinkedList *processHead = NULL;

	if(argc != 2)
	{
		printf("Invalid arguement size (should be only one, the file name)\n");
		exit(1);
	}
	processHead = parse(processHead, argv[1]);
	//run the different alogrithms, delete list at the end to help with mem leaks
	firstFit(processHead);
	bestFit(processHead);
	worstFit(processHead);
	nextFit(processHead);
	LL_deleteList(processHead);

	return 0;
}


LinkedList * parse(LinkedList *processHead, char* fname)
{
	char* line = NULL;
	char* token = NULL;
	FILE *fp = NULL;
	LinkedList *processTemp = NULL;

	fp = fopen(fname, "r");
	if(!fp)
	{
		printf("Error opening file\n");
		exit(1);
	}

	line = malloc(sizeof(char)*MEMORY_SIZE);

	while((fgets(line, MEMORY_SIZE, fp)) != NULL)
	{
		token = strtok(line, " ");
		if(token == NULL)
		{
			printf("Parse error\n");
			fclose(fp);
			free(line);
			exit(1);
		}
		processTemp = LL_create(token[0]);
		processHead = LL_addToBack(processHead, processTemp);

		token = strtok(NULL, "\n");
		if(token == NULL)
		{
			printf("Parse error\n");
			LL_deleteList(processHead);
			fclose(fp);
			free(line);
			exit(1);
		}
		if(!isNumber(token))
		{
			printf("Invalid number\n");
			LL_deleteList(processHead);
			fclose(fp);
			free(line);
			exit(1);
		}
		processTemp->p->size = atoi(token);
		if(processTemp->p->size < 1 || processTemp->p->size > MEMORY_SIZE)
		{
			printf("Invalid number\n");
			LL_deleteList(processHead);
			fclose(fp);
			free(line);
			exit(1);
		}

		processTemp->p->uses = 0;
		processTemp->p->location = -1;
		processTemp->p->end = -1;
	}

	fclose(fp);
	free(line);
	return processHead;
}

Boolean isNumber(char *number)
{
	int i = 0;

	for(i = 0; i < strlen(number); i++)
	{
		if(!isdigit(number[i]))
		{
			return false;
		}
	}
	return true;
}