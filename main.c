//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient1, int *detected_time, int *place); 
int isMet(int patient1,int patient2,int *place__2);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time; //환자번호, 환자나이, 환자확진일자 
    int placeHist[N_HISTORY];
    int i;
    
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    
    //1-2. loading each patient informations
    while (3== fscanf(fp,"%d %d %d", &pIndex,&age,&time)) 
    {
    	for(i=0;i<N_HISTORY;i++)             // N_HISTORY개의 장소 정보를 순서대로 배열에 저장 
    		{
			fscanf(fp,"%d", &placeHist[i]);
			}
			
    	ifct_element=ifctele_genElement(pIndex,age,time,placeHist); //포인터 
    	ifctdb_addTail(ifct_element); //링크드 리스트(데이터베이스)에 환자정보 저장 
    	
	}
	
	
    //1-3. FILE pointer close
    fclose(fp);
    
    
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK   
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu : ");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        switch(menu_selection)
        {
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT: //입력받은 환자에 대한 모든 정보 출력 
            	
            	printf("Patient index : ");
            	scanf("%d",&pIndex); //pIndex를 재활용하여 환자번호 입력받기 
            	
            	ifct_element=ifctdb_getData(pIndex);  //해당 환자의 정보에 관한 구조체 포인터 반환 받기 
            	ifctele_printElement(ifct_element); 
            	
                break;
                
            case MENU_PLACE: //입력받은 장소에서 확진판정받은 모든 환자에 대한 모든 정보 출력 
            	{
				char *place=(char *)malloc(sizeof(char)*MAX_PLACENAME); //문자열 입력받기에 용이하게 동적메모리 할당 받기 // 크기는 (장소이름최대개수) * (문자형변수 크기) 
				int cnt=0;      //해당장소에서 확진판정받은 환자 수를 세는 변수 선언 
    			char *PLACE;    //n번째 환자의 확진판정받은 장소를 받을  문자열 포인터 변수 생성 
    			
                printf("Place Name : ");
                scanf("%s",place);
                
    			place=strcat(place,"\0");   //문자열 끝에 NULL문자 추가 
    			
                for(pIndex=0;pIndex<ifctdb_len();pIndex++)   //환자 한명씩 환자의 확진장소와 입력받은 장소 비교 
                {
                	ifct_element=ifctdb_getData(pIndex); //포인터 
                	
                	PLACE=ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1));  //환자의 마지막 이동장소(=확진판정받은장소) 문자열 포인터로 받아옴 
                	
					if(strcmp(place,PLACE)==0)     //두 장소의 문자열 비교하여 일치할시 해당환자의 정보 출력  //strcmp 함수 : 두 문자열이 일치하는 경우에만 0을 반환 
						{
							ifctele_printElement(ifct_element);
							cnt++;
						}
				}
				
				printf("There are %i patients detected in %s. \n",cnt,place);
				
				free(place);  //할당받은 동적 메모리 해제 
				
                break;
            	}
                
            case MENU_AGE: 
				{
				int min,max;    //min:나이의 최소값 , max: 나이의 최대값 
				int cnt=0;      //해당하는 환자 명수 변수 
				
                printf("minimal age : ");
                scanf("%d",&min);
                printf("maximal age : ");
                scanf("%d", &max);
                
                for(pIndex=0;pIndex<ifctdb_len();pIndex++) //환자번호 0번부터 한명씩 나이가 조건에 일치하는지 비교 
                {
                	ifct_element=ifctdb_getData(pIndex);
                	age=ifctele_getAge(ifct_element);
                	if(age>=min && age<=max)  //환자나이와 입력받은 조건 비교 
                	{
                		ifctele_printElement(ifct_element);
                		cnt++;
					}
                	
				}
                printf("There are %d patients whose age is between %d and %d.\n",cnt,min,max);
                break;
            	}
                
            case MENU_TRACK:
                {
                int Patient; //입력받을 환자 번호 (= 첫 현재환자) 
                int patient; //현재환자 
                int infector; // 전파자 
                int firstInfector; //최초 전파자 
                
                //함수 작동시 변수의 값 변경을 저장하기 위해 포인터 변수 생성 
				int time;// 전파 시점 
				int *detected_time; //포인터 변수 생성 
				detected_time=&time; //전파 시점에 대한 포인터  
                 
				int place_1; //전파 장소 
				int *place_2;
				place_2= &place_1;
				
				
                
                printf("Patient index : ");
				scanf("%d", &Patient); //현재환자 변수에 입력값 받음 
				patient=Patient;  
				
				
				while(patient>=0)//조건 : 현재환자가 누군가 있음  // 최조전파자 찾은경우 patient값이 -1이 돼서 반복문 멈춤 
				{	
					ifct_element=ifctdb_getData(patient);
					*detected_time=ifctele_getinfestedTime(ifct_element);
					
					infector=trackInfester(patient, detected_time, place_2); //infector :전파자 환자번호 반환 받음 //전파자 없을 경우 -1 반환!!! 
					
					if(infector>=0)//전파자 있을때 //infetor: 환자번호 -> 0이상의 정수 
						printf("--> [TRACKING] patient %d is infected by %d (time : %d, place : %s)\n",patient,infector,time,ifctele_getPlaceName(place_1));  // 이 부분때문에 포인터 사용이 필요함 
					
					else  //전파자 없을때 -> infector : -1 
						firstInfector=patient; //최초전파자=현재환자
						
					patient=infector;  //현재환자=전파자 
						
				}
                
                printf("The first infector of %d is %d\n",Patient,firstInfector);
                
                
                break;
            	}
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    return 0;
}

