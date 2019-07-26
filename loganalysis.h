/*
 * loganalysis.h
 *
 *  Created on: 2019. 7. 8.
 *      Author: aoska
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef LOGANALYSIS_H_
#define LOGANALYSIS_H_

enum afc_machine_number{//프로그램 번호
	  ACM
	, ATVM
	, PAU
	, AGM
};

static const std::string afc_machine_name[] = {// 프로그램 이름
	  "ACM"
	, "ATVM"
	, "PAU"
	, "AGM"
};

enum acm_log_number{
	  Event
	, IOModule
	, IOProtocol
	, MainLog
	, Transaction
	, StepProcessor
	, PassengerProcessor
	, CoinModule
	, CoinProtocol
	, BillModule
	, BillProtocol
	, RechargeModule
	, RechargeProtocol
	, NetWork
	, EB
	, TM
	, USM
	, USM_H
};

static const std::string acm_log_name[] = {
	  "Event"
	, "IOModule"
	, "IOProtocol"
	, "MainLog"
	, "Transaction"
	, "StepProcessor"
	, "PassengerProcessor"
	, "CoinModule"
	, "CoinProtocol"
	, "BillModule"
	, "BillProtocol"
	, "RechargeModule"
	, "RechargeProtocol"
	, "NetWork"
	, "EB"
	, "TM"
	, "USM"
	, "USM_H"
};

enum transacion_number{//거래 번호
	  recharge
	, season_ticket_recharge
	, cardsale
};

static const std::string transacion_name[] = {// 거래 이름
	  "recharge"
	, "season_ticket_recharge"
	, "cardsale"
};

enum bill_shutter_number{//지폐 상태 번호
	  not_send_bill_open
	, not_receive_bill_open
	, not_send_bill_close
	, not_receive_bill_close
};

static const std::string bill_shutter_state[] = {// 지폐 상태명
	  "not_send_bill_open"
	, "not_receive_bill_open"
	, "not_send_bill_close"
	, "not_receive_bill_close"
};

enum inventory_number{//재고
	  coin_10
	, coin_50
	, coin_100
	, coin_500
	, bill_1000
	, bill_5000
	, bill_10000
	, bill_50000
};

static const std::string inventory_name[] = {//재고 각 이름
	  "coin_10   "
	, "coin_50   "
	, "coin_100  "
	, "coin_500  "
	, "bill_1000 "
	, "bill_5000 "
	, "bill_10000"
	, "bill_50000"
};

class Clog_analysis {
public:
//변수
//함수
	Clog_analysis();//생성자
	Clog_analysis(std::string log_filename);//생성자
	virtual ~Clog_analysis();
	int main(int argc, char* argv[]);
};

#endif /* LOGANALYSIS_H_ */
