#include "Fits.h"
#include "LinkedList.h"

#define MEMORY_SIZE 128

void firstFit(LinkedList *processHead)
{
	LinkedList *processTemp = processHead;
	LinkedList *processTemp2 = NULL;
	LinkedList *readyProcesses = NULL;
	LinkedList *readyHead = NULL;
	LinkedList *runningHead = NULL;

	//variables for the memory
	int memory[MEMORY_SIZE];
	int i = 0;
	int spaces = 0;
	//variables for the amount of holes
	int holeFlag = 0;
	int done = 0;
	int numOfHoles = 0;
	int avgHoles = 0;
	//variables for the amount of processes(and average processes)
	int numOfProcesses = 0;
	int totalLoads = 0;
	int avgProcesses = 0;
	//variables for current mem and to calculated average mem 
	int currentMem = 0;
	float avgMem = 0;
	int j = 0;
	char buffID[1000];


	printf("First Fit:\n");

	for(i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}

	//loops till end of list loading all processes
	while(processTemp != NULL)
	{
		//loads all processes into ready list
		readyProcesses = LL_createCopy(processTemp->p);
		//buffID[j] = processTemp->p->id;

		if(readyHead == NULL)
		{
			readyHead = readyProcesses;
		}
		else
		{
			readyHead = LL_addToBack(readyHead, readyProcesses);
		}

		processTemp = processTemp->next;
	}
	processTemp = NULL;

	readyProcesses = readyHead;
	while(readyProcesses != NULL)
	{
		numOfHoles = 1;
		done = 0;
		spaces = 0;

		if(readyProcesses->p->uses >= 3)
		{
			break;
		}

		for(i = 0; i < MEMORY_SIZE; i++)
		{
			if(memory[i] == 0)
			{
				spaces++;
			}
			else
			{
				spaces = 0;
			}

			if(!done)
			{
				if(spaces >= readyProcesses->p->size)
				{
					//space found, insert process
					readyProcesses->p->location = i + 1 - spaces;
					readyProcesses->p->end = readyProcesses->p->location + readyProcesses->p->size;
					readyProcesses->p->uses++;

					numOfProcesses++;
					totalLoads++;
					currentMem += readyProcesses->p->size;
					
					for(i = readyProcesses->p->location; i < readyProcesses->p->end; i++)
					{
						memory[i] = 1;
					}
					holeFlag = 0;
					for(i = 0; i < MEMORY_SIZE; i++)
					{
						if(memory[i] == 0)
						{
							if(holeFlag == 1)
							{
								//free mem started
								holeFlag = 2;
							}
						}
						else
						{
							if(holeFlag == 0)
							{
								//mem taken
								holeFlag = 1;
							}
							else if(holeFlag == 2)
							{
								//mem taken with gaps
								numOfHoles++;
								holeFlag = 0;
							}
						}
					}

					processTemp = readyProcesses->next;
					readyProcesses->next = NULL;
					runningHead = LL_addToBack(runningHead, readyProcesses);
					spaces = 0;
					done = 1;
					break;
				}

				if(i == 127)
				{				
					//space not found, remove one process (oldest)
					numOfProcesses--;
					currentMem -= runningHead->p->size;

					for(i = runningHead->p->location; i < runningHead->p->end; i++)
					{
						memory[i] = 0;
					}

					processTemp2 = runningHead;
					runningHead = runningHead->next;
					processTemp2->next = NULL;
					readyProcesses = LL_addToBack(readyProcesses, processTemp2);
					i = -1;
					spaces = 0;
				}
			}
		}
		avgMem += (float)currentMem / MEMORY_SIZE;
		avgProcesses += numOfProcesses;
		avgHoles += numOfHoles;
		buffID[j] = processTemp->p->id;
		printf("%c loaded, #processes = %d, #holes = %d, per_memusage = %f, cumulative per_memusage= %f\n",buffID[j-1]
			, numOfProcesses, numOfHoles, (float)currentMem / MEMORY_SIZE * 100, (float)avgMem / totalLoads * 100);

		readyProcesses = processTemp;
		j++;
	}

	if(totalLoads != 0)
	{
		printf("\nTotal loads= %d, avg processes= %f, avg holes= %f, cumulative per_mem= %f\n\n"
			, totalLoads, (float)avgProcesses / totalLoads, (float)avgHoles / totalLoads, avgMem / totalLoads * 100);
	}
	else
	{
		printf("\nTotal loads: 0, avg processes: 0, avg holes: 0, cumulative per_mem: 0\n\n");
	}

	LL_deleteCopyList(runningHead);
	LL_deleteCopyList(readyProcesses);
}