int trackInfester(int patient1, int *detected_time, int *place_2)  //현재환자에게 전파한 전파자를 구하는 홤수 
{
	int infector=-1; // 전파자 
	int place__1;   //확진된 장소 변수 
	int *place__2;
	place__2=&place__1;
	int MetTime; //최초전파자 후보와 접촉한 일자 
	int patient2;//비교대상 환자 번호 변수 
	 
	for(patient2=0;patient2<ifctdb_len();patient2++) //모든 환자 한명씩 대조
	{
		if(patient2 != patient1) //현재환자 본인과의 비교 제외 !
		{
			MetTime=isMet(patient1,patient2,place__2); //만난시간 반환 받음 //안만났을경우 -1 반환 받음 
			
			if(MetTime>0) //만났다면
			{
				if(MetTime< *detected_time) //지금까지 환자 중 만난시간이 가장 이를 경우 //현재환자가 확진된 일자와 비교 -> 현재환자와 전파자가 접촉한 일자는 현재환자가 확진된 일자보다 무조건 이름 
				{
					infector=patient2;         //전파자=비교대상환자 
					*detected_time=MetTime;    //전파시점=전파자와 접촉한 시점 
					*place_2=*place__2;        //전파장소= 전파자와 접촉한 장소 
				}
			}
		}
	}
	
	return infector ;  //전파자 환자번호 반환 
	
}

int isMet(int patient1,int patient2,int *place__2)    //두 환자가 접촉한 시점을 구하는 함수 (현재환자: 감염의심기간 , 비교대상환자 : 전파기간 일때)
{
	int i,m; //(현재환자의 몇번째 이동장소인지),(비교대상환자의 몇번쨰 이동장소인지)
	int time,place1,place2,MetTime=-1;   //(현재환자가 i번째 장소일때의 시점 변수),(현재환자가 시점이 time일때의 장소 변수),(비교대상환자가 시점이 time일때의 장소 변수),(두환자가 접촉한 시점 변수)
	
	void *ifct_element1; //현재환자에 대한 정보 포인터 변수 
	void *ifct_element2; //비교대상환자에 대한 정보 포인터 변수 
	
	ifct_element1=ifctdb_getData(patient1);
	ifct_element2=ifctdb_getData(patient2);
 
	//우선 현재환자의 '감염의심기간'과 비교대상환자의 '전파기간'이 단 하루라도  겹치는지 비교!! //
	int k,j;
	k=ifctele_getinfestedTime(ifct_element1);// k=현재환자의 확진일자 
	j=ifctele_getinfestedTime(ifct_element2);// j=비교대상환자의 확진일자 
	
	
	for(i=0;i<3;i++)//i번쨰 장소 //감염의심 기간 동안의 장소 // 0~2번째 장소 
	{ 
		time=k-(N_HISTORY-1-i);     //현재환자의 i번째 이동장소 '시점' 계산 
		if(time==j || time==j-1)    //그 시점이 비교대상환자의 전파기간과 겹치는지 비교 
		{
			place1=ifctele_getHistPlaceIndex(ifct_element1, i);   //현재환자의 i번째 이동장소 
			
			//그 계산된 '시점'에서의 대상환자 이동장소 계산 // 
			m=4-(j-time);  //비교대상환자의 몇번째 이동장소 인지 계산 
			place2=ifctele_getHistPlaceIndex(ifct_element2, m);  //비교대상환자의 time시점일떄의 이동장소(비교대상환자의 m 번째 이동장소)
			
			if(place1==place2)
			{
				MetTime=time;
				*place__2=place1;     //확진된 장소 변수 값에 두환자의 장소 값 대입 
				break;              //가장 이른 시점만 필요하기때문에 더 늦은 날짜에서의 접촉여부는 계산생략 
			}               
		}
		
	}

	return MetTime; //두 환자의 접촉시점 반환 
	
}
	
