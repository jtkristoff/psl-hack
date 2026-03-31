#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "psl-hack.h"

#define MAX_TAGS 16
#define MAX_TAG_LENGTH 16

char tags[MAX_TAGS][MAX_TAG_LENGTH];
int tag_counts[MAX_TAGS];
int num_of_tags = 0;

int main() {
    int line_num = 1;

    FILE *psl_fp = fopen("psl.csv", "r");
    if (psl_fp == NULL) {
        fprintf(stderr, "psl.csv file open error\n");
        exit(EXIT_FAILURE);
    }

    char *line_str = NULL;
    size_t capacity = 0;
    ssize_t length;

    while ((length = getline(&line_str, &capacity, psl_fp)) != -1) {
        if (line_str[0] == '\n')
            continue;
        if (line_str[0] == '/' && line_str[1] == '/')
            continue;
        if (line_str[0] == '#')
            continue;
        if (isspace(line_str[0]))
           continue;
        if (is_blank_line(line_str))
            continue;

        line_str[strcspn(line_str, "\n")] = '\0';

        char *token = strtok(line_str, ",");
        if (token == NULL) {
            fprintf(stderr, "Missing suffix on line %d\n", line_num);
            continue;
        }
        char *suffix = strdup(token);
        lower_string(suffix);

        token = strtok(NULL, ",");
        if (token == NULL) {
            fprintf(stderr, "Missing tag on line %d\n", line_num);
            continue;
        }
        char *tag = strdup(token);
        lower_string(tag);

        int tag_index = find_or_add_tag(tag);
        tag_counts[tag_index]++;
 
        printf("%d: %s,%s\n", line_num, suffix, tag);

        free(suffix);
        free(tag);
        line_num++;
    }

    for (int i = 0; i < num_of_tags; i++)
        printf("%s: %d\n", tags[i], tag_counts[i]);

    exit(EXIT_SUCCESS);
}

void lower_string(char *string) {
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        string[i] = (char) tolower(string[i]);
    }
    return;
}

int is_blank_line(char *string) {
    while (*string) {
        if (!isspace((unsigned char)*string))
            return 0;
        string++;
    }
    return 1;
}

int find_or_add_tag(char *string) {
    for (int i = 0; i < num_of_tags; i++)
        if (strcmp(tags[i], string) == 0)
            return i;

    strncpy(tags[num_of_tags], string, MAX_TAG_LENGTH - 1);
    tag_counts[num_of_tags] = 0;
    return num_of_tags++;
}
