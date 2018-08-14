/*
==============================================================
 Name        : interrupt.c
 Copyright   : Copyright (C) 早稲田大学マイクロマウスクラブ
 Description : 割り込み関数たちです．(多分)lpc11xx.hでプロトタイプ宣言されています．

 更新履歴
 2015/12/4		山上	コメント追加、tpの処理を分かりやすく変更
 2016/2/24		山上	2016年度用にピン設定を変更
 2017/10/14		嶋崎 加減速時の条件分岐ロジック誤りを修正(カウンタがオーバーフローする)
==============================================================
 */
#include "global.h"

//+++++++++++++++++++++++++++++++++++++++++++++++
//Mtu3IcCmDIntFunction
//割り込み関数、左モータの管理を行う
// 引数：無し
// 戻り値：無し
//+++++++++++++++++++++++++++++++++++++++++++++++
void Mtu3IcCmDIntFunc(void){		//====左モータ割り込み====

	uint16_t paraL0 = 40000;
	uint16_t paraL1 = 40000;

	pin_write(PA4,0);
	pin_write(PA6,0);

		//====加減速処理====
		//----減速処理----
		if(MF.FLAG.DECL){															//減速フラグが1である場合
			if(t_cnt_l > minindex) t_cnt_l--; //最低速度以上の場合テーブルカウンタを減らすことで減速
		}
		//----加速処理----
		else if(MF.FLAG.ACCL){														//加速フラグが1である場合
			if(t_cnt_l < maxindex) t_cnt_l++; //最低速度以下の場合テーブルカウンタを増やすことで加速
		}

		//----停止中の処理----
		if(MF.FLAG.STOP){															//停止フラグが1である場合
			t_cnt_l = minindex;														//テーブルカウンタをクリア
			interval_l = DEFGRC;													//左右モータもデフォルト
			paraL0 = interval_l;											//左モータの割り込み期設定
			//----デフォルトの速度----
		}else if(MF.FLAG.DEF){														//定速フラグが1である場合
			paraL0 = DEFGRC - dl;											//定速に対して制御をした値に設定
			//----その他のとき----
		}else{
			paraL0 = table[t_cnt_l] - dl;									//左モータインターバル設定
		}

		paraL1 = paraL0 + 4000;									//出力パルス部分を設定

		R_PG_Timer_SetTGR_C_MTU_U0_C3(paraL0);
		R_PG_Timer_SetTGR_D_MTU_U0_C3(paraL1);

}

