/*
 * loganalysis.cpp
 *
 *  Created on: 2019. 7. 8.
 *      Author: aoska
 */

#include "loganalysis.h"

#define FILENAME "aoska.log"

Clog_analysis::Clog_analysis() { // @suppress("Class members should be properly initialized")생성자
	// TODO Auto-generated constructor stub
}

Clog_analysis::Clog_analysis(std::string log_filename)//생성자 // @suppress("Class members should be properly initialized")
{
}

Clog_analysis::~Clog_analysis() {
	// TODO Auto-generated destructor stub
}

void ACM_analysis(std::string full_file_content);	//ACM 분석 함수
void ATVM_analysis();								//ATVM 분석 함수
void PAU_analysis();								//PAU 분석 함수
void AGM_analysis();								//AGM 분석 함수

int get_transaction_cancle(std::string transaction_content, int *transaction_cancle_flags);									//해당 거래 취소인지 구하는 함수
int get_transaction_fail(std::string transaction_content, int *transaction_fail_flags);										//해당 거래 실패인지 구하는 함수

void get_cardsale_fare(std::string transaction_content, int *cardsale_fare);												//총 판매 금액 운임 구하는 함수
void get_cardsale_adult_number(std::string transaction_content, int *cardsale_adult_number);								//카드 판매 어른 발매 수량 구하는 함수
void get_cardsale_adolescent_number(std::string transaction_content, int *cardsale_adolescent_number);						//카드 판매 청소년 발매 수량 구하는 함수
void get_cardsale_seasonticket_number(std::string transaction_content, int *cardsale_seasonticket_number);					//카드 판매 정기권 발매 수량 구하는 함수

void get_recharge_fare(std::string transaction_content, int *recharge_fare);												//총 보충 금액 운임 구하는 함수
void get_recharge_balance(std::string transaction_content, int *recharge_balance);											//총 보충 전 잔액 구하는 함수
void get_recharge_cardnum(std::string transaction_content, std::string *cardnum);											//카드 번호 구하는 함수
void get_recharge_cardtype(std::string transaction_content, std::string *card_type);										//카드 타입 구하는 함수
void get_season_ticket_frequency(std::string transaction_content, int *season_ticket_frequency);							//정기권 횟수 구하기
void get_recharge_after_balance(std::string transaction_content, int *recharge_after_balance);								//보충 후 잔액 구하기
int check_bill_shutter(std::string transaction_content, int *check_shutter_abnormal);										//성공했을시 bill shutter 열리고 닫혔는지 확인
void get_transaction_time(std::string transaction_content,std::string *transaction_time);									//거래 했을 때 거래 시간 구하는 함수
void get_transaction_inventory_before(std::string transaction_content, int transaction_inventory_before[]);					//거래 전 재고
void get_transaction_inventory_after(std::string transaction_content, int transaction_inventory_after[]);					//거래 후 재고
int get_compare_inventory(int transaction_inventory_before[], int transaction_inventory_after[], int *inventory_change);	//거래 전 재고와 거래 후 재고 비교 후 차이값 반환

