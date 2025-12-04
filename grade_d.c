#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "student_records.txt"
#define PROBATION_FILE "probation.txt"
#define SCHOLARSHIP_FILE "scholarship.txt"

#define MAX_BUFFER_SIZE 256

#define STUDENT_PRINTF_FORMAT "id:%s, name:%s, mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%s, cgpa:%.2lf\n"
#define STUDENT_SCANF_FORMAT "id:%7[^,], name:%39[^,], mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%2s, cgpa:%lf"

typedef struct
{
    char id[8];
    char name[40];
    int mark1, mark2, mark3;
    int a_mark;
    char grade[3];
    double cgpa;
} student;

typedef struct
{
    char id[20];
    char pass[20];
} cred;

cred teacher = {"ad", "12"};

void view_g();
void login();
void cgpa_calc();

void s_list();
void p_list();

void info();

void teacher_info();
void create_student_id();
void delete_student_id();

void compute_grade(student *s);

int main(void)
{
    int option1;

    while (1)
    {
        printf("==============================\n");
        printf("===== Grade Distribution =====\n");
        printf("==============================\n");
        printf("=== 1. View Grade          ===\n");
        printf("=== 2. Admin Access        ===\n");
        printf("=== 3. CGPA Calculator     ===\n");
        printf("=== 4. Scholarship List    ===\n");
        printf("=== 5. Probation List      ===\n");
        printf("=== 6. Contact Information ===\n");
        printf("=== 7. Exit                ===\n");
        printf("==============================\n");

        printf("Select an option: ");
        scanf("%d", &option1);

        switch (option1)
        {
        case 1:
            view_g();
            break;
        case 2:
            login();
            break;
        case 3:
            cgpa_calc();
            break;
        case 4:
            s_list();
            break;
        case 5:
            p_list();
            break;
        case 6:
            info();
            break;
        case 7:
            exit(0);
        default:
            printf("Invalid option.\n");
        }
    }
}

void view_g()
{
    FILE *file = fopen(FILENAME, "r");

    if (!file)
    {
        printf("File not found.\n");
        return;
    }

    char search_id[8], buf[MAX_BUFFER_SIZE];

    printf("\n\nEnter ID: ");
    scanf("%s", search_id);

    int found = 0;
    while (fgets(buf, MAX_BUFFER_SIZE, file))
    {
        student s;
        if (sscanf(buf, STUDENT_SCANF_FORMAT, s.id, s.name, &s.mark1, &s.mark2, &s.mark3, &s.a_mark, s.grade, &s.cgpa) == 8)
        {
            if (strcmp(s.id, search_id) == 0)
            {
                printf("Name: %s\nID: %s\nM1:%d M2:%d M3:%d\nAvg:%d\nGrade:%s\nCGPA:%.2lf\n", s.name, s.id, s.mark1, s.mark2, s.mark3, s.a_mark, s.grade, s.cgpa);
                found = 1;
            }
        }
    }
    if (!found)
        printf("Not found.\n");
    fclose(file);
}

void login()
{
    char id[20], pass[20];

    printf("\n\n==============================\n");
    printf("======== Teacher Login =======\n");
    printf("=== ID: ");
    scanf("%s", id);
    printf("=== Pass: ");
    scanf("%s", pass);

    if (!strcmp(id, teacher.id) && !strcmp(pass, teacher.pass))
    {
        teacher_info();
    }
    else
    {
        printf("ACCESS DENIED\n");
        printf("Returning to main menu ... \n\n");

        return;
    }
}

void teacher_info()
{
    int option2;

    while (1)
    {
        printf("===============================\n");
        printf("========= Admin Access ========\n");
        printf("===============================\n");
        printf("=== 1. Create Student ID    ===\n");
        printf("=== 2. Delete Student ID    ===\n");
        printf("=== 3. Return to Main Menu  ===\n");
        printf("===============================\n");

        printf("Select an option: ");

        scanf("%d", &option2);
        if (option2 == 1)
            create_student_id();

        else if (option2 == 2)
            delete_student_id();

        else if (option2 == 3)
            return;

        else
        {
            printf("Please Select a valid option\n");
            printf("Try again.\n");
        }
    }
}

