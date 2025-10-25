#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX_CITIES 30
#define MAX_ORDERS 50
#define VAN 0
#define TRUCK 1
#define LORRY 2
#define CAPACITY 0
#define  RATE_PER_KM 1
#define AVG_SPEED 2
#define FUEL_EFFICIENCY 3
int bestPath[MAX_CITIES];
int tempPath[MAX_CITIES];
int visited[MAX_CITIES];
int minDistance;
int  addNewCity(char cities[MAX_CITIES][100],int currentCityCount);
void  updateCity(char cities[MAX_CITIES][100],int currentCityCount);
int removeCity(char cities[MAX_CITIES][100],int currentCityCount);
void dislayCurentities(char cities[MAX_CITIES][100],int currentCityCount);
void storeDistances(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100]);
void displayDistances(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100],int currentCityCount);
void storeVehicleDetails(int vehicleTypes[3][4]);
int inputDeliveryOrder(int  orders[MAX_ORDERS][4],int vehicleTypes[3][4],int currentOrderCount);
double calcDeliveryCost(int D,int R,int W);
double calcEstimatedDiliveryTime(int D,int S);
double  calcFuelConsumption(int D,int E);
double fuelCost(double fuelUsed,int F );
double totOpretaionalCost(double deliveryCost,double fuelCost);
double calcProfit(double cost);
double calcCharge(double totCost,double profit);
void handleDeliveryOutput(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100],int currentCityCount,
                          int  orders[MAX_ORDERS][4],int vehicleTypes[3][4],int currentOrderCount
                          ,int *totalDeliveriesCompleted,int *totalDistanceCovered,double *totalDeliveryTimeHours,
                          double *totalRevenue,double *totalProfit,int *longestRoute,int *shortestRoute
                          ,int orderIndex,double deliveryData[MAX_ORDERS][7],int routesData[]);
void printReports(int orders[MAX_ORDERS][4],
                  double deliveryData[MAX_ORDERS][7],
                  int routesData[],int orderCount);

void saveRoutesToFile(char cities[MAX_CITIES][100], int distances[MAX_CITIES][MAX_CITIES], int cityCount);
int loadRoutesFromFile(char cities[MAX_CITIES][100], int distances[MAX_CITIES][MAX_CITIES]);
int loadDeliveriesFromFile(int orders[MAX_ORDERS][4],double deliveryData[MAX_ORDERS][7],int routesData[]);
void saveDeliveriesToFile(int orders[MAX_ORDERS][4],double deliveryData[MAX_ORDERS][7],int routesData[], int orderCount);
int handleCityManagement(char cities[MAX_CITIES][100],int *currentCityCount);
int handleDistanceManagement(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100],int currentCityCount);
double storeDeliveries(double deliveryData[MAX_ORDERS][7],double baseCost,
                       double fuelUsed,double fuelC,double totOp,
                       double profit,double custCharge,double estTime,int orderIndex
                      );