int main(int argc, char* argv[]){
	int i;									//인덱스용 변수
	std::string file_name;					//파일이름
	std::string kind_of_afc;				//AFC 종류
	int kind_of_afc_number;					//AFC 번호
	std::cout << "검색할 로그의 파일명을 입력해주세요." << std::endl;
	cin >> file_name;						//파일명 입력
	if(file_name == "매남" ){				//파일명 한글인 경우
		file_name = "aoska.log";			//이름끝에 log붙이기//맨끝이 log가 아닌 경우
	}
	if(file_name.find(".log") == -1 ){		//파일명끝 .log없는 경우
		file_name.append(".log");			//이름끝에 log붙이기//맨끝이 log가 아닌 경우
	}
	std::ifstream logfile_in(file_name);	//로그파일 입력 받는 곳
	if (logfile_in.is_open() == false) { 	//파일이 열리지 않는다면
		std::cout << file_name << " file can't find" << std::endl;
		return 0;
	}
	else{
		std::cout << "분석하는 로그의 AFC 종류에 해당하는 번호를 입력해주세요." << std::endl;
		std::cout << "( 보충기 : 1.ACM, 발매기 : 2.ATVM, 휴대용 정산기 : 3.PAU, 개집표기 : 4.AGM )" << std::endl;
		std::cin >> kind_of_afc_number; 												//AFC종류 번호 입력
		int afc_machine_size = sizeof(afc_machine_name)/ sizeof(afc_machine_name[0]); 	//머신배열 크기
		for (i = 0; i < afc_machine_size; i++) {
			if (kind_of_afc_number == i + 1) { 											//배열은 0부터 시작하니.
				break; 																	//kind_of_afc_number가 머신 번호인 경우 탈출
			}
		}
		if (i == afc_machine_size) {													//머신이름이 아닌 경우
			std::cout << "분석할 로그의 AFC 종류에 해당하는 번호를 제대로 입력하지 않으셨습니다." << endl;
			std::cout << "( 보충기 : 1.ACM, 발매기 : 2.ATVM, 휴대용 정산기 : 3.PAU, 개집표기 : 4.AGM )" << std::endl;
			return 0;
		}
		std::string full_file_content;													//로그 전체 내용 옮겨 적을곳
		logfile_in.seekg(0, std::ios::end);												// 위치 지정자를 파일 끝으로 옮긴다.
		int size = logfile_in.tellg();													// 그리고 그 위치를 읽는다. (파일의 크기)
		full_file_content.resize(size);													// 그 크기의 문자열을 할당한다.
		logfile_in.seekg(0, std::ios::beg);												// 위치 지정자를 다시 파일 맨 앞으로 옮긴다.
		logfile_in.read(&full_file_content[0], size);									// 파일 전체 내용을 읽어서 문자열에 저장한다.
		std::cout << afc_machine_name[i] << " log 분석 시작합니다." << std::endl;
		switch (i) {
		case ACM:																		//보충기인 경우
			ACM_analysis(full_file_content);											//ACM 분석 함수
			break;
		case ATVM:																		//발매기인 경우
			ATVM_analysis();															//ATVM 분석 함수
			break;
		case PAU:																		//휴대용 정산기인 경우
			PAU_analysis();																//PAU 분석 함수
			break;
		case AGM:																		//개집표기인 경우
			AGM_analysis();																//AGM 분석 함수
			break;
		}
	}
	return 0;
}

