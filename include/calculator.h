//
// Created by el_paco on 07/04/26.
//

#ifndef SUBNETCALCULATOR_CALCULATOR_H
#define SUBNETCALCULATOR_CALCULATOR_H

const char **parseIp(const char* ip, bool* result);

int ipClassToBits(char ip_class);

int BitsNeededToRepresentNumber(int target);

const char* getMask(int hosts_by_subnet);

void writeRangesToConsole(const char** ip_parts, char ip_class, int subnet_amount, int hosts_by_subnet);

void writeRangesToFile(const char** ip_parts, char ip_class, int subnet_amount, int hosts_by_subnet);

#endif //SUBNETCALCULATOR_CALCULATOR_H
