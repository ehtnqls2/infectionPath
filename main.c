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
    int pIndex, age, time; //ȯ�ڹ�ȣ, ȯ�ڳ���, ȯ��Ȯ������ 
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
    	for(i=0;i<N_HISTORY;i++)             // N_HISTORY���� ��� ������ ������� �迭�� ���� 
    		{
			fscanf(fp,"%d", &placeHist[i]);
			}
			
    	ifct_element=ifctele_genElement(pIndex,age,time,placeHist); //������ 
    	ifctdb_addTail(ifct_element); //��ũ�� ����Ʈ(�����ͺ��̽�)�� ȯ������ ���� 
    	
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
                
            case MENU_PATIENT: //�Է¹��� ȯ�ڿ� ���� ��� ���� ��� 
            	
            	printf("Patient index : ");
            	scanf("%d",&pIndex); //pIndex�� ��Ȱ���Ͽ� ȯ�ڹ�ȣ �Է¹ޱ� 
            	
            	ifct_element=ifctdb_getData(pIndex);  //�ش� ȯ���� ������ ���� ����ü ������ ��ȯ �ޱ� 
            	ifctele_printElement(ifct_element); 
            	
                break;
                
            case MENU_PLACE: //�Է¹��� ��ҿ��� Ȯ���������� ��� ȯ�ڿ� ���� ��� ���� ��� 
            	{
				char *place=(char *)malloc(sizeof(char)*MAX_PLACENAME); //���ڿ� �Է¹ޱ⿡ �����ϰ� �����޸� �Ҵ� �ޱ� // ũ��� (����̸��ִ밳��) * (���������� ũ��) 
				int cnt=0;      //�ش���ҿ��� Ȯ���������� ȯ�� ���� ���� ���� ���� 
    			char *PLACE;    //n��° ȯ���� Ȯ���������� ��Ҹ� ����  ���ڿ� ������ ���� ���� 
    			
                printf("Place Name : ");
                scanf("%s",place);
                
    			place=strcat(place,"\0");   //���ڿ� ���� NULL���� �߰� 
    			
                for(pIndex=0;pIndex<ifctdb_len();pIndex++)   //ȯ�� �Ѹ� ȯ���� Ȯ����ҿ� �Է¹��� ��� �� 
                {
                	ifct_element=ifctdb_getData(pIndex); //������ 
                	
                	PLACE=ifctele_getPlaceName(ifctele_getHistPlaceIndex(ifct_element, N_HISTORY-1));  //ȯ���� ������ �̵����(=Ȯ�������������) ���ڿ� �����ͷ� �޾ƿ� 
                	
					if(strcmp(place,PLACE)==0)     //�� ����� ���ڿ� ���Ͽ� ��ġ�ҽ� �ش�ȯ���� ���� ���  //strcmp �Լ� : �� ���ڿ��� ��ġ�ϴ� ��쿡�� 0�� ��ȯ 
						{
							ifctele_printElement(ifct_element);
							cnt++;
						}
				}
				
				printf("There are %i patients detected in %s. \n",cnt,place);
				
				free(place);  //�Ҵ���� ���� �޸� ���� 
				
                break;
            	}
                
            case MENU_AGE: 
				{
				int min,max;    //min:������ �ּҰ� , max: ������ �ִ밪 
				int cnt=0;      //�ش��ϴ� ȯ�� ��� ���� 
				
                printf("minimal age : ");
                scanf("%d",&min);
                printf("maximal age : ");
                scanf("%d", &max);
                
                for(pIndex=0;pIndex<ifctdb_len();pIndex++) //ȯ�ڹ�ȣ 0������ �Ѹ� ���̰� ���ǿ� ��ġ�ϴ��� �� 
                {
                	ifct_element=ifctdb_getData(pIndex);
                	age=ifctele_getAge(ifct_element);
                	if(age>=min && age<=max)  //ȯ�ڳ��̿� �Է¹��� ���� �� 
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
                int Patient; //�Է¹��� ȯ�� ��ȣ (= ù ����ȯ��) 
                int patient; //����ȯ�� 
                int infector; // ������ 
                int firstInfector; //���� ������ 
                
                //�Լ� �۵��� ������ �� ������ �����ϱ� ���� ������ ���� ���� 
				int time;// ���� ���� 
				int *detected_time; //������ ���� ���� 
				detected_time=&time; //���� ������ ���� ������  
                 
				int place_1; //���� ��� 
				int *place_2;
				place_2= &place_1;
				
				
                
                printf("Patient index : ");
				scanf("%d", &Patient); //����ȯ�� ������ �Է°� ���� 
				patient=Patient;  
				
				
				while(patient>=0)//���� : ����ȯ�ڰ� ������ ����  // ���������� ã����� patient���� -1�� �ż� �ݺ��� ���� 
				{	
					ifct_element=ifctdb_getData(patient);
					*detected_time=ifctele_getinfestedTime(ifct_element);
					
					infector=trackInfester(patient, detected_time, place_2); //infector :������ ȯ�ڹ�ȣ ��ȯ ���� //������ ���� ��� -1 ��ȯ!!! 
					
					if(infector>=0)//������ ������ //infetor: ȯ�ڹ�ȣ -> 0�̻��� ���� 
						printf("--> [TRACKING] patient %d is infected by %d (time : %d, place : %s)\n",patient,infector,time,ifctele_getPlaceName(place_1));  // �� �κж����� ������ ����� �ʿ��� 
					
					else  //������ ������ -> infector : -1 
						firstInfector=patient; //����������=����ȯ��
						
					patient=infector;  //����ȯ��=������ 
						
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

int trackInfester(int patient1, int *detected_time, int *place_2)  //����ȯ�ڿ��� ������ �����ڸ� ���ϴ� �c�� 
{
	int infector=-1; // ������ 
	int place__1;   //Ȯ���� ��� ���� 
	int *place__2;
	place__2=&place__1;
	int MetTime; //���������� �ĺ��� ������ ���� 
	int patient2;//�񱳴�� ȯ�� ��ȣ ���� 
	 
	for(patient2=0;patient2<ifctdb_len();patient2++) //��� ȯ�� �Ѹ� ����
	{
		if(patient2 != patient1) //����ȯ�� ���ΰ��� �� ���� !
		{
			MetTime=isMet(patient1,patient2,place__2); //�����ð� ��ȯ ���� //�ȸ�������� -1 ��ȯ ���� 
			
			if(MetTime>0) //�����ٸ�
			{
				if(MetTime< *detected_time) //���ݱ��� ȯ�� �� �����ð��� ���� �̸� ��� //����ȯ�ڰ� Ȯ���� ���ڿ� �� -> ����ȯ�ڿ� �����ڰ� ������ ���ڴ� ����ȯ�ڰ� Ȯ���� ���ں��� ������ �̸� 
				{
					infector=patient2;         //������=�񱳴��ȯ�� 
					*detected_time=MetTime;    //���Ľ���=�����ڿ� ������ ���� 
					*place_2=*place__2;        //�������= �����ڿ� ������ ��� 
				}
			}
		}
	}
	
	return infector ;  //������ ȯ�ڹ�ȣ ��ȯ 
	
}

int isMet(int patient1,int patient2,int *place__2)    //�� ȯ�ڰ� ������ ������ ���ϴ� �Լ� (����ȯ��: �����ǽɱⰣ , �񱳴��ȯ�� : ���ıⰣ �϶�)
{
	int i,m; //(����ȯ���� ���° �̵��������),(�񱳴��ȯ���� ����� �̵��������)
	int time,place1,place2,MetTime=-1;   //(����ȯ�ڰ� i��° ����϶��� ���� ����),(����ȯ�ڰ� ������ time�϶��� ��� ����),(�񱳴��ȯ�ڰ� ������ time�϶��� ��� ����),(��ȯ�ڰ� ������ ���� ����)
	
	void *ifct_element1; //����ȯ�ڿ� ���� ���� ������ ���� 
	void *ifct_element2; //�񱳴��ȯ�ڿ� ���� ���� ������ ���� 
	
	ifct_element1=ifctdb_getData(patient1);
	ifct_element2=ifctdb_getData(patient2);
 
	//�켱 ����ȯ���� '�����ǽɱⰣ'�� �񱳴��ȯ���� '���ıⰣ'�� �� �Ϸ��  ��ġ���� ��!! //
	int k,j;
	k=ifctele_getinfestedTime(ifct_element1);// k=����ȯ���� Ȯ������ 
	j=ifctele_getinfestedTime(ifct_element2);// j=�񱳴��ȯ���� Ȯ������ 
	
	
	for(i=0;i<3;i++)//i���� ��� //�����ǽ� �Ⱓ ������ ��� // 0~2��° ��� 
	{ 
		time=k-(N_HISTORY-1-i);     //����ȯ���� i��° �̵���� '����' ��� 
		if(time==j || time==j-1)    //�� ������ �񱳴��ȯ���� ���ıⰣ�� ��ġ���� �� 
		{
			place1=ifctele_getHistPlaceIndex(ifct_element1, i);   //����ȯ���� i��° �̵���� 
			
			//�� ���� '����'������ ���ȯ�� �̵���� ��� // 
			m=4-(j-time);  //�񱳴��ȯ���� ���° �̵���� ���� ��� 
			place2=ifctele_getHistPlaceIndex(ifct_element2, m);  //�񱳴��ȯ���� time�����ϋ��� �̵����(�񱳴��ȯ���� m ��° �̵����)
			
			if(place1==place2)
			{
				MetTime=time;
				*place__2=place1;     //Ȯ���� ��� ���� ���� ��ȯ���� ��� �� ���� 
				break;              //���� �̸� ������ �ʿ��ϱ⶧���� �� ���� ��¥������ ���˿��δ� ������ 
			}               
		}
		
	}

	return MetTime; //�� ȯ���� ���˽��� ��ȯ 
	
}
	
