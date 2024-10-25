#include <libc.h>

extern uint64_t sys_call(uint64_t rax, uint64_t rdi, uint64_t rsi, uint64_t r10, uint64_t r8);
extern void sys_Write();

setFontColor(uint32_t hexColor) {
	sys_call(7, hexColor, 0, 0, 0);
}

printByLenght(char * buf, uint64_t lenght) {
	sys_call(1, 1, (uint64_t) buf, lenght, 0);
}

void print(char * buf) {

    sys_call(1, 1, (uint64_t) buf, strlen(buf), 0);

}

char * getTime(){
	sys_call(4, 0, 0, 0, 0);
}


void scan(char * buf, uint32_t count) {
    sys_call(0, 0, (uint64_t) buf, count, 0);
}

int itoa(uint64_t value, char * buffer, int base) {
    char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

char getChar() {
    char buf[1] = {0};
    scan(buf, 1);
    return buf[0];
}

void putChar(char c) {
    char buf[2] = {c, '\0'};
    print(buf);
}

int strlen(const char * str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void setCursor(uint32_t x, uint32_t y) {
    sys_call(5, x, y, 0, 0);
}

void sleep(uint64_t seconds){
	sys_call(35, seconds, 0, 0, 0);
}

void strCpy(char * source, char * dest) {
    while ((*(dest++) = *(source++)) != '\0') {
        ;
    }
}

int strCmp(const char * s1, const char * s2) {
    int cmp = 0;
    while ((*(s1) != '\0' || *(s2) != '\0') && cmp == 0) {
        cmp = *(s1) - *(s2);
		s1++;
		s2++;
    }
    return cmp;
}

int isalpha(int c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

int toupper(int c) {
    if (isalpha(c) && c >= 'a') {
        c -= ('a'-'A');
    }
    return c;
}

int tolower(int c) {
    if (isalpha(c) && c <= 'Z') {
        c += ('a'-'A');
    }
    return c;
}

int strCaseCmp(const char * s1, const char * s2) {
	int cmp = 0;
    while ((*(s1) != '\0' || *(s2) != '\0') && cmp == 0) {
        cmp = toupper(*(s1)) - toupper(*(s2));
		s1++;
		s2++;
    }
    return cmp;
}

// int strStartsWith(const char * str, const char * start) {
// 	int length=strlen(start);
// 	for (int i = 0; i < length; i++){
// 		if (str[i] != start[i] || str[i] == '\0'){
// 			return 0;
// 		}
// 	}
// 	return 1;
// }

// strStartsWithCaseInsensitive(const char * str, const char * start) {
// 	char strLower[strlen(str)];
// 	char startLower[strlen(start)];
// 	strCpy(strLower, str);
// 	strCpy(startLower, start);
// 	toLower(strLower);
// 	toLower(startLower);
// 	return strStartsWith(strLower, startLower);
// }


// int toUpper(char* str){
// 	int i = 0;
// 	while(str[i] != '\0'){
// 		if(str[i] >= 'a' && str[i] <= 'z'){
// 			str[i] = str[i] - 32;
// 		}
// 		i++;
// 	}
// 	return 0;
// }
// int toLower(char* str){
// 	int i = 0;
// 	while(str[i] != '\0'){
// 		if(str[i] >= 'A' && str[i] <= 'Z'){
// 			str[i] = str[i] + 32;
// 		}
// 		i++;
// 	}
// 	return 0;
// }
