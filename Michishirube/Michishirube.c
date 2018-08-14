/************************************************************
		Micromouse プログラム
		2007年~2012年	H8/3694f版が運用される
		2013/03/01		LPC向けに移植					長谷川 健人
		2013/09/20		標準化のため色々弄る				高橋 有礼
 ************************************************************/
/*
==============================================================
 Name        : main.c
 Copyright   : Copyright (C) 早稲田大学マイクロマウスクラブ
 Description : main definition

 更新履歴
 2016/2/12　深山　一部コメント追加
 2017/6/27 標　連続探索走行の未完成版を実装
==============================================================
 */

#define EXTERN
#include "global.h"

/*===========================================================
		メイン関数
===========================================================*/
int main(void) {

	//====変数宣言====
	char mode = 0;
	
	
	//====初期化====
	R_PG_Clock_Set();					//クロック設定

	port_Init();						//portの初期化
	val_Init();							//各種変数の初期化
	timer_Init();						//タイマ系の初期化
	sensor_Init();						//センサ系の初期化
	uart_Init();					//シリアル通信の初期化
	
	
	//timer_start();
	while(1){
		pin_write(PB6,1);
		ms_wait(500);
		pin_write(PB7,1);
		ms_wait(500);
		pin_write(PC2,1);
		ms_wait(500);
		pin_write(PC3,1);
		ms_wait(500);
		pin_write(PB6,0);
		pin_write(PB7,0);
		pin_write(PC2,0);
		pin_write(PC3,0);
		ms_wait(500);
	}
	
	
	


	while(1){ // Main Loop

		//====モードセレクト====
		select_mode(&mode);
		ms_wait(100);
		//----選択項目の実行----
		switch(mode){
		case 0:	//----基準値を取る----
			get_base();											//get_base()はsensor.cに関数定義あり 　制御のための壁基準値取得
			//----情報をシリアル送信----
			uart_printf("base_l = %3d, ", base_l);				//UART_printf()はuart.cに関数定義あり
			uart_printf("base_r = %3d\r", base_r);
			ms_wait(500);
			break;
		case 1:	//----一次探索走行----
			

			goal_x = GOAL_X;									//ゴール座標を設定　　GOAL_Xはglobal.hにマクロ定義あり
			goal_y = GOAL_Y;									//ゴール座標を設定　　GOAL_Yはglobal.hにマクロ定義あり

			turn_R90();											//右を向く　　turn_R90()はdrive.cに関数定義あり
			set_position();										//後ろを壁にあて東西方向の中央へ　　set_position()はdrive.cに関数定義あり
			Wait;												//待機　　WAITはauxiliary.hにマクロ定義あり
			turn_L90();											//左を向く　　turn_L90()はdrive.cに関数定義あり
			set_position();										//後ろを壁にあて南北方向の中央へ
			Wait;												//待機

			get_wall_info();									//壁情報の初期化     get_wall_info()はsensor.cに関数定義あり
			searchA();											//ゴール区画まで探索，進行する　searchA()はsearch.cに関数定義あり
			goal_x = goal_y = 0;								//ゴール座標をスタート区画に設定
			Wait;												//待機
			searchA();											//戻ってくる

			goal_x = GOAL_X;									//ゴール座標設定
			goal_y = GOAL_Y;									//ゴール座標設定

			PIN_L(MDR_REF);										//ステッピングモータの励磁を切る　　PIN_Lはport.hに説明あり
			break;

			//----連続探索走行----
		case 2:
			PIN_H(MDR_REF);

			goal_x = GOAL_X;
			goal_y = GOAL_Y;

			turn_R90();
			set_position();
			Wait;
			turn_L90();
			set_position();
			Wait;

			searchSA();
			goal_x = goal_y = 0;
			searchSA();
			goal_x = GOAL_X;
			goal_y = GOAL_Y;

			turn_180();									//180度回転
			turn_dir(DIR_TURN_180);

			PIN_L(MDR_REF);
			break;


			/////////////////////////////////　　↓の二次探索走行とスラローム走行は未実装
			//----二次高速走行----
		case 3:
			PIN_H(MDR_REF);										//PIN_Hはport.hに説明あり
			PIN_L(MDR_REF);
			break;

			//----スラローム走行----
		case 4:
			PIN_H(MDR_REF);
			PIN_L(MDR_REF);
			break;
			//////////////////////////////////

			//----走行テスト----
		case 5:
			Wait;
			test_drive(&mode);									//test_drive()はdrive.cに関数定義あり
			ms_wait(100);
			break;

			//----センサ値, 差を確認----
			//LED点灯は要変更
		default:
			Wait;
			val_Init();
			MF.FLAG.CTRL = 1;	//制御許可
			while(!RET){
				pins_write(DISP_LEDS, 0, LED_NUM);											//pins_write()はport.cに関数定義あり
				uart_printf("ad_l: %4d ad_fl:%4d ad_fr:%4d ad_r:%4d ", ad_l, ad_fl, ad_fr, ad_r);
				uart_printf(" | dif_l: %4d dif_r:%4d\r\n", dif_l, dif_r);
				//----LEDが4つの場合----
				if(ad_fr > WALL_BASE_F){
					// ここ、ad_lになってましたよ！！
					pin_write(DISP_LEDS[0], ON);											//pin_write()はport.cに関数定義あり
				}
				if(ad_r > WALL_BASE_R){
					pin_write(DISP_LEDS[1], ON);
				}
				if(ad_l > WALL_BASE_L){
					pin_write(DISP_LEDS[2], ON);
				}
				if(ad_fl > WALL_BASE_F){
					pin_write(DISP_LEDS[3], ON);
				}
				ms_wait(1000);

			}
			MF.FLAG.CTRL = 0;
			ms_wait(100);
			break;
		}
		ms_wait(100);
	}

	return 0 ;
}
