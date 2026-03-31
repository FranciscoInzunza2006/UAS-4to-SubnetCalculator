#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

 static const char *getIp() {
    static char buf[17];
    printf("IP Address: ");
    scanf("%s", buf);
    buf[16] = '\0';

    return buf;
}

static const char **parseIp(const char* ip, bool* result) {
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

static int ipClassToBits(const char ip_class) {
    switch (ip_class) {
        case 'A': return 24;
        case 'B': return 16;
        case 'C': return 8;
        default: return 0;
    }
}

static int BitsNeededToRepresentNumber(int target) {
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

static void writeRangesToConsole(const char** ip_parts, const char ip_class, const int subnet_amount,
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

static void writeRangesToFile(const char** ip_parts, const char ip_class, const int subnet_amount, const int hosts_by_subnet) {
    FILE* out_file = fopen("output.txt", "w");
    if (out_file == nullptr) {
        perror("Error opening output file");
        return;
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
            default: exit(EXIT_FAILURE);
        }

        if (result <= 0) {
            perror("Error writing to file");
            return;
        }
    }

    fclose(out_file);
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

    const int bits_available = ipClassToBits(ip_class);
    if (bits_available == 0) return 1;


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

    int bits_needed = BitsNeededToRepresentNumber(*target_value);
    if (bits_needed > bits_available - 2) {
        printf("There isn't any available IP with these settings.");
        return 1;
    }

    *target_value = 1 << bits_needed; // Round up to next power if needed
    int* value_to_calculate = value_to_input == 'A' ? &hosts_by_subnet : &subnet_amount;
    *value_to_calculate = 1 << (bits_available - bits_needed);

    printf(
        "IP Address: %s\n"
        "IP Class: %c\n"
        "Total subnets: %d\n"
        "Hosts by subnet: %d\n",
        ip, ip_class, subnet_amount, hosts_by_subnet);

    writeRangesToConsole(ip_parts, ip_class, subnet_amount, hosts_by_subnet);
    writeRangesToFile(ip_parts, ip_class, subnet_amount, hosts_by_subnet);

    return 0;
}
