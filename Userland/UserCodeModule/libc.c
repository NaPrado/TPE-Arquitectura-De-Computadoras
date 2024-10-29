#include <libc.h>
#define READ 0
#define WRITE 1
#define GET_TIME 4
#define SET_CURSOR 5
#define SET_FONT_COLOR 7
#define SET_ZOOM 8
#define DRAW_RECTANGLE 9
#define SET_BACKGROUND_FONT_COLOR 10
#define SLEEP 35

void drawRectangle(Point topLeft, Point downRigth, uint32_t color) {
	sys_call(DRAW_RECTANGLE, (uint64_t)&topLeft, (uint64_t)&downRigth, (uint64_t)color,0);
}

void setFontColor(uint32_t hexColor) {
	sys_call(SET_FONT_COLOR, hexColor, 0, 0, 0);
}
void setBackGroundColor(uint32_t hexColor) {
    sys_call(SET_BACKGROUND_FONT_COLOR, hexColor, 0, 0, 0);
}

void nprint(char * buf, uint64_t lenght) {
	sys_call(WRITE, 1, (uint64_t) buf, lenght, 0);
}

void print(char * buf) {
    sys_call(WRITE, 1, (uint64_t) buf, strlen(buf), 0);
}

time * getTime() {
// char * getTime(){
	return sys_call(GET_TIME, 0, 0, 0, 0);
}

void programTime(char * buf) { 
    time * t = getTime();
    strCpy("dd/mm/yy 00:00:00", buf);
    char aux[3] = {0x00};
    itoa(t->day, aux, 16, 2);
    strNCpy(aux, buf, 2);
    itoa(t->month, aux, 16, 2);
    strNCpy(aux, buf+3, 2);
    itoa(t->year, aux, 16, 2);
    strNCpy(aux, buf+6, 2);
    itoa(t->hour, aux, 16, 2);
    strNCpy(aux, buf+9, 2);
    itoa(t->min, aux, 16, 2);
    strNCpy(aux, buf+12, 2);
    itoa(t->sec, aux, 16, 2);
    strNCpy(aux, buf+15, 2);
}

void programRectangle(uint32_t color) {
    static const Point rec_msg_point1 = {392, 712};
    static const Point rec_msg_point2 = {412, 744};
    static const char * rec_msg1 = "Rectangle drawn";
    static const char * rec_msg2 = "Press any key to continue";

    Point p1 = {64, 64};
    Point p2 = {960, 704};
    drawRectangle(p1, p2, color);
    setZoom(2);
    setCursor(rec_msg_point1.x, rec_msg_point1.y);
    print(rec_msg1);
    setZoom(1);
    setCursor(rec_msg_point2.x, rec_msg_point2.y);
    print(rec_msg2);
    getChar();
    drawRectangle(p1,p2,0x000000);
    setZoom(2);
    setCursor(rec_msg_point1.x, rec_msg_point1.y);
    setFontColor(0x000000);
    print(rec_msg1);
    setZoom(1);
    setCursor(rec_msg_point2.x, rec_msg_point2.y);
    print(rec_msg2);
}

void programHelp() {
    setCursor(BASE_CHAR_WIDTH*4, BASE_CHAR_HEIGHT*2);
    print("Commands:\n\t1-color\n\t2-date\n\t3-rec\n\t4-zoom in\n\t5-zoom out\n\t6-snake");
    while (getChar() == 0) {
        _hlt();
    }
    cleanFullScreen();
}

char getKey(){
    return sys_call(READ, 0, 0, 0, 0);
}

void scan(char * buf, uint32_t count) {
    for (int i = 0; i < count; i++) {
        char c = sys_call(READ, 0, 0, 0, 0);
        if (c == -1 || c == -2){
            _hlt();
            i--;
        } else {
            buf[i] = c;
        }
    }
}

int itoa(uint64_t value, char * buffer, int base, int n) {
    char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

    char end = 0;

	//Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
    } while (value /= base);

	// Terminate string in buffer.
    n -= digits;
    while (n > 0) {
        *p++ = '0';
        n--;
    }

    *p = 0x00;
    

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2) {
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
    sys_call(SET_CURSOR, x, y, 0, 0);
}

void setCharCursor(uint32_t x, uint32_t y) {
    setCursor(x*BASE_CHAR_WIDTH, y*BASE_CHAR_HEIGHT);
}

void sleep(uint64_t seconds){
	sys_call(SLEEP, seconds, 0, 0, 0);
}

void strCpy(char * source, char * dest) {
    while ((*(dest++) = *(source++)) != '\0') {
        ;
    }
}

void strNCpy(char * source, char * dest, int n) {
    while (n-- > 0) {
        (*(dest++) = *(source++));
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

void setZoom(char zoom) {
    sys_call(SET_ZOOM, (uint64_t)zoom, 0, 0, 0);
}

void cleanFullScreen() {
    drawRectangle((Point){0, 0}, (Point){DIM_X, DIM_Y}, 0x000000);
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
