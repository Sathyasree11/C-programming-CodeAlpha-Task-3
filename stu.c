#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char name[50];
    int age;
    char bloodGroup[10];
    char department[30];
    float marks;
};

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    int choice;
    while (1) {
        printf("\n==============================\n");
        printf("     STUDENT MANAGEMENT SYSTEM\n");
        printf("==============================\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search Student\n");
        printf("4. Update Student Record\n");
        printf("5. Delete Student Record\n");
        printf("6. Exit\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // clear newline from input buffer

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                printf("\nExiting... Thank you!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}

void addStudent() {
    FILE *fp = fopen("students.dat", "ab");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    struct Student s;
    printf("\nEnter Student ID: ");
    scanf("%d", &s.id);
    getchar();

    printf("Enter Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &s.age);
    getchar();

    printf("Enter Blood Group: ");
    fgets(s.bloodGroup, sizeof(s.bloodGroup), stdin);
    s.bloodGroup[strcspn(s.bloodGroup, "\n")] = '\0';

    printf("Enter Department: ");
    fgets(s.department, sizeof(s.department), stdin);
    s.department[strcspn(s.department, "\n")] = '\0';

    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("\nStudent record added successfully!\n");
}

void displayStudents() {
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    struct Student s;
    printf("\n%-5s %-20s %-5s %-10s %-15s %-10s\n", 
           "ID", "Name", "Age", "BloodGrp", "Department", "Marks");
    printf("-------------------------------------------------------------\n");

    while (fread(&s, sizeof(s), 1, fp)) {
        printf("%-5d %-20s %-5d %-10s %-15s %-10.2f\n",
               s.id, s.name, s.age, s.bloodGroup, s.department, s.marks);
    }

    fclose(fp);
}

void searchStudent() {
    FILE *fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    struct Student s;
    int id, found = 0;

    printf("\nEnter Student ID to search: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            printf("\nRecord Found:\n");
            printf("------------------------\n");
            printf("ID: %d\nName: %s\nAge: %d\nBlood Group: %s\nDepartment: %s\nMarks: %.2f\n",
                   s.id, s.name, s.age, s.bloodGroup, s.department, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nStudent ID %d not found!\n", id);

    fclose(fp);
}

void updateStudent() {
    FILE *fp = fopen("students.dat", "rb+");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    struct Student s;
    int id, found = 0;

    printf("\nEnter Student ID to update: ");
    scanf("%d", &id);
    getchar();

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            found = 1;

            printf("\nEnter New Details:\n");

            printf("Enter Name: ");
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = '\0';

            printf("Enter Age: ");
            scanf("%d", &s.age);
            getchar();

            printf("Enter Blood Group: ");
            fgets(s.bloodGroup, sizeof(s.bloodGroup), stdin);
            s.bloodGroup[strcspn(s.bloodGroup, "\n")] = '\0';

            printf("Enter Department: ");
            fgets(s.department, sizeof(s.department), stdin);
            s.department[strcspn(s.department, "\n")] = '\0';

            printf("Enter Marks: ");
            scanf("%f", &s.marks);

            fseek(fp, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);

            printf("\nRecord updated successfully!\n");
            break;
        }
    }

    if (!found)
        printf("\nStudent ID %d not found!\n", id);

    fclose(fp);
}

void deleteStudent() {
    FILE *fp = fopen("students.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (fp == NULL || temp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    struct Student s;
    int id, found = 0;

    printf("\nEnter Student ID to delete: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id != id)
            fwrite(&s, sizeof(s), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        printf("\nRecord deleted successfully!\n");
    else
        printf("\nStudent ID %d not found!\n", id);
}
