/************************************************************
		Micromouse �v���O����
		2007�N~2012�N	H8/3694f�ł��^�p�����
		2013/03/01		LPC�����ɈڐA					���J�� ���l
		2013/09/20		�W�����̂��ߐF�X�M��				���� �L��
 ************************************************************/
/*
==============================================================
 Name        : main.c
 Copyright   : Copyright (C) ����c��w�}�C�N���}�E�X�N���u
 Description : main definition

 �X�V����
 2016/2/12�@�[�R�@�ꕔ�R�����g�ǉ�
 2017/6/27 �W�@�A���T�����s�̖������ł�����
==============================================================
 */

#define EXTERN
#include "global.h"

/*===========================================================
		���C���֐�
===========================================================*/
int main(void) {

	//====�ϐ��錾====
	char mode = 0;
	
	
	//====������====
	R_PG_Clock_Set();					//�N���b�N�ݒ�

	port_Init();						//port�̏�����
	val_Init();							//�e��ϐ��̏�����
	timer_Init();						//�^�C�}�n�̏�����
	sensor_Init();						//�Z���T�n�̏�����
	uart_Init();					//�V���A���ʐM�̏�����
	
	
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

		//====���[�h�Z���N�g====
		select_mode(&mode);
		ms_wait(100);
		//----�I�����ڂ̎��s----
		switch(mode){
		case 0:	//----��l�����----
			get_base();											//get_base()��sensor.c�Ɋ֐���`���� �@����̂��߂̕Ǌ�l�擾
			//----�����V���A�����M----
			uart_printf("base_l = %3d, ", base_l);				//UART_printf()��uart.c�Ɋ֐���`����
			uart_printf("base_r = %3d\r", base_r);
			ms_wait(500);
			break;
		case 1:	//----�ꎟ�T�����s----
			

			goal_x = GOAL_X;									//�S�[�����W��ݒ�@�@GOAL_X��global.h�Ƀ}�N����`����
			goal_y = GOAL_Y;									//�S�[�����W��ݒ�@�@GOAL_Y��global.h�Ƀ}�N����`����

			turn_R90();											//�E�������@�@turn_R90()��drive.c�Ɋ֐���`����
			set_position();										//����ǂɂ��ē��������̒����ց@�@set_position()��drive.c�Ɋ֐���`����
			Wait;												//�ҋ@�@�@WAIT��auxiliary.h�Ƀ}�N����`����
			turn_L90();											//���������@�@turn_L90()��drive.c�Ɋ֐���`����
			set_position();										//����ǂɂ��ē�k�����̒�����
			Wait;												//�ҋ@

			get_wall_info();									//�Ǐ��̏�����     get_wall_info()��sensor.c�Ɋ֐���`����
			searchA();											//�S�[�����܂ŒT���C�i�s����@searchA()��search.c�Ɋ֐���`����
			goal_x = goal_y = 0;								//�S�[�����W���X�^�[�g���ɐݒ�
			Wait;												//�ҋ@
			searchA();											//�߂��Ă���

			goal_x = GOAL_X;									//�S�[�����W�ݒ�
			goal_y = GOAL_Y;									//�S�[�����W�ݒ�

			PIN_L(MDR_REF);										//�X�e�b�s���O���[�^�̗㎥��؂�@�@PIN_L��port.h�ɐ�������
			break;

			//----�A���T�����s----
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

			turn_180();									//180�x��]
			turn_dir(DIR_TURN_180);

			PIN_L(MDR_REF);
			break;


			/////////////////////////////////�@�@���̓񎟒T�����s�ƃX�����[�����s�͖�����
			//----�񎟍������s----
		case 3:
			PIN_H(MDR_REF);										//PIN_H��port.h�ɐ�������
			PIN_L(MDR_REF);
			break;

			//----�X�����[�����s----
		case 4:
			PIN_H(MDR_REF);
			PIN_L(MDR_REF);
			break;
			//////////////////////////////////

			//----���s�e�X�g----
		case 5:
			Wait;
			test_drive(&mode);									//test_drive()��drive.c�Ɋ֐���`����
			ms_wait(100);
			break;

			//----�Z���T�l, �����m�F----
			//LED�_���͗v�ύX
		default:
			Wait;
			val_Init();
			MF.FLAG.CTRL = 1;	//���䋖��
			while(!RET){
				pins_write(DISP_LEDS, 0, LED_NUM);											//pins_write()��port.c�Ɋ֐���`����
				uart_printf("ad_l: %4d ad_fl:%4d ad_fr:%4d ad_r:%4d ", ad_l, ad_fl, ad_fr, ad_r);
				uart_printf(" | dif_l: %4d dif_r:%4d\r\n", dif_l, dif_r);
				//----LED��4�̏ꍇ----
				if(ad_fr > WALL_BASE_F){
					// �����Aad_l�ɂȂ��Ă܂�����I�I
					pin_write(DISP_LEDS[0], ON);											//pin_write()��port.c�Ɋ֐���`����
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
