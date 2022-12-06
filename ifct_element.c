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
	int pIndex;//��ȣ ���� 
	int age;//���� ���� 
	int time;//�������� �ϼ��� ���� ���� 
	place_t placeHist[N_HISTORY];//�������� �̵���� 5����� enum�������� (����) (enum) place_t �迭  ������:(N_HISTORY) ��ũ�� 
}ifs_ele_t;

//����ü ���� ���� (�ν��Ͻ�)
static ifs_ele_t ifsarray[20]; //����ü �迭..20�� ȯ�� ... ���߿� linked list�� �ٲܰ���
static int ifs_cnt; //����ִ� ȯ�� ���� ��...�������� �ʱⰪ=0 

//����ü�� �������� 
void* ifctele_genElement(int index, int age, unsigned int detected_time,
	 int history_place[N_HISTORY])
	 {
	 	
		ifsarray[ifs_cnt].pIndex= index;
		ifsarray[ifs_cnt].age= age;
		ifsarray[ifs_cnt].time= detected_time;
		//for(int i=0;i<N_HISTORY;i++)
		{ifsarray[ifs_cnt].placeHist[N_HISTORY]= history_place[N_HISTORY];}
		 
		ifs_cnt++;
		
		
		return (void*)&ifsarray[ifs_cnt-1]; //���� �ּ�  
		 	
	 }

int ifctele_getAge(void* obj) // �����Լ����� �ҷ��� �Լ� .. ����ü �ȿ��� ���� ������ ���ִ� �Լ� 
{
	//ifs_ele_t *strPtr = & ifsarray[(ifs_ele_t *)obj]; //����ü ������ ??????????????????????????????????????????????????????????????????????????????????????? �𸣰��� 
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	return (strPtr-> age); //���� ��� ...����ü �����ͷ� ��� ���� 
}

void ifsele_printElement(void* obj)//����ü �޾Ƽ� ��ü  ��� �Լ� 
{
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	
	//print elements
	
}

