/*
==============================================================
 Name        : init.c
 Copyright   : Copyright (C) 早稲田大学マイクロマウスクラブ
 Description : 初期化関数たちです．

 更新履歴
 2015/12/4		山上	一部コメント追加、表示用LEDの設定を追加
 2016/2/6		深山	一部コメント追加
 2016/2/8		山上	port_Init関数内にPIN_SET(MDR_M3, 0);を追加
 2016/2/12		山上	sensor_Init関数内に発光部のIOCON設定を追加
 2016/2/24		山上	2016年度用にピン設定を変更
==============================================================
 */

#include "global.h"
/*============================================================
		初期化関数
============================================================*/
// 各種入出力の設定
void port_Init(void){
	PORTB.PMR.BIT.B6 = 0;
	PORTB.PMR.BIT.B7 = 0;

	PORTB.PDR.BIT.B6 = 0;
	PORTB.PDR.BIT.B7 = 0;
	
	PORTB.PCR.BIT.B6 = 0;
	PORTB.PCR.BIT.B7 = 0;
	
	PORT.PSRA.BIT.PSEL6 = 0;
	PORT.PSRA.BIT.PSEL7 = 0;
	
	//I/O関係のポート設定
	R_PG_IO_PORT_Set_P5();		//モタドラへの指示
	R_PG_IO_PORT_Set_PA();		//モタドラへの指示
	//R_PG_IO_PORT_Set_PB();
	R_PG_IO_PORT_Set_PC();		//

	PORTB.PDR.BIT.B6 = 1;
	PORTB.PDR.BIT.B7 = 1;
	PORTB.PODR.BIT.B6 = 1;
	PORTB.PODR.BIT.B7 = 1;
				
	PIN_L(MDR_REF);				//リファレンスを切る　ピン2_1の出力をLowに設定

	set_dir(FORWARD);			//進行方向を前へ

}


// センサー設定
void sensor_Init(void){
	//センサ受光部のポート設定 左から順に
	R_PG_ADC_12_Set_S12AD0();
	//センサ発光部初期設定
	R_PG_IO_PORT_Set_PE();
}


// センサ系，走行系，探索系　の変数初期化
void val_Init(void){
	//----センサ系----
	tp = 0;
	ad_l = ad_r = ad_fr = ad_fl = 0;
	base_l = base_r = 0;

	//----走行系----
	maxindex = MAXSPEED_S;			//最高速度初期化     MAXSPEED_Sはglobal.hにマクロ定義あり
	minindex = MINSPEED_S;			//最低速度初期化     MINSPEED_Sはglobal.hにマクロ定義あり
	MF.FLAGS = 0x80;				//フラグクリア＆停止状態  0x80=0b10000000

	//----探索系----
	goal_x = GOAL_X;        		//GOAL_Xはglobal.hにマクロ定義あり
	goal_y = GOAL_Y;        		//GOAL_Yはglobal.hにマクロ定義あり
	map_Init();						//マップの初期化
	PRELOC.PLANE = 0x00;			//現在地の初期化
	m_dir = 0;						//マウス方向の初期化

}

// タイマ初期化
void timer_Init(void){

	R_PG_Timer_Set_CMT_U0_C0();
	R_PG_Timer_Set_CMT_U0_C1();

	R_PG_Timer_Set_MTU_U0_C0();		//ブザー用初期設定
	R_PG_Timer_Set_MTU_U0_C1();		//右エンコーダ
	R_PG_Timer_Set_MTU_U0_C2();		//左エンコーダ
	R_PG_Timer_Set_MTU_U0_C3();		//左モータ
	R_PG_Timer_Set_MTU_U0_C4();		//右モータ

}
