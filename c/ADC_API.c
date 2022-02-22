#include "General.h"

GCE_XDATA TS_ADC_PARA G_Temp_Room;		//室温变量
GCE_XDATA TS_ADC_PARA G_Temp_Coil;		//管温变量
GCE_XDATA TS_ADC_PARA G_Hum_Para;		//湿度变量
GCE_XDATA TS_ADC_PARA G_Temp_Comp;		//压缩机温度变量
GCE_XDATA UI08 S_Temp_Updata_Count = 1; //室温、管温、湿度多久更新一次

//温度查表
// 5F--158F -15C--70C
GCE_CONST UI16 Temp_Tab_10k_3450[] =
	{
		75,	 // 0
		77,	 // 1
		79,	 // 2
		81,	 // 3
		83,	 // 4
		85,	 // 5
		86,	 // 6
		88,	 // 7
		90,	 // 8
		93,	 // 9
		95,	 // 10
		97,	 // 11
		99,	 // 12
		101, // 13
		103, // 14
		105, // 15
		108, // 16
		110, // 17
		112, // 18
		114, // 19
		117, // 20
		119, // 21
		121, // 22
		124, // 23
		126, // 24
		128, // 25
		131, // 26
		133, // 27
		136, // 28
		138, // 29
		141, // 30
		143, // 31
		146, // 32
		149, // 33
		151, // 34
		154, // 35
		156, // 36
		159, // 37
		162, // 38
		164, // 39
		167, // 40
		170, // 41
		173, // 42
		175, // 43
		178, // 44
		181, // 45
		184, // 46
		186, // 47
		189, // 48
		192, // 49
		195, // 50
		197, // 51
		200, // 52
		203, // 53
		206, // 54
		209, // 55
		211, // 56
		214, // 57
		217, // 58
		220, // 59
		223, // 60
		225, // 61
		228, // 62
		231, // 63
		234, // 64
		237, // 65
		239, // 66
		242, // 67
		245, // 68
		248, // 69
		251, // 70
		253, // 71
		256, // 72
		259, // 73
		261, // 74
		264, // 75
		267, // 76
		270, // 77
		272, // 78
		275, // 79
		278, // 80
		280, // 81
		283, // 82
		285, // 83
		288, // 84
		291, // 85
		293, // 86
		296, // 87
		298, // 88
		301, // 89
		303, // 90
		306, // 91
		308, // 92
		311, // 93
		313, // 94
		316, // 95
		318, // 96
		320, // 97
		323, // 98
		325, // 99
		327, // 100
		330, // 101
		332, // 102
		334, // 103
		336, // 104
		339, // 105
		341, // 106
		343, // 107
		345, // 108
		347, // 109
		349, // 110
		351, // 111
		353, // 112
		356, // 113
		358, // 114
		360, // 115
		362, // 116
		364, // 117
		365, // 118
		367, // 119
		369, // 120
		371, // 121
		373, // 122
		375, // 123
		377, // 124
		379, // 125
		380, // 126
		382, // 127
		384, // 128
		385, // 129
		387, // 130
		389, // 131
		390, // 132
		392, // 133
		394, // 134
		395, // 135
		397, // 136
		398, // 137
		400, // 138
		401, // 139
		403, // 140
		404, // 141
		406, // 142
		407, // 143
		409, // 144
		410, // 145
		412, // 146
		413, // 147
		415, // 148
		416, // 149
		417, // 150
		418, // 151
		419, // 152
		421	 // 153
};

GCE_CONST UI16 Temp_Tab_PRT24911[] =
	{
		26,
		28,
		29,
		31,
		33,
		35,
		37,
		39,
		41,
		43,
		45,
		48,
		50,
		53,
		56,
		58,
		61,
		64,
		68,
		71,
		74,
		78,
		82,
		85,
		89,
		94,
		98,
		102,
		107,
		111,
		116,
		121,
		126,
		131,
		137,
		142,
		148,
		154,
		160,
		166,
		172,
		179,
		188,
		192,
		199,
		206,
		213,
		220,
		228,
		235,
		243,
		251,
		259,
		267,
		275,
		283,
		291,
		300,
		308,
		317,
		326,
		334,
		343,
		352,
		361,
		370,
		379,
		388,
		397,
		406,
		416,
		425,
		434,
		443,
		452,
		462,
		471,
		480,
		489,
		498,
		507,
		516,
		525,
		534,
		543,
		551,
		560,
		569,
		577,
		586,
		594,
		602,
		610,
		618,
		626,
		634,
		642,
		650,
		657,
		665,
		672,
		679,
		686,
		694,
		700,
		707,
		714,
		720,
		727,
		733,
		739,
		746,
		752,
		757,
		763,
		769,
		774,
		780,
		785,
		790,
		796,
		801,
		806,
		810,
		815,
		820,
		824,
		829,
		833,
		837,
		841,
		845,
		849,
		853,
		857,
		861};

