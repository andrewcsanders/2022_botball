#define square_up_speed 50
#define line_speed_reduce 0.8
#define line_speed_increase 1.05

//////////////////////////////////////////////////////////////////////////////////

#define arm_motor_port 3
#define arm_analog_port 0
#define arm_range 3655
int arm_max;
int arm_min;

#define wrist_analog 1
#define wrist_motor 0	//white dot to screen
int wrist_delta;

#define claw_R 0
#define claw_L 1

//////////////////////////////////////////////////////////////////////////////////

#define max_0 1200
#define min_0 300
#define max_1 1500
#define min_1 500
#define max_2 0
#define min_2 0
#define max_3 0
#define min_3 0
int servo_positions[4];
int servo_controls[4][4];
int max,min;

////////////////////////////////////////////////////////////////////////////////// 

int accel(int speed);
int dccel(int speed);
void motors(int L,int R);
int square_up();
void rotate(int deg);
void gyro_drive(int speed,float time,int end);
void servo_control();
void smart_servo(int port, int perc, float time, int delay);
void line_follow(int speed, int side, float time);
void big_arm(int perc);
void calibrate_big_arm();
void move_wrist(int perc);
int drive_to_line(int speed,float time,int end);
void wrist_lock();
int drive_till_bump(int speed,float time,int end);
void line_up(int direction);
//servo positions
#define tenis_low_arm 78

