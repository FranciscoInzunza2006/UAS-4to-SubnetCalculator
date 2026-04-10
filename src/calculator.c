//
// Created by el_paco on 07/04/26.
//

#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"
#include "raylib.h"

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

int ipClassToBits(const char ip_class) {
    switch (ip_class) {
        case 'A': return 24;
        case 'B': return 16;
        case 'C': return 8;
        default: return 0;
    }
}

int BitsNeededToRepresentNumber(int target) {
    int bits_needed = 0;
    while (target > 0) {
        target /= 2;
        bits_needed++;
    }

    return bits_needed;
}

#define IP_CLASS_A_FORMAT "%d. %s.%d.%d.%d - %s.%d.%d.%d\n", i + 1, \
                            ip_parts[0], lower / 65536, lower % 65536 / 256, lower % 256, \
                            ip_parts[0], upper / 65536, upper % 65536 / 256, upper % 256
#define IP_CLASS_B_FORMAT "%d. %s.%s.%d.%d - %s.%s.%d.%d\n", i + 1, \
                            ip_parts[0], ip_parts[1], lower / 256, lower % 256, \
                            ip_parts[0], ip_parts[1], upper / 256, upper % 256
#define IP_CLASS_C_FORMAT "%d. %s.%s.%s.%d - %s.%s.%s.%d\n", i + 1, \
                            ip_parts[0], ip_parts[1], ip_parts[2], lower, \
                            ip_parts[0], ip_parts[1], ip_parts[2], upper

void writeRangesToConsole(const char** ip_parts, const char ip_class, const int subnet_amount,
                          const int hosts_by_subnet) {
    for (int i = 0; i < subnet_amount; i++) {
        const int lower = i * hosts_by_subnet;
        const int upper = (i + 1) * hosts_by_subnet - 1;
        switch (ip_class) {
            case 'A': printf(IP_CLASS_A_FORMAT);
                break;
            case 'B': printf(IP_CLASS_B_FORMAT);
                break;
            case 'C': printf(IP_CLASS_C_FORMAT);
                break;
            default: exit(EXIT_FAILURE);
        }
    }
}

bool writeRangesToFile(const char** ip_parts, const char ip_class, const int subnet_amount, const int hosts_by_subnet) {
    FILE* out_file = fopen("output.txt", "w");
    if (out_file == nullptr) {
        perror("Error opening output file");
        return false;
    }

    for (int i = 0; i < subnet_amount; i++) {
        const int lower = i * hosts_by_subnet;
        const int upper = (i + 1) * hosts_by_subnet - 1;
        int result;
        switch (ip_class) {
            case 'A': result = fprintf(out_file, IP_CLASS_A_FORMAT);
                break;
            case 'B': result = fprintf(out_file, IP_CLASS_B_FORMAT);
                break;
            case 'C': result = fprintf(out_file, IP_CLASS_C_FORMAT);
                break;
            default: return false;
        }

        if (result <= 0) {
            perror("Error writing to file");
            return false;
        }
    }

    fclose(out_file);
    return true;
}

// Rounded
const char* getMask(const int hosts_by_subnet) {
    unsigned int mask = ~(hosts_by_subnet-1);

    unsigned int a = mask % 256;
    mask /= 256;
    unsigned int b = mask % 256;
    mask /= 256;
    unsigned int c = mask % 256;
    mask /= 256;
    unsigned int d = mask % 256;

    return TextFormat("%d.%d.%d.%d", d, c, b, a);
}

const char* getRange(const char** ip_parts, const char ip_class, const int subnet, const int hosts_by_subnet) {
    const int i = subnet; // Macro trick
    const int lower = i * hosts_by_subnet;
    const int upper = (i + 1) * hosts_by_subnet - 1;

    switch (ip_class) {
        case 'A': return TextFormat(IP_CLASS_A_FORMAT);
        case 'B': return TextFormat(IP_CLASS_B_FORMAT);
        case 'C': return TextFormat(IP_CLASS_C_FORMAT);
        default: return "";
    }
}
