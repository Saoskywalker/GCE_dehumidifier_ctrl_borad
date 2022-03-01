#include "General.h"

GCE_XDATA WIFI_UPDATE Wifi_Updata;
GCE_XDATA UUI16 flag_wifi = {0};    //标志位
GCE_XDATA UI08 M_wifi_rssi_dsp = 0; // wifi 信号强度

GCE_XDATA UI08 mcu_dp_type = 0;                          //上报数据的DP
GCE_XDATA UI08 all_data_update_point = 0;                //全部数据上报时的计数值
GCE_XDATA UI08 Wifi_Rssi_DSP_delay_time = 0;             //
GCE_XDATA UI08 Wifi_Comm_Err_delay = WIFI_COMM_ERR_TIME; // wifi通信故障
GCE_XDATA UI08 Wifi_Comm_Err_Dsp_time = 0;               // wifi通信故障显示时间
GCE_XDATA UI08 Enter_LOW_POWER_delay_time = 150;         //进入睡眠延迟时间
GCE_XDATA UI08 wifi_work_state_old = WIFI_SATE_UNKNOW;   //上一次工作状态(默认未知网络状态)
GCE_XDATA UI08 wifi_tx_status = 0;
GCE_XDATA TU_FAN_Speed_Type Updata_speed = OFF_FAN;

GCE_XDATA UI08 SYS_ERR_type = 0;

//根据 protocol.c  结构体数组 DOWNLOAD_CMD_S download_cmd[] 制作
code UI08 TX_Data_TB_cmd[9] =
    {
        DPID_SWITCH,
        DPID_DEHUMIDIFY_SET_VALUE,
        DPID_FAN_SPEED_ENUM,
        DPID_MODE,
        DPID_HUMIDITY_INDOOR,
        DPID_FAULT,
        DPID_TANK_FULL,
        DPID_DEFROST_STATUS,
        DPID_PUMP,
};

void UART_Init(void)
{
    UART_Driver_Init();
    wifi_protocol_init();
}

/*************************************************
//名称        :	wifi_rssi_dsp
//功能        : wifi 产测时  显示wifi信号强度
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE 唐永强		2011/07/18
//修	改: 	GCE XXX
************************************************/
void wifi_rssi_Dsp(void)
{
    UI08 wifi_buf;
    wifi_buf = M_wifi_rssi_dsp;
    //
    if (wifi_buf > 99)
    {
        wifi_buf = 99;
    }
    //工廠模式時,信號低於60,不點亮Wifi指示燈
    if (wifi_buf >= 60)
    {
        LED_WIFI;
    }
    dig1_2_dsp(wifi_buf);
}
/*************************************************
//名称        :	wifi   操作时间片
//功能        :
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE 龙绍平
//修	改: 	GCE XXX
************************************************/
void prg_250ms_dsp(void)
{
    static UI08 ms1500_cnt = 0;
    //
    if (!_250mS_For_SYS)
    {
        return;
    }

    _Flash_250ms ^= 1;
    _Self_Test_wifi_TXD_en = 1;
    if (++ms1500_cnt >= 6)
    {
        ms1500_cnt = 0;
        _Flash_1500ms ^= 1;
    }
}

/*************************************************
//名称        :	wifi   操作时间片
//功能        :
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE 龙绍平
//修	改: 	GCE XXX
************************************************/
void prg_s_wifi(void)
{
    static UI08 cnt_10S = 0;
    UI08 wifi_work_state = 0;
    //
    if (!_1S_For_For_SYS)
    {
        return;
    }

    if (Wifi_Rssi_DSP_delay_time)
    {
        Wifi_Rssi_DSP_delay_time--;
    }

    if (Wifi_Comm_Err_delay)
    {
        if (--Wifi_Comm_Err_delay == 0)
        {
            _Wifi_Comm_Err = TRUE;
        }
        else
        {
            _Wifi_Comm_Err = FALSE;
        }
    }
    //
    if (Wifi_Comm_Err_Dsp_time)
    {
        Wifi_Comm_Err_Dsp_time--;
    }

    if (++cnt_10S >= 10) // 10S主动要求更新一次wifi的状态
    {
        cnt_10S = 0;
        _WIFI_10S_check_status = 1;
    }
    //因为涂鸦模块在上一次没有配网超过10S,重新上电会直接进入睡眠，客户要求修改不让直接进入睡眠,至少150S后才可以
    //如果在150S内读取到模块进入睡眠,则激活配网
    if (Enter_LOW_POWER_delay_time)
    {
        Enter_LOW_POWER_delay_time--;
        wifi_work_state = mcu_get_wifi_work_state(); //获取wifi状态
        if (wifi_work_state == WIFI_LOW_POWER)       //如果进入低功耗
        {
            _wifi_WakeUp_En = 1;                  //激活唤醒睡眠 SMART_CONFIG 配网
            wifi_work_state_old = WIFI_LOW_POWER; //标记上一次状态为睡眠状态
        }
        else
        {
            //由睡眠模式成功激活为 SMART_CONFIG 配网
            if ((wifi_work_state_old == WIFI_LOW_POWER) && (wifi_work_state == SMART_CONFIG_STATE))
            {
                Enter_LOW_POWER_delay_time = 0; //成功激活,退出检查
            }
        }
    }
}

