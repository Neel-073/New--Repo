#include <stdio.h>
#include <string.h>
#include "input.h"

#define MAX_TRAIN 100
#define MAX_PASSENGER_IN_ONE_TRAINE 500
#define NAME_LEN 50

// Ticket status enum
enum TicketStatus
{
    CONFIRMED,
    WAITING,
    CANCELED
};

// structure of railway trains
struct Railway
{
    int Train_number;
    char Train_name[NAME_LEN];
    char Source_station[NAME_LEN];
    char Destination_station[NAME_LEN];
    int Total_seat;
    int Available_seat;
};

// structure of passenger
struct User
{
    char Passenger_name[NAME_LEN];
    int Passenger_age;
    char Passenger_gender;
    int Seat_no;
    int Traine_no;
    enum TicketStatus Ticket_status;
};

struct Railway trains[MAX_TRAIN];
int Train_count = 0;

struct User passenger[MAX_PASSENGER_IN_ONE_TRAINE * MAX_TRAIN];
int Passenger_count = 0;

// Function prototypes of user
void Book_ticket();
void Cancel_ticket();
void Display_passenger();

// Function prototypes of manager
void Add_train();
void Cancel_train();
void Display_train();
void Display_all_train();

// file read write function
void writetofile_railway(struct Railway *R);
void writetofile_User(struct User *T);
int loadfile_railway(struct Railway *R);
int loadfile_User(struct User *T);

