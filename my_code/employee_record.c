#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// r+	Searches file. It is opened successfully fopen( ) loads it into memory and sets up a 
// pointer that points to the first character in it. Returns NULL, if unable to open the file.
// Syntax: size_t strcspn(const char *str1, const char *str2);
// vfprintf


#define NAME_LEN 17
#define MAX_EMP 10

struct Employee {
    char name[NAME_LEN];          // string (max length 17)
    unsigned short employeeId;    // 16-bit integer

    unsigned int isIntern   : 1;  // 1-bit flag
    unsigned int isSalaried : 1;  // 1-bit flag

    char gender;                  // character
    double salary;                // numeric value
};
enum Status {
    SUCCESS = 1,
    FAIL = 0
};

enum RecordField {
    FIELD_NAME,
    FIELD_EMPLOYEE_ID,
    FIELD_IS_INTERN,
    FIELD_IS_SALARIED,
    FIELD_GENDER,
    FIELD_SALARY
};


enum Status set_field (struct Employee *e,enum RecordField field,void *value) {
    if (!e || !value) return FAIL;

    switch(field){

        case FIELD_NAME:{
            char *str = (char *)value;
            if (strlen(str) >= NAME_LEN) return FAIL;
            strncpy(e->name, str, NAME_LEN-1);
            e->name[NAME_LEN-1] = '\0';
            return SUCCESS;
        }
        case FIELD_EMPLOYEE_ID:{
            unsigned short *id = (unsigned short *)value;
            if (*id == 0) return FAIL;
            e->employeeId = *id;
            return SUCCESS;
        }
        case FIELD_IS_INTERN:{
            unsigned int *flag=(unsigned int*)value;
            if (*flag > 1) return FAIL;
             e->isIntern = *flag;
            return SUCCESS;
        }  
        case FIELD_IS_SALARIED:{
            unsigned int *flag = (unsigned int *)value;
            if (*flag > 1) return FAIL;
            e->isSalaried = *flag;
            return SUCCESS;
        }
        case FIELD_GENDER:{
            char *g = (char *)value;
            if (*g != 'M' && *g != 'm' &&*g != 'F' && *g != 'f' && *g != 'O' && *g != 'o') return FAIL;
            e->gender = *g;
            return SUCCESS;
        }
        case FIELD_SALARY:{
            double *s = (double *)value;
            if (*s < 0) return FAIL;
            e->salary = *s;
            return SUCCESS;
        }
        default:
            return FAIL; 

    }
    
}

enum Status get_field(struct Employee *e, enum RecordField field, void *out) {
    if (!e || !out) return FAIL;

    switch(field) {

        case FIELD_NAME: {
            strcpy((char *)out, e->name);
            return SUCCESS;
        }

        case FIELD_EMPLOYEE_ID: {
            *(unsigned short *)out = e->employeeId;
            return SUCCESS;
        }

        case FIELD_IS_INTERN: {
            *(unsigned int *)out = e->isIntern;
            return SUCCESS;
        }

        case FIELD_IS_SALARIED: {
            *(unsigned int *)out = e->isSalaried;
            return SUCCESS;
        }

        case FIELD_GENDER: {
            *(char *)out = e->gender;
            return SUCCESS;
        }

        case FIELD_SALARY: {
            *(double *)out = e->salary;
            return SUCCESS;
        }

        default:
            return FAIL;
    }
}


int createFile(const char *filename)
{
    FILE *filePtr = fopen(filename, "r");  
    if (filePtr != NULL) {
        fclose(filePtr);
        return 1;   
    }

    filePtr = fopen(filename, "w");
    if (filePtr == NULL)
        return 0;

    fprintf(filePtr, "Employee Records\n");
    fprintf(filePtr, "----------------------\n");

    fclose(filePtr);
    return 1;
}



void writeLastUpdated(const char *filename)
{
    FILE *fp = fopen(filename, "a");   // append
    if (!fp) return;

    time_t now;
    char timeBuffer[64];

    time(&now);
    strftime(timeBuffer, sizeof(timeBuffer),
             "Last Updated: %d-%m-%Y %H:%M:%S\n",
             localtime(&now));

    fprintf(fp, "%s", timeBuffer);
    fprintf(fp, "----------------------\n");

    fclose(fp);
}




int writeEmployeeToFile(const char *filename, struct Employee *e) {
    FILE *fp = fopen(filename, "a");   // append mode
    if (fp == NULL)
        return 0;

    fprintf(fp, "Name: %s\n", e->name);
    fprintf(fp, "ID: %hu\n", e->employeeId);
    fprintf(fp, "Intern: %u\n", e->isIntern);
    fprintf(fp, "Salaried: %u\n", e->isSalaried);
    fprintf(fp, "Gender: %c\n", e->gender);
    fprintf(fp, "Salary: %.2f\n", e->salary);
    fprintf(fp, "----------------------\n");

    fclose(fp);
    return 1;
}

void rewriteFile(struct Employee *employees, int count) {
    FILE *fp = fopen("employees.txt", "w");   // overwrite
    if (!fp) return;

    fprintf(fp, "Employee Records\n");
    fprintf(fp, "----------------------\n");

    for (int i = 0; i < count; i++) {
        fprintf(fp, "Name: %s\n", employees[i].name);
        fprintf(fp, "ID: %hu\n", employees[i].employeeId);
        fprintf(fp, "Intern: %u\n", employees[i].isIntern);
        fprintf(fp, "Salaried: %u\n", employees[i].isSalaried);
        fprintf(fp, "Gender: %c\n", employees[i].gender);
        fprintf(fp, "Salary: %.2f\n", employees[i].salary);
        fprintf(fp, "----------------------\n");
    }
    fclose(fp);
} 

