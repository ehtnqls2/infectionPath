//
//  ifct_element.h
//  infection path
//
//  Created by Juyeop Kim on 2022/10/20.
//

#ifndef ifct_element_h
#define ifct_element_h

#define N_PLACE                     40
#define N_HISTORY                   5
#define MAX_PLACENAME               100

void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY]);
char* ifctele_getPlaceName(int placeIndex);
int ifctele_getHistPlaceIndex(void* obj, int index);   //환자의 i번째 이동장소 숫자 반환 함수 
unsigned int ifctele_getinfestedTime(void* obj);       // 환자의 확진일자 반환 함수 
int ifctele_getAge(void* obj);
void ifctele_printElement(void* obj);
#endif /* ifct_element_h */