void bestFit(LinkedList *processHead)
{
	LinkedList *processTemp = processHead;
	LinkedList *processTemp2 = NULL;
	LinkedList *readyProcesses = NULL;
	LinkedList *readyHead = NULL;
	LinkedList *runningHead = NULL;

	//include variables to keep track of the 'best' spaces I will be inserting to
	int memory[MEMORY_SIZE];
	int i = 0;
	int spaces = 0;
	int bestSpace = MEMORY_SIZE;
	int bestLoc = 0;

	//variables for the amount of holes
	int holeFlag = 0;
	int done = 0;
	int numOfHoles = 0;
	int avgHoles = 0;
	//variables for the amount of processes(and average processes)
	int numOfProcesses = 0;
	int totalLoads = 0;
	int avgProcesses = 0;
	//variables for current mem and to calculated average mem 
	int currentMem = 0;
	float avgMem = 0;

	int j = 0;
	char buffID[1000];

	printf("Best Fit:\n");

	for(i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}

	while(processTemp != NULL)
	{
		//loads all processes into ready list
		readyProcesses = LL_createCopy(processTemp->p);

		if(readyHead == NULL)
		{
			readyHead = readyProcesses;
		}
		else
		{
			readyHead = LL_addToBack(readyHead, readyProcesses);
		}

		processTemp = processTemp->next;
	}
	processTemp = NULL;

	readyProcesses = readyHead;
	while(readyProcesses != NULL)
	{
		numOfHoles = 1;
		done = 0;
		spaces = 0;
		bestSpace = MEMORY_SIZE;
		bestLoc = 0;

		if(readyProcesses->p->uses >= 3)
		{
			break;
		}

		for(i = 0; i < MEMORY_SIZE; i++)
		{

			if(memory[i] == 0)
			{
				spaces++;
				if(i == 127)
				{
					if(spaces >= readyProcesses->p->size)
					{
						if(spaces < bestSpace)
						{
							bestSpace = spaces;
							bestLoc = i + 1 - spaces;
						}
						done = 1;
						spaces = 0;
					}
				}
			}
			else
			{
				if(spaces >= readyProcesses->p->size)
				{
					if(spaces < bestSpace)
					{
						bestSpace = spaces;
						bestLoc = i - spaces;
					}
					done = 1;
				}
				spaces = 0;
			}

			if(!done)
			{
				if(i == 127)
				{				
					//space not found, remove one process
					numOfProcesses--;
					currentMem -= runningHead->p->size;

					for(i = runningHead->p->location; i < runningHead->p->end; i++)
					{
						memory[i] = 0;
					}

					processTemp2 = runningHead;
					runningHead = runningHead->next;
					processTemp2->next = NULL;
					readyProcesses = LL_addToBack(readyProcesses, processTemp2);
					i = -1;
					spaces = 0;
				}
			}
		}

		//space found, insert process
		readyProcesses->p->location = bestLoc;
		readyProcesses->p->end = readyProcesses->p->location + readyProcesses->p->size;
		readyProcesses->p->uses++;

		numOfProcesses++;
		totalLoads++;
		currentMem += readyProcesses->p->size;
		
		for(i = readyProcesses->p->location; i < readyProcesses->p->end; i++)
		{
			memory[i] = 1;
		}
		holeFlag = 0;
		for(i = 0; i < MEMORY_SIZE; i++)
		{
			if(memory[i] == 0)
			{
				if(holeFlag == 1)
				{
					//free mem started
					holeFlag = 2;
				}
			}
			else
			{
				if(holeFlag == 0)
				{
					//mem taken
					holeFlag = 1;
				}
				else if(holeFlag == 2)
				{
					//mem taken with gap
					numOfHoles++;
					holeFlag = 0;
				}
			}
		}
		processTemp = readyProcesses->next;
		readyProcesses->next = NULL;
		runningHead = LL_addToBack(runningHead, readyProcesses);

		avgMem += (float)currentMem / MEMORY_SIZE;
		avgProcesses += numOfProcesses;
		avgHoles += numOfHoles;
		buffID[j] = processTemp->p->id;
		printf("%c loaded, #processes = %d, #holes = %d, per_memusage = %f, cumulative per_memusage= %f\n",buffID[j-1]
			, numOfProcesses, numOfHoles, (float)currentMem / MEMORY_SIZE * 100, (float)avgMem / totalLoads * 100);

		readyProcesses = processTemp;
		j++;
	}

	if(totalLoads != 0)
	{
		printf("\nTotal loads= %d, avg processes= %f, avg holes= %f, cumulative per_mem= %f\n\n"
			, totalLoads, (float)avgProcesses / totalLoads, (float)avgHoles / totalLoads, avgMem / totalLoads * 100);
	}
	else
	{
		printf("\nTotal loads= 0, avg processes= 0, avg holes= 0, cumulative per_mem= 0\n\n");
	}

	LL_deleteCopyList(runningHead);
	LL_deleteCopyList(readyProcesses);
}

