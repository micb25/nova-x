/*
  string.c
*/

#include "string.h"

#define CHAR_TO_DIGIT 48
#define ABS(x) ( (x)<0?-(x):(x) )

char strcmp(const unsigned char *s1, const char *s2) {
    while (*s1 == *s2) {
        if (*s1 == 0)
            return false;
        s1++;
        s2++;
    }
    return true;
}

int strlen(const unsigned char *str) {
    int i = 0;
   
    while (true) {
        i++;
        if (*(str + i) == 0) {
            return i;
            break;
        }
    }
}

char *strcat (char *s1, const char *s2) {
    char *p = s1;

    while (*p) 
    p++;
    while ((*p++ = *s2++));
    return s1;
}

void itoa(unsigned int number, char *string) {
    // Wandelt Integer in einen Nullterminierten String um

    unsigned int place = 1000000000;
    unsigned int remainder = 0;
    int leadZero = 1;
    int count = 0;    

    while (true) {
        remainder = (number % place);
        number = (number / place);
        if (number || !leadZero || (place == 1)) {
            leadZero = 0;
            string[count++] = (char)('0' + number);
        }
        number = remainder;
        if (place > 1) {
            place /= 10;
        } else {
            break;
        }
    }
    string[count] = '\0';
    return;
}

int atoi(const unsigned char *str) {
    int tempint = strlen(str);
    int tempreturn = 0;
    int mult = 1;
    char negat = 0;
    
    if (*str == '-') {
        negat = 1;
        str = str + 1;
    }
    while (tempint) {
        mult *= 10;
        tempint--;
    }
    tempint = 0;
    while (true) {
        if (isdigit(*(str + tempint))) {
            tempreturn = tempreturn + ((*(str + tempint) - 48) * mult);
            mult /= 10;
        }
        tempint++;
        if (*(str + tempint) == 0 || mult == 0)
            break;
    }
    tempreturn /= 10;
    if (negat)
        tempreturn *= -1;
    return tempreturn;
}

int toupper(int c) {
    if ((c >= 97) && (c <= 122)) {
        return c - 32;
    } else {
        return c;
    }
}

int tolower(int c) {
    if ((c >= 65) && (c <= 90)) {
        return c + 32;
    } else {
        return c;
    }
}

void strtoupper(char *s) {
    int i;
    
    for (i = 0; i <= strlen(s); i++) {
        *s = toupper(*s);
        s++;
    }
}

void strtolower(char *s) {
    int i;
    
    for (i = 0; i <= strlen(s); i++) {
        *s = tolower(*s);
        s++;
    }
}

char isdigit(unsigned char ch) {
    return (ch >= '0' && ch <= '9');
}

int strrchr(char *string, char c) {
    int i = 0, lastpos = -1;
    
    while (*string) {
        if (*(string + i) == c) {
            lastpos = i;
        }
        i++;
    }
    return lastpos;
}