void WIFI_data_update(void)
{

    switch (mcu_dp_type)
    {
    case DPID_SWITCH: //开关(可下发可上报)
    {
        Wifi_Updata.SYS_Power_Status = G_SYS_Power_Status;
        wifi_tx_status = mcu_dp_bool_update(DPID_SWITCH, (UI08)(Wifi_Updata.SYS_Power_Status));
    }
    break;
    case DPID_DEHUMIDIFY_SET_VALUE: //目标湿度(可下发可上报)
    {
        if (G_SYS_Hum_Set >= 30)
        {
            Wifi_Updata.SYS_Hum_Set = G_SYS_Hum_Set;
            wifi_tx_status = mcu_dp_value_update(DPID_DEHUMIDIFY_SET_VALUE, Wifi_Updata.SYS_Hum_Set);
        }
        else
        {
            wifi_tx_status = mcu_dp_value_update(DPID_DEHUMIDIFY_SET_VALUE, 30);
        }
    }
    break;
    case DPID_FAN_SPEED_ENUM: //风速(可下发可上报)
    {
        Wifi_Updata.SYS_Fan_Tyde = Updata_speed;
        wifi_tx_status = mcu_dp_enum_update(DPID_FAN_SPEED_ENUM, Wifi_Updata.SYS_Fan_Tyde);
    }
    break;
    case DPID_MODE: //模式(可下发可上报)
    {
        if (G_SYS_Mode == mode_SYS_HUM)
        {
            if (G_SYS_Hum_Set == 25)
            {
                Wifi_Updata.SYS_Mode = 1;
                wifi_tx_status = mcu_dp_enum_update(DPID_MODE, 1);
            }
            else
            {
                Wifi_Updata.SYS_Mode = 0;
                wifi_tx_status = mcu_dp_enum_update(DPID_MODE, 0);
            }
        }
        else
        {
            Wifi_Updata.SYS_Mode = 2;
            wifi_tx_status = mcu_dp_enum_update(DPID_MODE, 2);
        }
    }
    break;
    case DPID_HUMIDITY_INDOOR: //室内湿度(只上报)
    {
        Wifi_Updata.Room_HUM = GET_ROOM_HUM();
        wifi_tx_status = mcu_dp_value_update(DPID_HUMIDITY_INDOOR, Wifi_Updata.Room_HUM); //枚举型数据上报;
    }
    break;
    case DPID_FAULT:
    {
        //故障告警(只上报)
        //备注:E1:温度传感器故障 E2:铜管传感器故障 E5:通讯故障 FU:满水
        Wifi_Updata.SYS_fault = SYS_ERR_type;
        wifi_tx_status = mcu_dp_fault_update(DPID_FAULT, Wifi_Updata.SYS_fault); //故障型数据上报;
    }
    break;

    case DPID_TANK_FULL: //水箱水位(只上报)
    {
        Wifi_Updata.Water_Full = G_Sys_Err.Water_Full;
        if (G_Sys_Err.Water_Full)
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_TANK_FULL, 1); // VALUE型数据上报;
        }
        else
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_TANK_FULL, 0); // VALUE型数据上报;
        }
    }
    break;
    case DPID_DEFROST_STATUS: //除霜状态(只上报)
    {
        Wifi_Updata.SYS_defrost_status = G_Def_Reg.Defrost_status;

        if (G_Def_Reg.Defrost_status)
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_DEFROST_STATUS, 1); // VALUE型数据上报;
        }
        else
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_DEFROST_STATUS, 0); // VALUE型数据上报;
        }
    }
    break;
    case DPID_PUMP: //自动排水(可下发可上报)
    {
        Wifi_Updata.SYS_Filter_Status = G_Filter_Status;

        if (G_Filter_Status)
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_PUMP, 1); // VALUE型数据上报;
        }
        else
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_PUMP, 0); // VALUE型数据上报;
        }
    }
    break;
    };
}

