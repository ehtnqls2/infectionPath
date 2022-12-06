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

//2차원배열 행은 장소개수 열은 장소알파벳개수 문자형배열로 저장한거임 ...+1은 null 문자때문에..아마도 
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
//장소에 해당하는 숫자를 입력받아서 해당 장소를 문자형으로 반환해주는 함수 
char* ifctele_getPlaceName(int placeIndex)
{
	return countryName[placeIndex]; //placeIndex 번째 행 전체  반환 
 } 

//구조체틀  
typedef struct ifs_ele{
	int index;//번호 정수 
	int age;//나이 정수 
	int time;//감염시점 일수로 따짐 정수 
	place_t place[N_HISTORY];//감염직전 이동경로 5개경로 enum변수선언 (정수) (enum) place_t 배열  사이즈:(N_HISTORY) 매크로 
}ifs_ele_t;

//구조체 변수 선언 (인스턴스)
//static ifs_ele_t ifsarray[20]; //구조체 배열..20명 환자 ... 나중에 linked list로 바꿀거임///배열 단점은 개수제한이 있다는점 ->링크드리스트(개수제한없음,구조체)를 사용 ,필요할때마다 동적메모리 핟당받아서 연결 
//static int ifs_cnt; //들어있는 환자 정보 수...전역변수 초기값=0 

//구조체에 정보저장 
void* ifctele_genElement(int index, int age, unsigned int detected_time,
	 int history_place[N_HISTORY])                                        // 구조체 하나 생성해주는 함수 
	 {
		ifs_ele_t *strPtr;
		strPtr =(ifs_ele_t *)malloc(sizeof(ifs_ele_t)); 
		strPtr->index=index; //포인터 변수 접근
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
		
		//이 함수에서 free하면 안됨	
	 }

int ifctele_getAge(void* obj) // 메인함수에서 불러올 함수 .. 구조체 안에서 나이 정보를 빼주는 함수 
{	//odj는 구조체임 
	//ifs_ele_t *strPtr = & ifsarray[(ifs_ele_t *)obj]; //구조체 포인터 ??????????????????????????????????????????????????????????????????????????????????????? 모르겠음 
	ifs_ele_t *strPtr = (ifs_ele_t *)obj; //구조체 포인터 정의와 동시에 초기값(특정 구조체의 포인터) 설정 
	return (strPtr-> age); //나이 출력 ...구조체 포인터로 멤버 접근 

}
int ifctele_getHistPlaceIndex(void* obj, int index) //index=몇번째 장소인지 
{
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	return (int)(strPtr->place[index]);
};

unsigned int ifctele_getinfestedTime(void* obj)
{
	ifs_ele_t *strPtr = (ifs_ele_t *)obj;
	return (strPtr->time);
};

void ifctele_printElement(void* obj)//구조체 받아서 전체  출력 함수 
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

