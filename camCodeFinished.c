#include <stdio.h>
int CharToInt(char);
int* ConvertCharacterArrayToIntArray(int*, char*);

// DEBUGGING CODE
// For debugging can output to more than one UART
  FILE* toJtag;
  FILE* fp;
// END OF DEBUGGING CODE

//void receive_ack();
int main()
{
// Note: If rbg is used, the values are effected by background color, and
// lighting. Thus, YUV must be used.
        int minCr = 110; // min Cr value when the puck is in the puckholder
        int maxCr = 150; // max Cr value when the puck is in the puckholder
        int minCb = 50; // min Cb value when the puck is in the puckholder
        int maxCb = 100; // max Cb value when the puck is in the puckholder
        int i;
        int check=2;        
        int numbers[6];
        int* arr;    
        char characterArray[26];
                
 // Here it is
           fp = fopen ("/dev/uartBlueTooth", "r+");  // for bluetooth
           fprintf(fp, "---------------NEW PROG START----------------\n\r");
          
           // Make a way to read the camera commands on computer
           printf("rm 010\r"); // This removes both ACK and NCK.
           printf("pm 1\r"); // Send only one line per command.
           printf("cr 19 32\r"); // Turns off the background adjusting
		       printf("cr 18 32\r"); // Turns off the background adjusting
           printf("nf 2\r"); // noise filter to accomidate pixel variations
           printf("sv 1 138\r"); // moves the pan servo to center view.
           usleep(1000000); // gives time for the servo to move
           printf("sv 0 133\r"); //the cam focuses on the puckholder           
           printf("vw 36 75 57 90\r"); // creat virtual window to zoom (to make sure to see the puck and nothing else)      
while (1==1){
           printf("gm\r"); // get mean value of the window

        //get feedback of the GM command
        for (i=0; i<25;){		
	        characterArray[i]= getchar();
        // Ignores the things that are not numbers
    	      if (((int)characterArray[i] < 58 && (int)characterArray[i] >47) || (int)characterArray[i] == 32 || (int)characterArray[i] ==13){
//Uncomment the code below to see what inputs where obtained.
//        		  fprintf(fp, "%c ", characterArray[i]); // print to computer
          		  if(characterArray[i]=='\r') break;
	 	          i++;
          		}
        }


// end of getting the characters
arr = ConvertCharacterArrayToIntArray(numbers, characterArray);
usleep(1000000);

// check for the puck
    if ((arr[0] >= minCr && arr[0] <= maxCr) && (arr[2] >= minCb && arr[2] <= maxCb)){
  // Test code
  // end it
      check = 1;
      fprintf(fp, "GOT THE PUCK\n\r"); // print to computer   
     }
    else{
        check = 0;
        fprintf(fp, "DONT GOT PUCK\n\r"); // print to computer
        }

if (check==1){
        printf("l0 1\r");
        printf("l1 1\r");
        usleep(1000000);
        }
else if (check==0){
        printf("l1 0\r");
        printf("l0 0\r");
        usleep(1000000);
        }
else{
        printf("sv 1 60");
        }

usleep(100000);
}
}

// functions
int CharToInt(char character){
        return (character - 48);
        }

int* ConvertCharacterArrayToIntArray(int *numbers, char characterArray[]){
//  resets the umber array
  numbers[6]=0; // This will hold the new array
  numbers[5]=0; // This will hold the new array
  numbers[4]=0; // This will hold the new array
  numbers[3]=0; // This will hold the new array
  numbers[2]=0; // This will hold the new array
  numbers[1]=0; // This will hold the new array
  numbers[0]=0; // This will hold the new array  
// end of reset  

  int position=1; // This keeps track of the spot in the character array
  int counter=0;
  //We have 6 numbers, so....
  for (counter = 0; counter < 6 && characterArray[position]!='\r';){
        while (characterArray[position] != ' ' && characterArray[position]!='\r'){           
		        numbers[counter] = numbers[counter]*10 + CharToInt(characterArray[position]);
        		position = position +1;
		        }
        position = position +1; // This skips the space
        counter = counter +1;   // This increments the array holding 
                                // the number values to the next spot.
        }
        return numbers;
}
