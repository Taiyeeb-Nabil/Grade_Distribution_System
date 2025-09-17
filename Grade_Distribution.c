#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "records.txt"
#define MAX_BUFFER_SIZE 256
#define STUDENT_SCANF_FORMAT "id:%7[^,], name:%39[^,], mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%2s, cgpa:%lf"
#define STUDENT_PRINTF_FORMAT "id:%s, name:%s, mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%s, cgpa:%.2lf\n"

typedef struct // Student structure
{
    char id[8];
    char name[40];
    int mark1;
    int mark2;
    int mark3;
    int a_mark;
    char grade[3];
    double cgpa;
} student;

typedef struct // Teacher credential structure
{
    char id[20];
    char pass[20];
} cred;

cred teacher = {"admin", "12345"}; // Default teacher credentials

void student_info();
void login();
void teacher_info();
void s_list();
void p_list();
void cgpa_calc();
void info();

void edit_marks();
void create_student_id();
void delete_student_id();
void view();

int main(void) // Main Menu
{
    int opt1;
    char buffer[MAX_BUFFER_SIZE];

    do
    {
        printf("=============================\n");
        printf("======= Welcome to NSU ======\n");
        printf("=============================\n");
        printf("=== 1. Check Student info ===\n");
        printf("=== 2. Teacher Login      ===\n");
        printf("=== 3. Scholarship List   ===\n");
        printf("=== 4. Probation List     ===\n");
        printf("=== 5. CGPA Calculator    ===\n");
        printf("=== 6. Contact Info       ===\n");
        printf("=== 7. Exit               ===\n");
        printf("=============================\n");

        printf("Select Option: ");
        scanf("%d", &opt1);

        switch (opt1)
        {
        case 1:
            student_info();
            break;
        case 2:
            login();
            break;
        case 3:
            s_list();
            break;
        case 4:
            p_list();
            break;
        case 5:
            cgpa_calc();
            break;
        case 6:
            info();
            break;
        case 7:
            printf("Thank you for using the program. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid option! Please reselect option.\n\n");
        }

    } while (1);

    return 0;
}

void student_info() // Student info search
{
    FILE *file;
    char search_id[8];
    char line_buffer[MAX_BUFFER_SIZE];
    int found = 0;

    printf("================= Student Info ================\n");
    printf("===  Enter the ID to search for:");
    scanf("%s", search_id);

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;

    file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    printf("\n---- Search Result ----\n");

    while (fgets(line_buffer, MAX_BUFFER_SIZE, file) != NULL)
    {
        student current_student;
        int fields = sscanf(line_buffer, "id:%7[^,], name:%39[^,], mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%2s, cgpa:%lf", current_student.id, current_student.name, &current_student.mark1, &current_student.mark2, &current_student.mark3, &current_student.a_mark, current_student.grade, &current_student.cgpa);
        if (fields != 8)
        {
            continue;
        }

        if (strcmp(search_id, current_student.id) == 0)
        {
            printf("Here is the record of Student id: %s\n", current_student.id);
            printf("Name: %s\n", current_student.name);
            printf("ID: %s\n", current_student.id);
            printf("Mark 1: %d\n", current_student.mark1);
            printf("Mark 2: %d\n", current_student.mark2);
            printf("Mark 3: %d\n", current_student.mark3);
            printf("Average Mark: %d\n", current_student.a_mark);
            printf("Grade: %s\n", current_student.grade);
            printf("CGPA: %.2lf\n", current_student.cgpa);
            found = 1;
            break;
        }
    }
    if (!found)
    {
        printf("No record\n");
    }

    fclose(file);
}

void login() // Login verification
{
    char id[20];
    char pass[20];

    printf("Enter id: ");
    scanf("%s", id);

    printf("Enter password: ");
    scanf("%s", pass);

    if (strcmp(id, teacher.id) == 0 && strcmp(pass, teacher.pass) == 0)
    {
        teacher_info();
    }
    else
    {
        printf("Wrong Credential!\n\n");
        return;
    }
}

void teacher_info() // Teacher Menu
{
    int opt;

    do
    {
        printf("=====================================\n");
        printf("============ Teacher Menu ===========\n");
        printf("=====================================\n");
        printf("=== 1. Create new Student ID      ===\n");
        printf("=== 2. Delete existing Student ID ===\n");
        printf("=== 3. View Student Info          ===\n");
        printf("=== 4. Return to Main Menu        ===\n");
        printf("=====================================\n");

        printf("Select Option: ");
        scanf("%d", &opt);

        switch (opt)
        {
        case 1:
            create_student_id();
            break;
        case 2:
            delete_student_id();
            break;
        case 3:
            view();
            break;
        case 4:
            return;
        default:
            printf("Incorrect option!\n");
        }
    } while (1);
}

