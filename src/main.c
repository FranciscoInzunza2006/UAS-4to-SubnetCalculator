#include <stdio.h>
#include <inttypes.h>
#include "raylib.h"

const char *getIp() {
    static char buf[17];
    printf("IP Address: ");
    scanf("%s", buf);
    buf[16] = '\0';

    return buf;
}

const char **parseIp(const char* ip, bool* result) {
    int octets;
    const char** str = TextSplit(ip, '.', &octets);

    // Validate ip
    if (octets != 4) goto error;

    for (int i = 0; i < 4; i++) {
        const char* octet = str[i];
        const char* c = octet;

        int value = 0;
        while (*c) {
            if (*c < '0' || *c > '9') goto error;
            value = (value * 10) + (*c - '0');
            c++;
        }

        if (value < 0 || value > 255) goto error;
    }

    *result = true;
    return str;
error:
    *result = false;
    return nullptr;
}

int main() {
    const char* ip = getIp();

    bool result;
    const char** ip_parts = parseIp(ip, &result);
    if (!result) {
        printf("Invalid IP");
        return 1;
    }

    char ip_class;
    printf("Clase de red (A/B/C) : ");
    scanf(" %c", &ip_class);

    int bits_available;
    switch (ip_class) {
        case 'A': bits_available = 24;
            break;
        case 'B': bits_available = 16;
            break;
        case 'C': bits_available = 8;
            break;
        default:
            printf("Invalid IṔ class");
            return 1;
            break;
    }

    char value_to_input;
    printf("Subredes (A) o Hosts x Subred (B) : ");
    scanf(" %c", &value_to_input);

    if (value_to_input != 'A' && value_to_input != 'B') {
        printf("Invalid value");
        return 1;
    }

    int subnet_amount;
    int hosts_by_subnet;
    int* target_value = value_to_input == 'A' ? &subnet_amount : &hosts_by_subnet;
    printf(value_to_input == 'A' ? "Subredes: " : "Hosts x Subred: ");
    scanf("%d", target_value);

    int x = *target_value;
    int bits_needed = 0;
    while (x > 0) {
        x /= 2;
        bits_needed++;
    }
    *target_value = 1 << bits_needed;

    int* value_to_calculate = value_to_input == 'A' ? &hosts_by_subnet : &subnet_amount;
    *value_to_calculate = 1 << (bits_available - bits_needed);

    printf(
        "IP Address: %s\n"
        "IP Class: %c\n"
        "Total subnets: %d\n"
        "Hosts by subnet: %d\n",
        ip, ip_class, subnet_amount, hosts_by_subnet);


    constexpr int OUT_BUFFER_SIZE = 1024;
    constexpr int OUT_BUFFER_LENGTH = 32;
    char save_buffer[OUT_BUFFER_SIZE * OUT_BUFFER_LENGTH];
    int in_buffer = 0;
    for (int i = 0; i < subnet_amount; i++) {
        const int lower = i * hosts_by_subnet;
        const int upper = (i + 1) * hosts_by_subnet - 1;

        switch (ip_class) {
            case 'A':
                printf("%d. %s.%d.%d.%d - %s.%d.%d.%d\n", i + 1,
                       ip_parts[0], lower / 65536, lower % 65536 / 256, lower % 256,
                       ip_parts[0], upper / 65536, upper % 65536 / 256, upper % 256);
                break;
            case 'B':
                printf("%d. %s.%s.%d.%d - %s.%s.%d.%d\n", i + 1,
                       ip_parts[0], ip_parts[1], lower / 256, lower % 256,
                       ip_parts[0], ip_parts[1], upper / 256, upper % 256);
                break;

            case 'C':
                printf("%d. %s.%s.%s.%d - %s.%s.%s.%d\n", i + 1,
                       ip_parts[0], ip_parts[1], ip_parts[2], lower,
                       ip_parts[0], ip_parts[1], ip_parts[2], upper);
                break;
            default: ;
        }
        // in_buffer++;
        // if (in_buffer >= OUT_BUFFER_LENGTH) {
        //     for
        //     in_buffer = 0;
        // }
    }


    return 0;
}
