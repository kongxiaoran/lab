#include "motion.h"
#include "objectdetector.h"
#include <iostream>
#include <cstdlib>
#include <signal.h>

using namespace std;

//
void got_signal(int);
void avoiding(ObjectDetector&);
void turn_corner(ObjectDetector,int);

//

const int MinTurnWidth =880;
const int x_speed_a=26;
const int x_speed=13;

const int speed_up_msec = 500;
//

int avoiding_times=1;
int turn_flag=1;
int TB=1;
int TC=1;
int TA=1;

char turn_depth='0';

ObjectDetector* RedObj;
ObjectDetector* BlueObj;
ObjectDetector* GreenObj;

int main(int argc, char ** argv)
{

	cout << "start " <<endl;

	signal(SIGTERM, got_signal);
	signal(SIGQUIT, got_signal);
	signal(SIGINT, got_signal);

	cout << "read char" << endl;

	cout<<"--------------list------------"<<endl;
	cout<<"æ?ââåšèœ¬è§äž?20Â°äœ¿çš"<<endl;
    cout<<"æ?ââåšèœ¬è§äž?0"<<endl;
    cout<<"æ?ââåšèœ¬è§äž?45"<<endl;
    cout<<"æaââåšèœ¬è§äž?0Â°äœ¿çš"<<endl;
    cout<<"æbââåšèœ¬è§äž?0Â°äœ¿çš"<<endl;
    cout<<"æcââåšèœ¬è§äž?5Â°äœ¿çš"<<endl;
	turn_depth=getchar();

	Motion::ins().walk_start();
	Motion::ins().walk(x_speed_a, 0, speed_up_msec);

	GreenObj=new ObjectDetector("GreenObj", ShowBinary | ShowSource | ShowDrawing);
	cout<<"start avoiding Green"<<endl;
	avoiding(*GreenObj);
  return 0;
}

void avoiding(ObjectDetector& obj){         //

		int c = 0;


		while (c != 27) {

			c = obj.process_by_color(10);   //10

			std::cout << "Postion: " << obj.postion() << " Width: " << obj.width() << std::endl;


			Motion::ins().walk(x_speed_a,-(800-obj.postion())/40);  //

		

			if(obj.width()>=MinTurnWidth){


	
				cout<<"reach"<<endl;
				return;
			}

			if (Motion::ins().fall_up()) {          //
				Motion::ins().walk_start();
				Motion::ins().walk(x_speed, 0, speed_up_msec);
			}
		}
	return;
}

void got_signal(int)
{
	exit(0);
}
