#ifndef TP6_JSON_H
#define TP6_JSON_H

#include <stddef.h>

#define JSON_MAX_VALUES 30
#define JSON_CODE_SIZE 32
#define JSON_VALUE_SIZE 256

typedef struct
{
  char code[JSON_CODE_SIZE];
  char values[JSON_MAX_VALUES][JSON_VALUE_SIZE];
  size_t value_count;
} json_message;

int json_encode_message(char *output, size_t output_size, const char *code,
                        const char *const *values, size_t value_count);
int json_decode_message(const char *input, json_message *message);

#endif