#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

void read_file(char file_name[], Fl flags);

int SqueezePrint (Fl flags, char c, char last);
int TabPrint (Fl flags, char c);
int EndPrint (Fl flags, char c);
int NumPrint (Fl flags, char c, int *count);
int NumNonBlankPrint(Fl flags, char c, char last, int *count);

int main(int argc, char *argv[]) {
  Fl flags = {0, 0, 0, 0, 0, 0};
  int f=0;
  Parser(&flags, argv, argc, &f);
  for (int i = 1 + f; i < argc; i++) {
    read_file(argv[i], flags);
  }
  return 0;
}

void read_file(char file_name[], Fl flags) {
  
  FILE *file = fopen(file_name, "rb");
  if (!file) {
    printf("s21_cat: %s: No such file or directory", file_name);
  } else {
    char c = fgetc(file);
    char last = '\n';
    int count = 1;
    int ft;
    int fs;
    while (c != EOF) {
        ft=0;
        fs=0;
        
        while (SqueezePrint (flags, c, last)) {
          last=c, 
          c=fgetc(file);
          fs=1;
          }
          if (fs==1){
            last='\n'; 
          NumPrint(flags, last, &count);
          EndPrint(flags,last);
          printf("\n");
          if(c==EOF) break;
          }
        NumNonBlankPrint(flags,c,last, &count);
        NumPrint(flags, last, &count);
        ft=TabPrint(flags, c);
        EndPrint(flags, c);
        if (!ft && c!=EOF) printf("%c", c);
      last = c;
      c = fgetc(file);
       
      
    }
    fclose(file);
  }
}
int TabPrint (Fl flags, char c){
  int flag=0;
  if (c == '\t' && flags.t)
        { printf("^I");
        flag=1;
        }
        return flag;

}
int EndPrint (Fl flags, char c){
  int flag=0;
  if (c == '\n' && flags.e)
        { printf("$");
        flag=1;
        }
        return flag;

}
int NumPrint (Fl flags, char c, int *count){
  int flag=0;
  if (c == '\n' && flags.n) {
     flag=1; 
        printf("     %d\t",  *count);
        *count=*count+1;  
      }
      return flag;
}

int NumNonBlankPrint(Fl flags, char c, char last, int *count){
  int flag=0;
  if(c!='\n' && flags.b && last=='\n') {
    flag=1;
    printf("     %d\t",  *count);
        *count=*count+1; 
  }
  return flag;
}

int SqueezePrint (Fl flags, char c, char last){
int flag=0;
  if(c=='\n' && flags.s && last=='\n') {
    flag=1; 
  }
  return flag;
}