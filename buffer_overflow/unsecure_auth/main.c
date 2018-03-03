#include <stdio.h>
#include <string.h>

char *pass = "security";

struct bad_auth {
    char buf[256];
    unsigned int valid;
};

int main(int argc, char *argv[]) {
    struct bad_auth password;
    password.valid = 0;
    printf("Password: ");
    scanf("%s", password.buf);
    if (strcmp(password.buf, pass) == 0) {
        password.valid = 1;
    }
    if (password.valid) {
        printf("Access Granted\n");
    }
    else {
        printf("Access Denied\n");
    }
}

