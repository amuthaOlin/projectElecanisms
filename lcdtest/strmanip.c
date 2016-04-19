#include <p24FJ128GB206.h>

#include "common.h"
#include "lcd.h"
#include "strmanip.h"


// void stringcpy(char* str1, char* str2){
//     uint8_t i;
//     for (i=1; i < 17; i++){
//         if (*str2){
//             *str1=*str2;
//             str1++;
//             str2++;
//         }
//         else {
//             if (*str1){
//             *str1=' ';
//             str1++;
//             }
//         }

//     }
// }
void CamelCase(char* str, uint8_t a, uint8_t b){
    while(*str){
        if(*str >= 'a' && *str <= 'z'){
        *str = ('A' + *str - 'a');
        str++;
        }
        str++;
    }
}

void Caesar(char* str, uint8_t freq, uint8_t shift){
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

void Brnrd(char* str, uint8_t a, uint8_t b){
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
void Leet(char* str,uint8_t a, uint8_t b){
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

void Pig(char* str,uint8_t a, uint8_t b){
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

void Missing(char* str, uint8_t freq, uint8_t a){
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

void Reverse(char* str, uint8_t a, uint8_t b){
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


void Scramble(char* str,uint8_t a, uint8_t b){
    //to do, 
}

void Nada(char* str,uint8_t a, uint8_t b){
    //nothing
}