void inputEmployee(struct Employee *e) {
    char name[NAME_LEN];
    unsigned short id;
    unsigned int isIntern, isSalaried;
    char gender;
    double salary;

    printf("Name: "); scanf(" %16[^\n]", name); 
    set_field(e, FIELD_NAME, name);
    printf("Employee ID: "); scanf("%hu", &id); 
    set_field(e, FIELD_EMPLOYEE_ID, &id);
    printf("Intern (0/1): "); scanf("%u", &isIntern); 
    set_field(e, FIELD_IS_INTERN, &isIntern);
    printf("Salaried (0/1): "); scanf("%u", &isSalaried); 
    set_field(e, FIELD_IS_SALARIED, &isSalaried);
    printf("Gender (M/F/O): "); scanf(" %c", &gender); 
    set_field(e, FIELD_GENDER, &gender);
    printf("Salary: "); scanf("%lf", &salary); 
    set_field(e, FIELD_SALARY, &salary);
}
int menu(void)
{
    int choice;
    printf("\n===== Employee Menu =====\n");
    printf("1. Add a record\n");
    printf("2. Update a record\n");
    printf("3. Delete a record\n");
    printf("5. Display all records\n");
    printf("0. Quit\n");
    printf("Select option: ");
    scanf("%d", &choice);
    return choice;
}


void addRecord(struct Employee **employees, int *count, const char *filename) {
    if (*count >= MAX_EMP) { printf("Maximum limit reached\n"); return; }

    struct Employee *temp = realloc(*employees, sizeof(struct Employee) * (*count + 1));
    if (!temp) { printf("Memory allocation failed\n"); return; }
    *employees = temp;

    inputEmployee(&(*employees)[*count]);
    writeEmployeeToFile(filename, &(*employees)[*count]);
    (*count)++;
    writeLastUpdated(filename);
    printf("Record added successfully\n");
}

void updateRecord(struct Employee *employees, int count) {
    int index;
    if (count == 0) { printf("No records to update\n"); return; }

    printf("Enter record number to update: ");
    scanf("%d", &index); index--;

    if (index < 0 || index >= count) { printf("Invalid record number\n"); return; }

    inputEmployee(&employees[index]);
    rewriteFile(employees, count);
    writeLastUpdated("employees.txt");
    printf("Record updated successfully\n");
}

void deleteRecord(struct Employee **employees, int *count) {
    int index;
    if (*count == 0) { printf("No records to delete\n"); return; }

    printf("Enter record number to delete: ");
    scanf("%d", &index); index--;

    if (index < 0 || index >= *count) { printf("Invalid record number\n"); return; }

    for (int i = index; i < *count - 1; i++)
        (*employees)[i] = (*employees)[i + 1];

    (*count)--;
    struct Employee *temp = realloc(*employees, sizeof(struct Employee) * (*count));
    if (temp || *count == 0) *employees = temp;

    rewriteFile(*employees, *count);
    writeLastUpdated("employees.txt");
    printf("Record deleted successfully\n");
}


void displayAllRecords(struct Employee *employees, int count) {
    if (count == 0) { printf("No records available\n"); return; }

    for (int i = 0; i < count; i++) {
        char name[NAME_LEN], gender;
        unsigned short id;
        unsigned int isIntern, isSalaried;
        double salary;

        get_field(&employees[i], FIELD_NAME, name);
        get_field(&employees[i], FIELD_EMPLOYEE_ID, &id);
        get_field(&employees[i], FIELD_IS_INTERN, &isIntern);
        get_field(&employees[i], FIELD_IS_SALARIED, &isSalaried);
        get_field(&employees[i], FIELD_GENDER, &gender);
        get_field(&employees[i], FIELD_SALARY, &salary);

        printf("\nEmployee %d\n", i + 1);
        printf("Name     : %s\n", name);
        printf("ID       : %hu\n", id);
        printf("Intern   : %u\n", isIntern);
        printf("Salaried : %u\n", isSalaried);
        printf("Gender   : %c\n", gender);
        printf("Salary   : %.2f\n", salary);
    }
}


int main() {
    int n = 0;
    struct Employee *employees = NULL;

    const char *filename = "employees.txt";
    if (!createFile(filename)) { printf("File creation failed\n"); return 1; }

    int choice;
    do {
        choice = menu();
        switch (choice) {
            case 1: printf("You selected: Add a new employee record\n");
                    addRecord(&employees, &n, filename); 
                    break;
            case 2: printf("You selected: Update an existing record\n");
                    updateRecord(employees, n); 
                    break;
            case 3: printf("You selected: Delete a record\n");
                    deleteRecord(&employees, &n); 
                    break;
            case 5: printf("You selected: Display all records\n");
                    displayAllRecords(employees, n); 
                    break;
            case 0: printf("Exiting program...\n"); 
                    break;
            default: printf("Invalid choice! Please select 1, 2, 3, 5 or 0.\n");
        }
    } while (choice != 0);

    free(employees);
    return 0;
}