void nextFit(LinkedList *processHead)
{
	LinkedList *processTemp = processHead;
	LinkedList *processTemp2 = NULL;
	LinkedList *readyProcesses = NULL;
	LinkedList *readyHead = NULL;
	LinkedList *runningHead = NULL;

	int memory[MEMORY_SIZE];
	int i = 0;
	int spaces = 0;
	int lastPos = 0;

	//variables for the amount of holes
	int holeFlag = 0;
	int done = 0;
	int looped = 0;
	int numOfHoles = 0;
	int avgHoles = 0;
	//variables for the amount of processes(and average processes)
	int numOfProcesses = 0;
	int totalLoads = 0;
	int avgProcesses = 0;
	//variables for current mem and to calculated average mem 
	int currentMem = 0;
	float avgMem = 0;

	int j = 0;
	char buffID[1000];

	printf("Next Fit:\n");

	for(i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}
	i = 0;

	while(processTemp != NULL)
	{
		//loads all processes into ready list
		readyProcesses = LL_createCopy(processTemp->p);

		if(readyHead == NULL)
		{
			readyHead = readyProcesses;
		}
		else
		{
			readyHead = LL_addToBack(readyHead, readyProcesses);
		}

		processTemp = processTemp->next;
	}
	processTemp = NULL;

	readyProcesses = readyHead;
	while(readyProcesses != NULL)
	{
		numOfHoles = 1;
		done = 0;
		spaces = 0;

		if(readyProcesses->p->uses >= 3)
		{
			break;
		}

		i = lastPos;
		if(i == MEMORY_SIZE)
		{
			i = 0;
		}
		for(i = i; i < MEMORY_SIZE; i++)
		{
			if(i == 0)
			{
				looped = 1;
			}


			if(memory[i] == 0)
			{
				spaces++;
			}
			else
			{
				spaces = 0;
			}

			if(!done)
			{
				if(spaces >= readyProcesses->p->size)
				{
					//space found, insert process
					readyProcesses->p->location = i + 1 - spaces;
					readyProcesses->p->end = readyProcesses->p->location + readyProcesses->p->size;
					lastPos = i;
					readyProcesses->p->uses++;

					numOfProcesses++;
					totalLoads++;
					currentMem += readyProcesses->p->size;
					
					for(i = readyProcesses->p->location; i < readyProcesses->p->end; i++)
					{
						memory[i] = 1;
					}
					holeFlag = 0;
					for(i = 0; i < MEMORY_SIZE; i++)
					{
						if(memory[i] == 0)
						{
							if(holeFlag == 1)
							{
								//free mem started
								holeFlag = 2;
							}
						}
						else
						{
							if(holeFlag == 0)
							{
								//mem taken
								holeFlag = 1;
							}
							else if(holeFlag == 2)
							{
								//mem taken with gap
								numOfHoles++;
								holeFlag = 0;
							}
						}
					}

					processTemp = readyProcesses->next;
					readyProcesses->next = NULL;
					runningHead = LL_addToBack(runningHead, readyProcesses);
					spaces = 0;
					done = 1;
					break;
				}

				if(i == 127)
				{
					if(looped)
					{		
						//space not found, remove one process
						numOfProcesses--;
						currentMem -= runningHead->p->size;

						for(i = runningHead->p->location; i < runningHead->p->end; i++)
						{
							memory[i] = 0;
						}

						processTemp2 = runningHead;
						runningHead = runningHead->next;
						processTemp2->next = NULL;
						readyProcesses = LL_addToBack(readyProcesses, processTemp2);
					}
					i = -1;
					spaces = 0;
				}
			}
		}
		avgMem += (float)currentMem / MEMORY_SIZE;
		avgProcesses += numOfProcesses;
		avgHoles += numOfHoles;
		buffID[j] = processTemp->p->id;
		printf("%c loaded, #processes = %d, #holes = %d, per_memusage = %f, cumulative per_memusage= %f\n",buffID[j-1]
			, numOfProcesses, numOfHoles, (float)currentMem / MEMORY_SIZE * 100, (float)avgMem / totalLoads * 100);

		readyProcesses = processTemp;
		j++;
	}

	if(totalLoads != 0)
	{
		printf("\nTotal loads= %d, avg processes= %f, avg holes= %f, cumulative per_mem= %f\n\n"
			, totalLoads, (float)avgProcesses / totalLoads, (float)avgHoles / totalLoads, avgMem / totalLoads * 100);
	}
	else
	{
		printf("\nTotal loads: 0, avg processes: 0, avg holes: 0, cumulative per_mem: 0\n\n");
	}

	LL_deleteCopyList(runningHead);
	LL_deleteCopyList(readyProcesses);
}

