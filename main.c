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


int trackInfester(int patient1, int *detected_time, int *place); //환자 번호, 감염날짜 숫자, 감염장소 숫자 포인터 
int isMet(int patient1,int patient2,int **place);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
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
    	for(i=0;i<N_HISTORY;i++)
    		{
			fscanf(fp,"%d", &placeHist[i]);
			}
    	ifct_element=ifctele_genElement(pIndex,age,time,placeHist); //포인터 
    	ifctdb_addTail(ifct_element);//링크드 리스트에 환자정보 저장 
    	
	}
	
	
    //1-3. FILE pointer close
    fclose(fp);
    
    
    do {
        printf("\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK   //15주차때 알고리즘 힌트 줄거임 
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
                
            case MENU_PATIENT: //14주차 실습 ppt맨마지막 참고 
            	
            	printf("Patient index : ");
            	scanf("%d",&pIndex);
            	ifct_element=ifctdb_getData(pIndex);
            	ifctele_printElement(ifct_element);
            	
                break;
                
            case MENU_PLACE:
            	{
				char *place=(char *)malloc(sizeof(char)*MAX_PLACENAME);
				int cnt=0;
    			char *PLACE;
    			
                printf("Place Name : ");
                scanf("%s",place);
                
    			place=strcat(place,"\0");
    			
                for(pIndex=0;pIndex<ifctdb_len();pIndex++) //환자 한명씩 확진장소 비교 
                {
                	ifct_element=ifctdb_getData(pIndex); //포인터 
                	PLACE=ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1));
                	
					if(strcmp(place,PLACE)==0)
						{
							ifctele_printElement(ifct_element);
							cnt++;
						}
				}
				printf("There are %i patients detected in %s. \n",cnt,place);
				
				free(place); 
				
                break;
            	}
                
            case MENU_AGE:
				{
				int min,max;
				int cnt=0;
				
                printf("minimal age : ");
                scanf("%d",&min);
                printf("maximal age : ");
                scanf("%d", &max);
                
                for(pIndex=0;pIndex<ifctdb_len();pIndex++)
                {
                	ifct_element=ifctdb_getData(pIndex);
                	age=ifctele_getAge(ifct_element);
                	if(age>=min && age<=max)
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
                int Patient;
                int patient; //현재환자 
                int infector; // 전파자 
                int firstInfector; //최초 전파자 
                int time;
				int *detected_time;
				detected_time=&time;
                int place_1;
				int *place_2;
				//Place= (int *)malloc(sizeof(int)); 
				//int *place;
				place_2=&place_1;
				//printf("%i",Place);
				
                
                printf("Patient index : ");
				scanf("%d", &Patient); //현재환자 변수에 입력값 받음 
				patient=Patient;
				//printf("%i\n",patient);
				//printf("%i\n",infector);
				//printf("%i\n",firstInfector);
				
				while(patient>=0)//조건 : 현재환자가 누군가 있음  // 최조전파자 찾은경우 patient값이 -1이 돼서 반복문 멈춤 
				{	
					ifct_element=ifctdb_getData(patient);
					*detected_time=ifctele_getinfestedTime(ifct_element);
					//printf("%i",time);
					//place=ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1);
					
					infector=trackInfester(patient, detected_time, place_2); //infector :전파자 환자번호 반환 받음 //전파자 없을 경우 -1 반환!!! 
					//printf("%i\n",infector);
					
					if(infector>=0)//전파자 있을때 //infetor: 환자번호 -> 0이상의 정수 
						printf("--> [TRACKING] patient %d is infected by %d (time : %d, place : %s)\n",patient,infector,time,ifctele_getPlaceName(place_1));   //전파된 시간이랑 장소 알아야함 어떻게??? 
					
					else  //전파자 없을때 -> infector : -1 
						{
						firstInfector=patient; //최초전파자=현재환자 
						//printf("%i\n",firstInfector);
						//printf("%i\n",patient);
						}
						
					patient=infector;  //현재환자=전파자 
						
				}
                //printf("%i",time);
                //printf("--> [TRACKING] patient %d is infected by %d (time : %d, place : %s)\n",pIndex,infector,time,place);
                printf("The first infector of %d is %d\n",Patient,firstInfector);
                //printf("%i\n",place_1);
                //printf("%i,%s",isMet(0,2,place),ifctele_getPlaceName(place));
                
                break;
            	}
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    return 0;
}

