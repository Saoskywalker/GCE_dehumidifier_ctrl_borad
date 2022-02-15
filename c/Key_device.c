
#include "General.h"

/* Private defines -----------------------------------------------------------*/
GCE_XDATA UI32 S_Key_Value_Flag = 0; //当前轮按键标志

GCE_XDATA UI16 S_Key_Data = 0; //当前按键按下值  未滤波

GCE_XDATA UI08 S_Key_New = 0;			  //新键值
GCE_XDATA UI08 S_Key_Old = 0;			  //上一次按键值
GCE_XDATA UI08 S_Key_One_Buf = 0;		  //滤波后单按键按下暂存
GCE_XDATA UI16 S_Key_Last = 0;			  //长按键按下时间
GCE_XDATA UI16 S_Comp_Test_Key_Count = 0; //用于判断压缩机强制测试模式进入按键识别

GCE_XDATA UI08 S_Shake_Count = 0;
GCE_XDATA UI08 G_Key_Number = 0; //滤波后识别出的按键

GCE_XDATA UI16 WIFI_Self_Test_delay_time = 0; //待機狀態下前5秒內,長按風速鍵5秒發送一次Wifi模产检模式指令

#define KEY_FILTER_COUNT 5 //单按键滤波次         数值为 1~254

//#define  PREE_DOWN  1  //按下识别
#define PREE_DOWN 0 //松开识别

#define TK_TOTAL 7
GCE_CONST UI32 S_TK_Number[TK_TOTAL] = {
	TK_8,  //按键1
	TK_7,  //按键2
	TK_6,  //按键3
	TK_5,  //按键4
	TK_4,  //按键5
	TK_28, //按键6
	TK_23, //按键7
};

void Prg_S_Key_Device(void)
{
	if (!_1S_For_For_SYS)
	{
		return;
	}

	if (WIFI_Self_Test_delay_time > 0)
	{
		WIFI_Self_Test_delay_time--;
	}
}

// *****************************************************************************
// 函数名称 : Dev_Read_Key_Data
// 功能说明 : 获取未滤波的按键状态
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 :
// 审核人员 :
// 审核日期 :
// 修改记录 : 2021.5.13  V1.0首次发布
// *****************************************************************************
void Dev_Read_Key_Data(void)
{
	UI08 i = 0;
	if (SOCAPI_TouchKeyStatus & 0x80) //重要步骤2:  触摸键扫描一轮标志，是否调用TouchKeyScan()一定要根据此标志位置起后
	{
		SOCAPI_TouchKeyStatus &= 0x7f;	   //重要步骤3: 清除标志位， 需要外部清除。
		S_Key_Value_Flag = TouchKeyScan(); //按键数据处理函数
		TouchKeyRestart();				   //启动下一轮转换
	}
	else
	{
		return;
	}

	S_Key_Data = 0;
	for (i = 0; i < TK_TOTAL; i++)
	{
		if (S_TK_Number[i] & S_Key_Value_Flag)
		{
			S_Key_Data |= (0x01 << i);
		}
	}
}

// *****************************************************************************
// 函数名称 : Dev_Get_Key_Number
// 功能说明 : 按键识别
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
//
// *****************************************************************************
void Dev_Get_Key_Number(void)
{
	UI08 i = 0;
	UI08 key_total_count = 0;
	UI08 number = 0;

	Prg_S_Key_Device();

	//
	if (_10mS_For_SYS) //根据不同按键底层，调整进入时间
	{
		Dev_Read_Key_Data(); //调用底层读取未滤波的按键值
	}

	if (!_10mS_For_SYS)
	{
		return;
	}

	if (S_Key_Last < 0xffff)
	{
		S_Key_Last++;
	}

	if (S_Comp_Test_Key_Count < 0xffff)
	{
		S_Comp_Test_Key_Count++;
	}

	for (i = 0; i < 8; i++)
	{
		if (S_Key_Data & (UI08)(0x01 << i))
		{
			key_total_count++;
			number = i + 1;
		}
	}

	if (key_total_count == 0)
	{
		if (S_Shake_Count == KEY_FILTER_COUNT)
		{
			G_Key_Number = S_Key_One_Buf;
		}

		S_Shake_Count = 0;
		S_Key_Last = 0;
		return;
	}
	else if (key_total_count == 1) //单按键
	{
		S_Key_New = number;
	}
	else if (key_total_count == 2) //双按键的组合按键
	{
		S_Shake_Count = 0xff;
	}
	else //同时按下3个或者3个以上按键，不处理
	{
		S_Key_New = 0;
		S_Key_Last = 0;
		S_Shake_Count = 0xff;
		return;
	}
	/////////////////////////////////////////////////////////////////////////////////
	if (S_Key_New != S_Key_Old)
	{
		S_Key_Old = S_Key_New;
		if (key_total_count == 1)
		{
			if (S_Shake_Count < KEY_FILTER_COUNT)
			{
				S_Shake_Count = 0;
			}
			else
			{
				S_Shake_Count = 0xff;
			}
		}
		S_Key_Last = 0;
		return;
	}

	//单按键
	if (S_Shake_Count < KEY_FILTER_COUNT)
	{
		S_Shake_Count++;
		if ((S_Shake_Count >= KEY_FILTER_COUNT) && (key_total_count == 1))
		{
			S_Key_One_Buf = S_Key_Old;
#if PREE_DOWN
			G_Key_Number = S_Key_Old;
			S_Shake_Count = 0xff;
#endif
		}
	}

	// 单按键或者双按键 长按识别
	/////////////////////////////////////////////////////////////////////////////////
	if ((S_Key_Last == 300) && (S_Key_Data == ((0x01 << (DOWN_KEY - 1)) | (0x01 << (SET_TIME_KEY - 1)))))
	{
		G_Key_Number = FAST_TEST_KEY;
		S_Shake_Count = 0xff;
	}

	if ((S_Key_Last == 1000) && (S_Key_Data == (0x01 << (POWER_KEY - 1))) && (S_Comp_Test_Key_Count > S_Key_Last) && ((S_Comp_Test_Key_Count - S_Key_Last) > 10) //上电前100按下视为无效
		&& ((S_Comp_Test_Key_Count - S_Key_Last) < (300 - 10))																									 //需要在上电全显示期间
	)
	{
		G_Key_Number = COMP_TEST_KEY;
		S_Shake_Count = 0xff;
	}

	//长按电源键
	// wifi复位/配网模式切换    在联网的状态下-wifi复位 未联网状态下切换配网方式
	if ((S_Key_Last == 500) && (S_Key_Data == (0x01 << (POWER_KEY - 1))))
	{
		G_Key_Number = WIFI_RESET_KEY;
		S_Shake_Count = 0xff;
	}

	//威技wifi模块产测
	/*
	待機狀態下前5秒內,長按風速鍵(SW2)5秒發送一次Wifi模产检模式指令
	連接產檢賬號成功後,Wifi指示燈點亮,,雙八燈顯示信號強度.立即清除模塊賬號信息並機臺恢復默認設置.
	退出條件:1.按電源鍵開機或斷電
			 2.進入產檢模式時間超過3分鐘.
	 工廠模式時,信號低於60,不點亮Wifi指示燈.
	*/
	if ((WIFI_Self_Test_delay_time > 0) && (S_Key_Data == (0x01 << (FAN_KEY - 1))))
	{
		WIFI_Self_Test_delay_time = WIFI_SELF_TEST_IN_TIMER;
		if (S_Key_Last == 500)
		{
			WIFI_Self_Test_delay_time = 0;

			G_Key_Number = WIFI_TEST_KEY;
			S_Shake_Count = 0xff;
		}
	}
}