void worstFit(LinkedList *processHead)
{
	LinkedList *processTemp = processHead;
	LinkedList *processTemp2 = NULL;
	LinkedList *readyProcesses = NULL;
	LinkedList *readyHead = NULL;
	LinkedList *runningHead = NULL;

	int memory[MEMORY_SIZE];
	int i = 0;
	int spaces = 0;
	int worstSpace = 0;
	int worstLoc = 0;

	//variables for the amount of holes
	int holeFlag = 0;
	int done = 0;
	int numOfHoles = 0;
	int avgHoles = 0;
	//variables for the amount of processes(and average processes)
	int numOfProcesses = 0;
	int totalLoads = 0;
	int avgProcesses = 0;
	//variables for current mem and to calculated average mem 
	int currentMem = 0;
	float avgMem = 0;

	int j = 0;
	char buffID[1000];

	printf("Worst Fit:\n");

	for(i = 0; i < MEMORY_SIZE; i++)
	{
		memory[i] = 0;
	}

	while(processTemp != NULL)
	{
		//loads all processes into ready list
		readyProcesses = LL_createCopy(processTemp->p);

		if(readyHead == NULL)
		{
			readyHead = readyProcesses;
		}
		else
		{
			readyHead = LL_addToBack(readyHead, readyProcesses);
		}

		processTemp = processTemp->next;
	}
	processTemp = NULL;

	readyProcesses = readyHead;
	while(readyProcesses != NULL)
	{
		numOfHoles = 1;
		done = 0;
		spaces = 0;
		worstSpace = 0;
		worstLoc = 0;

		if(readyProcesses->p->uses >= 3)
		{
			break;
		}

		for(i = 0; i < MEMORY_SIZE; i++)
		{

			if(memory[i] == 0)
			{
				spaces++;
				if(i == 127)
				{
					if(spaces >= readyProcesses->p->size)
					{
						if(spaces > worstSpace)
						{
							worstSpace = spaces;
							worstLoc = i + 1 - spaces;
						}
						done = 1;
						spaces = 0;
					}
				}
			}
			else
			{
				if(spaces >= readyProcesses->p->size)
				{
					if(spaces > worstSpace)
					{
						worstSpace = spaces;
						worstLoc = i - spaces;
					}
					done = 1;
				}
				spaces = 0;
			}

			if(!done)
			{
				if(i == 127)
				{				
					//space not found, remove one process
					numOfProcesses--;
					currentMem -= runningHead->p->size;

					for(i = runningHead->p->location; i < runningHead->p->end; i++)
					{
						memory[i] = 0;
					}

					processTemp2 = runningHead;
					runningHead = runningHead->next;
					processTemp2->next = NULL;
					readyProcesses = LL_addToBack(readyProcesses, processTemp2);
					i = -1;
					spaces = 0;
				}
			}
		}

		//space found, insert process
		readyProcesses->p->location = worstLoc;
		readyProcesses->p->end = readyProcesses->p->location + readyProcesses->p->size;
		readyProcesses->p->uses++;

		numOfProcesses++;
		totalLoads++;
		currentMem += readyProcesses->p->size;
		
		for(i = readyProcesses->p->location; i < readyProcesses->p->end; i++)
		{
			memory[i] = 1;
		}
		holeFlag = 0;
		for(i = 0; i < MEMORY_SIZE; i++)
		{
			if(memory[i] == 0)
			{	
				if(holeFlag == 1)
				{
					//free mem started
					holeFlag = 2;
				}
			}
			else
			{
				if(holeFlag == 0)
				{
					//mem taken
					holeFlag = 1;
				}
				else if(holeFlag == 2)
				{
					//mem taken with gap
					numOfHoles++;
					holeFlag = 0;
				}
			}
		}
		processTemp = readyProcesses->next;
		readyProcesses->next = NULL;
		runningHead = LL_addToBack(runningHead, readyProcesses);

		avgMem += (double)currentMem / MEMORY_SIZE;
		avgProcesses += numOfProcesses;
		avgHoles += numOfHoles;
		buffID[j] = processTemp->p->id;
		printf("%c loaded, #processes = %d, #holes = %d, per_memusage = %f, cumulative per_memusage= %f\n",buffID[j-1]
			, numOfProcesses, numOfHoles, (double)currentMem / MEMORY_SIZE * 100, (double)avgMem / totalLoads * 100);

		readyProcesses = processTemp;
		j++;
	}

	if(totalLoads != 0)
	{
		printf("\nTotal loads= %d, avg processes= %f, avg holes= %f, cumulative per_memusage= %f\n\n"
			, totalLoads, (double)avgProcesses / totalLoads, (double)avgHoles / totalLoads, avgMem / totalLoads * 100);
	}
	//probably would never happen but just for error checking 
	else
	{
		printf("\nTotal loads= 0, avg processes= 0, avg holes= 0, cumulative per_memusage= 0\n\n");
	}

	LL_deleteCopyList(runningHead);
	LL_deleteCopyList(readyProcesses);
}
