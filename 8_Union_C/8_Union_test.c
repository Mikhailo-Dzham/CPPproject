#include <stdio.h>
#include <stdlib.h>
#include "8_Union.h"

// --- Helper Functions for Menu ---

void demo_task_1() {
    printf("\n=== Task 1: Points ===\n");
    Point2D p1, p2;
    p1.type = COORD_CARTESIAN; p1.data.cart.x = 0; p1.data.cart.y = 0;
    p2.type = COORD_CARTESIAN; p2.data.cart.x = 3; p2.data.cart.y = 4;

    printf("Saving points to 'points.txt'...\n");
    FILE *f = fopen("points.txt", "w");
    if(f) {
        save_point2d(f, p1);
        save_point2d(f, p2);
        fclose(f);
    }

    printf("Loading points back...\n");
    f = fopen("points.txt", "r");
    if(f) {
        Point2D lp1, lp2;
        load_point2d(f, &lp1);
        load_point2d(f, &lp2);
        fclose(f);

        double dist = calculate_segment_length_C(lp1, lp2);
        printf("Loaded Dist: %.2f (Expected 5.00)\n", dist);
    }
}

void demo_task_2() {
    printf("\n=== Task 2: Money ===\n");
    Money m;
    m.type = MONEY_FULL; m.data.full.grn = 10; m.data.full.kop = 50;

    FILE *f = fopen("money.txt", "w");
    if(f) { save_money(f, m); fclose(f); }

    f = fopen("money.txt", "r");
    if(f) {
        Money lm;
        load_money(f, &lm);
        fclose(f);
        printf("Loaded Money: "); print_money_C(lm);
    }
}

void demo_task_5() {
    printf("\n=== Task 5: Shapes ===\n");
    Shape s;
    s.type = SHAPE_CIRCLE; s.data.circle_radius = 10.0;

    FILE *f = fopen("shape.txt", "w");
    if(f) { save_shape(f, s); fclose(f); }

    f = fopen("shape.txt", "r");
    if(f) {
        Shape ls;
        load_shape(f, &ls);
        fclose(f);
        printf("Loaded Circle Area: %.2f\n", calculate_area_C(ls));
    }
}

int main() {
    int choice;
    do {
        printf("\n=== C UNION FILE I/O DEMO ===\n");
        printf("1. Test Points I/O\n");
        printf("2. Test Money I/O\n");
        printf("5. Test Shapes I/O\n");
        printf("0. Exit\n");
        printf("Select: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: demo_task_1(); break;
            case 2: demo_task_2(); break;
            case 5: demo_task_5(); break;
            case 0: break;
            default: printf("Demo for this task not implemented yet.\n");
        }
    } while (choice != 0);
    return 0;
}