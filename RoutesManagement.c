#include <stdio.h> //for input output functions like printf, scanf
#include <stdlib.h>
#include <conio.h>
#include <windows.h> //for windows related functions (not important)
#include <string.h>  //string operations

/* List of Global Variable */
COORD coord = {0,0}; /* top left corner*/

const char *seasonStrings[4]= {"Spring", "Summer", "Autumn", "Winter"};

/*
    function : SetCursorPosition
    @param input: x and y coordinates
    @param output: moves the cursor in specified position of console
*/
void SetCursorPosition(int x,int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

typedef enum Season
{
    Spring=1,
    Summer=2,
    Autumn=3,
    Winter=4
} ;
typedef struct Route
{
    int code; // should be unique
    char country[50 + 1]; // should be all upper case
    enum Season season;
    int durationDays; // should be > 0
    double price; // should be > 0
} Route;

typedef struct node
{
    Route rt;
    struct node *next;
}
node;

node * push(node * head, Route r)
{
    node *hlp;
    if(!(hlp= (node *)malloc(sizeof(node))))
    {
        /*...*/
    }
    hlp->rt=r;
    hlp->next=head;
    head=hlp;
    return head; // резултат – ново начало на списъка
}

void printLinkedList(node *crnt)
{
    int count = 1;
    while(crnt)
    {
        printf("\n %i. Code: %i, To:%s, during %s for %i days. Cost: $%.2f", count, crnt->rt.code, crnt->rt.country, seasonStrings[crnt->rt.season-1], crnt->rt.durationDays, crnt->rt.price); // print the route details
        count++;
        crnt=crnt->next;
    }
}

void printRoutesBySeason(node *crnt, int season)
{
    int count = 1;
    while(crnt)
    {
        if (crnt->rt.season == season)
        {
            printf("\n %i. Code: %i, To:%s, during %s for %i days. Cost: $%.2f", count, crnt->rt.code, crnt->rt.country, seasonStrings[crnt->rt.season-1], crnt->rt.durationDays, crnt->rt.price); // print the route details
            count++;
        }

        crnt=crnt->next;
    }

    if(count == 1)
    {
        printf("There are no routes during that season!");
    }
}

void srtNm(node **ph)
{
    int ok;
    node **p;
    node *hlp;
    do
    {
        ok=1;
        for(p=ph; *(p+1); p++)
        {
            if(((*p)->rt.price) < ((*(p+1))->rt.price))
            {
                hlp=*p;
                *p=*(p+1);
                *(p+1)=hlp;
                ok=0;
            }
        }
    }
    while(!ok);
}

void PrintRoutesFromGivenCountry(node *head, char countryName[])
{
    node **ph= NULL,**p; // dynamic array of pointers to node
    node *crnt;
    int number=0;
    for(crnt=head; crnt; crnt=crnt->next)number++; //how many nodes
    number++; // for the end NULL
    ph=(node **)malloc(number*sizeof(node*));
    if(!ph)
    {
        /*…*/
    }
    for(p=ph,crnt=head; crnt; crnt=crnt->next) *p++=crnt;
    *p=NULL;
    srtNm(ph);
    int count = 1;
    for(p=ph; *p; p++)
    {
        if(strcmp(countryName,(*p)->rt.country) == 0)
        {
            printf("\n %i. Code: %i, To:%s, during %s for %i days. Cost: $%.2f",
                   count, (*p)->rt.code, (*p)->rt.country, seasonStrings[(*p)->rt.season-1], (*p)->rt.durationDays, (*p)->rt.price); // print the route details
            count++;
        }
    }

    if (count==1)
    {
        printf("There are no routes to that country!");
    }

    free(ph);
}

void writeListToFile(node *crnt)
{
    FILE *fileToWrite;
    rewind(fileToWrite);
    if((fileToWrite=fopen("Routes_temp.dat","wb"))!=NULL)
    {
        while(crnt)
        {
            fwrite(&crnt->rt,sizeof(Route),1,fileToWrite);
            crnt=crnt->next;
        }
    }

    fclose(fileToWrite); // close the file
    if(remove("Routes.dat") == 0)
        /*printf("File %s  deleted.\n", "Routes.dat")*/;
    else
        fprintf(stderr, "Error deleting the file %s.\n", "Routes.dat");

    if(rename("Routes_temp.dat","Routes.dat") == 0)
    {
        //printf("\nSaved successfully! ");
        //printf("%s has been rename %s.\n", "Routes_temp.dat", "Routes.dat");
    }
    else
    {
        fprintf(stderr, "Error renaming %s.\n", "Routes_temp.dat");
    }
}

node * deleteRouteByCode(node *head, int index,Route *r)
{
    node * prev=NULL,*crnt=head;
    for(; crnt&&(crnt->rt.code!=index); prev=crnt,crnt=crnt->next);
    if(crnt)
    {
        *r=crnt->rt;
        if(crnt == head)
        {
            head=crnt->next;
        }
        else
        {
            prev->next=crnt->next;
        }
        free(crnt);
    }
    else
    {
        printf("no such index in the list");
    }
    return head;
}

node * modifyRouteByCode(node *head, int index,Route *r)
{
    node *crnt=head;
    for(; crnt&&(crnt->rt.code!=index); crnt=crnt->next);
    *r=crnt->rt;
    if(crnt)
    {
        printf("\nOld country: %s", crnt->rt.country);
        printf("\nEnter new country: ");
        while(1)
        {
            scanf("%s", &crnt->rt.country);
            if(strlen(&crnt->rt.country) >0 && strlen(&crnt->rt.country)<50 )
            {
                break;
            }
            else
            {
                printf("\nInvalid string length!");
                fflush(stdin);
                printf("\nEnter new country: ");
            }
        }
        strupr(crnt->rt.country);// make it uppercase



        printf("\nOld season: %s",seasonStrings[crnt->rt.season-1]);
        printf("\nEnter new season as integer (Spring=1, Summer=2, Autumn=3, Winter=4): ");
        fflush(stdin);
        while(1)
        {
            if(scanf("%i", &crnt->rt.season)==1 && crnt->rt.season > 0 && crnt->rt.season < 5)
            {
                break;
            }
            else
            {
                printf("\nInvalid season integer!");
                fflush(stdin);
                printf("\nEnter new season as an integer (Spring=1, Summer=2, Autumn=3, Winter=4): ");
            }
        }

        printf("\nOld duration of the vacation: %u",crnt->rt.durationDays);
        printf("\nEnter new duration of the vacation: ");
        fflush(stdin);
        while(1)
        {
            if(scanf("%i", &crnt->rt.durationDays)==1 && crnt->rt.durationDays > 0 )
            {
                break;
            }
            else
            {
                printf("\nInvalid duration!");
                fflush(stdin);
                printf("\nEnter new duration of the vacation: ");
            }
        }

        printf("\nOld price: %.2f",crnt->rt.price);
        printf("\nEnter new price: ");
        fflush(stdin);
        while(1)
        {
            if(scanf("%lf",&crnt->rt.price)==1 && &crnt->rt.price > 0 )
            {
                break;
            }
            else
            {
                printf("\nInvalid price!");
                fflush(stdin);
                printf("\nEnter new price: ");
            }
        }
    }
    else
    {
        printf("There is no such route with this code in the list!");
    }

    return head;
}

int checkIfCodeIsUnique(node *head, int code)
{
    node * prev=NULL,*crnt=head;
    for(; crnt; prev=crnt,crnt=crnt->next)
    {
        if(crnt->rt.code==code)
        {
            return 0;
        }
    }

    return 1;
}

Route createNewRoute(node *head)
{
    Route newRoute;

    printf("\nEnter code: ");
    fflush(stdin);
    int code;
    while(1)
    {
        if(scanf("%i",&newRoute.code)==1 && newRoute.code > 0 && checkIfCodeIsUnique(head,newRoute.code)==1)
        {
            break;
        }
        else
        {
            printf("\nInvalid code or already existing!");
            fflush(stdin);
            printf("\nEnter new code: ");
        }
    }

    printf("\nEnter country: ");
    fflush(stdin);
    while(1)
    {
        scanf("%s", &newRoute.country);
        if(strlen(&newRoute.country) >0 && strlen(&newRoute.country)<50 )
        {
            break;
        }
        else
        {
            printf("\nInvalid string length!");
            fflush(stdin);
            printf("\nEnter new country: ");
        }
    }
    strupr(newRoute.country);// make it uppercase

    printf("\nEnter season as an integer (Spring=1, Summer=2, Autumn=3, Winter=4): ");
    fflush(stdin);
    while(1)
    {
        if(scanf("%i", &newRoute.season)==1 && newRoute.season > 0 && newRoute.season < 5)
        {
            break;
        }
        else
        {
            printf("\nInvalid season integer!");
            fflush(stdin);
            printf("\nEnter new season as an integer (Spring=1, Summer=2, Autumn=3, Winter=4): ");
        }
    }

    printf("\nEnter duration of the vacation: ");
    fflush(stdin);
    while(1)
    {
        if(scanf("%i", &newRoute.durationDays)==1 && newRoute.durationDays > 0 )
        {
            break;
        }
        else
        {
            printf("\nInvalid duration!");
            fflush(stdin);
            printf("\nEnter new duration of the vacation: ");
        }
    }

    printf("\nEnter price: ");
    fflush(stdin);
    while(1)
    {
        if(scanf("%lf",&newRoute.price)==1 && newRoute.price > 0 )
        {
            break;
        }
        else
        {
            printf("\nInvalid price!");
            fflush(stdin);
            printf("\nEnter new price: ");
        }
    }

    return newRoute;
}

void printMenu()
{
    system("cls"); //clear the console window
    SetCursorPosition(10,2); // move the cursor to postion 30, 10 from top-left corner
    printf("1. Add new Route");
    SetCursorPosition(10,4);
    printf("2. List all Routes");
    SetCursorPosition(10,6);
    printf("3. Save routes to file");
    SetCursorPosition(10,8);
    printf("4. List all Routes by country");
    SetCursorPosition(10,10);
    printf("5. List all Routes from given season");
    SetCursorPosition(10,12);
    printf("6. Delete route by code"); // option for deleting record
    SetCursorPosition(10,14);
    printf("7. Modify route by code"); // exit from the program
    SetCursorPosition(10,16);
    printf("8. Exit"); // exit from the program
    SetCursorPosition(10,18);
    printf("Your Choice: "); // enter the choice 1, 2, 3, 4, 5
}

node * loadRoutesFromFile()
{
    FILE *f;
    Route r;
    node *head=NULL;
    if (!(f=fopen("Routes.dat","rb")))
    {
        f = fopen("Routes.dat","wb+");
        if(f == NULL)
        {
            printf("Cannot open file");
            exit(1);
        }
    }

    rewind(f);
    do
    {
        if(!fread(&r,sizeof(r),1,f)) break;
        head=push(head,r);
    }
    while(1);
    fclose(f); // close the file

    return head;
}

int main()
{
    node *head=NULL;
    int index;
    Route r;
    head=loadRoutesFromFile();

    /* infinite loop continues until the break statement encounter*/
    while(1)
    {
        printMenu();
        fflush(stdin); // flush the input buffer
        int choice  = getche(); // get the input from keyboard
        switch(choice)
        {
        case '1':  // if user pressed 1 and wants to add route
            system("cls");

            Route newRoute = createNewRoute(head);
            printf("\nNew route added successfully! ");
            head=push(head,newRoute);
            break;


        case '2': // list all routes in given season
            system("cls");

            printLinkedList(head);

            getche();

            break;
        case '3': // write list to file
            system("cls");

            writeListToFile(head);
            printf("\nSaved successfully! ");
            getche();

            break;

        case '4': // show routes by country
            system("cls");

            char country[50+1];
            printf("\nEnter the country to filter by (case insensitive): ");
            while(1)
            {
                scanf("%s", &country);
                if(strlen(&country) >0 && strlen(&country)<50 )
                {
                    break;
                }
                else
                {
                    printf("\nInvalid string length!");
                    fflush(stdin);
                    printf("\nEnter new country: ");
                }
            }
            strupr(country);// make it uppercase

            PrintRoutesFromGivenCountry(head, country);

            getche();
            break;
        case '5': // show routes by season
            system("cls");

            int season;
            printf("\nEnter season as integer (Spring=1, Summer=2, Autumn=3, Winter=4): ");

            while(1)
            {
                if(scanf("%i", &season)==1 && season > 0 && season < 5)
                {
                    break;
                }
                else
                {
                    printf("\nInvalid season integer!");
                    fflush(stdin);
                    printf("\nEnter new season as an integer (Spring=1, Summer=2, Autumn=3, Winter=4): ");
                }
            }

            printRoutesBySeason(head,season);
            getche();
            break;

        case '6': //  delete by code
            system("cls");

            printf("\n Give the code of the route to delete: ");
            scanf("%i",&index);
            head = deleteRouteByCode(head,index,&r);

            printf("\n Successfully deleted this route:");
            printf("\n Code: %i, To:%s, during %s for %i days. Cost: $%.2f", r.code, r.country, seasonStrings[r.season-1], r.durationDays, r.price); // print the route details

            getche();
            break;

        case '7': //  modify by code
            system("cls");

            printf("\n Give the code of the route to modify: ");
            scanf("%i",&index);
            head = modifyRouteByCode(head,index,&r);

            printf("\n Successfully modified this route:");
            printf("\n Code: %i, To:%s, during %s for %i days. Cost: $%.2f", r.code, r.country, seasonStrings[r.season-1], r.durationDays, r.price); // print the route details

            getche();
            break;

        case '8':
            system("cls");

            writeListToFile(head);
            printf("\nGood bye! ");
            getche();


            exit(0);
            break; // exit from the program
        }
    }

    return 0;
}