/**
 * @brief  系统所有dp点信息上传,实现APP和muc数据同步
 * @param  Null
 * @return Null
 * @note   此函数SDK内部需调用，MCU必须实现该函数内数据上报功能，包括只上报和可上报可下发型数据
 */
void all_data_update(void)
{
    if (!uart_tx_OK)
    {
        return;
    }

    if (G_Power_Delay_Time > 0) //保证读完EEP 再上报
    {
        return;
    }

    if (!_all_status_updata_en)
    {
        return;
    }

    if (all_data_update_point > 9)
    {
        _all_status_updata_en = 0;
        return;
    }
    else
    {
        all_data_update_point++;
    }
    mcu_dp_type = TX_Data_TB_cmd[all_data_update_point - 1];

    WIFI_data_update();
}

/*****************************************************************************
函数名称 : data_update
功能描述 : 系统所有dp点信息上传,实现APP和muc数据同步
输入参数 : 无
返回参数 : 无
使用说明 : 此函数运用在 机台在运行过程中数据有变化上报(非初次上电上报)
*****************************************************************************/
void data_update(void) // wifi模块上电 全部上报一次
{
    UI08 tmp_hum;
    if (_all_status_updata_en)
    {
        return;
    }

    if (!_100mS_For_SYS)
    {
        return;
    }
    tmp_hum = GET_ROOM_HUM();

    //检查数据更新
    //==================================当前开关
    if (Wifi_Updata.SYS_Power_Status != G_SYS_Power_Status)
    {
        mcu_dp_type = DPID_SWITCH;
    }
    else if (Wifi_Updata.SYS_Hum_Set != G_SYS_Hum_Set)
    {
        if (G_SYS_Hum_Set == 25)
        {
            Wifi_Updata.SYS_Hum_Set = G_SYS_Hum_Set;
            return;
        }
        else
        {
            mcu_dp_type = DPID_DEHUMIDIFY_SET_VALUE;
        }
    }
    else if (Wifi_Updata.SYS_Fan_Tyde != Updata_speed)
    {
        mcu_dp_type = DPID_FAN_SPEED_ENUM;
    }
    else if (((Wifi_Updata.SYS_Mode == 0) && ((G_SYS_Mode != mode_SYS_HUM) || ((G_SYS_Hum_Set == 25)))) || ((Wifi_Updata.SYS_Mode == 1) && ((G_SYS_Hum_Set != 25) || (G_SYS_Mode == mode_DRY))) || ((Wifi_Updata.SYS_Mode == 2) && ((G_SYS_Mode != mode_DRY) || ((G_SYS_Hum_Set == 25)))))
    {
        mcu_dp_type = DPID_MODE;
    }
    else if (Wifi_Updata.Room_HUM != tmp_hum)
    {
        mcu_dp_type = DPID_HUMIDITY_INDOOR;
    }
    else if (Wifi_Updata.SYS_fault != SYS_ERR_type)
    {
        mcu_dp_type = DPID_FAULT;
    }
    else if (Wifi_Updata.Water_Full != G_Sys_Err.Water_Full)
    {
        mcu_dp_type = DPID_TANK_FULL;
    }
    else if (Wifi_Updata.SYS_defrost_status != G_Def_Reg.Defrost_status)
    {
        mcu_dp_type = DPID_DEFROST_STATUS;
    }
    else if (Wifi_Updata.SYS_Filter_Status != G_Filter_Status)
    {
        mcu_dp_type = DPID_PUMP;
    }
    else
    {
        return;
    }

    WIFI_data_update();
}

void Data_update_deal(void)
{
    if (!uart_tx_OK)
    {
        return;
    }
    //工厂测试模式(国富那边测试用)
    if (_Wifi_factory_test)
    {
        _Wifi_factory_test = 0;
        M_wifi_rssi_dsp = 0; //信号强度清零
        mcu_start_wifitest();
    }
    //长按电源键5S-wifi复位/配网模式切换,在联网的状态下-wifi复位 未联网状态下切换配网方式
    else if (_wifi_reset_En)
    {
        _wifi_reset_En = 0;
        mcu_reset_wifi();
    }
    else if (_wifi_WakeUp_En) //睡眠激活
    {
        _wifi_WakeUp_En = 0;
        mcu_set_wifi_mode(SMART_CONFIG); //激活为SMART_CONFIG配网方式(默认、快闪)
    }
    // wifi指示灯状态都是由wifi主动下发的
    //但测试中发现没联网的状态下  指示灯也常亮
    //所以每隔10S  主动向wifi模块问询一次状态
    //致网络状态 1.每隔10S主动问询  2.wifi模块状态变化主动下发  两种方式
    else if (_WIFI_10S_check_status)
    {
        _WIFI_10S_check_status = 0;
        mcu_get_wifi_connect_status();
    }
    //数据主动上报
    else if (_Wifi_Online_Status) //成功联上网,间隔100ms检查是否有数据需上报
    {
        data_update();
    }
}

