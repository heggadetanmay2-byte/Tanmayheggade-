#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 100

char grade_from_avg(double avg) {
    if (avg >= 90.0) return 'A';
    if (avg >= 80.0) return 'B';
    if (avg >= 70.0) return 'C';
    if (avg >= 60.0) return 'D';
    return 'F';
}

int main(void) {
    int n_students, n_subjects;

    printf("Student Marks Report\n");
    printf("----------------------\n");

    printf("Enter number of students: ");
    if (scanf("%d", &n_students) != 1 || n_students <= 0) {
        fprintf(stderr, "Invalid number of students.\n");
        return 1;
    }

    printf("Enter number of subjects: ");
    if (scanf("%d", &n_subjects) != 1 || n_subjects <= 0) {
        fprintf(stderr, "Invalid number of subjects.\n");
        return 1;
    }

    /* consume newline left by scanf before using fgets */
    int c; while ((c = getchar()) != '\n' && c != EOF) {}

    char (*names)[NAME_LEN] = malloc(sizeof(*names) * n_students);
    if (!names) { perror("malloc"); return 1; }

    double *totals = calloc(n_students, sizeof(double));
    if (!totals) { perror("calloc"); free(names); return 1; }

    double *marks = malloc(sizeof(double) * n_subjects);
    if (!marks) { perror("malloc"); free(names); free(totals); return 1; }

    for (int i = 0; i < n_students; ++i) {
        printf("\nStudent %d name: ", i + 1);
        if (!fgets(names[i], NAME_LEN, stdin)) {
            fprintf(stderr, "Failed to read name.\n");
            free(names); free(totals); free(marks); return 1;
        }
        /* Remove trailing newline */
        names[i][strcspn(names[i], "\n")] = '\0';

        double total = 0.0;
        for (int s = 0; s < n_subjects; ++s) {
            double m;
            while (1) {
                printf("  Enter marks for subject %d (0-100): ", s + 1);
                if (scanf("%lf", &m) != 1) {
                    fprintf(stderr, " Invalid input. Please enter a number.\n");
                    /* clear invalid input */
                    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                    continue;
                }
                if (m < 0.0 || m > 100.0) {
                    printf("  Marks must be between 0 and 100. Try again.\n");
                    continue;
                }
                break;
            }
            marks[s] = m;
            total += m;
        }
        /* consume newline before next fgets */
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}

        totals[i] = total;
    }

    printf("\n\nStudent Marks Report\n");
    printf("======================\n");
    printf("%-4s %-30s ", "No.", "Name");
    for (int s = 0; s < n_subjects; ++s) printf("Sub%-6d", s + 1);
    printf(" Total   Avg   Grade\n");
    printf("-------------------------------------------------------------------------------\n");

    double class_total_sum = 0.0;
    for (int i = 0; i < n_students; ++i) {
        double total = totals[i];
        double avg = total / n_subjects;
        char grade = grade_from_avg(avg);

        printf("%-4d %-30s ", i + 1, names[i]);
        /* Reconstructing per-subject marks isn't stored per student in this simplified version. */
        /* If per-subject marks display is required, the program can be extended to store a marks matrix. */
        for (int s = 0; s < n_subjects; ++s) printf("%-7s", "-");
        printf(" %-7.2f %-5.2f   %c\n", total, avg, grade);
        class_total_sum += total;
    }

    double class_avg = class_total_sum / (n_students * n_subjects);
    printf("\nClass average (per subject): %.2f\n", class_avg);

    free(names);
    free(totals);
    free(marks);

    return 0;
}
