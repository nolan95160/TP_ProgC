#include "json.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int append_char(char *output, size_t output_size, size_t *used, char value)
{
  if (*used + 1 >= output_size)
  {
    return -1;
  }
  output[(*used)++] = value;
  output[*used] = '\0';
  return 0;
}

static int append_json_string(char *output, size_t output_size, size_t *used,
                              const char *value)
{
  if (append_char(output, output_size, used, '"') != 0)
  {
    return -1;
  }
  for (const unsigned char *cursor = (const unsigned char *)value; *cursor != '\0'; cursor++)
  {
    const char *escape = NULL;
    switch (*cursor)
    {
      case '"': escape = "\\\""; break;
      case '\\': escape = "\\\\"; break;
      case '\b': escape = "\\b"; break;
      case '\f': escape = "\\f"; break;
      case '\n': escape = "\\n"; break;
      case '\r': escape = "\\r"; break;
      case '\t': escape = "\\t"; break;
      default: break;
    }
    if (escape != NULL)
    {
      for (size_t index = 0; escape[index] != '\0'; index++)
      {
        if (append_char(output, output_size, used, escape[index]) != 0)
        {
          return -1;
        }
      }
    }
    else if (*cursor < 0x20)
    {
      char escaped[7];
      int length = snprintf(escaped, sizeof(escaped), "\\u%04x", *cursor);
      for (int index = 0; index < length; index++)
      {
        if (append_char(output, output_size, used, escaped[index]) != 0)
        {
          return -1;
        }
      }
    }
    else if (append_char(output, output_size, used, (char)*cursor) != 0)
    {
      return -1;
    }
  }
  return append_char(output, output_size, used, '"');
}

int json_encode_message(char *output, size_t output_size, const char *code,
                        const char *const *values, size_t value_count)
{
  if (output == NULL || output_size == 0 || code == NULL ||
      value_count > JSON_MAX_VALUES || (value_count > 0 && values == NULL))
  {
    return -1;
  }

  size_t used = 0;
  output[0] = '\0';
  if (append_char(output, output_size, &used, '{') != 0 ||
      append_json_string(output, output_size, &used, "code") != 0 ||
      append_char(output, output_size, &used, ':') != 0 ||
      append_json_string(output, output_size, &used, code) != 0 ||
      append_char(output, output_size, &used, ',') != 0 ||
      append_json_string(output, output_size, &used, "valeurs") != 0 ||
      append_char(output, output_size, &used, ':') != 0 ||
      append_char(output, output_size, &used, '[') != 0)
  {
    return -1;
  }

  for (size_t index = 0; index < value_count; index++)
  {
    if (values[index] == NULL ||
        (index > 0 && append_char(output, output_size, &used, ',') != 0) ||
        append_json_string(output, output_size, &used, values[index]) != 0)
    {
      return -1;
    }
  }

  if (append_char(output, output_size, &used, ']') != 0 ||
      append_char(output, output_size, &used, '}') != 0)
  {
    return -1;
  }
  return 0;
}

typedef struct
{
  const char *cursor;
} json_parser;

static void skip_whitespace(json_parser *parser)
{
  while (isspace((unsigned char)*parser->cursor))
  {
    parser->cursor++;
  }
}

static int hex_value(char value)
{
  if (value >= '0' && value <= '9') return value - '0';
  if (value >= 'a' && value <= 'f') return value - 'a' + 10;
  if (value >= 'A' && value <= 'F') return value - 'A' + 10;
  return -1;
}

static int parse_hex_quad(json_parser *parser, unsigned int *value)
{
  *value = 0;
  for (int index = 0; index < 4; index++)
  {
    int digit = hex_value(*parser->cursor++);
    if (digit < 0)
    {
      return -1;
    }
    *value = (*value << 4) | (unsigned int)digit;
  }
  return 0;
}

static int append_codepoint(char *output, size_t output_size, size_t *used,
                            unsigned int codepoint)
{
  if (codepoint <= 0x7f)
  {
    return append_char(output, output_size, used, (char)codepoint);
  }
  if (codepoint <= 0x7ff)
  {
    return append_char(output, output_size, used, (char)(0xc0 | (codepoint >> 6))) != 0 ||
                   append_char(output, output_size, used, (char)(0x80 | (codepoint & 0x3f))) != 0
               ? -1 : 0;
  }
  if (codepoint <= 0xffff)
  {
    return append_char(output, output_size, used, (char)(0xe0 | (codepoint >> 12))) != 0 ||
                   append_char(output, output_size, used, (char)(0x80 | ((codepoint >> 6) & 0x3f))) != 0 ||
                   append_char(output, output_size, used, (char)(0x80 | (codepoint & 0x3f))) != 0
               ? -1 : 0;
  }
  return append_char(output, output_size, used, (char)(0xf0 | (codepoint >> 18))) != 0 ||
                 append_char(output, output_size, used, (char)(0x80 | ((codepoint >> 12) & 0x3f))) != 0 ||
                 append_char(output, output_size, used, (char)(0x80 | ((codepoint >> 6) & 0x3f))) != 0 ||
                 append_char(output, output_size, used, (char)(0x80 | (codepoint & 0x3f))) != 0
             ? -1 : 0;
}