void ACM_analysis(std::string full_file_content)//ACM 분석 함수
{
	std::string transaction_content;						//거래 내용 옮겨 적을곳
	std::string find_content_start = "거래 시작 ";			//거래 시작 부분
	std::string find_content_end = "///////////////////";	//거래 종료 부분
	std::string find_recharge_transaction = "카드 보충 ";		//거래 카드 보충 탐색
	std::string find_cardsale_transaction = "카드 판매 ";		//거래 카드 판매 탐색
	std::string find_season_tichet_transaction = "정기권 횟수: ";

	int find_content_start_number;				//거래 시작 지점
	int find_content_end_number;				//거래 종료 지점
	int transaction_number = 0;					//거래 개수(거래 건수)
	int recharge_transaction_number = 0;		//보충 거래 건수
	int cardsale_transaction_number = 0;		//판매 거래 건수
	int transaction_cancle_number = 0;			//거래 취소 건수
	int transaction_flag;						//현재 거래 종류
	int transaction_cancle_flag;				//현재 거래 실패(성공1 실패0)
	int check_shutter_abnormal;					//셔터 이상 검사
	int recharge_fare;							//카드 보충 운임
	std::string cardnum;						//카드 번호
	std::string cardtype;						//카드 타입
	int recharge_balance;						//잔액
	int recharge_after_balance;					//보충 후 잔액
	int season_ticket_frequency;				//정기권 횟수

	int cardsale_fare;							//카드 판매 운임
	int cardsale_adult_number;					//카드 판매 어른 발매 수량
	int cardsale_adolescent_number;				//카드 판매 청소년 발매 수량
	int cardsale_seasonticket_number;			//카드 판매 정기권 발매 수량
	std::string transaction_time;				//거래 시간
	int transaction_inventory_before[8];		//거래 전 재고
	int transaction_inventory_after[8];			//거래 후 재고
	int abnormal_discovery_flag;				//해당 거래 이상 거래 발견 되었는지
	int inventory_change;						//재고 변화

	while (true) {								//거래건수 없을때까지 무한루프(위에서 부터 분석 시작)
		if (full_file_content.find(find_content_start) == -1) {	//거래 시작 없는 경우
			std::cout << "보충 건수 : " << recharge_transaction_number << ", 판매 건수 : " << cardsale_transaction_number << std::endl;
			std::cout << "transaction_number(카드 거래 종류 상관없이) : " << transaction_number << std::endl;
			std::cout << "transaction_cancle_number : " << transaction_cancle_number << std::endl << std::endl;
			break;
		}
		else {	//거래 건수 찾은 경우
		full_file_content.erase(0, full_file_content.find(find_content_start) - find_content_start.length());	//분석시작부분 이전로그 삭제하기 위해
			find_content_start_number = full_file_content.find(find_content_start) - find_content_start.length(); //XX XX거래시작부터 시작
			find_content_end_number = full_file_content.find(find_content_end);
		}
		transaction_content = full_file_content.substr(	find_content_start_number,	//거래 내용은 거래 시작 문구부터
				(find_content_end_number - find_content_start_number + sizeof(find_content_end)));//( 거래 끝 문구 - 시작문구 + 끝 문구 길이 ) 개수만큼
		if(get_transaction_cancle(transaction_content, &transaction_cancle_flag) == 0){//거래 취소//해당 거래 취소인지 구하는 함수
			transaction_cancle_number++;//취소 건수 증가
		}
		else{
			get_transaction_time(transaction_content, &transaction_time);//거래 시간 구하는 함수
			if (transaction_content.find(find_recharge_transaction) != -1) {	//해당 거래건에서 카드 보충 문구가 있으면
				get_recharge_fare(transaction_content, &recharge_fare);	//보충 운임 얻기
				get_recharge_cardtype(transaction_content, &cardtype);//카드 타입 구하는 함수
				if (transaction_cancle_flag) {//거래 성공이면
					if(transaction_content.find(find_season_tichet_transaction) == -1){//해당거래에서 정기권 문구가 없는경우(일반 보충인 경우)
						get_recharge_cardnum(transaction_content, &cardnum);//카드 번호 구하는 함수
						get_recharge_balance(transaction_content, &recharge_balance);	//보충 잔액 구하는 함수
						get_recharge_after_balance(transaction_content, &recharge_after_balance);//보충 후 잔액 구하기
						transaction_flag = recharge;
					} else {//정기권 보충인 경우
						get_season_ticket_frequency(transaction_content, &season_ticket_frequency);	//정기권 횟수 구하는 함수
						transaction_flag = season_ticket_recharge;
					}
					recharge_transaction_number++;
				}
			} else if (transaction_content.find(find_cardsale_transaction)!= -1) {	//해당 거래건에서 카드 판매 문구가 있으면
				get_cardsale_fare(transaction_content, &cardsale_fare);	//판매 운임 얻기 // @suppress("Invalid arguments")
				get_cardsale_adult_number(transaction_content, &cardsale_adult_number);	//카드 판매 어른 발매 수량
				get_cardsale_adolescent_number(transaction_content, &cardsale_adolescent_number);	//카드 판매 청소년 발매 수량
				get_cardsale_seasonticket_number(transaction_content, &cardsale_seasonticket_number);	//카드 판매 정기권 발매 수량
				if (transaction_cancle_flag) {	//거래 성공이면
					transaction_flag = cardsale;
					cardsale_transaction_number++;
				}
			}
			++transaction_number;//성공 거래 건수 증가
			if(check_bill_shutter(transaction_content, &check_shutter_abnormal) >= 0 ){//셔터 열기 닫기에 오류가 있는 경우
				std::cout << transaction_number << "번째 거래 ( " << transacion_name[transaction_flag] << " )" << " 출력" << std::endl;
				std::cout << "거래 시간 : " << transaction_time << std::endl;
				if (transaction_flag == recharge) {
					std::cout << "카드 번호 : " << cardnum << std::endl << "보충 운임 : " << recharge_fare << std::endl << "카드 타입 : " << cardtype << std::endl << "카드 잔액 : "
					<< recharge_balance << std::endl << "보충 후 잔액 : "<< recharge_after_balance << std::endl;
				}
				else if (transaction_flag == season_ticket_recharge) {
					std::cout << "카드 번호 : " << cardnum << std::endl << "보충 운임 : " << recharge_fare << std::endl << "카드 타입 : " << cardtype << std::endl
					<< "정기권 횟수 : " << season_ticket_frequency << std::endl;
				}
				else if (transaction_flag == cardsale) {
					std::cout << "판매 운임 : " << cardsale_fare << std::endl
					<< "어른  발매 수량 : " << cardsale_adult_number << std::endl << "청소년 발매 수량 : " << cardsale_adolescent_number << std::endl
					<< "정기권 발매 수량 : " << cardsale_seasonticket_number << std::endl;
				}
				abnormal_discovery_flag = 1;//이상 발견
			}
			else if((transaction_flag == recharge) && (recharge_after_balance != recharge_fare + recharge_balance)){
				std::cout << transaction_number << "번째 거래 ( " << transacion_name[transaction_flag] << " )" << " 출력" << std::endl;
				std::cout << "거래 시간 : " << transaction_time << std::endl;
				std::cout << "보충 후 잔액이 (보충 운임 + 보충 전 잔액)과 다릅니다" << std::endl;
				std::cout << "카드 번호 : " << cardnum << std::endl << "보충 운임 : " << recharge_fare << std::endl << "카드 타입 : " << cardtype << std::endl << "카드 잔액 : "
				<< recharge_balance << std::endl << "보충 후 잔액 : "<< recharge_after_balance << std::endl;
				abnormal_discovery_flag = 1;//이상 발견
			}
			get_transaction_inventory_before(transaction_content, transaction_inventory_before);//거래 전 재고
			get_transaction_inventory_after (transaction_content, transaction_inventory_after);//거래 후 재고
			get_compare_inventory(transaction_inventory_before, transaction_inventory_after, &inventory_change);//거래 전후 차이 값
			if(abnormal_discovery_flag)//이상 발견
			{
				get_transaction_inventory_before(transaction_content, transaction_inventory_before);//거래 전 재고
				get_transaction_inventory_after (transaction_content, transaction_inventory_after);//거래 후 재고
				std::cout << "///// 거래 전 ///// | ///// 거래 후 /////" << std::endl;
				for(int i = 0; i < sizeof(transaction_inventory_before)/sizeof(transaction_inventory_before[0]); i++)
					std::cout << inventory_name[i] << " : " << transaction_inventory_before[i] << " | " << inventory_name[i] << " : " << transaction_inventory_after[i] << std::endl;
				std::cout << inventory_change << " : 거래 전후 차이 금액" << std::endl << std::endl;
			}
			if(inventory_change != cardsale_fare || inventory_change != recharge_fare){//투입 금액 비교
				std::cout << "거래 전후 차이 금액이 운임과 다릅니다" << std::endl;
			}
		}
		full_file_content.erase(0, find_content_end_number);//분석한 거래 끝자락부터 시작하기 위해
	}
}

