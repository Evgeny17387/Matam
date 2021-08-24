#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


char* foo(const char* str, int* x) // 1:str is not changed so we put const char* before it 
{ 
     
    int len=strlen(str); // use len instead of *x in case of x=null
    if(x!=NULL)  // 2: need to make sure x is not null
        *x = strlen(str); //3: x is a pointer to integer so we must derefrence it before assigning an integer to it 
        
    char* str2; // 4a:declare variable right before using it and don't leave it uninitialzied 
    
    str2 = malloc((len +1) * sizeof(char)); // 5: need to multiply by the size of char (sizeof(char)=1 but need to write)
                                           // need to allocate (len + 1) for the null terminator (strlen does not count it) 

    if(str2==NULL) return NULL; // 6:alloc error. we should always check if malloc succeeded or not

    int i; // 4b: declare variable right before using it and don't leave it uninitialzied
    
    for (i = 0; i < len; i++)
        str2[i] = str[len-1 - i];  // 7: need to substract 1 otherwise str2[0] will be '/0' 
                                  // and str2 will be treated as an empty string

    str2[len]='\0'; //8: strings must be null-terminated in c  
    if (len % 2 == 0) 
    { 
        printf("%s", str2); // 9a: len is even >> print reverse string
    } 
    
    if (len % 2 != 0) 
    { 
        printf("%s", str); // 9b: len is odd >> print original string 
    }    
            
    return str2; 
}