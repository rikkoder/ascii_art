#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

enum {display_height=50, display_width=150, w=3, h=20, d=5, letter_gap=5};
const double angVel = M_PI/400000; //rad/sec
double angDisp = angVel;	//rad
const int display_depth = (int)sqrt((display_width*display_width)+(display_height*display_height));
const int screenToEye = 1000;
const int objCentreToEye = screenToEye+display_depth/2;
char *ascii = " .,-~:;=!*#$@";

int min(int a, int b) {return a<b?a:b;}
int max(int a, int b) {return a>b?a:b;}

void rotate(double theta, int *x, int *y, int *z) {
	int _x = *x, _y = *y, _z = *z;

// 	*x = (_x*cos(theta)) + (_y*0) + (_z*sin(theta));
// 	*y = (_x*0) + (_y*1) + (_z*0);
// 	*z = (_x*sin(theta)) + (_y*0) + (_z*cos(theta));

	*x = (_x*cos(theta)) + (_y*0) + (-_z*sin(theta));
	*y = (_x*0) + (_y*1) + (_z*0);
	*z = (_x*sin(theta)) + (_y*0) + (_z*cos(theta));
}

int Index(char c, char *s) {
	int ind = -1;
	int i = 0;
	while(s[i] && ind==-1) {
		if(s[i]==c) ind = i;
		i++;
	}
	return ind;
}

