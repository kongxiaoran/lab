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

const int MinTurnWidth =600;
const int x_speed_a=20;
const int x_speed=13;

const int speed_up_msec = 500;
//1

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
	cout<<"按0——在转角为120°使用"<<endl;
    cout<<"按1——在转角为10"<<endl;
    cout<<"按2——在转角为145"<<endl;
    cout<<"按a——在转角为70°使用"<<endl;
    cout<<"按b——在转角为60°使用"<<endl;
    cout<<"按c——在转角为45°使用"<<endl;
	turn_depth=getchar();

	Motion::ins().walk_start();
	Motion::ins().walk(x_speed_a, 0, speed_up_msec);

	RedObj=new ObjectDetector("RedObj", ShowBinary | ShowSource | ShowDrawing);
	cout<<"start avoiding Red"<<endl;
	avoiding(*RedObj);
	avoiding_times++;

	TB++;


	delete RedObj;
	BlueObj=new ObjectDetector("BlueObj", ShowBinary | ShowSource | ShowDrawing);

	cout<<"start turn_corner"<<endl;
	turn_corner(*BlueObj,turn_flag);
	avoiding(*BlueObj);
	avoiding_times++;
	TA++;
	TB++;


	delete BlueObj;
	GreenObj=new ObjectDetector("GreenObj", ShowBinary | ShowSource | ShowDrawing);
	cout<<"start turn_corner"<<endl;
	turn_corner(*GreenObj,turn_flag);
	avoiding(*GreenObj);
	avoiding_times++;
	TA++;
	TB++;


	turn_corner(*GreenObj,turn_flag);
	delete GreenObj;

	cout<<"over---------------over"<<endl;
	Motion::ins().walk(15,0);
	usleep(10000000);

}

void avoiding(ObjectDetector& obj){         //

		int c = 0;


		while (c != 27) {

			c = obj.process_by_color(10);   //10

			std::cout << "Postion: " << obj.postion() << " Width: " << obj.width() << std::endl;


			Motion::ins().walk(x_speed_a,-(800-obj.postion())/40);  //

			if((700-obj.postion()<0)&&avoiding_times==1&&TC==1){    //
				cout<<"zhongweixina---panduan---"<<endl;
				turn_flag=-1;
				TC=0;
			}else{
				TC=0;}


			if(avoiding_times>1&&TA==1){                //S
				cout<<"zhuanbianqian"<<turn_flag<<endl;
				turn_flag=0-turn_flag;
				cout<<"zhuanbian"<<turn_flag<<endl;
				TA=0;
			}



			if(obj.width()>=MinTurnWidth&&TB==1){


				TB=0;
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

void turn_corner(ObjectDetector obj,int turn_flag)	//
{
	int c=0;
	cout<<"turn_flag="<<turn_flag<<endl;
	cout<<"avoiding_times="<<avoiding_times<<endl;
	if((obj.width()<100&&turn_flag==1)||(avoiding_times==4&&turn_flag==1)){
			int count=0;
			Motion::ins().walk(13,15);
			usleep(3200000);
			while(count<40&&c!=27)
			{
				c = obj.process_by_color(10);   //10
				std::cout << "Postion: " << obj.postion() << " Width: " << obj.width() << std::endl;
				cout<<"count=  **-12"<<count<<endl;
				Motion::ins().walk(10,-12);
				usleep(200000);
				count++;

				if(avoiding_times==4&&count==18){
					break;
				}
				if(obj.width()>100&&count>15&&avoiding_times!=4){
					break;
				}

			}
		}

		if((obj.width()<100&&turn_flag==-1)||(avoiding_times==4&&turn_flag==-1)){
			int count=0;
			Motion::ins().walk(13,-15);
			usleep(3200000);
			while(count<40&&c!=27)
			{
				//you
				c = obj.process_by_color(10);
				std::cout << "Postion: " << obj.postion() << " Width: " << obj.width() << std::endl;
				cout<<"count=  **12"<<count<<endl;
				Motion::ins().walk(10,12);
				usleep(200000);
				count++;

				if(avoiding_times==4&&count==18){
					break;
				}
				if(obj.width()>100&&count>15&&avoiding_times!=4){
					cout<<"finished turn_corner"<<endl;
					cout<<"found!!!!!!!!!!"<<endl;
					break;
				}
			}
		}


		if(avoiding_times==4){
			cout<<"in use turn_depth!!"<<endl;


			int x=16;

			if(turn_flag==1){
				x=0-x;
			}

			if(turn_depth=='1'){
				cout<<"use depth_1"<<endl;
				Motion::ins().walk(13,x);
				usleep(1300000);
			}else if(turn_depth=='2'){
				cout<<"use depth_2"<<endl;
				Motion::ins().walk(13,x);
				usleep(1650000);
			}else if(turn_depth=='3'){
				cout<<"use_depth_3"<<endl;
				Motion::ins().walk(13,x);
				usleep(2000000);
			}else if(turn_depth=='0'){
				cout<<"use depth_0"<<endl;
			}else if(turn_depth=='a'){
				cout<<"use depth_-1"<<endl;
				Motion::ins().walk(13,(0-x));
				usleep(1000000);
			}else if(turn_depth=='b'){
				cout<<"use depth_-2"<<endl;
				Motion::ins().walk(13,(0-x));
				usleep(1400000);
			}else if(turn_depth=='c'){
				cout<<"use depth_-3"<<endl;
				Motion::ins().walk(13,(0-x));
				usleep(1800000);
			}


		}

	return;
}

void got_signal(int)
{
	exit(0);
}

