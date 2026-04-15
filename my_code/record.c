
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NAME_LEN 17
#define MAX_EMP 10

struct Employee {
    char name[NAME_LEN];          
    unsigned short employeeId;    

    unsigned int isIntern   : 1;  
    unsigned int isSalaried : 1;  

    char gender;                  
    double salary;                
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

// Generic setter function type (matches wrapper functions)
typedef enum Status (*SetterFunc)(struct Employee *, void *);
typedef enum Status (*GetterFunc)(struct Employee *, void *);

// --- setter
enum Status setName(struct Employee *e, void *val) {
    return val && e ? strncpy(e->name, val, NAME_LEN-1), e->name[NAME_LEN-1]='\0', SUCCESS : FAIL;
}

enum Status setEmployeeId(struct Employee *e, void *val) {
    if (!e || !val) return FAIL;
    unsigned short *id = val;
    if (*id == 0) return FAIL;
    e->employeeId = *id;
    return SUCCESS;
}

enum Status setIsIntern(struct Employee *e, void *val) {
    if (!e || !val) return FAIL;
    unsigned int *flag = val;
    if (*flag > 1) return FAIL;
    e->isIntern = *flag;
    return SUCCESS;
}

enum Status setIsSalaried(struct Employee *e, void *val) {
    if (!e || !val) return FAIL;
    unsigned int *flag = val;
    if (*flag > 1) return FAIL;
    e->isSalaried = *flag;
    return SUCCESS;
}

enum Status setGender(struct Employee *e, void *val) {
    if (!e || !val) return FAIL;
    char *g = val;
    if (*g != 'M' && *g != 'F' && *g != 'O' && *g != 'm' && *g != 'f' && *g != 'o') return FAIL;
    e->gender = *g;
    return SUCCESS;
}

enum Status setSalary(struct Employee *e, void *val) {
    if (!e || !val) return FAIL;
    double *s = val;
    if (*s < 0) return FAIL;
    e->salary = *s;
    return SUCCESS;
}

// --- Getter 
enum Status getName(struct Employee *e, void *out) {
    if (!e || !out) return FAIL;
    strcpy(out, e->name);
    return SUCCESS;
}

enum Status getEmployeeId(struct Employee *e, void *out) {
    if (!e || !out) return FAIL;
    *(unsigned short *)out = e->employeeId;
    return SUCCESS;
}

enum Status getIsIntern(struct Employee *e, void *out) {
    if (!e || !out) return FAIL;
    *(unsigned int *)out = e->isIntern;
    return SUCCESS;
}

enum Status getIsSalaried(struct Employee *e, void *out) {
    if (!e || !out) return FAIL;
    *(unsigned int *)out = e->isSalaried;
    return SUCCESS;
}

enum Status getGender(struct Employee *e, void *out) {
    if (!e || !out) return FAIL;
    *(char *)out = e->gender;
    return SUCCESS;
}

enum Status getSalary(struct Employee *e, void *out) {
    if (!e || !out) return FAIL;
    *(double *)out = e->salary;
    return SUCCESS;
}

// Arrays of function pointers
SetterFunc setters[] = { setName, setEmployeeId, setIsIntern, setIsSalaried, setGender, setSalary };
GetterFunc getters[] = { getName, getEmployeeId, getIsIntern, getIsSalaried, getGender, getSalary };


int createFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp) { fclose(fp); return 1; }
    fp = fopen(filename, "w");
    if (!fp) return 0;
    fprintf(fp, "Employee Records\n----------------------\n");
    fclose(fp);
    return 1;
}

void writeLastUpdated(const char *filename) {
    FILE *fp = fopen(filename, "a");
    if (!fp) return;
    time_t now;
    char buf[64];
    time(&now);
    strftime(buf, sizeof(buf), "Last Updated: %d-%m-%Y %H:%M:%S\n----------------------\n", localtime(&now));
    fprintf(fp, "%s", buf);
    fclose(fp);
}

int writeEmployeeToFile(const char *filename, struct Employee *e) {
    FILE *fp = fopen(filename, "a");
    if (!fp) return 0;
    fprintf(fp, "Name: %s\nID: %hu\nIntern: %u\nSalaried: %u\nGender: %c\nSalary: %.2f\n----------------------\n",
            e->name, e->employeeId, e->isIntern, e->isSalaried, e->gender, e->salary);
    fclose(fp);
    return 1;
}

int main() {
    int n;
    printf("Enter number of employees (max 10): ");
    scanf("%d", &n);
    if (n > MAX_EMP) n = MAX_EMP;

    const char *filename = "record_emp.txt";
    if (!createFile(filename)) { printf("File creation failed\n"); return 1; }

    struct Employee *employees = calloc(n, sizeof(struct Employee));
    if (!employees) { printf("Allocation Failed\n"); return 1; }

    writeLastUpdated(filename);

    for (int i = 0; i < n; i++) {
        printf("\nEnter details for employee %d:\n", i+1);

        char name[NAME_LEN]; 
        unsigned short id;
        unsigned int isIntern, isSalaried; 
        char gender; 
        double salary;

        printf("Name: "); 
        scanf(" %16[^\n]", name);
        setters[FIELD_NAME](&employees[i], name);

        printf("Employee ID: "); 
        scanf("%hu", &id);
        setters[FIELD_EMPLOYEE_ID](&employees[i], &id);

        printf("Intern (0/1): "); 
        scanf("%u", &isIntern);
        setters[FIELD_IS_INTERN](&employees[i], &isIntern);

        printf("Salaried (0/1): "); 
        scanf("%u", &isSalaried);
        setters[FIELD_IS_SALARIED](&employees[i], &isSalaried);

        printf("Gender (M/F/O): "); 
        scanf(" %c", &gender);
        setters[FIELD_GENDER](&employees[i], &gender);

        printf("Salary: "); 
        scanf("%lf", &salary);
        setters[FIELD_SALARY](&employees[i], &salary);

        writeEmployeeToFile(filename, &employees[i]);
    }

    printf("\n--- Employee Records ---\n");
    for (int i = 0; i < n; i++) {
        char name[NAME_LEN]; 
        unsigned short id;
        unsigned int isIntern, isSalaried; 
        char gender; 
        double salary;

        getters[FIELD_NAME](&employees[i], name);
        getters[FIELD_EMPLOYEE_ID](&employees[i], &id);
        getters[FIELD_IS_INTERN](&employees[i], &isIntern);
        getters[FIELD_IS_SALARIED](&employees[i], &isSalaried);
        getters[FIELD_GENDER](&employees[i], &gender);
        getters[FIELD_SALARY](&employees[i], &salary);

        printf("\nEmployee %d:\nName: %s\nID: %hu\nIntern: %u\nSalaried: %u\nGender: %c\nSalary: %.2f\n",
               i+1, name, id, isIntern, isSalaried, gender, salary);
    }

    free(employees);
    printf("\nEmployee data written to file successfully in %s\n", filename);
    return 0;
}
