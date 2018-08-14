/*
==============================================================
 Name        : drive.h
 Copyright   : Copyright (C) 早稲田大学マイクロマウスクラブ
 Description : 走行関連の関数たちです．

  更新履歴
 2015/12/11		山上	コメントとSLA7078用のマクロを追加
 2016/2/24		山上	2016年度用にピン設定を変更
==============================================================
*/

/*ヘッダファイルの読み込み*/
#include "port.h"

#ifndef DRIVE_H_										//対応ファイルで一度も読み込まれていない場合以下を定義

	#define DRIVE_H_									//読み込んだことを表す

/*============================================================
		各種定数･変数宣言
============================================================*/
	//----ドライバIC関連----
	#define MDR_CLK_L	P0_8		//クロック端子左
	#define MDR_CLK_R	P1_9		//クロック端子右
// 上OK

	#define MDR_CW_L	P2_2		//IC分配回路のCW/CCW左
	#define MDR_CW_R	P3_5		//IC分配回路のCW/CCW右
	#define MDR_REF		P2_9		//リファレンス

	#define MDR_M1		P0_6		//励磁切り替えロジック1
	#define MDR_M2		P3_4		//励磁切り替えロジック2
	#define MDR_M3		P2_1		//励磁切り替えロジック3

	/* 励磁切り替えロジック
	・SLA7078MPRT
	  M1 | M2 | M3 |励磁モード
	   L |  L |  L | 2相励磁(Mode8固定)
	   L |  H |  L | 2相励磁(ModeF固定)
	   H |  L |  L | 1-2相励磁(2分割)
	   H |  H |  L | 1-2相励磁(ModeF固定)
	   L |  L |  H |  W1-2相（ 4分割）
	   L |  H |  H | 4W1-2相（16分割）
	   H |  L |  H | 2W1-2相（ 8分割）
	   H |  H |  H | スリープモード2

	・SLA7073MPRT
	  M1 | M2 | 励磁モード
	   L |  L | 4W1-2相（16分割）
	   L |  H | 2W1-2相（ 8分割）
	   H |  L |  W1-2相（ 4分割）
	   H |  H |   1-2相（ 2分割）
	*/



	//----動作方向関連----
	#define MT_FWD_L	1			//CW/CCWで前に進む出力(左)
	#define MT_BACK_L	0			//CW/CCWで後ろに進む出力(左)
	#define MT_FWD_R	1			//CW/CCWで前に進む出力(右)
	#define MT_BACK_R	0		//CW/CCWで後ろに進む出力(右)

	#define FORWARD	0x00			//前進向き
	#define BACK	0x11			//後退
	#define TURN_L	0x01			//回転向き(左)
	#define TURN_R	0x10			//回転向き(右)

	//----方向転換用定数----
	#define DIR_TURN_R90	0x01	//右90度回転
	#define DIR_TURN_L90	0xff	//左90度回転
	#define DIR_TURN_180	0x02	//180度回転


	//====変数====
#ifdef EXTERN										//対応ファイルでEXTERNが定義されている場合
	/*グローバル変数の定義*/
	const uint16_t table[] = {
		#include "table.h"
	};												//table.hに貼り付けた値を保持する配列
	volatile uint16_t t_cnt_l, t_cnt_r;					//テーブルカウンタ
	volatile uint16_t minindex, maxindex;				//最低速度・最大速度
	volatile uint16_t interval_l, interval_r;			//左右インターバル
	volatile uint16_t pulse_l, pulse_r;					//左右パルスカウンタ
	int16_t	dl, dr;										//比例制御量
#else												//対応ファイルでEXTERNが定義されていない場合
	/*グローバル変数の宣言*/
	extern const uint16_t table[];					//table.hに貼り付けた値を保持する配列
	extern volatile uint16_t t_cnt_l, t_cnt_r;			//テーブルカウンタ
	extern volatile uint16_t minindex, maxindex;		//最低速度・最大速度
	extern volatile uint16_t interval_l, interval_r;	//左右インターバル
	extern volatile uint16_t pulse_l, pulse_r;			//左右パルスカウンタ
	extern int16_t	dl, dr;								//比例制御量
#endif

/*============================================================
		関数プロトタイプ宣言
============================================================*/
	//====走行系====
	//----基幹関数----
	void driveA(uint16_t, unsigned char);	//加速走行
	void driveD(uint16_t, unsigned char);	//減速走行
	void driveAD(uint16_t, unsigned char);	//加減速走行
	void driveU(uint16_t, unsigned char);	//等速走行(前の速度を維持)
	void driveC(uint16_t, unsigned char);	//定速走行(あらかじめ決まった速度)
	void set_dir(unsigned char);		//進む方向の設定

	void drive_start();
	void drive_stop(unsigned char);

	//----簡易上位関数----
	void half_sectionA();		//加速半区画
	void half_sectionD();		//減速半区画
	void a_section();			//加減速一区画
	void a_sectionU();			//等速一区画
	void turn_R90();			//右90回転
	void turn_L90();			//左90回転
	void turn_180();			//180度回転
	void set_position();		//位置合わせ

	//----走行試験----
	void test_drive(char *mode);	//走行試験

#endif /* DRIVE_H_ */