void create_student_id() // Create new student ID
{
    FILE *file;
    student new_student;

    printf("===== Enter Student Information =====\n");
    printf("=== ID: ");
    scanf("%s", new_student.id);

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;

    printf("=== Name: ");
    fgets(new_student.name, sizeof(new_student.name), stdin);

    new_student.name[strcspn(new_student.name, "\n")] = '\0';
    printf("=== Mark 1: ");
    scanf("%d", &new_student.mark1);

    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    printf("=== Mark 2: ");
    scanf("%d", &new_student.mark2);
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    printf("=== Mark 3: ");
    scanf("%d", &new_student.mark3);
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
    printf("\n");

    new_student.a_mark = (new_student.mark1 + new_student.mark2 + new_student.mark3) / 3;

    if (new_student.a_mark >= 93)
    {
        strcpy(new_student.grade, "A");
        new_student.cgpa = 4.0;
    }
    else if (new_student.a_mark >= 90)
    {
        strcpy(new_student.grade, "A-");
        new_student.cgpa = 3.7;
    }
    else if (new_student.a_mark >= 87)
    {
        strcpy(new_student.grade, "B+");
        new_student.cgpa = 3.3;
    }
    else if (new_student.a_mark >= 83)
    {
        strcpy(new_student.grade, "B");
        new_student.cgpa = 3.0;
    }
    else if (new_student.a_mark >= 80)
    {
        strcpy(new_student.grade, "B-");
        new_student.cgpa = 2.7;
    }
    else if (new_student.a_mark >= 77)
    {
        strcpy(new_student.grade, "C");
        new_student.cgpa = 2.3;
    }
    else if (new_student.a_mark >= 73)
    {
        strcpy(new_student.grade, "D");
        new_student.cgpa = 2.0;
    }
    else if (new_student.a_mark >= 70)
    {
        strcpy(new_student.grade, "E");
        new_student.cgpa = 1.7;
    }
    else
    {
        strcpy(new_student.grade, "F");
        new_student.cgpa = 0.0;
    }

    file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        printf("Could not open file for writing.");
        return;
    }

    fprintf(file, STUDENT_PRINTF_FORMAT, new_student.id, new_student.name, new_student.mark1, new_student.mark2, new_student.mark3, new_student.a_mark, new_student.grade, new_student.cgpa);
    fclose(file);

    printf("==== Student added successfully! ====\n\n");
}

void delete_student_id() // Delete existing student ID
{
    FILE *file, *temp_file;
    char search_id[8];
    char line_buffer[MAX_BUFFER_SIZE];
    int found = 0;

    printf("Enter the ID to delete: ");
    scanf("%s", search_id);

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;

    file = fopen(FILENAME, "r");

    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    temp_file = fopen("temp.txt", "w");

    if (temp_file == NULL)
    {
        printf("Error: Could not open temporary file for writing.\n");
        fclose(file);
        return;
    }

    while (fgets(line_buffer, MAX_BUFFER_SIZE, file) != NULL)
    {
        student current_student;
        sscanf(line_buffer, "id:%7[^,], name:%39[^,], mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%2s, cgpa:%lf", current_student.id, current_student.name, &current_student.mark1, &current_student.mark2, &current_student.mark3, &current_student.a_mark, current_student.grade, &current_student.cgpa);
        if (strcmp(search_id, current_student.id) == 0)
        {
            found = 1;
            printf("Record of Student id: %s deleted successfully.\n", current_student.id);
            continue;
        }
        fprintf(temp_file, "%s", line_buffer);
    }

    fclose(file);
    fclose(temp_file);

    if (found)
    {
        remove(FILENAME);
        rename("temp.txt", FILENAME);
    }
    else
    {
        remove("temp.txt");
    }

    if (!found)
    {
        printf("No record found with ID: %s\n", search_id);
    }
}

void view() // View all student records
{
    FILE *file;
    char line_buffer[MAX_BUFFER_SIZE];

    file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    printf("\n---- All Student Records ----\n");

    while (fgets(line_buffer, MAX_BUFFER_SIZE, file) != NULL)
    {
        student current_student;

        sscanf(line_buffer, "id:%7[^,], name:%39[^,], mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%2s, cgpa:%lf", current_student.id, current_student.name, &current_student.mark1, &current_student.mark2, &current_student.mark3, &current_student.a_mark, current_student.grade, &current_student.cgpa);

        printf("Name: %s\n", current_student.name);
        printf("ID: %s\n", current_student.id);
        printf("Mark 1: %d\n", current_student.mark1);
        printf("Mark 2: %d\n", current_student.mark2);
        printf("Mark 3: %d\n", current_student.mark3);
        printf("Average Mark: %d\n", current_student.a_mark);
        printf("Grade: %s\n", current_student.grade);
        printf("CGPA: %.2lf\n", current_student.cgpa);
        printf("=================================\n");
    }

    fclose(file);
}

