#include <kipr/wombat.h>
#include <mobility.h>
#include <mic.h>
#include <mission.h>
#include <camera.h>

int main()
{
    if(setup(1)){
        Arkansas_Alabama_Part_1();
        //return1_color();
        //grab_poms();
        //botguy_setup();
        //big_arm(40);
        //smart_servo(1,100,2,0);
        //lock_setup();
        //big_arm(20);
        /*
        int i=0;
        while(i<8){
        	rotate(90);
            i=i+1;
        }
        //*/
    }
    shutdown();
    return 0;
}

/*
Functions:
  gyro_drive(speed, seconds, decell)	decell = 1/0
  drive_to_line(speed, seconds, decell)
  drive_till_bump(speed, seconds, decell)
  smart_servo(port,percentage,length,delay)	-Threaded
  line_follow(speed,position,seconds)	-See Reese for explaination
  rotate(degree)						
  square_up()
  line_up(direction)						1=forward,-1=backward

Variables:
  wrist_delta = (45-250)						-Threaded

Constants:
  claw_L
  claw_R
*/