int main() {

	//2d screen init
	char **display = (char**) malloc((display_height+2)*sizeof(char*));
	for(int i=0;i<display_height+2;i++) {
		char *row = (char*) malloc((display_width+2)*sizeof(char));
		row[0] = '#'; row[display_width+1] = '#';

		if(i==0 || i==display_height+1) memset(row+1, '#', display_width*sizeof(char));
		else memset(row+1, ' ', display_width*sizeof(char));

		display[i] = row;
	}


	//3d space init, space[x][y][z]
	int ***space = (int***) malloc(display_width*sizeof(int**));
	for(int i=0;i<display_width;i++) {
		int **y = (int**) malloc(display_height*sizeof(int*));
		for(int j=0;j<display_height;j++) {
			int *z = (int*) malloc(display_depth*sizeof(int));
			memset(z, 0, display_depth);
			y[j] = z;
		}
		space[i] = y;
	}

	for(int i=0;i<display_width;i++) {
		for(int j=0;j<display_height;j++) {
			for(int k=0;k<display_depth;k++) {
				//shifting origin at center
				int x = i - display_width/2;
				int y = j - display_height/2;
				int z = k - display_depth/2;

				//R
				if(-(2*(w+letter_gap)+h/2+2*w)<=x && x<=-(2*(w+letter_gap)+h/2+w) && -h/2<=y && y<=h/2 && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;
				if(-(2*(w+letter_gap)+h/2+w)<=x && x<=-(2*(w+letter_gap)+h/4) && (h/2-w<=y && y<=h/2 || 0<=y && y<=w) && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;
				if(sqrt((h-4*w)*(h-4*w)/16 - (y-h/4)*(y-h/4)) <= (x+2*(w+letter_gap)+h/4+w)
						&& (x+2*(w+letter_gap)+h/4+w) <= sqrt((h*h)/16 - (y-h/4)*(y-h/4)) && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;
				if(-(2*(w+letter_gap)+h/2+w)<=x && x<=-(2*(w+letter_gap)) && -min(x+2*(w+letter_gap)+h/2+w, h/2)<=y && y<-(x+2*(w+letter_gap)+h/2) && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;

				//i
				if(-(letter_gap+2*w)<=x && x<=-(letter_gap+w) && -h/2<=y && y<h/2-w-2 && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;
				if(4*((x+w+letter_gap+w/2)*(x+w+letter_gap+w/2) + (y-(h-w)/2)*(y-(h-w)/2) + z*z) <= (w*w))
					space[i][j][k] = 1;

				//K
				if(-w<=x && x<=0 && -h/2<=y && y<=h/2 && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;
				if(0<=x && x<h/2+w && (((x-w)<y && y<=min(x, h/2)) || (-min(x, h/2)<=y && y<-(x-w))) && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;

				//K
				if(-w<=x-(h/2+letter_gap+2*w) && x-(h/2+letter_gap+2*w)<=0 && -h/2<=y && y<=h/2 && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;
				if(0<=x-(h/2+letter_gap+2*w) && x-(h/2+letter_gap+2*w)<h/2+w &&
						(((x-(h/2+letter_gap+2*w)-w)<y && y<=min(x-(h/2+letter_gap+2*w), h/2))
						 || (-min(x-(h/2+letter_gap+2*w), h/2)<=y && y<-(x-(h/2+letter_gap+2*w)-w)))
						&& -d/2<=z && z<=d/2)
					space[i][j][k] = 1;

				//U
				int _x = 3*w + 2*(h/2+letter_gap);
				if((_x<=x && x<_x+w || _x+letter_gap+w<=x && x<_x+letter_gap+2*w) && -(h/2-w)<=y && y<=h/2 && -d/2<=z && z<=d/2)
					space[i][j][k] = 1;
				if(-sqrt((letter_gap*letter_gap)/4 - (x-(_x+w+letter_gap/2))*(x-(_x+w+letter_gap/2))) >= (y+(h-letter_gap)/2-w)
						&& (y+(h-letter_gap)/2-w) >= -sqrt((letter_gap+2*w)*(letter_gap+2*w)/4 - (x-(_x+w+letter_gap/2))*(x-(_x+w+letter_gap/2)))
						&& -d/2<=z && z<=d/2)
					space[i][j][k] = 1;
			}
		}
	}


	int run = 2;
	time_t oldTime = time(NULL);
	while(1) {
		time_t newTime = time(NULL);
		double timeTaken = difftime(oldTime, newTime);
// 		oldTime = newTime;

		//duplicate original state
		int ***duplicateSpace = (int***)malloc(display_width*sizeof(int**));
		for(int i=0;i<display_width;i++) {
			int **y = (int**)malloc(display_height*sizeof(int*));
			for(int j=0;j<display_height;j++) {
				int *z = (int*) malloc(display_depth*sizeof(int));
				for(int k=0;k<display_depth;k++) z[k] = space[i][j][k];
				y[j] = z;
			}
			duplicateSpace[i] = y;
		}

		//rotate
		for(int i=0;i<display_width;i++) {
			for(int j=0;j<display_height;j++) {
				for(int k=0;k<display_depth;k++) {
					int x = i - display_width/2;
					int y = j - display_height/2;
					int z = k - display_depth/2;

					if(duplicateSpace[i][j][k]==1) {
						duplicateSpace[i][j][k] = 0;
// 						rotate(angVel*timeTaken, &x, &y, &z);
						rotate(angDisp, &x, &y, &z);
						angDisp += angVel;
						int _i = x + display_width/2;
						int _j = y + display_height/2;
						int _k = z + display_depth/2;
						duplicateSpace[_i][_j][_k] = -1;	//so that it doesn't get rotated again in the same loop
					}
				}
			}
		}

		//update the state of rotated coords
		for(int i=0;i<display_width;i++) {
			for(int j=0;j<display_height;j++) {
				for(int k=0;k<display_depth;k++) {
					if(duplicateSpace[i][j][k]==-1) duplicateSpace[i][j][k] = 1;
				}
			}
		}
		

		//clear previous screen
		for(int i=1;i<display_height+1;i++) {
			memset(display[i]+1, ' ', display_width);
		}


		//2d projection
		for(int i=0;i<display_width;i++) {
			for(int j=0;j<display_height;j++) {
				for(int k=0;k<display_depth;k++) {
					int x = i - display_width/2;
					int y = j - display_height/2;
					int z = k - display_depth/2;

					int _x = (screenToEye*x)/(z+objCentreToEye);
					int _y = (screenToEye*y)/(z+objCentreToEye);

					int _i = _x + display_width/2 + 1;
					int _j = _y + display_height/2 + 1;

					double dist = z + (double)display_depth/2;
					int distToASCII = ceil(12*dist/display_depth);
					char ch = ascii[distToASCII];

					if(duplicateSpace[i][j][k]==1 && distToASCII>Index(display[_j][_i], ascii))
						display[_j][_i] = ch;	//[row][column]
// 						display[_j][_i] = '#';
				}
			}
		}


		//free duplicate space
		for(int i=0;i<display_width;i++) {
			for(int j=0;j<display_height;j++) {
				free(duplicateSpace[i][j]);
			}
			free(duplicateSpace[i]);
		}
		free(duplicateSpace);


		//display
		printf("\033[H");
		for(int i=display_height+1;i>=0;i--) {	//reverse traverse as screen is printed from top to bottom (opp to Yaxis)
			printf("%s", display[i]);
			printf("\n");
		}

		run -= 1;
	}


	//clear data
	for(int i=0;i<display_width;i++) {
		for(int j=0;j<display_height;j++) free(space[i][j]);
		free(space[i]);
	}
	free(space);

	for(int i=0;i<display_height+2;i++) free(display[i]);
	free(display);

	return 0;
}
