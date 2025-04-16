#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBJECTS 6
#define MAX_NAME 100
#define MAX_STUDENTS 100

const char* subjectNames[MAX_SUBJECTS] = {
    "Tamil", "English", "Mathematics", "Physics", "Chemistry" , "Computer Science"};

typedef struct {
    int rollNo;
    char name[MAX_NAME];
    int marks[MAX_SUBJECTS];
    int total;
    float average;
    char grade[3];
    char result[15];
    int passStatus[MAX_SUBJECTS];
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

void calculateResult(Student* s) {
    s->total = 0;
    int passedSubjects = 0;
    int isDistinction = 1;

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        s->total += s->marks[i];
        if (s->marks[i] >= 50) {
            s->passStatus[i] = 1;
        } else {
            s->passStatus[i] = 0;
            isDistinction = 0;
        }
        if (s->marks[i] >= 35) passedSubjects++;
    }

    s->average = s->total / (float)MAX_SUBJECTS;


    if (s->average >= 90) strcpy(s->grade, "S");
    else if (s->average >= 80) strcpy(s->grade, "A");
    else if (s->average >= 70) strcpy(s->grade, "B");
    else if (s->average >= 60) strcpy(s->grade, "C");
    else if (s->average >= 50) strcpy(s->grade, "D");
    else strcpy(s->grade, "F");


    if (passedSubjects == MAX_SUBJECTS) {
        if (isDistinction && s->average >= 75) {
            strcpy(s->result, "Distinction");
        } else {
            strcpy(s->result, "Pass");
        }
    } else {
        strcpy(s->result, "Fail");
    }
}


void insertStudent(Student s) {
    int pos = 0;
    while (pos < studentCount && students[pos].rollNo < s.rollNo) {
        pos++;
    }


    if (pos < studentCount && students[pos].rollNo == s.rollNo) {
        for (int i = studentCount; i > pos; i--) {
            students[i] = students[i - 1];
        }
        studentCount++;
        students[pos] = s;
    } else {
        for (int i = studentCount; i > pos; i--) {
            students[i] = students[i - 1];
        }
        students[pos] = s;
        studentCount++;
    }
}


void loadFromCSV(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return;

    char line[512];
    fgets(line, sizeof(line), fp); //
    while (fgets(line, sizeof(line), fp)) {
        Student s;
        char* token = strtok(line, ",");
        s.rollNo = atoi(token);
        token = strtok(NULL, ",");
        strcpy(s.name, token);

        for (int i = 0; i < MAX_SUBJECTS; i++) {
            token = strtok(NULL, ",");
            s.marks[i] = atoi(token);
        }

        calculateResult(&s);
        insertStudent(s);
    }

    fclose(fp);
}


void exportToCSV(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to open %s\n", filename);
        return;
    }

    fprintf(fp, "Roll No,Name");
    for (int i = 0; i < MAX_SUBJECTS; i++)
        fprintf(fp, ",%s", subjectNames[i]);
    fprintf(fp, ",Total,Average,Grade,Result\n");

    for (int i = 0; i < studentCount; i++) {
        fprintf(fp, "%d,%s", students[i].rollNo, students[i].name);
        for (int j = 0; j < MAX_SUBJECTS; j++)
            fprintf(fp, ",%d", students[i].marks[j]);
        fprintf(fp, ",%d,%.2f,%s,%s\n",
                students[i].total, students[i].average,
                students[i].grade, students[i].result);
    }

    fclose(fp);
    printf("Data exported to %s\n", filename);
}

void addStudent() {
    Student s;
    printf("Enter Roll No: ");
    scanf("%d", &s.rollNo);
    getchar();
    printf("Enter Name: ");
    fgets(s.name, MAX_NAME, stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("Enter marks for %s: ", subjectNames[i]);
        scanf("%d", &s.marks[i]);
    }

    calculateResult(&s);
    insertStudent(s);
    exportToCSV("student_results.csv");
}


void viewStudents() {
    for (int i = 0; i < studentCount; i++) {
        printf("\nRoll No: %d\nName: %s\n", students[i].rollNo, students[i].name);
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            printf("%s: %d - %s\n", subjectNames[j], students[i].marks[j],
                   students[i].passStatus[j] ? "Pass" : "Fail");
        }
        printf("Total: %d, Average: %.2f\nGrade: %s\nResult: %s\n",
               students[i].total, students[i].average,
               students[i].grade, students[i].result);
    }
}


void findTopper() {
    if (studentCount == 0) {
        printf("No students available.\n");
        return;
    }
    int topIndex = 0;
    for (int i = 1; i < studentCount; i++) {
        if (students[i].total > students[topIndex].total)
            topIndex = i;
    }
    printf("\nTopper: %s (Roll No: %d) - Total: %d, Average: %.2f, Grade: %s, Result: %s\n",
           students[topIndex].name,
           students[topIndex].rollNo,
           students[topIndex].total,
           students[topIndex].average,
           students[topIndex].grade,
           students[topIndex].result);
}

void searchStudent() {
    int roll;
    printf("Enter roll number to search: ");
    scanf("%d", &roll);
    for (int i = 0; i < studentCount; i++) {
        if (students[i].rollNo == roll) {
            printf("Found: %s - Total: %d, Average: %.2f, Grade: %s, Result: %s\n",
                   students[i].name,
                   students[i].total,
                   students[i].average,
                   students[i].grade,
                   students[i].result);
            return;
        }
    }
    printf("Student with roll number %d not found.\n", roll);
}

int main() {
    loadFromCSV("student_results.csv");

    int choice;
    do {
        printf("\n---- Student Management Menu ----\n");
        printf("1. Add Student\n2. View All Students\n3. Export to CSV\n4. Search by Roll No\n5. Find Topper\n0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: exportToCSV("student_results.csv"); break;
            case 4: searchStudent(); break;
            case 5: findTopper(); break;
            case 0: printf("Exiting.\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
