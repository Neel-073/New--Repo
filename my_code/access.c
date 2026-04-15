#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
    int User_ID;
    char Name[50];
    int Access_Level;
    int Status;
} Access;

void Add_user(Access *access, int *count);
void Update_user(Access *access, int count);
void Delete_user(Access *access, int *count);
void display_user(Access *access, int count);
void Display_All(Access *access, int count);
void saveToFile(Access *access, int count);
void loadFromFile(Access *access, int *count);

int main()
{

    int n, count = 0;
    printf("Enter number of user MAX 50:\n");
    scanf("%d", &n);
    Access access[n];
    loadFromFile(access, &count);

    int choise;

    do
    {
        printf("Select menu for operation\n:");
        printf("1.Add User \n:");
        printf("2.Update User \n:");
        printf("3.Delete User \n:");
        printf("4.Display User \n:");
        printf("5.Display all User \n:");
        printf("0. EXIT \n:");
        scanf("%d", &choise);
        getchar();
        switch (choise)
        {
        case 1:
        {
            Add_user(access, &count);
            break;
        }
        case 2:
        {
            Update_user(access, count);
            break;
        }
        case 3:
        {
            Delete_user(access, &count);
            break;
        }
        case 4:
        {
            display_user(access, count);
            break;
        }
        case 5:
        {
            Display_All(access, count);
            break;
        }
        case 0:
        {
            saveToFile(access, count);
            printf("Exiting....\n");
            break;
        }

        default:
        {
            printf("Enter valid choise:");
            break;
        }
        }
    } while (choise != 0);

    return 0;
}

void Add_user(Access *access, int *count)
{
    printf("Enter user data as following:\n");

    printf("Enter user id:");
    scanf("%d", &access[*count].User_ID);

    getchar();
    printf("Enter name of user:");
    fgets(access[*count].Name, sizeof(access[*count].Name), stdin);

    printf("Enter user access level:");
    scanf("%d", &access[*count].Access_Level);

    printf("Enter user status:");
    scanf("%d", &access[*count].Status);

    (*count)++;
    printf("User added successfully\n");
}

void Update_user(Access *access, int count)
{
    int i, cho;

    printf("Enter user id that you want to update:");
    scanf("%d", &cho);
    for (i = 0; i < count; i++)
    {
        if (cho == access[i].User_ID)
        {
            getchar();
            printf("Enter new name of user:");
            fgets(access[i].Name, sizeof(access[i].Name), stdin);

            printf("Enter new user access level:");
            scanf("%d", &access[i].Access_Level);

            printf("Enter new user status:");
            scanf("%d", &access[i].Status);

            printf("User updated successfully\n");
            return;
        }
    }

    printf("User not found\n");
}
void Delete_user(Access *access, int *count)
{
    int cho, i, j;
    printf("Enter user id that you want to delete:");
    scanf("%d", &cho);

    for (i = 0; i < *count; i++)
    {
        if (cho == access[i].User_ID)
        {
            for (j = i; j < *count - 1; j++)
            {
                access[j] = access[j + 1];
            }
            (*count)--;
            printf("User deleted successfully\n");
            return;
        }
    }
    printf("User not found\n");
}

void display_user(Access *access, int count)
{
    int cho, i;
    printf("Enter user id to display:");
    scanf("%d", &cho);

    for (i = 0; i < count; i++)
    {
        if (cho == access[i].User_ID)
        {
            printf("User data:\n");
            printf("ID: %d\nName: %sAccess Level: %d\nStatus: %d\n",
                   access[i].User_ID,
                   access[i].Name,
                   access[i].Access_Level,
                   access[i].Status);
            return;
        }
    }
    printf("User not found\n");
}

void Display_All(Access *access, int count)
{
    int i;
    if (count == 0)
    {
        printf("No users available\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        printf("\nUser %d\n", i + 1);
        printf("ID: %d\n", access[i].User_ID);
        printf("Name: %s", access[i].Name);
        printf("Access Level: %d\n", access[i].Access_Level);
        printf("Status: %d\n", access[i].Status);
    }
}
void saveToFile(Access *access, int count)
{
    FILE *fp = fopen("users.txt", "w");

    if (fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fprintf(fp, "%d\n", count); // save number of users first

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%d %s %d %d\n",
                access[i].User_ID,
                access[i].Name,
                access[i].Access_Level,
                access[i].Status);
    }

    fclose(fp);
    printf("Data saved to file successfully\n");
}
void loadFromFile(Access *access, int *count)
{
    FILE *fp = fopen("users.txt", "r");

    if (fp == NULL)
    {
        printf("No previous data found\n");
        return;
    }

    fscanf(fp, "%d\n", count);

    for (int i = 0; i < *count; i++)
    {
        fscanf(fp, "%d ", &access[i].User_ID);
        fgets(access[i].Name, sizeof(access[i].Name), fp);
        fscanf(fp, "%d %d\n",
               &access[i].Access_Level,
               &access[i].Status);
    }

    fclose(fp);
    printf("Data loaded from file successfully\n");
}