void wifi_status_FALSE_To_TRUE(void)
{
    if (_Wifi_Online_Status == FALSE)
    {
        _all_status_updata_en = 1;
        all_data_update_point = 0;
    }
}
/*************************************************
//名称        :	wifi模式设置
//功能        :
//入口参数    :	无
//出口参数    :	无
//构	建: 	GCE 龙绍平
//修	改: 	GCE XXX
************************************************/
void wifi_status_DSP(void)
{
    UI08 wifi_work_state_buf = 0;
    // clear buf
    WiFi_LED_Locate_buf = 0;
    //
    wifi_work_state_buf = mcu_get_wifi_work_state();
    switch (wifi_work_state_buf)
    {
    case SMART_CONFIG_STATE: // smart config 配置状态 LED 快闪 ，led 闪烁请用户完成   间隔闪烁 250ms
    {
        if (_Flash_250ms)
        {
            WiFi_LED_Locate_buf = 1;
        }
        _Wifi_Online_Status = FALSE;
    }
    break;
    //
    case AP_STATE: // AP 配置状态 LED 慢闪  间隔闪烁 1500ms
    {
        if (_Flash_1500ms)
        {
            WiFi_LED_Locate_buf = 1;
        }
        _Wifi_Online_Status = FALSE;
    }
    break;
    //
    case WIFI_NOT_CONNECTED: // WIFI 配置完成，正在连接路由器，LED 常暗
    {
        wifi_status_FALSE_To_TRUE();
        _Wifi_Online_Status = TRUE; // 2021-07-23
        ;                           // LED不亮
    }
    break;
    //
    case WIFI_CONNECTED: //路由器连接成功 LED 常亮
    {
        WiFi_LED_Locate_buf = 1;
        wifi_status_FALSE_To_TRUE();
        _Wifi_Online_Status = TRUE;
    }
    break;

    case WIFI_CONN_CLOUD: //成功连接上云
    {
        WiFi_LED_Locate_buf = 1;
        wifi_status_FALSE_To_TRUE();
        _Wifi_Online_Status = TRUE;
    }
    break;
    //
    default:
    {
        _Wifi_Online_Status = FALSE;
    }
    break;
    }
}

void Wifi_UPdata_for_SYSdata_Deal(void)
{
    UI08 err_buf = 0;
    if ((G_Set_Fan_Tyde_EN == DISABLE) || (G_Def_Reg.Defrost_status == TRUE))
    {
        if (G_Fan_Tyde_Out_Buf != OFF_FAN)
        {
            Updata_speed = G_Fan_Tyde_Out_Buf;
        }
        else
        {
            Updata_speed = G_SYS_Fan_Tyde;
        }
    }
    else
    {
        Updata_speed = G_SYS_Fan_Tyde;
    }

    /*
        if(G_Sys_Err.temp_room_err)     //温度感应器短路或者开路  E1
        {
            err_buf|=0x01;
        }
    */
    if (G_Sys_Err.temp_coil_err) //铜管感应器短路或者开路  E2
    {
        err_buf |= 0x01;
    }

    if (G_Sys_Err.hum_Sensor_err) // E3
    {
        err_buf |= 0x02;
    }

    if (GET_COM_STATUS() == ERROR) //通信错误  E5
    {
        err_buf |= 0x04;
    }

    if (G_Sys_Err.temp_comp_err) // E6
    {
        err_buf |= 0x08;
    }

    if ((err_buf == 0) && (SYS_ERR_type != 0))
    {
        _all_status_updata_en = 1;
        all_data_update_point = 0;
    }

    SYS_ERR_type = err_buf;
}

void WIFI_Deal(void)
{
    Wifi_UPdata_for_SYSdata_Deal();
    prg_250ms_dsp();
    prg_s_wifi();
    wifi_status_DSP();

    wifi_uart_service();
    all_data_update();
    Data_update_deal();
}
