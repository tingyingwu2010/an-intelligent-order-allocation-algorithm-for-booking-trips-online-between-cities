#pragma once
#include <string>
#include <set>
#include <map>
#include"linkList.h"
using namespace std;
#ifndef BBX_CONSTRUCTION_H
#define BBX_CONSTRUCTION_H


/*
�˿�ID	�µ�ʱ��	ԤԼʱ��	��������	����/����	��·ID	��������ϸ��ַ	���������꾭γ��	Ŀ�ĵ���ϸ��ַ	Ŀ�ĵ����꾭γ��
�ɳ�ʱ��	����˾��id	�ɳ�ʱ˾�����꾭γ��	����	�ϳ�ʱ��	�³�ʱ��	�������	����״̬  λ������	��������
18��
*/
const int SMALL_MAX = 10;
const int MAX = 5000;
const int NEIGH_NUM = 5;
const double max_capacity = 4.0;
double NO_COMPUTER = -100000;
//NO��Ч�ͻ�		BOOK�����ͻ�	Carpooling_Instantƴ����ʱ�ͻ�	
const int FLAG_NO = 0;
const int FLAG_BOOK = 1;
const int FLAG_CARPOOLING_INSTANT = 2;
const int FLAG_CARPOOLING_ORDER = 3;
//�����ʾ	0�����ף�1�����ڣ�2������
const int LOCAL_HAICANG = 0;
const int LOCAL_INLANDS = 1;
const int LOCAL_XINGLIN = 2;
const int VEHICLE_CAPACITY = 4;
const int minTD = 45;

//118.112533,24.555763�����ִ��ţ�				��					118.143434,24.427385����ʯ��̨��ַ��
//118.082457,24.500666�����״��ţ�				��					118.20628,24.50037������ɽ��
const double MAX_DIS = (sqrt((118.112533 - 118.143434) * (118.112533 - 118.143434) + (24.555763 - 24.427385) * (24.555763 - 24.427385)) +
	sqrt((118.082457 - 118.20628) * (118.082457 - 118.20628) + (24.500666 - 24.50037) * (24.500666 - 24.50037))) / 2.0 / 2.0 * 1000;
const int inf = 999999;
const int pop_num = 3;
const int FUNC_NUM = 2;
double BOARD_WEIGHT = 0.9;
double dis_metric[MAX][MAX];
double *dis_point;
vector<double> dis_center;
bool U[MAX];											//�����Ƿ�ʹ�õı��

double alpha = 1.0;

vector<double> dis_array;
vector<double> time_array;
vector<int> index_array;
vector<double> arcValues;
vector<double> dis_to_exports;
vector<int> inds;
vector<int> dis_inds;
double	dis_export_customers[MAX];

double total_wait[MAX];                     //the total delay time before i

double weights[pop_num][FUNC_NUM] = { 1.0,0,0.5,0.5,0,1.0 };
int startT = 480, overT = 540;


//��ͳ���ȵ�·������	��	·���е�1���ͻ���ԤԼʱ��
vector<int> index_tras;
vector<double> time_order_firsts;
pQueue pq;
int cust_num;							//�ͻ�����Ŀ

double a_time[MAX];                         //arrival time arrays
double w_time[MAX];                         //wait time	arrays
double l_time[MAX];                         //leave time arrays
double max_wait[MAX];						//the max wait time���Ϸ��ӳ�ʱ��
static struct Customer {
	string id;								//����id
	string receiving_time;					//�µ�ʱ��
	double receiving_seconds;
	double order_time;						//ԤԼʱ��
	string order_time_format;				//24ʱ��ʽ��ԤԼʱ��
	string order_date;						//ԤԼ����
	string order_type;						//ԤԼ����
	double demand;							//����
	string road_id;							//��·id 
	string src_address;						//��������ϸ��ַ
	string src_coordinate;					//�����ؾ�γ��
	string dest_address;					//Ŀ�ĵ���ϸ��ַ
	string dest_coordinate;					//Ŀ�ĵؾ�γ��
	string delivery_time;					//�ɳ�ʱ��
	string driver_id;						//˾��id
	string driver_coordinate;				//˾����γ��
	string car_type;						//��������
	string boarding_time;					//�ϳ�ʱ��
	string getoff_time;						//�³�ʱ��
	double cost;							//����
	int state;								//����������
	string scheduling_id;
	int location_type;
	int cust_type;
	double wait_time;
}customer[MAX];				//�ͻ�������


							/*
							���״���
							24.494350, 118.076230
							���׳���
							24.552766, 117.912872
							����������ٿͻ���һ����	���״��ţ�һ����	���׳���

							*/
							//��������
string bridge_coordinates = "24.494350,118.076230";
string export_coordinates = "24.552766,117.912872";
//��������
string bridge_coordinates_almond = "24.549322, 118.106789";
string export_coordinates_almond = "24.590472, 118.004321";

double virtual_point[][2] = { { 24.494350,118.076230 },{ 24.552766,117.912872 } };
double virtual_point_almond[][2] = { { 24.549322 ,118.106789 },{ 24.590472 ,118.004321 } };
//��������������ŷʽ����
double o1 = sqrt((virtual_point[0][0] - virtual_point[1][0]) *(virtual_point[0][0] - virtual_point[1][0]) + (virtual_point[0][1] - virtual_point[1][1]) * (virtual_point[0][1] - virtual_point[1][1]));
//��������������ŷʽ����
double o_almond = sqrt((virtual_point_almond[0][0] - virtual_point_almond[1][0]) *(virtual_point_almond[0][0] - virtual_point_almond[1][0]) + (virtual_point_almond[0][1] - virtual_point_almond[1][1]) * (virtual_point_almond[0][1] - virtual_point_almond[1][1]));


struct Route {							//�ṹ�壺·��
	vector<int> customers_on;				//·���к��еĿͻ�����
	vector<int> customers_off;			    //�¿�˳��
	string export_coordinates;				//��������
	string center_coordinates;				//��������
	double demand;
	int T_early;
	double dis_off;
	bool modify;
	double f[FUNC_NUM];											//0	����	�ϳ���ʻ���룬	1	�����³���ʻ����
	double sp;													//���ݳ���������ԤԼʱ���뵱ǰʱ��Ĳ�ֵ���������Ľ����̶ȣ�ֵԽ��Խ����
	Route() {
		modify = true;
		sp = 0.001;
	}
};

struct Chromosome {						//��
	vector<Route> routes;				//·������
	double f[FUNC_NUM];
	int demand;
};
Chromosome EP[pop_num];
vector<Route> fin_routes;
vector<Route> out_routes;

#endif#pragma once
