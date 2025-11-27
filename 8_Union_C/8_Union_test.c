#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "8_Union.h"

// --- Helper Functions ---
// Отримує потік виводу (екран або файл)
FILE* get_output_stream(char *filename_buffer) {
    int choice;
    printf("\n[OUTPUT] Select destination:\n1. Console\n2. File\n> ");
    scanf("%d", &choice);
    if (choice == 2) {
        printf("Enter output filename: ");
        scanf("%s", filename_buffer);
        FILE *f = fopen(filename_buffer, "w");
        if (!f) { printf("Error opening file! Using Console.\n"); return stdout; }
        return f;
    }
    return stdout;
}

// Функція для відкриття файлу на читання (якщо обрано)
FILE* get_input_file() {
    char filename[100];
    printf("Enter input filename: ");
    scanf("%s", filename);
    FILE *f = fopen(filename, "r");
    if (!f) printf("Error opening file!\n");
    return f;
}

// --- TASK RUNNERS ---

void run_task_1() {
    printf("\n--- Task 1: Points Distance ---\n");
    Point2D p1, p2;
    int choice;
    printf("[INPUT] 1. Console  2. File\n> ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Point 1:\n"); p1 = input_point2d_console();
        printf("Point 2:\n"); p2 = input_point2d_console();
    } else {
        FILE *f = get_input_file();
        if (!f) return;
        if (!load_point2d(f, &p1) || !load_point2d(f, &p2)) {
            printf("Error reading points from file.\n"); fclose(f); return;
        }
        fclose(f);
    }

    double dist = calculate_segment_length_C(p1, p2);

    char fname[100];
    FILE *out = get_output_stream(fname);
    fprintf(out, "Task 1 Result: Distance = %.4f\n", dist);
    if (out != stdout) { fclose(out); printf("Saved to %s\n", fname); }
}

void run_task_2() {
    printf("\n--- Task 2: Money Display ---\n");
    Money m;
    int choice;
    printf("[INPUT] 1. Console  2. File\n> ");
    scanf("%d", &choice);

    if (choice == 1) m = input_money_console();
    else {
        FILE *f = get_input_file();
        if (!f) return;
        load_money(f, &m);
        fclose(f);
    }

    // Для грошей у нас вивід специфічний, просто покажемо на екран
    // Або запишемо в файл
    char fname[100];
    FILE *out = get_output_stream(fname);
    if (out == stdout) {
        print_money_C(m);
    } else {
        fprintf(out, "Money Object Dump: Type=%d ", m.type);
        if(m.type==MONEY_FULL) fprintf(out, "%d UAH %d kop\n", m.data.full.grn, m.data.full.kop);
        else fprintf(out, "%d total kop\n", m.data.only_kop);
        fclose(out);
        printf("Saved to %s\n", fname);
    }
}

void run_task_3() {
    printf("\n--- Task 3: Collinear Vectors ---\n");
    Vector v1, v2, v3;
    int choice;
    printf("[INPUT] 1. Console  2. File\n> ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Vector 1:\n"); v1 = input_vector_console();
        printf("Vector 2:\n"); v2 = input_vector_console();
        printf("Vector 3:\n"); v3 = input_vector_console();
    } else {
        FILE *f = get_input_file();
        if (!f) return;
        if (!load_vector(f, &v1) || !load_vector(f, &v2) || !load_vector(f, &v3)) {
             printf("Error reading vectors.\n"); fclose(f); return;
        }
        fclose(f);
    }

    int res = are_collinear_C(v1, v2, v3);

    char fname[100];
    FILE *out = get_output_stream(fname);
    fprintf(out, "Task 3 Result: Vectors are %s\n", res ? "COLLINEAR" : "NOT COLLINEAR");
    if (out != stdout) { fclose(out); printf("Saved to %s\n", fname); }
}

void run_task_4() {
    printf("\n--- Task 4: 3D Distance ---\n");
    Point3D p1, p2;
    int choice;
    printf("[INPUT] 1. Console  2. File\n> ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Point 1:\n"); p1 = input_point3d_console();
        printf("Point 2:\n"); p2 = input_point3d_console();
    } else {
        FILE *f = get_input_file();
        if (!f) return;
        load_point3d(f, &p1); load_point3d(f, &p2);
        fclose(f);
    }

    double dist = calculate_distance_3d_C(p1, p2);

    char fname[100];
    FILE *out = get_output_stream(fname);
    fprintf(out, "Task 4 Result: 3D Distance = %.4f\n", dist);
    if (out != stdout) { fclose(out); printf("Saved to %s\n", fname); }
}

void run_task_5() {
    printf("\n--- Task 5: Shape Properties ---\n");
    Shape s;
    int choice;
    printf("[INPUT] 1. Console  2. File\n> ");
    scanf("%d", &choice);

    if (choice == 1) s = input_shape_console();
    else {
        FILE *f = get_input_file();
        if (!f) return;
        load_shape(f, &s);
        fclose(f);
    }

    double P = calculate_perimeter_C(s);
    double A = calculate_area_C(s);

    char fname[100];
    FILE *out = get_output_stream(fname);
    fprintf(out, "Task 5 Result:\nPerimeter: %.4f\nArea: %.4f\n", P, A);
    if (out != stdout) { fclose(out); printf("Saved to %s\n", fname); }
}

void run_task_6() {
    printf("\n--- Task 6: Division ---\n");
    AnyNumber n1, n2;
    int choice;
    printf("[INPUT] 1. Console  2. File\n> ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Numerator:\n"); n1 = input_number_console();
        printf("Denominator:\n"); n2 = input_number_console();
    } else {
        FILE *f = get_input_file();
        if (!f) return;
        load_number(f, &n1); load_number(f, &n2);
        fclose(f);
    }

    AnyNumber res = divide_numbers_C(n1, n2);

    char fname[100];
    FILE *out = get_output_stream(fname);
    fprintf(out, "Task 6 Result: ");
    if (res.type == NUM_STRING_INF) fprintf(out, "INFINITY\n");
    else if (res.type == NUM_STRING_NAN) fprintf(out, "NaN (Error)\n");
    else fprintf(out, "%.4f\n", res.data.d_val);

    if (out != stdout) { fclose(out); printf("Saved to %s\n", fname); }
}

// --- MAIN MENU ---

int main() {
    int choice;
    do {
        printf("\n============================\n");
        printf("      PROJECT 8: UNION      \n");
        printf("============================\n");
        printf("1. Task 1 (Points 2D)\n");
        printf("2. Task 2 (Money)\n");
        printf("3. Task 3 (Vectors)\n");
        printf("4. Task 4 (Points 3D)\n");
        printf("5. Task 5 (Shapes)\n");
        printf("6. Task 6 (Numbers)\n");
        printf("0. Exit\n");
        printf("Select Task: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: run_task_1(); break;
            case 2: run_task_2(); break;
            case 3: run_task_3(); break;
            case 4: run_task_4(); break;
            case 5: run_task_5(); break;
            case 6: run_task_6(); break;
            case 0: printf("Goodbye.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
    return 0;
}