int handleCityAvailableCheck(int currentCityCount);
int main()
{
    int choice=0;
    int currentCityCount=0;
    char cities[MAX_CITIES][100];
    int distances[MAX_CITIES][MAX_CITIES]= {0};
    int vehicleTypes[3][4];
    int  orders[MAX_ORDERS][4];
    int currentOrderCount=0;

    int totalDistanceCovered = 0;
    double totalDeliveryTimeHours = 0.0;
    double totalRevenue = 0.0;
    double totalProfit= 0.0;
    int longestRoute=0;
    int shortestRoute=0;
    double deliveryData[MAX_ORDERS][7];
    int routesData[2];
    currentCityCount = loadRoutesFromFile(cities, distances);
    currentOrderCount = loadDeliveriesFromFile(orders,deliveryData,routesData);
    printf("Loaded %d cities and %d delivery records from files.\n",
           currentCityCount, currentOrderCount);
    int totalDeliveriesCompleted = currentOrderCount;
    int status=0;
    do
    {
        printf("=============================================================== \n");
        printf("\t\tLOGISTIC MANAGEMENT SYSTEM \n");
        printf("===============================================================\n");
        printf("\t \t 1. CITY  MANAGEMENT \n");
        printf("\t \t 2. VIEW ALL CITIES\n");
        printf("\t \t 3. DISTANCE MANAGEMENT \n");
        printf("\t \t 4. PLACE ORDER \n");
        printf("\t \t 5. PRINT REPORTS \n");
        printf("\t \t 6. EXIT \n");
        printf("===============================================================\n");
        printf("\nEnter your choice:");
        scanf(" %d",&choice);

        switch(choice)
        {
        case 1:
            handleCityManagement(cities,&currentCityCount);
            break;
        case 2:
            dislayCurentities(cities,currentCityCount);
            break;
        case 3:
            status=handleCityAvailableCheck(currentCityCount);
            if(status==-1)
            {
                printf("There are no data available ..\n");
            }
            else
            {
                handleDistanceManagement(distances,cities,currentCityCount);
            }
            break;
        case 4:
            storeVehicleDetails(vehicleTypes);
            status=handleCityAvailableCheck(currentCityCount);
            if(status==-1)
            {
                printf("There are no data available ..\n");
            }
            else
            {
                currentOrderCount=inputDeliveryOrder(orders,vehicleTypes,currentOrderCount);
                int orderIndex=currentOrderCount-1;
                handleDeliveryOutput(distances,cities,currentCityCount,orders,vehicleTypes,currentOrderCount,
                                     &totalDeliveriesCompleted,&totalDistanceCovered,&totalDeliveryTimeHours,&totalRevenue,&totalProfit
                                     ,&longestRoute,&shortestRoute,orderIndex,deliveryData,routesData);
            }

            break;

        case 5:
            status=handleCityAvailableCheck(currentCityCount);
            if(status==-1)
            {
                printf("There are no data available ..\n");
            }
            else
            {
                printReports(orders, deliveryData,routesData,currentOrderCount);
            }
            break;
        case 6:
            saveRoutesToFile(cities, distances, currentCityCount);
            saveDeliveriesToFile(orders,deliveryData,routesData, currentOrderCount);
            printf("\nData saved successfully. Exiting program...\n");
            return 0;
            break;
        default:
            printf("Invalid");
        }

    }
    while(choice!=-1);

    saveRoutesToFile(cities, distances, currentCityCount);
    saveDeliveriesToFile(orders,deliveryData,routesData,currentOrderCount);
    printf("\nData saved successfully to routes.txt and deliveries.txt\n");

    return 0;
}
int handleCityManagement(char cities[MAX_CITIES][100],int *currentCityCount)
{
    int choice=0;
    do
    {
        printf("\n=============================================================== \n");
        printf("\t\t CITY MANAGEMENT \n");
        printf("===============================================================\n");
        printf("\t \t 1. ADD CITY\n");
        printf("\t \t 2. UPDATE CITY\n");
        printf("\t \t 3. REMOVE CITY\n");
        printf(" \t \t 4. BACK TO MAIN MENU\n");
        printf("===============================================================\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
        case 1:
            *currentCityCount = addNewCity(cities, *currentCityCount);
            break;
        case 2:
            updateCity(cities, *currentCityCount);
            break;
        case 3:
            *currentCityCount = removeCity(cities, *currentCityCount);
            break;
        case 4:
            printf("Returning to Main Menu...\n");
            system("cls");
            return 0;
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
    while(choice!=-1);

}
int handleDistanceManagement(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100],int currentCityCount)
{

    int choice=0;
    do
    {
        printf("\n===============================================================\n");
        printf("\t\t DISTANCE MANAGMENT\n");
        printf("===============================================================\n");
        printf("\t \t 1. STORE DISTANCES\n");
        printf("\t \t 2. DISPLAY DISTANCES\n");
        printf("\t \t 3. BACK TO MAIN MENU\n");
        printf("===============================================================\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
        case 1:
            storeDistances(distances,cities);
            break;
        case 2:
            displayDistances(distances,cities,currentCityCount);
            break;
        case 3:
            printf("Returning to Main Menu...\n");
            system("cls");
            return 0;
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
    while(choice!=-1);
}

int  addNewCity(char cities[MAX_CITIES][100],int currentCityCount)
{
    char status='N';
    char tempCity[MAX_CITIES][100];
    int index=0;
    int tempStatus=1;
    printf("\n===============================================================\n");
    printf("\t\t ADD CITY\n");
    printf("===============================================================\n");
    do
    {
        tempStatus = 1;
        printf("\nEnter City Name:");
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
    printf("\n===============================================================\n");
    printf("\t\t UPDATE CITY\n");
    printf("===============================================================\n");
    printf("Enter old name of the city:");
    scanf(" %s",oldCity);

    for(int i=0; i<currentCityCount; i++)
    {
        if(strcmp(oldCity, cities[i]) == 0)
        {
            printf("\nEnter New City Name:");
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
    printf("\n===============================================================\n");
    printf("\t\t REMOVE CITY\n");
    printf("===============================================================\n");
    printf("\nEnter the city name you want to remove:");
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

int handleCityAvailableCheck(int currentCityCount)
{
    if(currentCityCount>0)
    {
        return 1;
    }
    else
    {
        return -1;
    }

}
void dislayCurentities(char cities[MAX_CITIES][100],int currentCityCount)
{
    printf("\n===============================================================\n");
    printf("\t\t ALL CITIES \n");
    printf("===============================================================\n");
    int status=handleCityAvailableCheck(currentCityCount);
    if(status==-1)
    {
        printf("There are no data available \n");
    }
    else
    {
        printf("\n+------------------------------------------------------------+\n");
        printf("| %-6s | %-48s |\n", "ID", "       CITY NAME");
        printf("+------------------------------------------------------------+\n");
        for(int i=0; i<currentCityCount; i++)
        {

            printf("| %-6d | %-48s |\n", i, cities[i]);

        }
        printf("+------------------------------------------------------------+\n\n");
    }

}
void storeDistances(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100])
{
    int city1=0,city2=0,distance=0;

    printf("\n===============================================================\n");
    printf("\t\t STORE DISTANCES\n");
    printf("===============================================================\n");
    printf("\nEnter index of the start city:");
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
    printf("\n===============================================================\n");
    printf("\t\t STORED DISTANCES\n");
    printf("===============================================================\n");
    printf("\n");
    printf("\n+--------------------------------------------------------+\n");
    printf("| %-15s |", "From/To");
    for (int i = 0; i < currentCityCount; i++)
        printf(" %-10s |", cities[i]);
    printf("\n+--------------------------------------------------------+\n");

    for (int j = 0; j < currentCityCount; j++)
    {
        printf("| %-15s |", cities[j]);
        for (int k = 0; k < currentCityCount; k++)
        {
            if(j==k)
            {
                printf(" %-10d |", distances[j][k]);
            }
            else if(distances[j][k]==0)
            {
                printf(" %-10s |", "N/A");
            }
            else
            {
                printf(" %-10d |", distances[j][k]);
            }
        }
        printf("\n+--------------------------------------------------------+\n");
    }
    printf("\n");
}
void storeVehicleDetails(int vehicleTypes[3][4])
{

    vehicleTypes[VAN][CAPACITY]=1000;
    vehicleTypes[VAN][RATE_PER_KM]=30;
    vehicleTypes[VAN][FUEL_EFFICIENCY]=12;
    vehicleTypes[VAN][AVG_SPEED]=60;

    vehicleTypes[TRUCK][CAPACITY]=5000;
    vehicleTypes[TRUCK][RATE_PER_KM]=40;
    vehicleTypes[TRUCK][FUEL_EFFICIENCY]=6;
    vehicleTypes[TRUCK][AVG_SPEED]=50;


    vehicleTypes[LORRY][CAPACITY]=10000;
    vehicleTypes[LORRY][RATE_PER_KM]=80;
    vehicleTypes[LORRY][FUEL_EFFICIENCY]=4;
    vehicleTypes[LORRY][AVG_SPEED]=45;


}

int inputDeliveryOrder(int  orders[MAX_ORDERS][4],int vehicleTypes[3][4],int currentOrderCount)
{

    int tempOrders[MAX_ORDERS][4];
    int tempIndex=0;
    int vehicle=0;
    char ch='N';

    printf("\n===============================================================\n");
    printf("\t\t PLACE ORDER\n");
    printf("===============================================================\n");
    do
    {
        printf("\nEnter Source City Index:");
        scanf(" %d",&tempOrders[tempIndex][0]);

        printf("Enter  Destination City Index:");
        scanf(" %d",&tempOrders[tempIndex][1]);

        if(tempOrders[tempIndex][0]==tempOrders[tempIndex][1])
        {
            printf("Source and destination cannot be same enter again.\n");
        }
    }
    while(tempOrders[tempIndex][0]==tempOrders[tempIndex][1]);
    do
    {
        printf("Enter weight(in kg):");
        scanf(" %d",&tempOrders[tempIndex][2]);

        printf("Enter  vehicle  type(VAN=1,Truck=2,Lorry=3):");
        scanf(" %d",&vehicle);


        if(tempOrders[tempIndex][2]>vehicleTypes[vehicle-1][CAPACITY])
        {
            printf("This capacity cant carry from this vehicle type..choose other one.\n");

        }
    }
    while(tempOrders[tempIndex][2]>vehicleTypes[vehicle-1][CAPACITY]);

    orders[currentOrderCount][0]=tempOrders[tempIndex][0];
    orders[currentOrderCount][1]=tempOrders[tempIndex][1];
    orders[currentOrderCount][2]=tempOrders[tempIndex][2];
    orders[currentOrderCount][3]=vehicle;

    printf("Your order placed successfully.\n");
    printf("Order details\n");
    printf("Order index(rembember this): %d",currentOrderCount);
    printf("\nSource city index %d ..\n",orders[currentOrderCount][0]);
    printf("Destination city index %d ..\n",orders[currentOrderCount][1]);
    printf("Weight %d ..\n",orders[currentOrderCount][2]);
    printf("Vehicle Type %d ..\n\n",orders[currentOrderCount][3]);
    currentOrderCount++;
    tempIndex++;
    return currentOrderCount;
}


double calcDeliveryCost(int D,int R,int W)
{

    double cost=D*R*(1+W*(1/10000.0));
    return cost;
}

double calcEstimatedDiliveryTime(int D,int S)
{
    double time=(double)D/S;
    return time;
}

double  calcFuelConsumption(int D,int E)
{
    double fuelUsed=D/E;
    return fuelUsed;
}

double fuelCost(double fuelUsed,int F )
{
    double cost=fuelUsed*F;
    return cost;
}

double totOpretaionalCost(double deliveryCost,double fuelCost)
{

    double totCost=deliveryCost+fuelCost;
    return totCost;
}

double calcProfit(double cost)
{
    double profit =cost*0.25;
    return profit;
}

double calcCharge(double totCost,double profit)
{
    double customerCharge=totCost+profit;
    return customerCharge;
}


void findLeastCostRouteRecursive(int distances[MAX_CITIES][MAX_CITIES],
                                 int currentCity, int destCity,
                                 int totalCities, int currentDistance, int depth)
{
    tempPath[depth] = currentCity;

    if (currentCity == destCity)
    {
        if (currentDistance < minDistance)
        {
            minDistance = currentDistance;
            for (int i = 0; i <= depth; i++)
                bestPath[i] = tempPath[i];
            bestPath[depth + 1] = -1;
        }
        return;
    }


    for (int i = 0; i < totalCities; i++)
    {
        if (!visited[i] && distances[currentCity][i] > 0)
        {
            visited[i] = 1;
            findLeastCostRouteRecursive(distances, i, destCity, totalCities, currentDistance + distances[currentCity][i], depth + 1);
            visited[i] = 0;
        }
    }
}


int findLeastCostRoute(int distances[MAX_CITIES][MAX_CITIES], int totalCities, int source, int dest)
{
    minDistance = INT_MAX;
    for (int i = 0; i < totalCities; i++)
        visited[i] = 0;

    visited[source] = 1;
    findLeastCostRouteRecursive(distances, source, dest, totalCities, 0, 0);
    return minDistance;
}

void handleDeliveryOutput(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100],int currentCityCount,
                          int  orders[MAX_ORDERS][4],int vehicleTypes[3][4],int currentOrderCount
                          ,int *totalDeliveriesCompleted,int *totalDistanceCovered,double *totalDeliveryTimeHours,
                          double *totalRevenue,double *totalProfit,int *longestRoute,int *shortestRoute
                          ,int orderIndex,double deliveryData[MAX_ORDERS][7],int routesData[]
                         )
{

    int from=orders[orderIndex][0];
    int to=orders[orderIndex][1];
    int D=findLeastCostRoute(distances,currentCityCount,from,to);
    int vehicle=orders[orderIndex][3]-1;
    int weight=orders[orderIndex][2];
    int R=vehicleTypes[vehicle][RATE_PER_KM];
    int E=vehicleTypes[vehicle][FUEL_EFFICIENCY];
    int S=vehicleTypes[vehicle][AVG_SPEED];
    double baseCost=calcDeliveryCost(D,R,weight);
    double fuelUsed=calcFuelConsumption(D,E);
    double fuelC=fuelCost(fuelUsed,310);
    double totOp=totOpretaionalCost(baseCost,fuelC);
    double profit=calcProfit(totOp);
    double custCharge=calcCharge(totOp,profit);
    double estTime=calcEstimatedDiliveryTime(D,S);
    printf("\n=============================================================\n");
    printf("DELIVERY COST ESTIMATION\n");
    printf("-------------------------------------------------------------\n");
    printf("From: %s\n", cities[from]);
    printf("To: %s\n", cities[to]);
    printf("Minimum Distance: %d km\n", D);
    printf("Vehicle: ");
    if(vehicle == 1)
    {
        printf("Van\n");
    }
    else if(vehicle == 2)
    {
        printf("Truck\n");
    }
    else
    {
        printf("Lorry\n");
    }
    printf("Weight: %d kg\n", weight);
    printf("-------------------------------------------------------------\n");
    printf("Base Cost: %d x %d x (1 + %d/10000) = %.2f LKR\n", D, R, weight, baseCost);
    printf("Fuel Used: %.2f L\n", fuelUsed);
    printf("Fuel Cost: %.2f LKR\n", fuelC);
    printf("Operational Cost: %.2f LKR\n", totOp);
    printf("Profit: %.2f LKR\n", profit);
    printf("Customer Charge: %.2f LKR\n", custCharge);
    printf("Estimated Time: %.2f hours\n", estTime);
    printf("=============================================================\n");


    storeDeliveries(deliveryData,baseCost,fuelUsed,fuelC,totOp,profit,custCharge,estTime,orderIndex);
    *totalDeliveriesCompleted+=1;
    *totalDistanceCovered+=D;
    *totalDeliveryTimeHours+=estTime;
    *totalRevenue+=custCharge;
    *totalProfit+=profit;
    if (*totalDeliveriesCompleted == 1)
    {
        *longestRoute = D;
        *shortestRoute = D;
    }
    else
    {
        if (D > *longestRoute)
            *longestRoute = D;
        if (D < *shortestRoute)
            *shortestRoute = D;
    }
    routesData[0]=*longestRoute;
    routesData[1]=*shortestRoute;

}
double storeDeliveries(double deliveryData[MAX_ORDERS][7],double baseCost,
                       double fuelUsed,double fuelC,double totOp,
                       double profit,double custCharge,double estTime,int orderIndex
                      )
{

    deliveryData[orderIndex][0]=baseCost;
    deliveryData[orderIndex][1]=fuelUsed;
    deliveryData[orderIndex][2]=fuelC;
    deliveryData[orderIndex][3]=totOp;
    deliveryData[orderIndex][4]=profit;
    deliveryData[orderIndex][5]=custCharge;
    deliveryData[orderIndex][6]=estTime;
}
void calculateTotalsFromFile(double deliveryData[MAX_ORDERS][7], int orderCount,
                             int *totalDeliveriesCompleted,
                             double *totalRevenue,
                             double *totalProfit,
                             double *totalDeliveryTimeHours)
{
    *totalDeliveriesCompleted = orderCount;
    *totalRevenue = 0;
    *totalProfit = 0;
    *totalDeliveryTimeHours = 0;

    for (int i = 0; i < orderCount; i++)
    {
        *totalRevenue += deliveryData[i][5];
        *totalProfit += deliveryData[i][4];
        *totalDeliveryTimeHours += deliveryData[i][6];
    }
}


void printReports(int orders[MAX_ORDERS][4],
                  double deliveryData[MAX_ORDERS][7],int routesData[],
                  int orderCount)
{
    int totalDeliveriesCompleted = 0;
    double totalRevenue = 0, totalProfit = 0, totalDeliveryTimeHours = 0;


    calculateTotalsFromFile(deliveryData, orderCount,
                            &totalDeliveriesCompleted,
                            &totalRevenue, &totalProfit,
                            &totalDeliveryTimeHours);
    printf("\n\n===============================================================\n");
    printf("\t\tDELIVERY REPORTS \n");
    printf("===============================================================\n");
    printf("\nTotal Deliveries Completed : %d\n", totalDeliveriesCompleted);



    if (totalDeliveriesCompleted > 0)
    {
        double avgTime = totalDeliveryTimeHours / totalDeliveriesCompleted;
        printf("Average Delivery Time      : %.2f hours\n", avgTime);
    }
    else
    {
        printf("Average Delivery Time      : N/A (no deliveries)\n");
    }
    printf("Min Distance               : %d \n",routesData[1]);
    printf("Long Distance              : %d \n",routesData[0]);
    printf("Total Revenue              : %.2f LKR\n", totalRevenue);
    printf("Total Profit               : %.2f LKR\n", totalProfit);
    printf("\n===============================================================\n\n");

}

void saveRoutesToFile(char cities[MAX_CITIES][100], int distances[MAX_CITIES][MAX_CITIES], int cityCount)
{
    FILE *fp = fopen("routes.txt", "w");
    if (fp == NULL)
    {
        printf("Cannot open routes.txt.\n");
        return;
    }

    fprintf(fp, "%d\n", cityCount);
    for (int i = 0; i < cityCount; i++)
    {
        fprintf(fp, "%s\n", cities[i]);
    }

    for (int i = 0; i < cityCount; i++)
    {
        for (int j = 0; j < cityCount; j++)
        {
            fprintf(fp, "%d ", distances[i][j]);
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}

int loadRoutesFromFile(char cities[MAX_CITIES][100], int distances[MAX_CITIES][MAX_CITIES])
{
    FILE *fp = fopen("routes.txt", "r");
    if (fp == NULL)
    {
        printf("No saved route file found.\n");
        return 0;
    }

    int cityCount;
    fscanf(fp, "%d\n", &cityCount);

    for (int i = 0; i < cityCount; i++)
    {
        fgets(cities[i], 100, fp);
        cities[i][strcspn(cities[i], "\n")] = '\0';
    }

    for (int i = 0; i < cityCount; i++)
    {
        for (int j = 0; j < cityCount; j++)
        {
            fscanf(fp, "%d", &distances[i][j]);
        }
    }

    fclose(fp);
    return cityCount;
}
void saveDeliveriesToFile(int orders[MAX_ORDERS][4],double deliveryData[MAX_ORDERS][7],int routesData[],int orderCount)
{
    FILE *fp = fopen("deliveries.txt", "w");
    if (fp == NULL)
    {
        printf("Cannot open deliveries.txt .\n");
        return;
    }

    fprintf(fp, "%d\n", orderCount);
    for (int i = 0; i < orderCount; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fprintf(fp, "%d ", orders[i][j]);
        }
        for (int j = 0; j < 7; j++)
        {
            fprintf(fp, "%.2lf ", deliveryData[i][j]);
        }

        fprintf(fp, "\n");
    }

    fprintf(fp, "\n%d %d\n", routesData[0], routesData[1]);
    fclose(fp);
}
int loadDeliveriesFromFile(int orders[MAX_ORDERS][4],double deliveryData[MAX_ORDERS][7],int routesData[])
{
    FILE *fp = fopen("deliveries.txt", "r");
    if (fp == NULL)
    {
        printf("No saved deliveries found.\n");
        return 0;
    }

    int orderCount;
    fscanf(fp, "%d\n", &orderCount);

    for (int i = 0; i < orderCount; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fscanf(fp, "%d", &orders[i][j]);
        }
        for (int j = 0; j < 7; j++)
        {
            fscanf(fp, "%lf", &deliveryData[i][j]);
        }
    }
    fscanf(fp,"%d",&routesData[0]);
    fscanf(fp,"%d",&routesData[1]);
    fclose(fp);
    return orderCount;
}