void ATVM_analysis()//ATVM 분석 함수
{
	std::cout << "log의 종류를 입력하세요" << std::endl;
	std::cout << "ATVM 분석 함수 종료" << std::endl;
}
void PAU_analysis()//PAU 분석 함수
{
	std::cout << "log의 종류를 입력하세요" << std::endl;
	std::cout << "PAU 분석 함수 종료" << std::endl;
}
void AGM_analysis()//AGM 분석 함수
{
	std::cout << "log의 종류를 입력하세요" << std::endl;
	std::cout << "AGM 분석 함수 종료" << std::endl;
}

int get_transaction_cancle(std::string transaction_content, int *transaction_cancle_flags)//해당 거래 취소인지 구하는 함수
{
	std::string find_transaction_cancle = "거래 취소";//탐색 문자열
	*transaction_cancle_flags = transaction_content.find(find_transaction_cancle) == -1 ? 1 : 0;//문구 없으면(거래 성공) 1 문구 있으면(거래 취소) 0
	return *transaction_cancle_flags;
}

int get_transaction_fail(std::string transaction_content, int *transaction_fail_flags)//해당 거래 실패인지 구하는 함수
{
	std::string find_transaction_fail = "거래 실패";//탐색 문자열
	*transaction_fail_flags = transaction_content.find(find_transaction_fail) == -1 ? 1 : 0;//문구 없으면(거래 성공) 1 문구 있으면(거래 실패) 0
	return *transaction_fail_flags;
}

