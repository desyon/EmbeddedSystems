#include "../include/Printf.h"

#define END_OF_STRING '\0'  //set EoS for nicer code
#define FORMAT_IDENT '%'    //set format input identifier

#define INTEGER 'd'         //set identifier char for integer
#define UNSIGNED 'u'        //set identifier char for unsigned integer
#define CHARACTER 'c'       //set identifier char for characters
#define STRING 's'          //set identifier char for strings
#define HEXADECIMAL 'x'     //set identifier char for hex numbers
#define BINARY 'b'          //set identifier char for binary numbers
#define PERCENT '%'         //set identifier char for percent signs

char *printD(char *, const void *, const int);
char *printU(char *, const void *, unsigned int);
char *printC(char *, const void *, const char);
char *printS(char *, const void *, char *);
char *printX(char *, const void *, const unsigned int);
char *printB(char *, const void *, const unsigned int);

char *intToBaseString(char *, const void *, unsigned int, unsigned int);

static const char digits[] = "0123456789abcdef";

char *Printf(char *destination, const void *end, const char *format, ...) {
  //check if any argument is empty
  if (!destination || !end || !format) {
    return nullptr;
  }

  va_list argList;
  va_start(argList, format);

  char *nextChar = destination;

  for (const char *position = format; END_OF_STRING != *position; position++) {
    //check if formatted print is needed
    if (FORMAT_IDENT != *position) {
      nextChar = printC(nextChar, end, *position);
      continue;
    } else {
      char formatChar = *(position + 1);
      switch (formatChar) {
        case INTEGER:
          nextChar = printD(nextChar, end, va_arg(argList, int));
          break;
        case UNSIGNED:
          nextChar = printU(nextChar, end, va_arg(argList, unsigned
              int));
          break;
        case CHARACTER:
          nextChar = printC(nextChar, end, static_cast<char>(va_arg(argList, int)));
          break;
        case STRING:
          //nextChar = printS(nextChar, end, va_arg(argList, char*));
          break;
        case HEXADECIMAL:
          nextChar = printX(nextChar, end, va_arg(argList, unsigned
              int));
          break;
        case BINARY:
          nextChar = printB(nextChar, end, va_arg(argList, unsigned
              int));
          break;
        case PERCENT:
          nextChar = printC(nextChar, end, '%');
          break;
        default:
          return nullptr; //if unrecognized return empty string
      }

      position++;   //increase position pointer to skip next char
    }
  }

  va_end(argList);

  *nextChar = END_OF_STRING;
  return destination;
}

char *printD(char *destination, const void *end, const int value) {
  if (end <= destination) return nullptr;

  if (0 > value) {
    //print unary '-' if value is below 0
    destination = printC(destination, end, '-');
    //print absolute value of the number
    destination = intToBaseString(destination, end, static_cast<unsigned int>(value * -1), 10);
  } else {
    destination = intToBaseString(destination, end, static_cast<unsigned int>(value), 10);
  }

  return ++destination;
}

char *printU(char *destination, const void *end, const unsigned int value) {
  if (end <= destination) return nullptr;

  destination = intToBaseString(destination, end, value, 10);

  return ++destination;
}

char *printC(char *destination, const void *end, const char value) {
  if (end <= destination) return nullptr;

  *destination = value;

  return ++destination;
}

//char *printS(char *destination, const void *end, char *value) {
//  if (end <= destination) return nullptr;
//
//  for (value; END_OF_STRING != value; value++) {
//    *destination = *value;
//    destination++;
//  }
//
//  return destination;
//}

char *printX(char *destination, const void *end, const unsigned int value) {
  if (end <= destination) return nullptr;

  //print hexadecimal prefix
  destination = printC(destination, end, '0');
  destination = printC(destination, end, 'x');

  //print hexadecimal number
  destination = intToBaseString(destination, end, value, 16);

  return ++destination;
}

char *printB(char *destination, const void *end, const unsigned int value) {
  if (end <= destination) return nullptr;

  //print binary prefix
  destination = printC(destination, end, '0');
  destination = printC(destination, end, 'b');

  //print binary number
  destination = intToBaseString(destination, end, value, 2);

  return ++destination;
}

char *intToBaseString(char *destination, const void *end, unsigned int value, unsigned int base) {
  if (end <= destination) return nullptr;

  char digit = digits[value % base];    //get char from array "digits"
  value = value / base;                 //shift the value by the amount of base

  //recursion if the there are still digits left
  if (value) {
    destination = intToBaseString(destination, end, value, base);
    destination++;
  }

  *destination = digit;    //write the digit
  return destination;    //increase and return the pointer
}