// *****************************************************************************
// 函数名称 : Adc_Data_Init
// 功能说明 : 变量初始化
// 入口参数 : 无
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：
// *****************************************************************************
void Adc_Data_Init(void)
{
	//	G_Temp_Room.AD_value = 528;
	G_Temp_Room.value = 77;
	G_Temp_Room.value_buf = 77;
	G_Temp_Room.C_value = 40;
	G_Temp_Room.status = AI_NORMAL;

	//	G_Temp_Coil.AD_value = 528;
	G_Temp_Coil.value = 36;
	G_Temp_Coil.status = AI_NORMAL;

	//	G_Hum_Para.AD_value = 256;
	G_Hum_Para.value = 60;
	G_Hum_Para.status = AI_NORMAL;

	G_Temp_Room.C_value = 25 + 15;
	G_Temp_Room.value = 77;
	G_Temp_Room.value_buf = 77;
	G_Temp_Room.status = AI_NORMAL;
	G_Temp_Room.Err_Cut_Count = 0;
	G_Temp_Room.Err_Short_Count = 0;
	G_Temp_Room.Err_Normal_Count = 0;

	G_Temp_Coil.C_value = 25 + 15;
	G_Temp_Coil.value = 77;
	G_Temp_Coil.value_buf = 77;
	G_Temp_Coil.status = AI_NORMAL;
	G_Temp_Coil.Err_Cut_Count = 0;
	G_Temp_Coil.Err_Short_Count = 0;
	G_Temp_Coil.Err_Normal_Count = 0;

	G_Hum_Para.value = 60;
	G_Hum_Para.value_buf = 60;
	G_Hum_Para.status = AI_NORMAL;
	G_Hum_Para.Err_Cut_Count = 0;
	G_Hum_Para.Err_Short_Count = 0;
	G_Hum_Para.Err_Normal_Count = 0;

	G_Temp_Comp.C_value = 25 + 15;
	G_Temp_Comp.value = 77;
	G_Temp_Comp.value_buf = 77;
	G_Temp_Comp.status = AI_NORMAL;
	G_Temp_Comp.Err_Cut_Count = 0;
	G_Temp_Comp.Err_Short_Count = 0;
	G_Temp_Comp.Err_Normal_Count = 0;
}

// *****************************************************************************
// 函数名称 : F_C
// 功能说明 : F转换为C
// 入口参数 : _cfdata 度F值
// 出口参数 : 回传度C值
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：  原始公式 四舍五入 (X-32)*5/9+0.5
//           优化公式 ((X-32)*10+9)/18
// *****************************************************************************
static UI08 F_C(UI08 _cfdata)
{
	UI08 buf = 0;
	if (_cfdata > 32)
	{
		buf = ((_cfdata - 32) * 10 + 9) / 18;
	}
	else
	{
		buf = ((32 - _cfdata) * 10 + 9) / 18;
	}

	return buf;
}

// *****************************************************************************
// 函数名称 : ADC_lookup
// 功能说明 : 二分法查表
// 入口参数 : _value 需要查表的值  *_ptr 表的首地址   _tab_leng 表的长度
// 出口参数 : 回传_value  在表的下标值
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// *****************************************************************************
static UI16 ADC_lookup(UI16 _value, UI16 *_ptr, UI16 _tab_leng)
{
	UI16 low = 0;
	UI16 high = 0;
	UI16 mid = 0;
	UI16 x = 0;
	UI16 y = 0;

	high = _tab_leng - 1;
	if (_value <= *_ptr)
	{
		return low;
	}

	if (_value >= *(_ptr + high))
	{
		return high;
	}
	//
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (*(_ptr + mid) == _value)
		{
			return mid;
		}
		else if (*(_ptr + mid) > _value)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	if (_value < *(_ptr + mid))
	{
		x = *(_ptr + mid) - _value;
		y = _value - *(_ptr + mid - 1);
		if (x <= y)
		{
			return mid;
		}
		else
		{
			return mid - 1;
		}
	}
	else
	{
		x = *(_ptr + mid + 1) - _value;
		y = _value - *(_ptr + mid);
		if (x <= y)
		{
			return mid + 1;
		}
		else
		{
			return mid;
		}
	}
}