void Mtu3IcCmCIntFunc(void){
	pin_write(PA4,1);
	pin_write(PA6,0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++
//Mtu4IcCmDIntFunc
//	16ビットタイマー1の割り込み関数、右モータの管理を行う
// 引数：無し
// 戻り値：無し
//+++++++++++++++++++++++++++++++++++++++++++++++
void Mtu4IcCmDIntFunc(void){			//====右モータ割り込み====
	uint16_t paraR0 = 40000;
	uint16_t paraR1 = 40000;

	pin_write(P54,1);
	pin_write(P55,0);

	//====加減速処理====
	//----減速処理----
	if(MF.FLAG.DECL){															//減速フラグが1である場合
		if(t_cnt_r > minindex) t_cnt_r--; //最低速度以上の場合テーブルカウンタを減らすことで減速
	}
	//----加速処理----
	else if(MF.FLAG.ACCL){														//加速フラグが1である場合
		if(t_cnt_r < maxindex) t_cnt_r++; //最低速度以下の場合テーブルカウンタを増やすことで加速
	}
	//----停止中の処理----
	if(MF.FLAG.STOP){															//停止フラグが1である場合
		t_cnt_r = minindex;														//テーブルカウンタをクリア
		interval_r = DEFGRC;													//左右モータもデフォルト
		paraR0 = interval_r;											//右モータの割り込み期設定
		//----デフォルトの速度----
	}else if(MF.FLAG.DEF){														//定速フラグが1である場合
		paraR0 = DEFGRC - dr;											//定速に対して制御をした値に設定
		//----その他のとき----
	}else{
		paraR0 = table[t_cnt_r] - dr;									//右モータインターバル設定
	}

	paraR1 = paraR0 + 4000;									//出力パルス部分を設定

	R_PG_Timer_SetTGR_C_MTU_U0_C4(paraR0);
	R_PG_Timer_SetTGR_D_MTU_U0_C4(paraR1);

}

void Mtu4IcCmCIntFunc(void){
	pin_write(P54,1);
	pin_write(P55,0);
}
/*------------------------------------------------------------
		32bitタイマ割り込み
------------------------------------------------------------*/
//+++++++++++++++++++++++++++++++++++++++++++++++
//Cmt0IntFunc
//32ビットタイマー0の割り込み関数、センサ値の取得と制御を行う
// 引数：無し
// 戻り値：無し
//+++++++++++++++++++++++++++++++++++++++++++++++
void Cmt1IntFunc(void){
	pulse_l++;
	 
	switch(tp){
	//----センサ処理----
	case 0:
		//左センサ値の取得
		pin_write(PE1,1);								//発光部LEDをON
		
		S12AD.ADANS0.WORD = 0x02;
		R_PG_ADC_12_StartConversionSW_S12AD0();					//光が強まるまで少し待機
		R_PG_ADC_12_GetResult_S12AD0(ad_res);
		ad_l = ad_res[1];										//受光部のアナログ値を記録
		
		break;
	case 1:
		pin_write(PE1,0);
		
		//正面センサ値の取得
		pin_write(PE0,1);								//発光部LEDをON
		pin_write(PE2,1);								//発光部LEDをON
		pin_write(PE4,1);
		

		S12AD.ADANS0.WORD = 0x01;
		R_PG_ADC_12_StartConversionSW_S12AD0();
		R_PG_ADC_12_GetResult_S12AD0(ad_res);
		ad_fr = ad_res[0];
		
		S12AD.ADANS0.WORD = 0x04;
		R_PG_ADC_12_StartConversionSW_S12AD0();
		R_PG_ADC_12_GetResult_S12AD0(ad_res);
		ad_ff = ad_res[2];
		
		S12AD.ADANS0.WORD = 0x0f;
		R_PG_ADC_12_StartConversionSW_S12AD0();
		R_PG_ADC_12_GetResult_S12AD0(ad_res);
		ad_fl = ad_res[4];
		

		break;
	case 2:
		pin_write(PE0,0);								//発光部LEDをON
		pin_write(PE2,0);								//発光部LEDをON
		pin_write(PE4,0);

	
		//右センサ値の取得
		pin_write(PE3,1);								//発光部LEDをON
		//stay(1000);
		
		//S12AD.ADCSR.BIT.ADST = 0;

		S12AD.ADANS0.WORD = 0x08;
		R_PG_ADC_12_StartConversionSW_S12AD0();
		//S12AD.ADCSR.BIT.ADST = 1;
		R_PG_ADC_12_GetResult_S12AD0(ad_res);

		ad_r = ad_res[3];								//受光部のアナログ値を記録
//		UART_printf("[%4d]\n", ad_l);
															//発光部LEDをOFF
		break;

		//----制御処理----
	case 3:
		pin_write(PE3,0);
		
		//制御フラグがあれば制御
		if(MF.FLAG.CTRL){
			//基準値からの差を見る
			dif_l = (int16_t)(ad_l - base_l);
			dif_r = (int16_t)(ad_r - base_r);

			//制御の判断
			if((SREF_MIN_L < dif_l) && (dif_l < SREF_MAX_L))
				dl = CONT * dif_l;	//比例制御値を決定
			else
				dl = 0;	//基準から外れていれば制御はかけない
			if((SREF_MIN_R < dif_r) && (dif_r < SREF_MAX_R))
				dr = CONT * dif_r;
			else
				dr = 0;

		}else{
			//制御フラグがなければ制御値0
			dl = dr = 0;
		}
		break;
	}

	//====タスクポインタを進める====
	tp = (tp+1) % 4;

}

/*void S12ad0IntFunc(void){
	R_PG_ADC_12_GetResult_S12AD0(ad_res);
}*/