void get_cardsale_fare(std::string transaction_content, int *get_cardsale_fare)//총 판매 금액 운임 구하는 함수
{
	std::string find_cardsale_fare = "카드 총 판매 예정 금액 : ";//탐색 문자열
	int fare_digits = 6;//금액 자리수 값(십만 단위)
	*get_cardsale_fare = stoi(transaction_content.substr(transaction_content.find(find_cardsale_fare) + find_cardsale_fare.length(), fare_digits));//카드 총판매금액 만큼
}

void get_cardsale_adult_number(std::string transaction_content, int *cardsale_adult_number)//카드 판매 어른 발매 수량 구하는 함수
{
	std::string find_cardsale_adult_number = "일 반 용 카드 발매 수량 : ";//탐색 문자열
	if(transaction_content.find(find_cardsale_adult_number) != -1)//문구 있으면
		*cardsale_adult_number = transaction_content.at(transaction_content.find(find_cardsale_adult_number) + find_cardsale_adult_number.length() ) - '0'; // '?' 에서 '0'을빼면 숫자
	if(*cardsale_adult_number <= 0)
		*cardsale_adult_number = 0;
}

void get_cardsale_adolescent_number(std::string transaction_content, int *cardsale_adolescent_number)//카드 판매 청소년 발매 수량 구하는 함수
{
	std::string find_cardsale_adolescent_number = "청소년용 카드 발매 수량 : ";//탐색 문자열
	if(transaction_content.find(find_cardsale_adolescent_number) != -1)//문구 있으면
		*cardsale_adolescent_number = transaction_content.at(transaction_content.find(find_cardsale_adolescent_number) + find_cardsale_adolescent_number.length() ) - '0'; // '?' 에서 '0'을빼면 숫자
	if(*cardsale_adolescent_number <= 0)
		*cardsale_adolescent_number = 0;
}

void get_cardsale_seasonticket_number(std::string transaction_content, int *cardsale_seasonticket_number)//카드 판매 정기권 발매 수량 구하는 함수
{
	std::string find_cardsale_seasonticket_number = "정기권용 카드 발매 수량 : ";//탐색 문자열
	if(transaction_content.find(find_cardsale_seasonticket_number) != -1)//문구 있으면ㄴ
		*cardsale_seasonticket_number = transaction_content.at(transaction_content.find(find_cardsale_seasonticket_number) + find_cardsale_seasonticket_number.length() ) - '0'; // '?' 에서 '0'을빼면 숫자
	if(*cardsale_seasonticket_number <= 0)
		*cardsale_seasonticket_number = 0;
}

void get_recharge_fare(std::string transaction_content, int *recharge_fare)//총 보충 금액 운임 구하는 함수
{
	std::string find_recharge_fare = "보충 금액: ";//탐색 문자열
	int fare_digits = 6;//금액 자리수 값(십만 단위)
	*recharge_fare = stoi(transaction_content.substr(transaction_content.find(find_recharge_fare) + find_recharge_fare.length(), fare_digits));//카드 총 보충 금액 만큼
}

void get_recharge_balance(std::string transaction_content, int *recharge_balance)//보충 잔액 구하는 함수
{
	std::string find_recharge_balance = "카드 잔액: ";//탐색 문자열
	int fare_digits = 6;	//금액 자리수 값(십만 단위)
	*recharge_balance = stoi(transaction_content.substr(transaction_content.find(find_recharge_balance) + find_recharge_balance.length(), fare_digits));//카드 보충 전 금액 만큼
}