void create_student_id()
{
    FILE *file;
    char buff[MAX_BUFFER_SIZE];

    student s;

    printf("Enter ID: ");
    scanf("%s", s.id);

    file = fopen(FILENAME, "r");
    if (file)
    {
        while (fgets(buff, MAX_BUFFER_SIZE, file))
        {
            student t;
            sscanf(buff, STUDENT_SCANF_FORMAT, t.id, t.name, &t.mark1, &t.mark2, &t.mark3, &t.a_mark, t.grade, &t.cgpa);
            if (!strcmp(s.id, t.id))
            {
                printf("ID Found.\n");
                fclose(file);
                return;
            }
        }
        fclose(file);
    }

    printf("Name: ");
    getchar();
    fgets(s.name, 40, stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    do
    {
        printf("Mark1: ");
        scanf("%d", &s.mark1);
    } while (s.mark1 < 0 || s.mark1 > 100);
    do
    {
        printf("Mark2: ");
        scanf("%d", &s.mark2);
    } while (s.mark2 < 0 || s.mark2 > 100);
    do
    {
        printf("Mark3: ");
        scanf("%d", &s.mark3);
    } while (s.mark3 < 0 || s.mark3 > 100);

    s.a_mark = (s.mark1 + s.mark2 + s.mark3) / 3;
    compute_grade(&s);

    file = fopen(FILENAME, "a");
    fprintf(file, STUDENT_PRINTF_FORMAT, s.id, s.name, s.mark1, s.mark2, s.mark3, s.a_mark, s.grade, s.cgpa);
    fclose(file);

    printf("Student added!\n");
}

void delete_student_id()
{
    FILE *file = fopen(FILENAME, "r"), *temp = fopen("temp.txt", "w");
    if (!file || !temp)
    {
        printf("File error.\n");
        return;
    }

    char target[8], buf[MAX_BUFFER_SIZE];
    printf("Enter ID to delete: ");
    scanf("%s", target);

    int found = 0;
    while (fgets(buf, MAX_BUFFER_SIZE, file))
    {
        student s;
        sscanf(buf, STUDENT_SCANF_FORMAT, s.id, s.name, &s.mark1, &s.mark2, &s.mark3, &s.a_mark, s.grade, &s.cgpa);
        if (!strcmp(s.id, target))
        {
            found = 1;
            continue;
        }
        fputs(buf, temp);
    }

    fclose(file);
    fclose(temp);
    if (found)
    {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
        printf("Deleted.\n");
    }
    else
    {
        remove("temp.txt");
        printf("Not found.\n");
    }
}

void s_list()
{
    FILE *in = fopen(FILENAME, "r"), *out = fopen(SCHOLARSHIP_FILE, "w");
    if (!in || !out)
    {
        printf("Error.\n");
        return;
    }
    char buf[MAX_BUFFER_SIZE];
    int count = 0;

    while (fgets(buf, MAX_BUFFER_SIZE, in))
    {
        student s;
        sscanf(buf, STUDENT_SCANF_FORMAT, s.id, s.name, &s.mark1, &s.mark2, &s.mark3, &s.a_mark, s.grade, &s.cgpa);
        if (s.cgpa >= 3.5)
        {
            fprintf(out, "%s,%s,%.2lf\n", s.id, s.name, s.cgpa);
            count++;
        }
    }

    fclose(in);
    fclose(out);
    printf(count ? "List created.\n" : "None eligible.\n");
}

void p_list()
{
    FILE *in = fopen(FILENAME, "r"), *out = fopen(PROBATION_FILE, "w");
    if (!in || !out)
    {
        printf("Error.\n");
        return;
    }
    char buf[MAX_BUFFER_SIZE];
    int count = 0;

    while (fgets(buf, MAX_BUFFER_SIZE, in))
    {
        student s;
        sscanf(buf, STUDENT_SCANF_FORMAT, s.id, s.name, &s.mark1, &s.mark2, &s.mark3, &s.a_mark, s.grade, &s.cgpa);
        if (s.cgpa < 2.0)
        {
            fprintf(out, "%s,%s,%.2lf\n", s.id, s.name, s.cgpa);
            count++;
        }
    }

    fclose(in);
    fclose(out);
    printf(count ? "List created.\n" : "None in probation.\n");
}

void cgpa_calc()
{
    int m1, m2, m3;
    do
    {
        printf("Mark1: ");
        scanf("%d", &m1);
    } while (m1 < 0 || m1 > 100);
    do
    {
        printf("Mark2: ");
        scanf("%d", &m2);
    } while (m2 < 0 || m2 > 100);
    do
    {
        printf("Mark3: ");
        scanf("%d", &m3);
    } while (m3 < 0 || m3 > 100);

    int avg = (m1 + m2 + m3) / 3;
    student s;
    s.a_mark = avg;
    compute_grade(&s);

    printf("Avg:%d Grade:%s CGPA:%.2lf\n", avg, s.grade, s.cgpa);
}

void compute_grade(student *s)
{
    if (s->a_mark >= 93)
    {
        strcpy(s->grade, "A");
        s->cgpa = 4.0;
    }
    else if (s->a_mark >= 90)
    {
        strcpy(s->grade, "A-");
        s->cgpa = 3.7;
    }
    else if (s->a_mark >= 87)
    {
        strcpy(s->grade, "B+");
        s->cgpa = 3.3;
    }
    else if (s->a_mark >= 83)
    {
        strcpy(s->grade, "B");
        s->cgpa = 3.0;
    }
    else if (s->a_mark >= 80)
    {
        strcpy(s->grade, "B-");
        s->cgpa = 2.7;
    }
    else if (s->a_mark >= 77)
    {
        strcpy(s->grade, "C");
        s->cgpa = 2.3;
    }
    else if (s->a_mark >= 73)
    {
        strcpy(s->grade, "D");
        s->cgpa = 2.0;
    }
    else if (s->a_mark >= 70)
    {
        strcpy(s->grade, "E");
        s->cgpa = 1.7;
    }
    else
    {
        strcpy(s->grade, "F");
        s->cgpa = 0.0;
    }
}

void info()
{
    printf("\nAddress: Bashundhara, Dhaka-1229, Bangladesh\n");
    printf("Contact Number: +880-2-55668200\n");
    printf("Mail Address: registrar@northsouth.edu\n\n");

    printf("Available Sunday to Thursday: 09:00 a.m. to 05:00 p.m.\n\n");
}