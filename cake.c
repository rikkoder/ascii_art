#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defining colors - sybolic constans with string values;
#define WHITE "\033[1;37m"
#define RED "\033[0;31m"
#define LRED "\033[1;31m"
#define CYAN "\033[0;36m"
#define LCYAN "\033[1;36m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define GREY "\033[1;30m"
#define LGREY "\033[0;37m"
#define NC "\033[0;37m" //NC=no color

#define ROW 14

void main(int argc, char *argv[]){
	int COL = 26; //26=6*3+8
	char *name = malloc(6*sizeof(char));
	memset(name, ' ', 6);//if no name is given , it will print empty space
	if(argc==2){
		name = argv[1];
		COL = strlen(name)*3+8;
	}
	for(int i=0;i<ROW;i++){
		for(int j=0;j<COL;j++){
			int candle = (j>4 && (j-5)%3==0 && j<COL-5);
			if(i<2);
			else if(i==2){
				if(candle)printf(LRED"*"NC);// fire -red;
				else printf(" ");
			}
			else if(i==3){
				if(candle)printf(WHITE"|"NC);	//candle -white;
				else printf(" ");
			}
			else if(i==4){
				if(j<3||j>COL-4)printf(" ");
				else if(candle)printf(WHITE"|"NC);	//candle -white;
				else printf(YELLOW"_"NC); 
			}
			else if(i==5){
				if(j<3||j>COL-4)printf(" ");
				else if(j==3||j==COL-4)printf(YELLOW"|"NC); //this ain't candle;
				else if((j-4)%3==0)printf(YELLOW"\\");	//well we can ommit NC here
				else if((j-4)%3==1)printf(CYAN"%c"NC, name[(j-5)/3]);	//printing char of string below candle;
				else printf("/");
			}
			else if(i==6||i==7){
				if(j==3||j==COL-4)printf(YELLOW"|"NC); //boundary of cake 
				else if(candle){
					if(i==6)printf(LCYAN"|"NC); //this aint candle too, candle condition is used to match the pos below candles as u can see; so let it be light cyan :)
					else printf(RED"o"NC); //let it be red cherr?
				}
				else printf(" ");
			}
			else if(i<ROW-2){
				if(j==3||j==COL-4)printf(YELLOW"|"NC);
				else if(j>3 && j<COL-4){
					if(i==8)printf(LCYAN"-"NC);
					else if(i==9||i==10)printf(BROWN"#"NC);
					else printf(YELLOW"_"NC);
				}
				else if(i==ROW-3 && j==0 || i==ROW-4 && j==1)printf(LGREY"/"NC);	//lgrey -boundary of plate
				else if(i==ROW-5 && (j==2||j==COL-3))printf(LGREY"_"NC);
				else if(i==ROW-3 && j==COL-1 || i==ROW-4 && j==COL-2)printf(LGREY"\\"NC);
				else printf(" ");
			}
			else if(i==ROW-2){
				if(j==0||j==COL-1)printf(LGREY"|"NC);
				else if(j<5||j>COL-6)printf(" ");
				else printf(GREY"_"NC);	//shading effect
			}
			else if(i==ROW-1){
				if(j==1)printf("\\");
				else if(j==COL-2)printf("/");
				else if(j>1 && j<COL-2)printf("_");
				else printf(" ");
			}
			else printf(" ");
		}
		printf("\n");
	}
}		
