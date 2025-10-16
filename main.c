#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CITIES 30
int  addNewCity(char cities[MAX_CITIES][100],int currentCityCount);
void  updateCity(char cities[MAX_CITIES][100],int currentCityCount);
int removeCity(char cities[MAX_CITIES][100],int currentCityCount);
void dislayCurentities(char cities[MAX_CITIES][100],int currentCityCount);
void storeDistances(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100]);
void displayDistances(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100],int currentCityCount);
int main()
{
    int choice=0;
    int currentCityCount=0;
    char cities[MAX_CITIES][100];
    int distances[MAX_CITIES][MAX_CITIES]= {0};
    do
    {
        printf("Logistic Management System \n");
        printf("1.Add City \n");
        printf("2.update city \n");
        printf("3.remove city \n");
        printf("4.Display all cities\n");
        printf("5.Store Distances \n");
        printf("6.Display distances \n");
        printf("Enter your choice:");
        scanf(" %d",&choice);

        switch(choice)
        {
        case 1:
            currentCityCount=addNewCity(cities,currentCityCount);
            break;
        case 2:
            updateCity(cities,currentCityCount);
            break;
        case 3:
            currentCityCount=removeCity(cities,currentCityCount);
            break;
        case 4:
            dislayCurentities(cities,currentCityCount);
            break;
        case 5:
            storeDistances(distances,cities);
            break;
        case 6:
            displayDistances(distances,cities,currentCityCount);
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
                break;
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
    return currentCityCount;
}

void  updateCity(char cities[MAX_CITIES][100],int currentCityCount)
{
    char oldCity[100],newCity[100];
    int found=0;
    printf("Enter old name of the city:");
    scanf(" %s",oldCity);

    for(int i=0; i<currentCityCount; i++)
    {
        if(strcmp(oldCity, cities[i]) == 0)
        {
            printf("Enter New City Name:");
            scanf(" %s",newCity);
            strcpy(cities[i],newCity);
            printf("City name updated successfull\n");
            found=1;
            break;
        }
    }
    if(found==0)
    {

        printf("City not found !!\n");

    }
}

int removeCity(char cities[MAX_CITIES][100],int currentCityCount)
{
    char removeCity[100];
    int found=0;
    printf("Enter the city name you want to remove:");
    scanf(" %s",removeCity);

    for(int i=0; i<currentCityCount; i++)
    {
        if(strcmp(removeCity, cities[i]) == 0)
        {
            for(int j=i; j<currentCityCount-1; j++)
            {
                strcpy(cities[j],cities[j+1]);
            }
            found=1;
            printf("City removed Successfully \n");
            currentCityCount--;
        }
    }

    if(found==0)
    {
        printf("City not found !!\n");
    }
    return currentCityCount;
}

void dislayCurentities(char cities[MAX_CITIES][100],int currentCityCount)
{
    for(int i=0; i<currentCityCount; i++)
    {

        printf("%d %s \n",i,cities[i]);
    }
}
void storeDistances(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100])
{
    int city1=0,city2=0,distance=0;

    printf("Enter index of the start city:");
    scanf(" %d",&city1);
    printf("Enter index of the end city:");
    scanf(" %d",&city2);
    if(city1==city2)
    {
        printf("distance between same city is zero \n");
        distances[city1][city2] = 0;
    }
    else
    {
        printf("Enter distance between two cities:");
        scanf(" %d",&distance);

        distances[city1][city2]=distance;
        distances[city2][city1]=distance;
        printf("You stored distances from %s to %s successfully. \n ",cities[city1],cities[city2]);
    }


}

void displayDistances(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100],int currentCityCount)
{

    printf("\n");
    printf("%-15s", "From/To");
    for (int i = 0; i < currentCityCount; i++)
        printf("%-10s\t", cities[i]);
        printf("\n");
    for (int j = 0; j < currentCityCount; j++)
    {
        printf("%-15s\t", cities[j]);
        for (int k = 0; k < currentCityCount; k++)
        {
            if(j==k)
            {
                printf("%-10d\t",distances[j][k]);
            }
            else if(distances[j][k]==0)
            {
                printf("%-15s\t","N/A");
            }
            else
            {
                printf("%-10d\t", distances[j][k]);
            }
        }
        printf("\n");
    }
    printf("\n");
}