// *****************************************************************************
// 函数名称 : NTC_10K3450_To_Temp
// 功能说明 : 温度传感器故障判断，及温度更新
// 入口参数 : _ad_st温度结构体地址   _ad_buf 10位AD值  _updata_en 湿度运行更新标志
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：  NTC_10K3450，下拉10K电阻 温度计算及故障判断
//
// *****************************************************************************
static void NTC_10K3450_To_Temp(TS_ADC_PARA *_ad_st, UI16 _ad_buf, TE_FuncState _updata_en)
{
	UI08 value_buf = 0;
	UI08 c_buf;
	if (_ad_buf < 30)
	{
		_ad_st->Err_Short_Count = 0;
		_ad_st->Err_Normal_Count = 0;

		_ad_st->Err_Cut_Count++;
		if (_ad_st->Err_Cut_Count >= 2)
		{
			_ad_st->status = AI_CUT;
			value_buf = 0;
		}
	}
	else if (_ad_buf > 500)
	{
		_ad_st->Err_Cut_Count = 0;
		_ad_st->Err_Normal_Count = 0;

		_ad_st->Err_Short_Count++;
		if (_ad_st->Err_Short_Count >= 2)
		{
			_ad_st->status = AI_SHORT;
			value_buf = 0;
		}
	}
	else
	{
		_ad_st->Err_Cut_Count = 0;
		_ad_st->Err_Short_Count = 0;

		_ad_st->Err_Normal_Count++;
		if (_ad_st->Err_Normal_Count >= 6) // 800*6=4.8s
		{
			_ad_st->status = AI_NORMAL;
		}

		value_buf = 5 + ADC_lookup(_ad_buf, Temp_Tab_10k_3450, 154); //表从5F开始
	}

	if (_updata_en)
	{
		_ad_st->value = value_buf;

		if (_ad_st->value <= 32)
		{
			c_buf = 15 - F_C(_ad_st->value); //偏移15是为避免负数处理
		}
		else
		{
			c_buf = F_C(_ad_st->value) + 15;
		}
		_ad_st->C_value = c_buf;
	}
}

// *****************************************************************************
// 函数名称 : NTC_50K3970_To_Temp
// 功能说明 : 温度传感器故障判断，及温度更新
// 入口参数 : _ad_st温度结构体地址   _ad_buf 10位AD值  _updata_en 湿度运行更新标志
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ：  NTC_10K3450，下拉10K电阻 温度计算及故障判断
//
// *****************************************************************************
static void NTC_50K3970_To_Temp(TS_ADC_PARA *_ad_st, UI16 _ad_buf, TE_FuncState _updata_en)
{
	UI08 value_buf = 0;
	UI08 c_buf = 0;
	if (_ad_buf < 15)
	{
		_ad_st->Err_Short_Count = 0;
		_ad_st->Err_Normal_Count = 0;

		_ad_st->Err_Cut_Count++;
		if (_ad_st->Err_Cut_Count >= 2)
		{
			_ad_st->status = AI_CUT;
			value_buf = 0;
		}
	}
	else if (_ad_buf > 1016)
	{
		_ad_st->Err_Cut_Count = 0;
		_ad_st->Err_Normal_Count = 0;

		_ad_st->Err_Short_Count++;
		if (_ad_st->Err_Short_Count >= 2)
		{
			_ad_st->status = AI_SHORT;
			value_buf = 0;
		}
	}
	else
	{
		_ad_st->Err_Cut_Count = 0;
		_ad_st->Err_Short_Count = 0;

		_ad_st->Err_Normal_Count++;
		if (_ad_st->Err_Normal_Count >= 6) // 800*6=4.8s
		{
			_ad_st->status = AI_NORMAL;
		}

		value_buf = ADC_lookup(_ad_buf, Temp_Tab_PRT24911, 136); //表从-15℃开始
	}

	if (_updata_en)
	{

		_ad_st->C_value = value_buf;

		// if (_ad_st->value <= 32)
		// {
		// 	c_buf = 15 - F_C(_ad_st->value); //偏移15是为避免负数处理
		// }
		// else
		// {
		// 	c_buf = F_C(_ad_st->value) + 15;
		// }
		// _ad_st->C_value = c_buf;
	}
}