void s_list() // Scholarship list
{
    printf("=== Students applicable for Scholarship ===\n");
    FILE *file;
    char line_buffer[MAX_BUFFER_SIZE];
    int scholarship_count = 0;

    file = fopen(FILENAME, "r");

    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    while (fgets(line_buffer, MAX_BUFFER_SIZE, file) != NULL)
    {
        student current_student;
        sscanf(line_buffer, "id:%7[^,], name:%39[^,], mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%2s, cgpa:%lf", current_student.id, current_student.name, &current_student.mark1, &current_student.mark2, &current_student.mark3, &current_student.a_mark, current_student.grade, &current_student.cgpa);
        if (current_student.cgpa >= 3.5)
        {
            printf("Name: %s, ID: %s, CGPA: %.2lf\n", current_student.name, current_student.id, current_student.cgpa);
            scholarship_count++;
        }
    }

    if (scholarship_count == 0)
    {
        printf("No students are eligible for scholarship.\n");
    }

    fclose(file);
}

void p_list() // Probation list
{
    printf("=== Students on Probation List ===\n");

    FILE *file;
    char line_buffer[MAX_BUFFER_SIZE];
    int probation_count = 0;

    file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    while (fgets(line_buffer, MAX_BUFFER_SIZE, file) != NULL)
    {
        student current_student;
        sscanf(line_buffer, "id:%7[^,], name:%39[^,], mark1:%d, mark2:%d, mark3:%d, a_mark:%d, grade:%2s, cgpa:%lf", current_student.id, current_student.name, &current_student.mark1, &current_student.mark2, &current_student.mark3, &current_student.a_mark, current_student.grade, &current_student.cgpa);
        if (current_student.cgpa < 2.0)
        {
            printf("Name: %s, ID: %s, CGPA: %.2lf\n", current_student.name, current_student.id, current_student.cgpa);
            probation_count++;
        }
    }

    if (probation_count == 0)
    {
        printf("No students are on the probation list.\n");
    }

    fclose(file);
}

void cgpa_calc() // CGPA Calculator
{
    int mark1, mark2, mark3;
    int a_mark;
    char grade[3];
    double cgpa;

    printf("Enter Mark 1: ");
    scanf("%d", &mark1);
    printf("Enter Mark 2: ");
    scanf("%d", &mark2);
    printf("Enter Mark 3: ");
    scanf("%d", &mark3);

    a_mark = (mark1 + mark2 + mark3) / 3;

    if (a_mark >= 93)
    {
        strcpy(grade, "A");
        cgpa = 4.0;
    }
    else if (a_mark >= 90 && a_mark <= 92)
    {
        strcpy(grade, "A-");
        cgpa = 3.7;
    }
    else if (a_mark >= 87 && a_mark <= 89)
    {
        strcpy(grade, "B+");
        cgpa = 3.3;
    }
    else if (a_mark >= 83 && a_mark <= 86)
    {
        strcpy(grade, "B");
        cgpa = 3.0;
    }
    else if (a_mark >= 80 && a_mark <= 82)
    {
        strcpy(grade, "B-");
        cgpa = 2.7;
    }
    else if (a_mark >= 77 && a_mark <= 79)
    {
        strcpy(grade, "C");
        cgpa = 2.3;
    }
    else if (a_mark >= 73 && a_mark <= 76)
    {
        strcpy(grade, "D");
        cgpa = 2.0;
    }
    else if (a_mark >= 70 && a_mark <= 72)
    {
        strcpy(grade, "E");
        cgpa = 1.7;
    }
    else
    {
        strcpy(grade, "F");
        cgpa = 0.0;
    }

    printf("Average Mark: %d\n", a_mark);
    printf("Grade: %s\n", grade);
    printf("CGPA: %.2lf\n", cgpa);
}

void info() // Contact info
{
    printf("\nAddress: Bashundhara, Dhaka-1229, Bangladesh\n");
    printf("Contact Number: +880-2-55668200\n");
    printf("Mail Address: registrar@northsouth.edu\n\n");

    printf("Available Sunday to Thursday: 09:00 a.m. to 05:00 p.m.\n\n");
}
