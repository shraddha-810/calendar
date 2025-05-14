#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define YEAR 2025

const char* day_names[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

int month_name_to_number(const char *month) {
    char m[10];
    strncpy(m, month, 9);
    for (int i = 0; m[i]; i++) m[i] = tolower(m[i]);

    if (strcmp(m, "january") == 0) return 1;
    if (strcmp(m, "february") == 0) return 2;
    if (strcmp(m, "march") == 0) return 3;
    if (strcmp(m, "april") == 0) return 4;
    if (strcmp(m, "may") == 0) return 5;
    if (strcmp(m, "june") == 0) return 6;
    if (strcmp(m, "july") == 0) return 7;
    if (strcmp(m, "august") == 0) return 8;
    if (strcmp(m, "september") == 0) return 9;
    if (strcmp(m, "october") == 0) return 10;
    if (strcmp(m, "november") == 0) return 11;
    if (strcmp(m, "december") == 0) return 12;

    return atoi(month); // fallback
}

const char* check_fixed_holiday(int day, int month) {
    if (day == 26 && month == 1) return "Republic Day";
    if (day == 14 && month == 4) return "Ambedkar Jayanti";
    if (day == 1 && month == 5) return "Maharashtra Day";
    if (day == 15 && month == 8) return "Independence Day";
    return NULL;
}

int is_vacation(int day, int month) {
    return (month == 5 && day >= 15) ||
           (month == 6 && day <= 15) ||
           (month == 10 && day >= 15) ||
           (month == 11 && day <= 15);
}

int is_exam_day(int day, int month) {
    return (month == 6 && day >= 2 && day <= 15);
}

int is_pl_day(int day, int month) {
    return (month == 5 && day >= 16 && day <= 30);
}

int is_third_saturday(int day, int month) {
    struct tm t = {0};
    t.tm_year = YEAR - 1900;
    t.tm_mon = month - 1;

    int count = 0;
    for (int d = 1; d <= 31; d++) {
        t.tm_mday = d;
        if (mktime(&t) == -1 || t.tm_mon != month - 1) break;
        if (t.tm_wday == 6) {
            count++;
            if (count == 3 && d == day) return 1;
        }
    }
    return 0;
}

int main() {
    char input[100];
    int day, month;

    while (1) {
        printf("Enter date: ");
        fgets(input, sizeof(input), stdin);

        if (strncmp(input, "0 0", 3) == 0) break;

        char *token1 = strtok(input, " /-\n");
        char *token2 = strtok(NULL, " /-\n");

        if (!token1 || !token2) {
            printf("Invalid\n");
            continue;
        }

        day = atoi(token1);
        month = month_name_to_number(token2);

        if (day < 1 || day > 31 || month < 1 || month > 12) {
            printf("Invalid\n");
            continue;
        }

        struct tm date = {0};
        date.tm_year = YEAR - 1900;
        date.tm_mon = month - 1;
        date.tm_mday = day;

        if (mktime(&date) == -1 || date.tm_mon != month - 1) {
            printf("Invalid\n");
            continue;
        }

        printf("%02d-%02d-%d (%s): ", day, month, YEAR, day_names[date.tm_wday]);

        const char* holiday = check_fixed_holiday(day, month);
        if (holiday) {
            printf("%s\n", holiday);
        } else if (date.tm_wday == 0) {
            printf("Sunday Holiday\n");
        } else if (is_third_saturday(day, month)) {
            printf("3rd Saturday Holiday\n");
        } else if (is_vacation(day, month)) {
            printf("Vacation\n");
        } else if (is_exam_day(day, month)) {
            printf("ESE Exam\n");
        } else if (is_pl_day(day, month)) {
            printf("Preparation Leave (PL)\n");
        } else {
            printf("Working Day\n");
        }
    }

    return 0;
}