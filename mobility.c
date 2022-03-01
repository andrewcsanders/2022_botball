#include <kipr/wombat.h>
#include <mobility.h>
#include <stdlib.h>
#include <mic.h>
#include <mission.h>
#include </home/root/Documents/KISS/BabyReese/Create21-22/include/camera.h>

//////////////////////////////////////////////////////////////////////////////////

int servo_controls[4][4]={{0,20,1,0},{0,50,1,0},{0,100,1,0},{0,100,1,0}};

//////////////////////////////////////////////////////////////////////////////////

int accel(int speed){
    if(speed<75){create_drive_direct(speed,speed);return 0;}
    int m_speed,s_seconds=seconds();
    int per_sec=400;
    while((seconds()-s_seconds)*per_sec<speed){
        m_speed=(seconds()-s_seconds)*per_sec;
        create_drive_direct(m_speed,m_speed);
        msleep(15);
    }

    return 1;
}

//////////////////////////////////////////////////////////////////////////////////
int dccel(int speed){
    if(speed<75){create_drive_direct(0,0);return 0;}
    int per_sec=300,s_seconds=seconds();
    int m_speed=speed-((seconds()-s_seconds)*per_sec);
    while(m_speed>1){
        m_speed=speed-((seconds()-s_seconds)*per_sec);
        create_drive_direct(m_speed,m_speed);
        msleep(15);
    }
    create_drive_direct(0,0);
    return 1;
}

//////////////////////////////////////////////////////////////////////////////////
void motors(int L, int R){
    create_drive_direct(L,R);
}