static int parse_string(json_parser *parser, char *output, size_t output_size)
{
  if (*parser->cursor++ != '"')
  {
    return -1;
  }
  size_t used = 0;
  output[0] = '\0';
  while (*parser->cursor != '\0' && *parser->cursor != '"')
  {
    unsigned char value = (unsigned char)*parser->cursor++;
    if (value == '\\')
    {
      value = (unsigned char)*parser->cursor++;
      switch (value)
      {
        case '"': case '\\': case '/': break;
        case 'b': value = '\b'; break;
        case 'f': value = '\f'; break;
        case 'n': value = '\n'; break;
        case 'r': value = '\r'; break;
        case 't': value = '\t'; break;
        case 'u':
        {
          unsigned int codepoint;
          if (parse_hex_quad(parser, &codepoint) != 0)
          {
            return -1;
          }
          if (codepoint >= 0xd800 && codepoint <= 0xdbff)
          {
            unsigned int low;
            if (parser->cursor[0] != '\\' || parser->cursor[1] != 'u')
            {
              return -1;
            }
            parser->cursor += 2;
            if (parse_hex_quad(parser, &low) != 0 || low < 0xdc00 || low > 0xdfff)
            {
              return -1;
            }
            codepoint = 0x10000 + ((codepoint - 0xd800) << 10) + (low - 0xdc00);
          }
          else if (codepoint >= 0xdc00 && codepoint <= 0xdfff)
          {
            return -1;
          }
          if (append_codepoint(output, output_size, &used, codepoint) != 0)
          {
            return -1;
          }
          continue;
        }
        default: return -1;
      }
    }
    else if (value < 0x20)
    {
      return -1;
    }
    if (append_char(output, output_size, &used, (char)value) != 0)
    {
      return -1;
    }
  }
  if (*parser->cursor++ != '"')
  {
    return -1;
  }
  return 0;
}

static int expect(json_parser *parser, char expected)
{
  skip_whitespace(parser);
  if (*parser->cursor != expected)
  {
    return -1;
  }
  parser->cursor++;
  return 0;
}

static int parse_values(json_parser *parser, json_message *message)
{
  if (expect(parser, '[') != 0)
  {
    return -1;
  }
  skip_whitespace(parser);
  if (*parser->cursor == ']')
  {
    parser->cursor++;
    return 0;
  }
  while (message->value_count < JSON_MAX_VALUES)
  {
    if (parse_string(parser, message->values[message->value_count], JSON_VALUE_SIZE) != 0)
    {
      return -1;
    }
    message->value_count++;
    skip_whitespace(parser);
    if (*parser->cursor == ']')
    {
      parser->cursor++;
      return 0;
    }
    if (*parser->cursor++ != ',')
    {
      return -1;
    }
    skip_whitespace(parser);
  }
  return -1;
}

int json_decode_message(const char *input, json_message *message)
{
  if (input == NULL || message == NULL)
  {
    return -1;
  }
  json_parser parser = {.cursor = input};
  int has_code = 0;
  int has_values = 0;
  memset(message, 0, sizeof(*message));
  if (expect(&parser, '{') != 0)
  {
    return -1;
  }

  while (1)
  {
    char key[JSON_CODE_SIZE];
    skip_whitespace(&parser);
    if (parse_string(&parser, key, sizeof(key)) != 0 || expect(&parser, ':') != 0)
    {
      return -1;
    }
    if (strcmp(key, "code") == 0 && !has_code)
    {
      if (parse_string(&parser, message->code, sizeof(message->code)) != 0)
      {
        return -1;
      }
      has_code = 1;
    }
    else if (strcmp(key, "valeurs") == 0 && !has_values)
    {
      if (parse_values(&parser, message) != 0)
      {
        return -1;
      }
      has_values = 1;
    }
    else
    {
      return -1;
    }

    skip_whitespace(&parser);
    if (*parser.cursor == '}')
    {
      parser.cursor++;
      break;
    }
    if (*parser.cursor++ != ',')
    {
      return -1;
    }
    skip_whitespace(&parser);
  }

  skip_whitespace(&parser);
  return has_code && has_values && *parser.cursor == '\0' ? 0 : -1;
}