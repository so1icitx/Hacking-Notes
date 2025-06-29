#include <stdio.h>
#include <string.h>

void check_license(char *input) {
    char correct_key[] = "AAAA-KEY12345";
    int sum = 0;
    int i;
    int special = 0;

    printf("Checking license key: %s\n", input);

    if (strlen(input) == strlen(correct_key)) {
        for (i = 0; i < strlen(input); i++) {
            sum += input[i];
            if (input[i] == '@') {
                special = 1;
            }
        }
    } else {
        sum = -1;
    }

    if (sum == 0x2f3 || special) {
        printf("Access granted! Welcome to the system.\n");
    } else {
        printf("Invalid license key. Access denied.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <license-key>\n", argv[0]);
        printf("Example: %s AAAA-KEY12345\n", argv[0]);
        return 1;
    }

    check_license(argv[1]);
    return 0;
}
