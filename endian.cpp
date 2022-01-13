#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t bytes = 0x12345678;
static uint8_t big[4] = {0x12, 0x34, 0x56, 0x78};
static uint8_t little[4] = {0x78, 0x56, 0x34, 0x12};

int bk_strcmp(const char *str1, const char* str2)
{
    while (*str1 && *str1 == *str2) {str1++; str2++;}

    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int main(int /*argc*/, char** /*argv*/)
{
    printf("dupa\n");

    if (((uint8_t*)(&bytes))[0] == little[0])
    {
        printf("Little: %xd: %xd\n", bytes, little[0]);
    }
    else if (((uint8_t*)(&bytes))[0] == big[0])
    {
        printf("Big: %xd: %xd\n", bytes, big[0]);
    }
    else
    {
        printf("Mixed: %xd: big: %xd little: %xd\n", bytes, *(uint32_t*)big, *(uint32_t*)little);
    }

    if (bytes == *(uint32_t*)little)
    {
        printf("Little: %xd: %xd\n", bytes, *(uint32_t*)little);
    }
    else if (bytes == *(uint32_t*)big)
    {
        printf("Big: %xd: %xd\n", bytes, *(uint32_t*)big);
    }
    else
    {
        printf("Mixed: %xd: big: %xd little: %xd\n", bytes, *(uint32_t*)big, *(uint32_t*)little);
    }

    char dupa[]={"dupa"};
    char dupa1[]={"dupa1"};
    char dups[]={"dups"};

    printf("bk: %s - %s, %d\n", dupa, dupa1, bk_strcmp(dupa, dupa1));
    printf("bk: %s - %s, %d\n", dupa1, dupa, bk_strcmp(dupa1, dupa));
    printf("bk: %s - %s, %d\n", dupa, dupa, bk_strcmp(dupa, dupa));
    printf("bk: %s - %s, %d\n", dupa, dups, bk_strcmp(dupa, dups));
    printf("bk: %s - %s, %d\n", dups, dupa, bk_strcmp(dups, dupa));

    printf("%s - %s, %d\n", dupa, dupa1, strcmp(dupa, dupa1));
    printf("%s - %s, %d\n", dupa1, dupa, strcmp(dupa1, dupa));
    printf("%s - %s, %d\n", dupa, dupa, strcmp(dupa, dupa));
    printf("%s - %s, %d\n", dupa, dups, strcmp(dupa, dups));
    printf("%s - %s, %d\n", dups, dupa, strcmp(dups, dupa));

    return 0;
}

