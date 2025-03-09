#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *number;
    char *word;
} numb_translate;

int open_file(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        write(1, "Dict Error\n", 11);
    return fd;
}

char *read_file(int fd) {
    char *text = malloc(4096);
    if (!text) {
        write(1, "Dict Error\n", 11);
        return NULL;
    }
    ssize_t bytes_read = read(fd, text, 4095);
    if (bytes_read == -1) {
        free(text);
        write(1, "Dict Error\n", 11);
        return NULL;
    }
    text[bytes_read] = '\0';
    return text;
}

int fakeAtoi(char *str) {
    int result = 0, signo = 1, i = 0, has_digit = 0;

    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n') i++;

    if (str[i] == '-' || str[i] == '+') {
        signo = (str[i] == '-') ? -1 : 1;
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
        has_digit = 1;
    }

    if (!has_digit) return -1;
    return result * signo;
}

int ft_SplitInput(int args, char *argsv[]) {
    if (args < 2 || !argsv[1]) {
        write(1, "Error\n", 6);
        return -1;
    }
    return fakeAtoi(argsv[1]);
}

numb_translate *ft_splitDictionary(char *dict_content) {
    if (!dict_content) {
        write(1, "Dict Error\n", 11);
        return NULL;
    }

    int count = 0;
    for (int i = 0; dict_content[i] != '\0'; i++)
        if (dict_content[i] == '\n') count++;

    numb_translate *dictionary = malloc(sizeof(numb_translate) * (count + 1));
    if (!dictionary) {
        write(1, "Memory Error\n", 13);
        return NULL;
    }

    int i = 0, j = 0;
    while (dict_content[i] != '\0' && j < count) {
        while (dict_content[i] == ' ' || dict_content[i] == '\n') i++;

        int start = i;
        while (dict_content[i] >= '0' && dict_content[i] <= '9') i++;
        int num_length = i - start;
        dictionary[j].number = strndup(dict_content + start, num_length);

        while (dict_content[i] == ' ' || dict_content[i] == ':') i++;

        start = i;
        while (dict_content[i] != '\n' && dict_content[i] != '\0') i++;
        int word_length = i - start;
        dictionary[j].word = strndup(dict_content + start, word_length);

        if (dict_content[i] == '\n') i++;
        j++;
    }
    
    dictionary[j].number = NULL; 
    return dictionary;
}

void ft_transform(int value, numb_translate *dict) {
    if (value == 0) {
        printf("zero\n");
        return;
    }

    char result[100] = "";
    
    // Manejo de números grandes
    if (value >= 1000) {
        int miles = value / 1000;
        value %= 1000;
        for (int i = 0; dict[i].number != NULL; i++) {
            if (fake_atoi(dict[i].number) == miles) {
                strcat(result, dict[i].word);
                strcat(result, " thousand ");
                break;
            }
        }
    }

    // Centenas
    if (value >= 100) {
        int centenas = value / 100;
        value %= 100;
        for (int i = 0; dict[i].number != NULL; i++) {
            if (fake_atoi(dict[i].number) == centenas) {
                strcat(result, dict[i].word);
                strcat(result, " hundred ");
                break;
            }
        }
    }

    // Decenas y unidades
    if (value > 0) {
        for (int i = 0; dict[i].number != NULL; i++) {
            if (fake_atoi(dict[i].number) == value) {
                strcat(result, dict[i].word);
                break;
            }
        }
    }

    printf("%s\n", result);
}


int main(int args, char *argsv[]) {
    int value = ft_SplitInput(args, argsv);
    if (value == -1)
        return 1;

    int fd = open_file("*.dict");
    if (fd == -1)
        return 1;

    char *dict_content = read_file(fd);
    if (!dict_content)
        return 1;

    numb_translate *dict = ft_splitDictionary(dict_content);
    if (!dict)
        return 1;

    ft_transform(value, dict);

    free(dict_content);
    for (int i = 0; dict[i].number != NULL; i++) {
        free(dict[i].number);
        free(dict[i].word);
    }
    free(dict);
    close(fd);
    return 0;
}