void get_recharge_cardnum(std::string transaction_content, std::string *cardnum)//카드 번호 구하는 함수
{
	std::string find_recharge_cardnum = "카드 번호: ";//탐색 문자열
	int num_digits = 16;//카드 자리수 값(최대 16자리 단위)
	*cardnum = transaction_content.substr(transaction_content.find(find_recharge_cardnum) +find_recharge_cardnum.length(), num_digits);	//카드 번호 만큼
}

void get_season_ticket_frequency(std::string transaction_content, int *season_ticket_frequency)//정기권 횟수 구하기
{
	std::string find_season_ticket_balance = "정기권 횟수: ";//탐색 문자열
	int fare_digits = 2;	//횟수 자리수 값(십단위)
	*season_ticket_frequency = stoi(transaction_content.substr(transaction_content.find(find_season_ticket_balance) + find_season_ticket_balance.length(), fare_digits));//정기권 횟수 만큼
}

void get_recharge_cardtype(std::string transaction_content, std::string *card_type)//카드 타입 구하는 함수
{
	std::string find_recharge_cardnum = "카드 타입: ";//탐색 문자열
	switch(transaction_content.at(transaction_content.find(find_recharge_cardnum) +find_recharge_cardnum.length())){
	case 'T' :
		*card_type = "T-money";
		break;
	case 'H' :
		*card_type = "Hanaro";
		break;
	case 'E' :
		*card_type = "Cashbee";
		break;
	case 'M' :
		*card_type = "Mybe";
		break;
	case 'S' :
		*card_type = "Season_Ticket";
		break;
	default:
		*card_type = "error";
		break;
	}
}

void get_recharge_after_balance(std::string transaction_content, int *recharge_after_balance)//보충 후 잔액 구하기
{
	std::string find_recharge_after_balance = "보충 후 잔액: ";//탐색 문자열
	int fare_digits = 6;	//금액 자리수 값(십만 단위)
	*recharge_after_balance = stoi(transaction_content.substr(transaction_content.find(find_recharge_after_balance) + find_recharge_after_balance.length(), fare_digits));//카드 충전 후 금액 만큼
}

int check_bill_shutter(std::string transaction_content, int *check_shutter_abnormal)//성공했을시 bill shutter 열리고 닫혔는지 확인
{
	std::string find_command_send_open = "명령 송신 | 셧터 열기";//탐색 문자열
	std::string find_command_receive_open = "명령 수신 | 지폐처리장치, 셧터 열기 결과: ";//탐색 문자열           //1
	std::string find_command_send_close = "명령 송신 | 셧터 닫기";//탐색 문자열
	std::string find_command_receive_close = "셧터 닫기 | 지폐처리장치, 수행 결과: ["; //탐색 문자열             //1]
	*check_shutter_abnormal = -1;//이상없을시
	if(transaction_content.find(find_command_send_open) == -1)//셧터 열기 명령 송신하지 않았다면
		*check_shutter_abnormal = 0;
	if((transaction_content.at(transaction_content.find(find_command_receive_open) + find_command_receive_open.length() ) - '0') == 0) // '?' 에서 '0'을빼면 숫자//셧터 열기 결과:  1이 아닌 0인 경우
		*check_shutter_abnormal = 1;
	if(transaction_content.find(find_command_send_close) == -1)//셧터 열기 명령 송신 후 셧터 닫기 명령 송신하지 않았다면
		*check_shutter_abnormal = 2;
	if((transaction_content.at(transaction_content.find(find_command_receive_close) + find_command_receive_close.length() ) - '0') == 0) // '?' 에서 '0'을빼면 숫자//셧터 닫기 결과:  1이 아닌 0인 경우
		*check_shutter_abnormal = 3;
	if(*check_shutter_abnormal >= 0)
		std::cout << bill_shutter_state[*check_shutter_abnormal] << std::endl;
	return *check_shutter_abnormal;
}

