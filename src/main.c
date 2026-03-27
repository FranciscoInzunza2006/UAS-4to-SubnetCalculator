#include <stdio.h>
#include <inttypes.h>
#include "raylib.h"

typedef uint_fast8_t byte;

// Pedir clase
// Pedir IP
// Conseguir subredes, hosts*subred
// Calcular el otro basado en los resultados

int main() {
    char buf[1024];
    printf("ID de red: ");
    scanf("%s", buf);

    char class;
    printf("Clase de red (A/B/C) : ");
    scanf(" %c", &class);

    int subnet_amount;
    int hosts_by_subnet;

    char a;
    printf("Subredes (A) o Hosts x Subred (B) : ");
    scanf(" %c", &a);

    if (a != 'A' && a != 'B') {
        printf("Invalid value");
        return 1;
    }

    int *p = a == 'A' ? &subnet_amount : &hosts_by_subnet;
    printf(a == 'A' ? "Subredes: " : "Hosts x Subred: ");
    scanf("%d", p);

    int x = *p;
    int bits_needed = 0;
    while (x > 0) {
        x /= 2;
        bits_needed++;
    }
    subnet_amount = 1 << bits_needed;

    // Check if not enough bits
    int bits_available = 0;
    switch (class) {
        case 'A': bits_available = 24;
            break;
        case 'B': bits_available = 16;
            break;
        case 'C': bits_available = 8;
            break;
    }

    int *pp = a == 'A' ? &hosts_by_subnet : &subnet_amount;
    *pp = 1 << (bits_available - bits_needed);
    //for (int i = 0; i < bits_needed; i++) {*pp *= 2;}


    printf(
        "IP: %s\n"
        "# de subredes: %d\n"
        "# de Hosts x Subred: %d\n",
        buf, subnet_amount, hosts_by_subnet);


    int octets;
    const char** str = TextSplit(buf, '.', &octets);
    if (octets != 4) {
        printf("Invalid IP");
        return 1;
    }

    for (int i = 0; i < subnet_amount; i++) {
        switch (class) {
            case 'A':
                break;
            case 'B':
                break;
            case 'C':
                printf("%d. %s.%s.%s.%d - %s.%s.%s.%d\n", i+1, str[0], str[1], str[2], i * hosts_by_subnet,
                     str[0], str[1], str[2], (i+1) * hosts_by_subnet-1);
                break;
        }
    }


    return 0;
}
