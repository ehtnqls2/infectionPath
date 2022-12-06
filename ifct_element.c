//
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

//2�����迭 ���� ��Ұ��� ���� ��Ҿ��ĺ����� �������迭�� �����Ѱ��� ...+1�� null ���ڶ�����..�Ƹ��� 
char countryName[N_PLACE+1][MAX_PLACENAME] = 
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};
//��ҿ� �ش��ϴ� ���ڸ� �Է¹޾Ƽ� �ش� ��Ҹ� ���������� ��ȯ���ִ� �Լ� 
char* ifctele_getPlaceName(int placeIndex)
{
	return countryName[placeIndex]; //placeIndex ��° �� ��ü  ��ȯ 
 } 

//����üƲ  
typedef struct ifs_ele{
	int index;//��ȣ ���� 
	int age;//���� ���� 
	int time;//�������� �ϼ��� ���� ���� 
	place_t place[N_HISTORY];//�������� �̵���� 5����� enum�������� (����) (enum) place_t �迭  ������:(N_HISTORY) ��ũ�� 
}ifs_ele_t;

//����ü ���� ���� (�ν��Ͻ�)
//static ifs_ele_t ifsarray[20]; //����ü �迭..20�� ȯ�� ... ���߿� linked list�� �ٲܰ���///�迭 ������ ���������� �ִٴ��� ->��ũ�帮��Ʈ(�������Ѿ���,����ü)�� ��� ,�ʿ��Ҷ����� �����޸� ����޾Ƽ� ���� 
//static int ifs_cnt; //����ִ� ȯ�� ���� ��...�������� �ʱⰪ=0 

//����ü�� �������� 
void* ifctele_genElement(int index, int age, unsigned int detected_time,
	 int history_place[N_HISTORY])                                        // ����ü �ϳ� �������ִ� �Լ� 
	 {
		ifs_ele_t *strPtr;
		strPtr =(ifs_ele_t *)malloc(sizeof(ifs_ele_t)); 
		strPtr->index=index; //������ ���� ����
		strPtr->age=age;
		strPtr->time=detected_time;
		
		//strPtr->place[N_HISTORY]=(place_t)history_place[N_HISTORY];
		
		int i;
		for(i=0;i<N_HISTORY;i++)
		{
			strPtr->place[i]=(place_t)history_place[i]; 
		}
		
		
		//{ifsarray[ifs_cnt].placeHist[N_HISTORY]= history_place[N_HISTORY];}
		 
		//ifs_cnt++;
		
		return strPtr;
		
		//�� �Լ����� free�ϸ� �ȵ�	
	 }

int ifctele_getAge(void* obj) // �����Լ����� �ҷ��� �Լ� .. ����ü �ȿ��� ���� ������ ���ִ� �Լ� 
{	//odj�� ����ü�� 
	//ifs_ele_t *strPtr = & ifsarray[(ifs_ele_t *)obj]; //����ü ������ ??????????????????????????????????????????????????????????????????????????????????????? �𸣰��� 
	ifs_ele_t *strPtr = (ifs_ele_t *)obj; //����ü ������ ���ǿ� ���ÿ� �ʱⰪ(Ư�� ����ü�� ������) ���� 
	return (strPtr-> age); //���� ��� ...����ü �����ͷ� ��� ���� 

}
int ifctele_getHistPlaceIndex(void* obj, int index) //index=���° ������� 
{
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	return (int)(strPtr->place[index]);
};

unsigned int ifctele_getinfestedTime(void* obj)
{
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	return (strPtr->time);
};

void ifctele_printElement(void* obj)//����ü �޾Ƽ� ��ü  ��� �Լ� 
{
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	
	//print elements
	printf("--------------------------------------\n");
	printf("Patient index : %d\n",strPtr->index);
	printf("Patient age : %d\n", strPtr->age);
	printf("Detected time : %d\n",strPtr->time);
	printf("Path History : ");
	int i;
	for(i=0;i<N_HISTORY;i++)
	{
		printf("%s(%i)",ifctele_getPlaceName(strPtr->place[i]),(strPtr->time)-(N_HISTORY -i-1));
		if(i<N_HISTORY -1)
			printf("->");
	}
	printf("\n--------------------------------------\n");

	
	
}

