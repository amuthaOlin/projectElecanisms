#include <p24FJ128GB206.h>

#include "common.h"
#include "lcd.h"
#include "strm.h"

void strm_CamelCase(char* str, uint8_t a, uint8_t b){
    while(*str){
        if(*str >= 'a' && *str <= 'z'){
        *str = ('A' + *str - 'a');
        str++;
        }
        str++;
    }
}

void strm_Caesar(char* str, uint8_t freq, uint8_t shift){
    while(*str){
        str++;
        if (*(str-1)!=' '){
            if(*str >= 'A' && *str <= 'z'){
            *str = *str+shift;
            str=str+freq; // Only shift every freq letters
            }
        }
    }
}

void strm_Brnrd(char* str, uint8_t a, uint8_t b){
    char newstr[17] = "                ";
    char* newstrptr= newstr;
    char* temp1 = newstrptr;
    char* temp2 = str;
    while(*str){
        if(*str == 'a' || *str == 'e' || *str == 'i' || *str == 'o' || *str == 'u' || *str == 'A' || *str == 'E' || *str == 'I' || *str == 'O' || *str == 'U'){ 

            str++;
        }
        else{
            *newstrptr=*str;
            str++;
            newstrptr++;
        }
    }
    strcpy(temp2,temp1);    
}
void strm_Leet(char* str,uint8_t a, uint8_t b){
    while(*str){
        if(*str == 'A' || *str == 'a'){
        *str = '4';
        }
        else if (*str == 'E' || *str == 'e'){
        *str = '3';
        }
        else if (*str == 'G' || *str == 'g'){
        *str = '6';
        }
        else if (*str == 'L' || *str == 'l'){
        *str = '1';
        }
        else if (*str == 'O' || *str == 'o'){
        *str = '0';
        }
        else if (*str == 'S' || *str == 's'){
        *str = '5';
        }
        else if (*str == 'T' || *str == 't'){
        *str = '7';
        }
        str++;
    }
}

void strm_Pig(char* str,uint8_t a, uint8_t b){
    char newstr[17] ="                ";
    char* newstrptr=newstr;
    char* newstrbgn=newstr;
    uint8_t word =1;
    char first;
    char* wordbgn=str;
    char* strbgn=str;
    uint8_t len=0;
    uint8_t count=0;
    while(*str){
        
        if (word==1){
            first=*wordbgn;
            word=0;
            len=0;
        }
        if (*str==' '){
            uint8_t i=0;
            while (i<len-1){
                wordbgn++;
                *newstrptr=*wordbgn;
                if (count<17){newstrptr++;count++;} 
                i++;
            }
            *newstrptr=first;
            if (count<17){newstrptr++;count++;}
            *newstrptr='a';
            if (count<17){newstrptr++;count++;}
            *newstrptr='y';
            if (count<17){newstrptr++;count++;}
            *newstrptr=' ';
            if (count<17){newstrptr++;count++;}
            word=1;
            wordbgn+=2;
        }
        str++;
        len++;

    }
    int i=0;
    while (i<len-1){
        wordbgn++;
        *newstrptr=*wordbgn;
        if (count<17){newstrptr++;count++;}
        i++;
        }
    *newstrptr=first;
    if (count<17){newstrptr++;count++;}
    *newstrptr='a';
    if (count<17){newstrptr++;count++;}
    *newstrptr='y';
    if (count<17){newstrptr++;count++;}
    *newstrptr=' ';
    if (count<17){newstrptr++;count++;}

    strcpy(strbgn,newstrbgn);
}

void strm_Missing(char* str, uint8_t freq, uint8_t a){
    char newstr[17] = "                ";
    char* newstrptr= newstr;
    char* temp1 = newstrptr;
    char* temp2 = str;
    uint8_t count=1;
    while(*str){
        if(count%freq==0){ 
            if (*str!=' ' && *(str-1)!=' '){
            str++;}
        }
        else{
            *newstrptr=*str;
            str++;
            newstrptr++;
        }
    count++;
    }
    strcpy(temp2,temp1);    
}

void strm_Reverse(char* str, uint8_t a, uint8_t b){
    uint8_t len=0;
    char newstr[17] = "                ";
    char* newstrptr= newstr;
    char* temp1 = newstrptr;
    char* temp2 = str;
    // char *end = string + strlen(string)-1;
    while(*str){
        str++;
        len++;
    }
    str=temp2;
    newstrptr+=len-1;
    while(*str){
        *newstrptr=*str;
        newstrptr--;
        str++;
    }
    strcpy(temp2,temp1);  
}


void strm_Scramble(char* str,uint8_t a, uint8_t b){
    //to do, 
}

void strm_Nada(char* str,uint8_t a, uint8_t b){
    //nothing
}

void strm_genPush(char* command, char* name){
    char newstr[33] = "                                ";
    char push[13]="Push button ";
    char* pushptr =push;
    char* newstrptr= newstr;
    char* temp1 = newstrptr;
    char* temp2 = command;
    while(*pushptr){
        *newstrptr=*pushptr;
        newstrptr++;
        pushptr++;
    }
    while(*name){
        *newstrptr=*name;
        newstrptr++;
        name++;
    }
    *newstrptr='!';
    newstrptr++;
    strcpy(temp2,temp1);
}

void strm_genSet(char* command, char* name, char* val){
    char newstr[33] = "                                ";
    char set[5]="Set ";
    char to[5]=" to ";
    char* setptr =set;
    char* toptr =to;
    char* newstrptr= newstr;
    char* temp1 = newstrptr;
    char* temp2 = command;
    while(*setptr){
        *newstrptr=*setptr;
        newstrptr++;
        setptr++;
    }
    while(*name){
        *newstrptr=*name;
        newstrptr++;
        name++;
    }
    while(*toptr){
        *newstrptr=*toptr;
        newstrptr++;
        toptr++;
    }
    while(*val){
        *newstrptr=*val;
        newstrptr++;
        val++;
    }
    *newstrptr='!';
    newstrptr++;
    strcpy(temp2,temp1);
}
    