//////////////////////////////////////////////////////////////////////////////////
int square_up(){
    int R,L,loops_bl=99;
    //reverse if on black
    if(get_create_lfcliff_amt()<1800 || get_create_rfcliff_amt()<1800){   
        while(get_create_lfcliff_amt()<1800 || get_create_rfcliff_amt()<1800){
            create_drive_direct(-100,-100);
            msleep(10);
        }msleep(200);}
    while(loops_bl>=4){//forward to black
        //square white if >1st time through
        if(loops_bl<90){
            //printf("black -> white\n");
            while(!(get_create_lfcliff_amt()>1800 && get_create_rfcliff_amt()>1800)){
                if(get_create_lfcliff_amt()<1800){L=square_up_speed*-1.5;}else{L=square_up_speed;}
                if(get_create_rfcliff_amt()<1800){R=square_up_speed*-1.5;}else{R=square_up_speed;}
                create_drive_direct(L,R);
                msleep(20);
            }
            //create_drive_direct(square_up_speed*-1,square_up_speed*-1);
            //msleep(100);
        }
        loops_bl=0;
        //square up black
        //printf("black\n");
        while(!(get_create_lfcliff_amt()<1800 && get_create_rfcliff_amt()<1800)){
            if(get_create_lfcliff_amt()>1800){L=square_up_speed;}else{L=square_up_speed*-1.5;loops_bl=loops_bl+1;}
            if(get_create_rfcliff_amt()>1800){R=square_up_speed;}else{R=square_up_speed*-1.5;loops_bl=loops_bl+1;}
            create_drive_direct(L,R);
            msleep(20);
        }
        //printf("%d\n",loops_bl);
    }
    create_drive_direct(0,0);
    return loops_bl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void rotate(int deg){
    int fast=100;
    int speed,prev=0,theta=0;
    float modif;
    modif=slope_interp(abs(deg),rotate_modif);
    deg=-1*deg*modif;
    set_create_total_angle(0);
    while(theta>deg+1 || theta<deg-1){
        theta=get_create_total_angle();

        //total_angle sometimes glitches out and this saves it
        if(theta>prev+10 || theta<prev-10){set_create_total_angle(prev);printf("\n=_=\n");theta=get_create_total_angle();}

        prev=theta;
        speed=(10*(deg-theta))^4;
        if(speed> 	fast){speed=   fast;}
        if(speed<-1*fast){speed=-1*fast;}
        create_drive_direct(speed*-1,speed*1);
        msleep(15);
        //printf("%d ",theta);
    }
    printf("rotate:%f\n",(get_create_total_angle()/modif*-1));
    motors(0,0);
}

//////////////////////////////////////////////////////////////////////////////////
void gyro_drive(int speed,float time,int end){
    int theta=0,prev=0;
    float startTime = seconds();
    set_create_total_angle(0);
    accel(speed);
    while(1){
        theta=get_create_total_angle();
        //total_angle sometimes glitches out and this saves it
        if(theta>prev+10 || theta<prev-10){set_create_total_angle(prev);printf("\n=_=\n");theta=get_create_total_angle();}
        prev=theta;
        create_drive_direct(speed+(theta*15),speed+(theta*-15));
        if(seconds()-startTime>time){break;}
        msleep(15);
    }
    if(end){dccel(speed);}
}

//////////////////////////////////////////////////////////////////////////////////
void smart_servo(int port, int perc, float time, int delay){
    servo_controls[port][0]=get_servo_position(port);
    //msleep(50);
    //printf("%d\n",(int)servo_controls[port]);
    servo_controls[port][1]=perc;
    servo_controls[port][2]=time*100;
    servo_controls[port][3]=seconds()+delay;
}

//////////////////////////////////////////////////////////////////////////////////
void servo_control(){
    // 					|start pos|,|end pos|,|time|, |start time|
    float psi;
    enable_servos();
    msleep(100);
    while(1){
        if(seconds()-servo_controls[0][3]<=servo_controls[0][2]/100.0 && seconds()>servo_controls[0][3]){
            psi=servo_controls[0][0]+(((max_0-min_0)*(servo_controls[0][1]/100.0)+min_0)-servo_controls[0][0])*(seconds()-servo_controls[0][3])/(servo_controls[0][2]/100.0);
            set_servo_position(0,(int)psi);
            //printf("%f	",psi);
        }
        if(seconds()-servo_controls[1][3]<=servo_controls[1][2]/100.0 && seconds()>servo_controls[1][3]){
            psi=servo_controls[1][0]+(((max_1-min_1)*(servo_controls[1][1]/100.0)+min_1)-servo_controls[1][0])*(seconds()-servo_controls[1][3])/(servo_controls[1][2]/100.0);
            set_servo_position(1,(int)psi);
            //printf("%f	",psi);
        }
        if(seconds()-servo_controls[2][3]<=servo_controls[2][2]/100.0 && seconds()>servo_controls[2][3]){
            psi=servo_controls[2][0]+(((max_2-min_2)*(servo_controls[2][1]/100.0)+min_2)-servo_controls[2][0])*(seconds()-servo_controls[2][3])/(servo_controls[2][2]/100.0);
            set_servo_position(2,(int)psi);
            //printf("%f	",psi);
        }
        if(seconds()-servo_controls[3][3]<=servo_controls[3][2]/100.0 && seconds()>servo_controls[3][3]){
            psi=servo_controls[3][0]+(((max_3-min_3)*(servo_controls[3][1]/100.0)+min_3)-servo_controls[3][0])*(seconds()-servo_controls[3][3])/(servo_controls[3][2]/100.0);
            set_servo_position(3,(int)psi);
            //printf("%f	",psi);
        }

        msleep(30);
    }
}

//////////////////////////////////////////////////////////////////////////////////
void line_follow(int speed,int side,float time)
{
    //int speed=50;
    int s_time=seconds(),mod;
    int cliff;
    if(max+100<get_create_lfcliff_amt() || min-100>get_create_lfcliff_amt())
    {max=get_create_lfcliff_amt(),min=get_create_lfcliff_amt()-200;}
    accel(speed);
    switch(side){
        case 2:
            printf("side:2\n");
            max=get_create_lfcliff_amt(),min=get_create_lfcliff_amt()-200;
            while(seconds()-s_time<time){
                cliff=get_create_lfcliff_amt();
                if(cliff>max){max=cliff;}
                if(cliff<min){min=cliff;}
                mod=(cliff-(max+min)/2)/((max-min)/100+5);
                create_drive_direct(speed-mod,speed+mod);
                msleep(15);
                printf("%d ",mod);

            }break;
        case 1:
            printf("side:1\n");
            max=get_create_lfcliff_amt(),min=get_create_lfcliff_amt()-200;
            while(seconds()-s_time<time){
                cliff=get_create_lfcliff_amt();
                if(cliff>max){max=cliff;}
                if(cliff<min){min=cliff;}
                mod=(cliff-(max+min)/2)/((max-min)/100+5);
                create_drive_direct(speed-mod,speed+mod);
                msleep(15);
                printf("%d ",mod);

            }break;
        case -1:
            printf("side:-1\n");
            max=get_create_lfcliff_amt(),min=get_create_lfcliff_amt()-200;
            while(seconds()-s_time<time){
                cliff=get_create_lfcliff_amt();
                if(cliff>max){max=cliff;}
                if(cliff<min){min=cliff;}
                mod=(cliff-(max+min)/2)/((max-min)/100+5);
                create_drive_direct(speed-mod,speed+mod);
                msleep(15);
                printf("%d ",mod);

            }break;
        case -2:
            printf("-2\n");

            while(seconds()-s_time<time){
                cliff=get_create_rfcliff_amt();
                if(cliff>max){max=cliff;}
                if(cliff<min){min=cliff;}
                mod=(cliff-(max+min)/2)/((max-min)/100+5);
                create_drive_direct(speed+mod,speed-mod);
                msleep(15);
                printf("%d ",mod);

            }break;
    }
    create_drive_direct(0,0);
    printf("\nMax: %d\nMin: %d\n\n",max,min);
}

//////////////////////////////////////////////////////////////////////////////////

void big_arm(int perc)
{
    if(perc>100){perc=100;printf("ERROR-Code: Out of bounds percentage\n");}if(perc<0){perc=0;printf("ERROR-Code: Out of bounds percentage\n");}
    int trg_pos=((perc/100.0)*(arm_max-arm_min))+arm_min;
    //printf("%d\n",trg_pos);//debug
    int pos = analog(arm_analog_port);
    int prev_pos = pos,m_speed=700;
    int tick=0,motion=0,cable_fliped=1;
    while(pos-trg_pos>20 || pos-trg_pos<-20){
        pos=analog(arm_analog_port);
        /*if(tick>=20){
            //if(motion==-1 && prev_pos+10<=pos){cable_fliped=cable_fliped*-1;printf("ERROR-Physical: Reversed arm motor cable port: %d ID:1\n",arm_motor_port);}
            //if(motion== 1 && prev_pos-10>=pos){cable_fliped=cable_fliped*-1;printf("ERROR-Physical: Reversed arm motor cable port: %d ID:2\n",arm_motor_port);}
            prev_pos=pos;tick=0;

        }*/
        if(1){m_speed=(abs(pos-trg_pos)^2)+300;}
        if(pos>trg_pos){mav(arm_motor_port,-1*m_speed*cable_fliped);motion=-1;}
        if(pos<trg_pos){mav(arm_motor_port,   m_speed*cable_fliped);motion= 1;}
        msleep(20);
        tick=tick+1;
        //if(tick%5==0){printf("%d\n",pos);}//debug
    }
    mav(arm_motor_port,0);
}

//------------------------------------------------------------------------------//
arm_min=2000;
void calibrate_big_arm()
{
    int pos = analog(arm_analog_port),prev_pos;
    int tick=0,motion=-1,cable_fliped=1;
    int min=pos,tock=0;
    while(tock<20)
    {
        pos=analog(arm_analog_port);
        if(tick>=7){
            if(motion==-1 && prev_pos+10<=pos ){cable_fliped=cable_fliped*-1;printf("ABORT-Physical: Reversed arm motor cable port!: %d ID:A1\n",arm_motor_port);ABORT=1;}
            //if(motion== 1 && prev_pos-10>=pos){cable_fliped=cable_fliped*-1;printf("ERROR-Physical: Reversed arm motor cable port: %d ID:2.1\n",arm_motor_port);}
            prev_pos=pos;tick=0;
        }
        mav(arm_motor_port,cable_fliped*-700);
        
        tick=tick+1,tock=tock+1;
        if(min>analog(arm_analog_port)){min=pos;tock=0;/*printf(",new min");*/}
        msleep(20);
		//printf("loop\n");
    }
    printf("\nBig arm min: %d\n",min);
    arm_min=min;arm_max=min+arm_range;
    mav(arm_motor_port,0);
}
//////////////////////////////////////////////////////////////////////////////////
int drive_to_line(int speed,float time,int end){
    int theta=0,prev=0;
    int broken=0;
    float startTime = seconds();
    set_create_total_angle(0);
    accel(speed);
    while(1){
        theta=get_create_total_angle();
        //total_angle sometimes glitches out and this saves it
        if(theta>prev+10 || theta<prev-10){set_create_total_angle(prev);printf("\n=_=\n");theta=get_create_total_angle();}
        prev=theta;
        create_drive_direct(speed+(theta*15),speed+(theta*-15));
        if(seconds()-startTime>time){break;}
        msleep(15);

        if(get_create_lfcliff_amt()<1800 || get_create_rfcliff_amt()<1800){broken=1;printf("drive_to_line: line detected");break;}
    }
    if(end){dccel(speed);}
    if(broken){return(1);}else{return(0);}
}

//////////////////////////////////////////////////////////////////////////////////
int wrist_delta=35;

void wrist_lock()
{
    int max=0;
	while(arm_min>600){
        msleep(50);
        if(analog(wrist_analog)>max){max=analog(wrist_analog);}
    }
    printf("%d\n",max);
    motor(wrist_motor,-15);
    while(max-analog(wrist_analog)<50){
        msleep(10);
        printf("%d\n",analog(wrist_analog));
    }
    freeze(wrist_motor);
    msleep(300);
    cmpc(wrist_motor);
    motor(wrist_motor,5);
    msleep(600);
    motor(wrist_motor,0);
    msleep(500);
    int pos_cnt,mtr_spd;
    //printf("yes\n");
    while(1)
    {
        pos_cnt=gmpc(wrist_motor);
        mtr_spd=-0.1*(pos_cnt-wrist_delta);
        if(pos_cnt>wrist_delta+2 || pos_cnt<wrist_delta-2){
            motor(wrist_motor,mtr_spd);
        }
        msleep(100);
        //printf("%f			%d\n",-0.1*(pos_cnt-wrist_delta),wrist_delta);
    }
}

//////////////////////////////////////////////////////////////////////////////////

int drive_till_bump(int speed,float time,int end){
    int theta=0,prev=0;
    int broken=0;
    float startTime = seconds();
    set_create_total_angle(0);
    accel(speed);
    while(1){
        theta=get_create_total_angle();
        //total_angle sometimes glitches out and this saves it
        if(theta>prev+10 || theta<prev-10){set_create_total_angle(prev);printf("\n=_=\n");theta=get_create_total_angle();}
        prev=theta;
        create_drive_direct(speed+(theta*15),speed+(theta*-15));
        if(seconds()-startTime>time){break;}
        msleep(15);

        if(get_create_lbump() || get_create_rbump()){broken=1;printf("drive_to_bump: wall detected");break;}
    }
    if(end){dccel(speed);}
    if(broken)
    {
        gyro_drive(-150,0.2,1);
        return(1);
    }else{return(0);}
}

//////////////////////////////////////////////////////////////////////////////////

void ticker_drive(){
    //to be worked on

    //the speed at which ticks change changes if create is against a wall
    //ticks to be used to measure distance
}

//////////////////////////////////////////////////////////////////////////////////

void line_up(int direction)
{

    int R_switch,L_switch;
    float R_speed,L_speed;
    int L_amt=get_create_lfcliff_amt();
    int R_amt=get_create_rfcliff_amt();	
    if(L_amt<1800){L_switch=0;}else{L_switch=1;}
    if(R_amt<1800){R_switch=0;}else{R_switch=1;}//properly sets up the switching

    if(direction==1){L_speed=square_up_speed,R_speed=square_up_speed;}else{L_speed=square_up_speed*-1,R_speed=square_up_speed*-1;direction=-1;}
    //sets up speed based on direction

    if(L_amt<1900){L_speed=L_speed*-1;}
    if(R_amt<1900){R_speed=R_speed*-1;}

    //printf("%f		%f",L_speed,R_speed);
    while((abs(L_speed)>30 || abs(R_speed)>30)){
        L_amt=get_create_lfcliff_amt();
        R_amt=get_create_rfcliff_amt();	

        if(L_amt<1800)
        {
            if(L_switch){
                L_speed=L_speed*line_speed_reduce*-1;
                L_switch=0;
            }/*else{
                L_speed=L_speed*line_speed_increase;
            }//*/
        }else{
            if(!L_switch){
                L_speed=L_speed*line_speed_reduce*-1;
                L_switch=1;
            }/*else{
                L_speed=L_speed*line_speed_increase;
            }//*/
        }

        if(R_amt<1800)
        {
            if(R_switch){
                R_speed=R_speed*line_speed_reduce*-1;
                R_switch=0;
            }/*else{
                R_speed=R_speed*line_speed_increase;
            }//*/
        }else{
            if(!R_switch){
                R_speed=R_speed*line_speed_reduce*-1;
                R_switch=1;
            }/*else{
                R_speed=R_speed*line_speed_increase;
            }//*/
        }
        //printf("%f	%f	AMT: %d %d\n",L_speed,R_speed,L_amt,R_amt);
        create_drive_direct(L_speed,R_speed);
        msleep(15);
    }
    create_drive_direct(0,0);
    msleep(100);
    float dif=400;//coef=0.1;
    while(abs(dif)>80){
        dif=(get_create_rfcliff_amt()-get_create_lfcliff_amt())*direction;
        if(dif>0){create_drive_direct(-15,15);}else{create_drive_direct(15,-15);}
        printf("%f\n",dif);
        msleep(15);
    }
    create_drive_direct(0,0);
}
