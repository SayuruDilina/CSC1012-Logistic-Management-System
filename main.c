#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CITIES 30
int  addNewCity(char cities[MAX_CITIES][100],int currentCityCount);
int main()
{
    int choice=0;
    int currentCityCount=0;
    char cities[MAX_CITIES][100];

    do
    {
        printf("Logistic Management System \n");
        printf("1.Add City \n");

        printf("Enter your choice:");
        scanf(" %d",&choice);

        switch(choice)
        {
        case 1:
            currentCityCount=addNewCity(cities,currentCityCount);
            break;
        default:
            printf("Invalid");

        }

    }
    while(choice!=-1);
    return 0;
}

int  addNewCity(char cities[MAX_CITIES][100],int currentCityCount)
{
    char status='N';
    char tempCity[MAX_CITIES][100];
    int index=0;
    int tempStatus=1;
    do
    {
        tempStatus = 1;
        printf("Enter City Name:");
       scanf(" %s",tempCity[index]);


        for(int i=0; i<currentCityCount; i++)
        {
            if(strcmp(tempCity[index], cities[i]) == 0)
            {
                printf("City already Added, try other one \n");
                tempStatus=0;
            }
        }

        if(tempStatus==1)
        {
            strcpy(cities[currentCityCount], tempCity[index]);
            printf("City added Successfully \n");
            index++;
            currentCityCount++;

        }
        printf("\n");
        printf("Do you want to add another city(Y/N):");
        scanf(" %c",&status);
        printf("\n");
    }
    while(status != 'N');

}
