#pragma once

#include <Arduino.h>

#include <SD.h>

extern SPIClass &sdSPI;

void sdSPI_init();
void sdSPI_test();
bool sdSPI_write_csv(const char *path, const char *data, const char *csv_headers);
bool sdSPI_read_csv(const char *path);
bool sdSPI_file_exists(const char *path);
