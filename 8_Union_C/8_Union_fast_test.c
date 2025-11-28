#include <stdio.h>
#include <stdlib.h>
#include "8_Union.h"

// Structure to describe a single test case
typedef struct {
    int task_id;
    const char* input_file;
    const char* output_file;
    const char* description;
} TestCase;

void run_test_case(TestCase tc) {
    printf("[RUNNING] %s (Task %d)...\n", tc.description, tc.task_id);

    FILE *in = fopen(tc.input_file, "r");
    if (!in) {
        printf("  [ERROR] Cannot open input file: %s\n", tc.input_file);
        return;
    }

    FILE *out = fopen(tc.output_file, "w");
    if (!out) {
        printf("  [ERROR] Cannot open output file: %s\n", tc.output_file);
        fclose(in);
        return;
    }

    // Processing logic depending on the task
    switch(tc.task_id) {
        case 1: { // Points
            Point2D p1, p2;
            if (load_point2d(in, &p1) && load_point2d(in, &p2)) {
                double dist = calculate_segment_length_C(p1, p2);
                fprintf(out, "Input: %s\nResult Distance: %.4f\n", tc.input_file, dist);
                printf("  [OK] Result written to %s\n", tc.output_file);
            } else printf("  [FAIL] Parsing error\n");
            break;
        }
        case 2: { // Money
            Money m;
            if (load_money(in, &m)) {
                fprintf(out, "Loaded Money Type: %d\n", m.type);
                if(m.type == MONEY_FULL) fprintf(out, "%d UAH %d kop\n", m.data.full.grn, m.data.full.kop);
                else fprintf(out, "%d kop total\n", m.data.only_kop);
                printf("  [OK] Result written to %s\n", tc.output_file);
            } else printf("  [FAIL] Parsing error\n");
            break;
        }
        case 3: { // Vectors
            Vector v1, v2, v3;
            if (load_vector(in, &v1) && load_vector(in, &v2) && load_vector(in, &v3)) {
                int res = are_collinear_C(v1, v2, v3);
                fprintf(out, "Vectors are: %s\n", res ? "COLLINEAR" : "NOT COLLINEAR");
                printf("  [OK] Result written to %s\n", tc.output_file);
            } else printf("  [FAIL] Parsing error (need 3 vectors)\n");
            break;
        }
        case 4: { // 3D
            Point3D p1, p2;
            if (load_point3d(in, &p1) && load_point3d(in, &p2)) {
                double dist = calculate_distance_3d_C(p1, p2);
                fprintf(out, "3D Distance: %.4f\n", dist);
                printf("  [OK] Result written to %s\n", tc.output_file);
            } else printf("  [FAIL] Parsing error\n");
            break;
        }
        case 5: { // Shape
            Shape s;
            if (load_shape(in, &s)) {
                double area = calculate_area_C(s);
                double perim = calculate_perimeter_C(s);
                fprintf(out, "Shape Type: %d\nArea: %.4f\nPerimeter: %.4f\n", s.type, area, perim);
                printf("  [OK] Result written to %s\n", tc.output_file);
            } else printf("  [FAIL] Parsing error\n");
            break;
        }
        case 6: { // Numbers
            AnyNumber n1, n2;
            if (load_number(in, &n1) && load_number(in, &n2)) {
                AnyNumber res = divide_numbers_C(n1, n2);
                fprintf(out, "Division Result: ");
                if(res.type == NUM_STRING_INF) fprintf(out, "INFINITY\n");
                else if(res.type == NUM_STRING_NAN) fprintf(out, "NaN\n");
                else fprintf(out, "%.4f\n", res.data.d_val);
                printf("  [OK] Result written to %s\n", tc.output_file);
            } else printf("  [FAIL] Parsing error\n");
            break;
        }
    }

    fclose(in);
    fclose(out);
}

int main() {
    // List of all tests to run
    TestCase tests[] = {
        {1, "test_t1_mixed.txt", "res_t1.txt", "Task 1: Mixed Coordinates"},
        {2, "test_t2_full.txt",  "res_t2a.txt", "Task 2: Full Money"},
        {2, "test_t2_kop.txt",   "res_t2b.txt", "Task 2: Kopecks Only"},
        {3, "test_t3_collinear.txt", "res_t3.txt", "Task 3: Collinear Vectors"},
        {4, "test_t4_sphere.txt", "res_t4.txt", "Task 4: Spherical 3D"},
        {5, "test_t5_all.txt",    "res_t5.txt", "Task 5: Trapezoid Area"},
        {6, "test_t6_int_dbl.txt","res_t6a.txt", "Task 6: Int / Double"},
        {6, "test_t6_zero.txt",   "res_t6b.txt", "Task 6: Division by Zero"}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("=== STARTING FAST BATCH TESTING (C) ===\n");
    for(int i=0; i<num_tests; i++) {
        run_test_case(tests[i]);
    }
    printf("=== ALL TESTS FINISHED ===\n");
    return 0;
}