void get_transaction_time(std::string transaction_content, std::string *transaction_time)//거래 했을 때 거래 시간 구하는 함수
{
	std::string find_transaction_time = "[";//탐색 문자열
	std::string transaction_time_length = "00:00:00.000";
	*transaction_time = transaction_content.substr(transaction_content.find(find_transaction_time) + find_transaction_time.length(), transaction_time_length.length());//거래 시간 만큼
}

void get_transaction_inventory_before(std::string transaction_content, int transaction_inventory_before[])//거래 전 재고
{
	std::string find_transaction_inventory_before = "거래 전 기기내 재고 금액 | 총재고: ";//탐색 문자열
	std::string find_transaction_inventory[] = {
		  "동전함     10원 수량:", "동전함     50원 수량:", "동전함    100원 수량:", "동전함    500원 수량:"
		, "지폐함   1000원 수량:", "지폐함   5000원 수량:", "지폐함  10000원 수량:", "지폐함  50000원 수량:"
	};
	int inventory_digits = 4;	//동전, 지폐 개수의 자리수 값(최대 천 단위)
	if(transaction_content.find(find_transaction_inventory_before) != -1){//거래 전 기기 재고 금액 존재할 경우
		for(int i = 0; i < sizeof(find_transaction_inventory)/sizeof(find_transaction_inventory[0]); i++)
			transaction_inventory_before[i] = stoi(transaction_content.substr(transaction_content.find(find_transaction_inventory[i])    + find_transaction_inventory[i].length()   , inventory_digits));
	}
}

void get_transaction_inventory_after(std::string transaction_content, int transaction_inventory_after[])//거래 후 재고
{
	std::string find_transaction_inventory_after = "거래 후 기기내 재고 금액 | 총재고: ";//탐색 문자열
	std::string find_transaction_inventory[] = {
		  "동전함     10원 수량:", "동전함     50원 수량:", "동전함    100원 수량:", "동전함    500원 수량:"
		, "지폐함   1000원 수량:", "지폐함   5000원 수량:", "지폐함  10000원 수량:", "지폐함  50000원 수량:"
	};
	int inventory_digits = 4;	//동전, 지폐 개수의 자리수 값(최대 천 단위)
	if(transaction_content.find(find_transaction_inventory_after) != -1){//거래 전 기기 재고 금액 존재할 경우
		transaction_content.erase(0, transaction_content.find(find_transaction_inventory_after) + find_transaction_inventory_after.length());//앞의 거래전 금액 텍스트 삭제하기 위해
		for(int i = 0; i < sizeof(find_transaction_inventory)/sizeof(find_transaction_inventory[0]); i++)
			transaction_inventory_after[i] = stoi(transaction_content.substr(transaction_content.find(find_transaction_inventory[i])    + find_transaction_inventory[i].length()   , inventory_digits));
	}
}

int get_compare_inventory(int transaction_inventory_before[], int transaction_inventory_after[], int *inventory_change)//거래 전 재고와 거래 후 재고 비교 후 차이값 반환
{//
	int compare_value = 0;//전후재고 차이값(거래성공시 운임이랑 같아야함)
	int currency_unit = 5;//화폐 금액 단위
	std::string difference_amount;//차이 금액
	for(int i = 0; i < 8; i++){
		if(i%2 == 0)
			currency_unit *= 2;
		else
			currency_unit *= 5;
		if((transaction_inventory_after[i] - transaction_inventory_before[i]) > 0)//차액이 있을떄
			compare_value += ((transaction_inventory_after[i] - transaction_inventory_before[i]) * currency_unit);//각 단위 합산 = 각단위 차이개수 * 단위 값
	}
	std::cout << "///투입금액///" << std::endl;
	for(int i = 0; i < 8; i++)
		if((transaction_inventory_after[i] - transaction_inventory_before[i]) > 0)//차액이 있을떄
			std::cout << inventory_name[i] << " : " << transaction_inventory_after[i] - transaction_inventory_before[i] << std::endl;
	*inventory_change = compare_value;//거래 전후 재고 차이값 저장
	return compare_value;
}

