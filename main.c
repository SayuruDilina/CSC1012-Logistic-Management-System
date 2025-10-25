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
                          ,int orderIndex,double deliveryData[MAX_ORDERS][7]);
void printReports(int totalDeliveriesCompleted,int totalDistanceCovered,double totalDeliveryTimeHours,
                  double totalRevenue,double totalProfit,int longestRoute,int shortestRoute);
void saveRoutesToFile(char cities[MAX_CITIES][100], int distances[MAX_CITIES][MAX_CITIES], int cityCount);
int loadRoutesFromFile(char cities[MAX_CITIES][100], int distances[MAX_CITIES][MAX_CITIES]);
int loadDeliveriesFromFile(int orders[MAX_ORDERS][4]);
void saveDeliveriesToFile(int orders[MAX_ORDERS][4], int orderCount);
int handleCityManagement(char cities[MAX_CITIES][100],int *currentCityCount);
int handleDistanceManagement(int distances[MAX_CITIES][MAX_CITIES],char cities[MAX_CITIES][100],int currentCityCount);
double storeDeliveries(double deliveryData[MAX_ORDERS][7],double baseCost,
                       double fuelUsed,double fuelC,double totOp,
                       double profit,double custCharge,double estTime,int orderIndex
                      );
int main()
{
    int choice=0;
    int currentCityCount=0;
    char cities[MAX_CITIES][100];
    int distances[MAX_CITIES][MAX_CITIES]= {0};
    int vehicleTypes[3][4];
    int  orders[MAX_ORDERS][4];
    int currentOrderCount=0;
    int totalDeliveriesCompleted = 0;
    int totalDistanceCovered = 0;
    double totalDeliveryTimeHours = 0.0;
    double totalRevenue = 0.0;
    double totalProfit= 0.0;
    int longestRoute=0;
    int shortestRoute=0;
    double deliveryData[MAX_ORDERS][7];

    currentCityCount = loadRoutesFromFile(cities, distances);
    currentOrderCount = loadDeliveriesFromFile(orders);
    printf("Loaded %d cities and %d delivery records from files.\n",
           currentCityCount, currentOrderCount);
    do
    {
        printf("Logistic Management System \n");
        printf("1.City Management \n");
        printf("2.Display all cities\n");
        printf("3.Distance Management \n");
        printf("4.Place order \n");
        printf("5.Print reports \n");
        printf("6.Exit and Save Data\n");
        printf("Enter your choice:");
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
            handleDistanceManagement(distances,cities,currentCityCount);
            break;
        case 4:
            storeVehicleDetails(vehicleTypes);
            currentOrderCount=inputDeliveryOrder(orders,vehicleTypes,currentOrderCount);
            int orderIndex=currentOrderCount-1;
            handleDeliveryOutput(distances,cities,currentCityCount,orders,vehicleTypes,currentOrderCount,
                                 &totalDeliveriesCompleted,&totalDistanceCovered,&totalDeliveryTimeHours,&totalRevenue,&totalProfit
                                 ,&longestRoute,&shortestRoute,orderIndex,deliveryData);
            break;

        case 5:
            printReports(totalDeliveriesCompleted,totalDistanceCovered,totalDeliveriesCompleted,totalRevenue,totalProfit,longestRoute,shortestRoute);
            break;
        case 6:
            saveRoutesToFile(cities, distances, currentCityCount);
            saveDeliveriesToFile(orders, currentOrderCount);
            printf("\nData saved successfully. Exiting program...\n");
            return 0;
            break;
        default:
            printf("Invalid");
        }

    }
    while(choice!=-1);

    saveRoutesToFile(cities, distances, currentCityCount);
    saveDeliveriesToFile(orders, currentOrderCount);
    printf("\nData saved successfully to routes.txt and deliveries.txt\n");

    return 0;
}
int handleCityManagement(char cities[MAX_CITIES][100],int *currentCityCount)
{
    int choice=0;
    do
    {
        printf("\n=== City Management ===\n");
        printf("1. Add City\n");
        printf("2. Update City\n");
        printf("3. Remove City\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
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
        printf("\n=== Distance Management ===\n");
        printf("1.Store Distances\n");
        printf("2.Display Distances\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
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

    do
    {
        printf("Enter Source City Index:");
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
                          ,int orderIndex,double deliveryData[MAX_ORDERS][7]
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

void printReports(int totalDeliveriesCompleted,int totalDistanceCovered,double totalDeliveryTimeHours,
                  double totalRevenue,double totalProfit,int longestRoute,int shortestRoute )
{
    printf("\n=================== DELIVERY REPORTS ===================\n");
    printf("Total Deliveries Completed : %d\n", totalDeliveriesCompleted);
    printf("Total Distance Covered     : %d km\n", totalDistanceCovered);
    if (totalDeliveriesCompleted > 0)
    {
        double avgTime = totalDeliveryTimeHours / totalDeliveriesCompleted;
        printf("Average Delivery Time      : %.2f hours\n", avgTime);
    }
    else
    {
        printf("Average Delivery Time      : N/A (no completed deliveries)\n");
    }
    printf("Total Revenue              : %.2f LKR\n", totalRevenue);
    printf("Total Profit               : %.2f LKR\n", totalProfit);
    printf("Longest Route Distance      : %d km\n", longestRoute);
    printf("Shortest Route Distance     : %d km\n", shortestRoute);
    printf("=========================================================\n");
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
void saveDeliveriesToFile(int orders[MAX_ORDERS][4], int orderCount)
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
        fprintf(fp, "\n");
    }

    fclose(fp);
}
int loadDeliveriesFromFile(int orders[MAX_ORDERS][4])
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
    }

    fclose(fp);
    return orderCount;
}