//some change 
int main()
{
    Train_count = loadfile_railway(trains);
    Passenger_count = loadfile_User(passenger);
    int choice;

    do
    {
    menu:
        // menu to select between user and railway manager
        printf("\n--_Menu of choice ---\n");
        printf("1. Login as User\n");
        printf("2. Login as Railway manager\n");
        printf("0. Exit\n");
        printf("---------------------------\n");
        choice = readInt("Enter choice: ", 0);

        switch (choice)
        {
        case 1:
        {
            int user_choice;
            do
            {
                // menu to select  passenger operation
                printf("\n--_Menu of choice for railway user ---\n");
                printf("1. Book a ticket\n");
                printf("2. Cancle a ticket\n");
                printf("3. Display Passenger Details:\n");
                printf("4. Desplay available train\n");
                printf("0. Exit\n");
                printf("---------------------------\n");
                user_choice = readInt("Enter choise of user:", 0);

                switch (user_choice)
                {
                case 1:
                {
                    printf("You are booking ticket \n");
                    printf("---------------------\n");
                    Book_ticket();
                    break;
                }
                case 2:
                {
                    printf("You are cancaling ticket \n");
                    printf("---------------------\n");
                    Cancel_ticket();
                    break;
                }
                case 3:
                {
                    Display_passenger();
                    break;
                }
                case 4:
                {
                    Display_all_train();
                    break;
                }
                case 0:
                {
                    printf("Exiting user menu.\n");
                    writetofile_User(passenger);
                    goto menu;
                    break;
                }
                default:
                    printf("Invalid choice! Try again.\n");
                }

            } while (user_choice != 0);

            break;
        }
        case 2:
        {

            int manager_choice;
            do
            {
                // menu to select railway manager operation
                printf("\n--_Menu of choice for railway manager ---\n");
                printf("1. Add a train\n");
                printf("2. Cancle a train\n");
                printf("3. Display Details of a train:\n");
                printf("4. Desplay all train\n");
                printf("0. Exit\n");
                printf("---------------------------\n");
                manager_choice = readInt("Enter choise of manager:", 0);
                switch (manager_choice)
                {
                case 1:
                {
                    printf("You are adding train \n");
                    printf("---------------------\n");
                    Add_train();
                    break;
                }
                case 2:
                {
                    printf("You are cancaling train \n");
                    printf("---------------------\n");
                    Cancel_train();
                    break;
                }
                case 3:
                {
                    Display_train();
                    break;
                }
                case 4:
                {
                    Display_all_train();
                    break;
                }
                case 0:
                {
                    printf("Exiting manager menu.\n");

                    writetofile_railway(trains);
                    goto menu;
                    break;
                }
                default:
                    printf("Invalid choice! Try again.\n");
                }

            } while (manager_choice != 0);

            break;
        }
        case 0:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice! Try again.\n");
            break;
        }
    } while (choice != 0);

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------
void Book_ticket()
{
    int train_no = readInt("Enter train no that you want to book(>0): ", 1);

    for (int i = 0; i < Train_count; i++)
    {
        if (train_no == trains[i].Train_number)
        {
            // NAME
            while (!readString("Enter passenger name (max 49 chars): ",
                               passenger[Passenger_count].Passenger_name,
                               sizeof(passenger[Passenger_count].Passenger_name)))
                ;

            // AGE
            passenger[Passenger_count].Passenger_age =
                readInt("Enter age (>0): ", 1);

            // GENDER
            while (1)
            {
                char temp;
                printf("Enter gender (M/F/O): ");
                temp = getchar();
                while (getchar() != '\n')
                    ;

                if (temp == 'M' || temp == 'm' || temp == 'F' || temp == 'f' || temp == 'O' || temp == 'o')
                {
                    passenger[Passenger_count].Passenger_gender = temp;
                    break;
                }
                printf("Invalid gender\n");
            }

            passenger[Passenger_count].Traine_no = trains[i].Train_number;

            // AUTOMATIC SEAT
            if (trains[i].Available_seat > 0)
            {
                passenger[Passenger_count].Seat_no =
                    trains[i].Total_seat - trains[i].Available_seat + 1;

                passenger[Passenger_count].Ticket_status = CONFIRMED;
                trains[i].Available_seat--;

                printf("Ticket CONFIRMED. Seat No: %d\n",
                       passenger[Passenger_count].Seat_no);
            }
            else
            {
                passenger[Passenger_count].Seat_no = -1;
                passenger[Passenger_count].Ticket_status = WAITING;

                printf("No seats available. Ticket in WAITING list.\n");
            }

            Passenger_count++;

            writetofile_User(passenger);

            return;
        }
    }

    printf("Invalid train number\n");
}

//-----------------------------------------------------------------------------------------------------------------------------------

void Cancel_ticket()
{
    char temp_name[NAME_LEN];

    while (!readString("Enter passenger name to cancel ticket: ",
                       temp_name, sizeof(temp_name)))
        ;

    for (int i = 0; i < Passenger_count; i++)
    {
        if (strcmp(temp_name, passenger[i].Passenger_name) == 0)
        {
            if (passenger[i].Ticket_status == CANCELED)
            {
                printf("Ticket already canceled\n");
                return;
            }

            int train_index = -1;
            // Find train index
            for (int j = 0; j < Train_count; j++)
            {
                if (passenger[i].Traine_no == trains[j].Train_number)
                {
                    train_index = j;
                    break;
                }
            }

            if (passenger[i].Ticket_status == CONFIRMED && train_index != -1)
            {
                trains[train_index].Available_seat++;
            }

            passenger[i].Ticket_status = CANCELED;
            printf("Ticket successfully canceled\n");

            // Waiting list promotion
            if (train_index != -1)
            {
                for (int k = 0; k < Passenger_count; k++)
                {
                    if (passenger[k].Traine_no == trains[train_index].Train_number &&
                        passenger[k].Ticket_status == WAITING)
                    {
                        passenger[k].Ticket_status = CONFIRMED;
                        passenger[k].Seat_no = trains[train_index].Total_seat - trains[train_index].Available_seat + 1;
                        trains[train_index].Available_seat--;
                        printf("Passenger %s promoted from WAITING to CONFIRMED. Seat No: %d\n",
                               passenger[k].Passenger_name, passenger[k].Seat_no);
                        break;
                    }
                }
            }
            return;
        }
    }

    printf("Passenger not found\n");
}

//-----------------------------------------------------------------------------------------------------------------------------------

void Display_passenger()
{
    char temp_name[50];

    // GET A NAME OF PASSENGER TO DISPLAY DETAILS
    while (!readString("Enter passenger name that want to see details(max 49 chars): ", temp_name, sizeof(temp_name)))
        ;

    printf("\n Passenger Details \n");
    printf("---------------------\n");
    for (int i = 0; i < Passenger_count; i++)
    {
        // CHEACK IF NAME IS VALID OR NOT
        if (strcmp(temp_name, passenger[i].Passenger_name) == 0)
        {

            printf("Name=%s \n", passenger[i].Passenger_name);
            printf("age=%d \n", passenger[i].Passenger_age);
            printf("Gender=%c \n", passenger[i].Passenger_gender);
            printf("Seat no=%d \n", passenger[i].Seat_no);
            printf("traine no=%d \n", passenger[i].Traine_no);
            if (passenger[i].Ticket_status == CONFIRMED)
                printf("Status = CONFIRMED\n");
            else if (passenger[i].Ticket_status == WAITING)
                printf("Status = WAITING\n");
            else
                printf("Status = CANCELED\n");

            return;
        }
    }
    printf("NO user is there of that name\n");
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Add_train()
{
    // CHEACK IF MAX LIMITE IS REACHED
    if (Train_count >= MAX_TRAIN)
    {
        printf("Train entry is full\n");
        printf("_______________________________\n");
        return;
    }

    trains[Train_count].Train_number = Train_count + 1;

    // TRAINE NAME
    while (!readString("Enter Train name (max 49 chars): ", trains[Train_count].Train_name, sizeof(trains[Train_count].Train_name)))
        ;

    // TRAIN SOURCE STATION
    while (!readString("Enter Train Source station (max 49 chars): ", trains[Train_count].Source_station, sizeof(trains[Train_count].Source_station)))
        ;

    // TRAIN DESTINATION STATION
    while (!readString("Enter Train Destination station (max 49 chars): ", trains[Train_count].Destination_station, sizeof(trains[Train_count].Destination_station)))
        ;

    // MAX SEAT IN TRAIN
    while (1)
    {
        int temp_count = readInt("Enter number of seat in train (>0 and <=500):", 0);
        if (temp_count <= MAX_PASSENGER_IN_ONE_TRAINE)
        {
            trains[Train_count].Total_seat = temp_count;
            break;
        }
        printf("Enter seat is greater than max limit of train\n");
    }

    // AVAILABLE SEAT IN TRAIN
    trains[Train_count].Available_seat = trains[Train_count].Total_seat;

    // TRAINE COUNT INCREASE
    Train_count++;

    writetofile_railway(trains);

    printf("Train added...\n");
    printf("_______________________________\n");
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Cancel_train()
{
    int train_no = readInt("Enter train no that you want to cancel(>0):", 0);

    int train_index = -1;
    for (int i = 0; i < Train_count; i++)
    {
        if (train_no == trains[i].Train_number)
        {
            train_index = i;
            break;
        }
    }

    if (train_index == -1)
    {
        printf("Train not found to cancel\n");
        printf("_______________________________\n");
        return;
    }

    // Cancel all passengers on this train
    for (int i = 0; i < Passenger_count; i++)
    {
        if (passenger[i].Traine_no == train_no && passenger[i].Ticket_status != CANCELED)
        {
            passenger[i].Ticket_status = CANCELED;
        }
    }

    // Remove train from array
    for (int j = train_index; j < Train_count - 1; j++)
    {
        trains[j] = trains[j + 1];
    }
    Train_count--;

    printf("Train canceled successfully, all associated tickets canceled\n");
    printf("_______________________________\n");
}

//-----------------------------------------------------------------------------------------------------------------------------------

void Display_train()
{
    // NO TRAIN TO SHOW
    if (Train_count == 0)
    {
        printf("No traine are avaliable:\n");
        printf("_______________________________\n");
    }
    int train_no = readInt("Enter traine no to see details(>0):", 0);

    printf("Detaile of  train \n");
    printf("---------------------\n");
    for (int i = 0; i < Train_count; i++)
    {
        // FIND TRAIN TO SHOW DETAILS
        if (train_no == trains[i].Train_number)
        {
            printf("\n Train_number =  %d", trains[i].Train_number);
            printf("\n Train_name =  %s", trains[i].Train_name);
            printf("\n Source_station =  %s", trains[i].Source_station);
            printf("\n Destination_station =  %s", trains[i].Destination_station);
            printf("\n Total_seat =  %d", trains[i].Total_seat);
            printf("\n Available_seat =  %d", trains[i].Available_seat);
            printf("\n___________________________________________________________\n");
            return;
        }
    }
    printf("Enter valid traine number\n");
    printf("_______________________________\n");
}
//-----------------------------------------------------------------------------------------------------------------------------------

void Display_all_train()
{
    printf("All trains \n");
    printf("---------------------\n");

    // PRINT DETAILS OF ALL TRAINS
    for (int i = 0; i < Train_count; i++)
    {
        printf("\n Train_number =  %d", trains[i].Train_number);
        printf("\n Train_name =  %s", trains[i].Train_name);
        printf("\n Source_station =  %s", trains[i].Source_station);
        printf("\n Destination_station =  %s", trains[i].Destination_station);
        printf("\n Total_seat =  %d", trains[i].Total_seat);
        printf("\n Available_seat =  %d", trains[i].Available_seat);
        printf("\n");
    }
    if (Train_count == 0)
    {
        printf("NO Traine to show details\n");
    }
    printf("_____________________________________________");
}
//----------------------------------------------------------------------------------------------------------------------------------

void writetofile_railway(struct Railway *R)
{
    FILE *fp = fopen("trains.dat", "ab");
    if (fp == NULL)
    {
        printf("File open error\n");
        return;
    }

    fwrite(&R[Train_count - 1], sizeof(struct Railway), 1, fp);
    fclose(fp);
}

//-----------------------------------------------------------------------------------------------------------------------------------

int loadfile_railway(struct Railway *R)
{
    FILE *fp1 = fopen("trains.dat", "rb");
    if (fp1 == NULL)
    {
        return 0;
    }

    int count = 0;
    while (count < MAX_TRAIN && fread(&R[count], sizeof(R[count]), 1, fp1) == 1)
    {
        count++;
    }

    fclose(fp1);
    return count;
}

//-----------------------------------------------------------------------------------------------------------------------------------

void writetofile_User(struct User *T)
{
    FILE *fp = fopen("passengers.dat", "ab"); 
    if (fp == NULL)
    {
        printf("File open error\n");
        return;
    }
    
    fwrite(&T[Passenger_count - 1], sizeof(struct User), 1, fp);
    fclose(fp);
}

//-----------------------------------------------------------------------------------------------------------------------------------

int loadfile_User(struct User *T)
{
    FILE *fp2 = fopen("passengers.dat", "rb");
    if (fp2 == NULL)
    {
        return 0;
    }

    int count = 0;
    while (count < MAX_PASSENGER_IN_ONE_TRAINE * MAX_TRAIN &&
           fread(&T[count], sizeof(T[count]), 1, fp2) == 1)
    {
        count++;
    }
    fclose(fp2);
    return count;
}