// *****************************************************************************
// 函数名称 : Read_Hum_Value
// 功能说明 : 湿度传感器故障判断，及湿度更新
// 入口参数 : _ad_st湿度结构体地址   _ad_buf 10位AD值  _updata_en 湿度运行更新标志
// 出口参数 : 无
// 当前版本 : V1.0
// 编写人员 : 许荣乾
// 审核人员 :
// 审核日期 :
// 修改记录 :   V1.0首次发布
// 备注     ： 神农  M53R 驱动口HUM2接30电阻，驱动口HUM1接1K电阻，之后接神农  M53R传感器，
//           神农  M53R传感器并接510K电阻  湿度传感器故障判断
// *****************************************************************************
static void Read_Hum_Value(TS_ADC_PARA *_ad_st, UI16 _ad_buf, TE_FuncState _updata_en)
{
	UI08 _value_buf = 0;
	//与电源板通信 800ms一次
	if (_ad_buf < 60) //湿度传感器连接线断开判断(非传感器断开)
	{
		_ad_st->Err_Short_Count = 0;
		_ad_st->Err_Normal_Count = 0;

		_ad_st->Err_Cut_Count++;
		if (_ad_st->Err_Cut_Count >= 2)
		{
			_ad_st->status = AI_CUT;
			_value_buf = 0;
		}
	}
	else if (_ad_buf > 975) //湿度传感器连接线断开判断
	{
		_ad_st->Err_Cut_Count = 0;
		_ad_st->Err_Normal_Count = 0;

		_ad_st->Err_Short_Count++;
		if (_ad_st->Err_Short_Count >= 2)
		{
			_ad_st->status = AI_CUT;
			_value_buf = 0;
		}
	}
	else
	{
		_ad_st->Err_Short_Count = 0;
		_ad_st->Err_Cut_Count = 0;

		_ad_st->Err_Normal_Count++;
		if (_ad_st->Err_Normal_Count >= 6) // 800*6=4.8s
		{
			_ad_st->status = AI_NORMAL;
		}
		_value_buf = _ad_st->value_buf;
	}

	if (_updata_en)
	{
		_ad_st->value = _value_buf;
	}
}

// *****************************************************************************
// 函数名称 : Temp_Time_1S_Deal
// 功能说明 : 1S变量处理函数
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
static void Temp_Time_1S_Deal(void)
{
	if (!_1S_For_For_SYS)
	{
		return;
	}

	if (S_Temp_Updata_Count < 0xff)
	{
		S_Temp_Updata_Count++;
	}
}

// *****************************************************************************
// 函数名称 : Temp_Deal
// 功能说明 : 温湿度驱动底层调用，温湿度转换
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
void Temp_Deal(void)
{
	UI16 ad_buf = 0;
	TE_FuncState temp_updata_en = DISABLE;

	Temp_Time_1S_Deal();

	ADC_Sample();//调用AD采样底层，完成AD采样

	if (!_100mS_For_SYS)
	{
		return;
	}

	if (S_Temp_Updata_Count >= 5) //要求每5秒更新一次值
	{
		S_Temp_Updata_Count = 0;
		temp_updata_en = ENABLE;
	}

	ad_buf = GET_ROOM_TEMP_AD();							   //获取室温AD
	NTC_10K3450_To_Temp(&G_Temp_Room, ad_buf, temp_updata_en); //室温计算       度F

	ad_buf = GET_COIL_TEMP_AD();							   //获取管温AD
	NTC_10K3450_To_Temp(&G_Temp_Coil, ad_buf, temp_updata_en); //蒸发器管温计算     度F

	ad_buf = GET_ROOM_HUM_AD();							 //获取湿度AD
	Read_Hum_Value(&G_Hum_Para, ad_buf, temp_updata_en); //湿度传感器湿度转换 故障判断

	ad_buf = GET_COMP_TEMP_AD();							   //获取压缩机温度AD
	NTC_50K3970_To_Temp(&G_Temp_Comp, ad_buf, temp_updata_en); //室温计算       度F
}

/******************* (C) COPYRIGHT 2010 GCE *****END OF FILE****/