int trackInfester(int patient1, int *detected_time, int *place_2)
{
	int infector=-1; // 전파자
	//int time=*detected_time;//현재환자의 확진일자 
	int **place_3;
	place_3=&place_2;
	int MetTime; //최초전파자 후보와 접촉한 일자 
	int patient2;
	 
	for(patient2=0;patient2<ifctdb_len();patient2++) //모든 환자 한명씩 대조 //환자 명수 변경해 ////////////////////////////////////////////////////////////////////////////////////////////////// 
	{
		if(patient2 != patient1) //현재환자 본인과의 비교 제외 !
		{
			//printf("patient1: %i, patient2: %i\n",patient1,patient2);
			MetTime=isMet(patient1,patient2,place_3); //만난시간 반환 받음 //안만났을경우 -1 반환 받음 
			//printf("%i",MetTime);
			//printf("%i\n",MetTime);
			
			if(MetTime>0) //만났다면 // 같은장소&&같은시간 
			{
				if(MetTime< *detected_time) //지금까지 환자 중 만난시간이 가장 이를 경우 //현재환자가 확진된 일자와 비교 -> 현재환자와 전파자가 접촉한 일자는 현재환자가 확진된 일자보다 무조건 이름 
				{
					infector=patient2; //전파자=비교대상환자 
					*detected_time=MetTime;
					//printf("%i\n",*detected_time); //전파된 날짜 설정 
				}
			}
		}
	}
	//printf("%i",patient2);
	//printf("%i", infector);
	
	return infector ;
	
}

int isMet(int patient1,int patient2,int **place_3)
{
	int i,m,time,place1,place2,MetTime=-1;
	
	void *ifct_element1; //현재환자에 대한 정보 포인터 
	void *ifct_element2; //비교대상환자에 대한 정보 포인터 
	
	ifct_element1=ifctdb_getData(patient1);
	ifct_element2=ifctdb_getData(patient2);
 
	//우선 현재환자의 '감염의심기간'과 비교대상환자의 '전파기간'이 단 하루라도  겹치는지 비교 
	int k,j;
	k=ifctele_getinfestedTime(ifct_element1);//현재환자의 확진일자 
	j=ifctele_getinfestedTime(ifct_element2);//비교대상환자의 확진일자 
	
	

	for(i=0;i<3;i++)//i번쨰 장소 //감염의심 기간 동안의 장소 // 0~2번째 장소 
	{ 
		time=k-(N_HISTORY-1-i);//현재환자의 i번째 이동장소 '시점' 계산 
		if(time==j || time==j-1)//그 시점이 비교대상환자의 전파기간과 겹치는지 비교//겹치는 경우 
		{
			place1=ifctele_getHistPlaceIndex(ifct_element1, i);//현재환자의 i번째 이동장소 
			//printf("%i\n",place1);
			//그 계산된 '시점'에서의 대상환자 이동장소 계산 // 
			m=4-(j-time); //비교대상환자의 몇번째 이동장소 인지 계산 
			place2=ifctele_getHistPlaceIndex(ifct_element2, m);//비교대상환자의 time시점일떄의 이동장소(비교대상환자의 m 번째 이동장소)
			//printf("%i%i\n",place1,place2);
			if(place1==place2)
			{
				//printf("%i\n",place1);
				//printf("%i\n",place2);
				MetTime=time; //만난시간=i번째 이동장소 시점 
				**place_3=place1;
				//printf("%i",*place);
				
				break;
			} //가장 이른 시점만 필요 
		}
		
	}
	//printf("%i\n",place1);
	//printf("%i\n",place2);
	//printf("%i",*place);
	return MetTime; 
	
}
	