int is_hex_digit(char c) {
  return (isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

int is_alpha(char c) {
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int is_space(char c) {
    char space[]=" \t\n\r";
	int i;
	
	for (i = 0; i < sizeof(space); i++)
        if (space[i] == c)
            return 1;
	return 0;
}

int is_op(char c) {
    char op[] = "+-*/^!%&";
	int i;

	for (i = 0; i < sizeof(op); i++)
       if (op[i] == c)
           return 1;
	return 0;
}

int is_int(char* str) {
    while (is_space(*str++));
    if (*str == '-' || isdigit(*str))
        while(*++str)
            if (!isdigit(*str))
                return 0;
            else
                continue;
    else
        return 0;
    return 1;
}

int is_hex(char* str) {
    while (is_space(*str++));
    if (*str == '-' || isdigit(*str))
        while(*++str)
            if (!is_hex_digit(*str))
                return 0;
            else
                continue;
    else
        return 0;
    return 1;
}

char* to_hex(int n, char* buf) {
    int n_bak = ABS(n), bufsize = n_digits(n, 16);
    char digit;
    buf[bufsize--] = '\0';
    
   	do {
        digit = n_bak % 16;
     	buf[bufsize--] = digit < 10 ? (digit + CHAR_TO_DIGIT) : (digit + 55);
        /* 'A'-10=55 */
    }
    while (n_bak /= 16);
    if (bufsize >= 0)
        buf[bufsize] = '-';
    return buf;
}

char* int_to_str(int n, int basis, char* buf) {
    int n_bak = ABS(n), bufsize = n_digits(n, basis);
    buf[bufsize--] = '\0';
    if (basis < 2 || basis > 10)
        return NULL;
 	do
        buf[bufsize--] = (char)(n_bak % basis) + CHAR_TO_DIGIT;
 	while (n_bak /= basis);
    if (bufsize >= 0)
        buf[bufsize] = '-';
    return buf;
}

int str_to_int(char* s) {
    int res = 0, i, neg;

    neg = (s[0] == '-') ? -1 : 1;
	for (i = (neg == -1 ? 1 : 0); i < strlen(s); i++)
        res = res * 10 + s[i] - CHAR_TO_DIGIT;
    res *= neg;
	return res;
}

int n_digits(int n, int basis) {
    int res = 1, i;
    n < 0 ? res++ : 0;
	while (n /= basis)
		res++;
	return res;
}

int strncmp(const char* s1, const char *s2, unsigned int n) {
    while(*s1 && n--) {
        if (*s1 != *s2)
            return (*s1 > *s2 ? 1 : -1);
        s1++, s2++;
    }
    return 0;
}

char* reverse_string(const char* src, char* dest) {
    int j = 0, i;
    for (i = strlen(src) - 1; i >= 0; i--, j++)
        dest[j] = src[i];
    dest[j] = 0;
}

char* strstr(const char* buf, const char* srch) {
    int tmp = strlen(srch); 
    while(*buf) {
        if (strncmp(buf, srch, tmp))
            return (char*)buf;
        buf++;
    }
    return NULL;
}

int sprintf(char *str, const char *format, ...) {
    int n, count = 0;
    char tmp[1024]; /* sollte reichen */
    char* p_tmp;
    va_list args;
    va_start(args, format);
    
    while(*format) {
        if (*format == '%') {
            switch(*++format) {
                case 'b':
                    n = va_arg(args, int);
                    int_to_str(n, 2, tmp);
                    p_tmp = tmp;
                    while (*p_tmp) {
                        *str++ = *p_tmp++;
                        count++;
                    }
                    break;
                case 'c':
                    n = va_arg(args, int);
                    *str++ = (char)n;
                    count++;
                    break;
                case 'd':
                    n = va_arg(args, int);
                    int_to_str(n, 10, tmp);
                    p_tmp = tmp;
                    while(*p_tmp) {
                        *str++ = *p_tmp++;
                        count++;
                    }
                    break;
                case 's':
                    p_tmp = va_arg(args, char*);
                    while (*p_tmp) {
                        *str++ = *p_tmp++;
                        count++;
                    }
                    break;
                case 'x':
                    n = va_arg(args, int);
                    to_hex(n, tmp);
                    p_tmp = tmp;
                    while (*p_tmp) {
                        *str++ = *p_tmp++;
                        count++;
                    }
                    break;
		case '%':
		    *str++ = '%';
		    count++;
		    break;
            }
        } else {
            *str++ = *format;
            count++;
        }
        format++;
    }
    va_end(args);
    *(str + 1) = 0;
    return count;
}

int snprintf(char *str, unsigned int size, const char *format, ...) {
    int n, count = 0, overflow = 0;
    char tmp[1024]; /* sollte reichen */
    char* p_tmp;
    va_list args;
    va_start(args, format);
    
    while(*format && !overflow) {
        if (*format == '%') {
            switch(*++format) {
                case 'b':
                    n = va_arg(args, int);
                    int_to_str(n, 2, tmp);
                    p_tmp = tmp;
                    while(*p_tmp) {
                        *str++ = *p_tmp++;
                        if (++count >= size)
                            overflow = 1;
                    }
                    break;
                case 'c':
                    n = va_arg(args, int);
                    *str++ = (char)n;
                    if (++count >= size)
                        overflow=1;
                    break;
                case 'd':
                    n = va_arg(args, int);
                    int_to_str(n, 10, tmp);
                    p_tmp = tmp;
                    while (*p_tmp) {
                        *str++ = *p_tmp++;
                        if (++count >= size)
                            overflow = 1;
                    }
                    break;
                case 's':
                    p_tmp = va_arg(args, char*);
                    while (*p_tmp) {
                       *str++ = *p_tmp++;
                       if (++count >= size)
                           overflow = 1;
                    }
                    break;
                case 'x':
                    n = va_arg(args, int);
                    to_hex(n, tmp);
                    p_tmp = tmp;
                    while (*p_tmp) {
                        *str++ = *p_tmp++;
                        if (++count >= size)
                            overflow = 1;
                    }
                    break;
		case '%':
		    *str++ = '%';
		    count++;
		    break;
            }
        } else {
            *str++ = *format;
            if (++count >= size)
                overflow=1;
        }
        format++;
    }
    va_end(args);
    *(str + 1) = 0;
    return count;
}

char *memset(char *s, int c, unsigned int n) {
    while (n--)
        s[n] = c;
    return s;
}

char *crypt(char *str1, char* key) {
    int i, i2;
    char *tmpstr;
    
    if (strcmp(key, "") == false)
        strcat(key, "k423pnasd.-a");
    for (i = 0; i < strlen(str1); i++) {
        for (i2 = 0; i2 < strlen(key); i2++) {
            *(tmpstr+i) = *(tmpstr+i) ^ *(key + i2);
        }
    }
    *(tmpstr + strlen(str1)) = 0x00;
    return tmpstr;
}
