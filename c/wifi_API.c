#include "General.h"

GCE_XDATA WIFI_UPDATE Wifi_Updata;
GCE_XDATA UUI16 flag_wifi = {0};    //��־λ
GCE_XDATA UI08 M_wifi_rssi_dsp = 0; // wifi �ź�ǿ��

GCE_XDATA UI08 mcu_dp_type = 0;                          //�ϱ����ݵ�DP
GCE_XDATA UI08 all_data_update_point = 0;                //ȫ�������ϱ�ʱ�ļ���ֵ
GCE_XDATA UI08 Wifi_Rssi_DSP_delay_time = 0;             //
GCE_XDATA UI08 Wifi_Comm_Err_delay = WIFI_COMM_ERR_TIME; // wifiͨ�Ź���
GCE_XDATA UI08 Wifi_Comm_Err_Dsp_time = 0;               // wifiͨ�Ź�����ʾʱ��
GCE_XDATA UI08 Enter_LOW_POWER_delay_time = 150;         //����˯���ӳ�ʱ��
GCE_XDATA UI08 wifi_work_state_old = WIFI_SATE_UNKNOW;   //��һ�ι���״̬(Ĭ��δ֪����״̬)
GCE_XDATA UI08 wifi_tx_status = 0;
GCE_XDATA TU_FAN_Speed_Type Updata_speed = OFF_FAN;

GCE_XDATA UI08 SYS_ERR_type = 0;

//���� protocol.c  �ṹ������ DOWNLOAD_CMD_S download_cmd[] ����
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
//����        :	wifi_rssi_dsp
//����        : wifi ����ʱ  ��ʾwifi�ź�ǿ��
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE ����ǿ		2011/07/18
//��	��: 	GCE XXX
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
    //���Sģʽ�r,��̖���60,���c��Wifiָʾ��
    if (wifi_buf >= 60)
    {
        LED_WIFI;
    }
    dig1_2_dsp(wifi_buf);
}
/*************************************************
//����        :	wifi   ����ʱ��Ƭ
//����        :
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE ����ƽ
//��	��: 	GCE XXX
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
//����        :	wifi   ����ʱ��Ƭ
//����        :
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE ����ƽ
//��	��: 	GCE XXX
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

    if (++cnt_10S >= 10) // 10S����Ҫ�����һ��wifi��״̬
    {
        cnt_10S = 0;
        _WIFI_10S_check_status = 1;
    }
    //��ΪͿѻģ������һ��û����������10S,�����ϵ��ֱ�ӽ���˯�ߣ��ͻ�Ҫ���޸Ĳ���ֱ�ӽ���˯��,����150S��ſ���
    //�����150S�ڶ�ȡ��ģ�����˯��,�򼤻�����
    if (Enter_LOW_POWER_delay_time)
    {
        Enter_LOW_POWER_delay_time--;
        wifi_work_state = mcu_get_wifi_work_state(); //��ȡwifi״̬
        if (wifi_work_state == WIFI_LOW_POWER)       //�������͹���
        {
            _wifi_WakeUp_En = 1;                  //�����˯�� SMART_CONFIG ����
            wifi_work_state_old = WIFI_LOW_POWER; //�����һ��״̬Ϊ˯��״̬
        }
        else
        {
            //��˯��ģʽ�ɹ�����Ϊ SMART_CONFIG ����
            if ((wifi_work_state_old == WIFI_LOW_POWER) && (wifi_work_state == SMART_CONFIG_STATE))
            {
                Enter_LOW_POWER_delay_time = 0; //�ɹ�����,�˳����
            }
        }
    }
}

void WIFI_data_update(void)
{

    switch (mcu_dp_type)
    {
    case DPID_SWITCH: //����(���·����ϱ�)
    {
        Wifi_Updata.SYS_Power_Status = G_SYS_Power_Status;
        wifi_tx_status = mcu_dp_bool_update(DPID_SWITCH, (UI08)(Wifi_Updata.SYS_Power_Status));
    }
    break;
    case DPID_DEHUMIDIFY_SET_VALUE: //Ŀ��ʪ��(���·����ϱ�)
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
    case DPID_FAN_SPEED_ENUM: //����(���·����ϱ�)
    {
        Wifi_Updata.SYS_Fan_Tyde = Updata_speed;
        wifi_tx_status = mcu_dp_enum_update(DPID_FAN_SPEED_ENUM, Wifi_Updata.SYS_Fan_Tyde);
    }
    break;
    case DPID_MODE: //ģʽ(���·����ϱ�)
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
    case DPID_HUMIDITY_INDOOR: //����ʪ��(ֻ�ϱ�)
    {
        Wifi_Updata.Room_HUM = GET_ROOM_HUM();
        wifi_tx_status = mcu_dp_value_update(DPID_HUMIDITY_INDOOR, Wifi_Updata.Room_HUM); //ö���������ϱ�;
    }
    break;
    case DPID_FAULT:
    {
        //���ϸ澯(ֻ�ϱ�)
        //��ע:E1:�¶ȴ��������� E2:ͭ�ܴ��������� E5:ͨѶ���� FU:��ˮ
        Wifi_Updata.SYS_fault = SYS_ERR_type;
        wifi_tx_status = mcu_dp_fault_update(DPID_FAULT, Wifi_Updata.SYS_fault); //�����������ϱ�;
    }
    break;

    case DPID_TANK_FULL: //ˮ��ˮλ(ֻ�ϱ�)
    {
        Wifi_Updata.Water_Full = G_Sys_Err.Water_Full;
        if (G_Sys_Err.Water_Full)
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_TANK_FULL, 1); // VALUE�������ϱ�;
        }
        else
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_TANK_FULL, 0); // VALUE�������ϱ�;
        }
    }
    break;
    case DPID_DEFROST_STATUS: //��˪״̬(ֻ�ϱ�)
    {
        Wifi_Updata.SYS_defrost_status = G_Def_Reg.Defrost_status;

        if (G_Def_Reg.Defrost_status)
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_DEFROST_STATUS, 1); // VALUE�������ϱ�;
        }
        else
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_DEFROST_STATUS, 0); // VALUE�������ϱ�;
        }
    }
    break;
    case DPID_PUMP: //�Զ���ˮ(���·����ϱ�)
    {
        Wifi_Updata.SYS_Filter_Status = G_Filter_Status;

        if (G_Filter_Status)
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_PUMP, 1); // VALUE�������ϱ�;
        }
        else
        {
            wifi_tx_status = mcu_dp_bool_update(DPID_PUMP, 0); // VALUE�������ϱ�;
        }
    }
    break;
    };
}

/**
 * @brief  ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
 * @param  Null
 * @return Null
 * @note   �˺���SDK�ڲ�����ã�MCU����ʵ�ָú����������ϱ����ܣ�����ֻ�ϱ��Ϳ��ϱ����·�������
 */
void all_data_update(void)
{
    if (!uart_tx_OK)
    {
        return;
    }

    if (G_Power_Delay_Time > 0) //��֤����EEP ���ϱ�
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
�������� : data_update
�������� : ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
������� : ��
���ز��� : ��
ʹ��˵�� : �˺��������� ��̨�����й����������б仯�ϱ�(�ǳ����ϵ��ϱ�)
*****************************************************************************/
void data_update(void) // wifiģ���ϵ� ȫ���ϱ�һ��
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

    //������ݸ���
    //==================================��ǰ����
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
    //��������ģʽ(�����Ǳ߲�����)
    if (_Wifi_factory_test)
    {
        _Wifi_factory_test = 0;
        M_wifi_rssi_dsp = 0; //�ź�ǿ������
        mcu_start_wifitest();
    }
    //������Դ��5S-wifi��λ/����ģʽ�л�,��������״̬��-wifi��λ δ����״̬���л�������ʽ
    else if (_wifi_reset_En)
    {
        _wifi_reset_En = 0;
        mcu_reset_wifi();
    }
    else if (_wifi_WakeUp_En) //˯�߼���
    {
        _wifi_WakeUp_En = 0;
        mcu_set_wifi_mode(SMART_CONFIG); //����ΪSMART_CONFIG������ʽ(Ĭ�ϡ�����)
    }
    // wifiָʾ��״̬������wifi�����·���
    //�������з���û������״̬��  ָʾ��Ҳ����
    //����ÿ��10S  ������wifiģ����ѯһ��״̬
    //������״̬ 1.ÿ��10S������ѯ  2.wifiģ��״̬�仯�����·�  ���ַ�ʽ
    else if (_WIFI_10S_check_status)
    {
        _WIFI_10S_check_status = 0;
        mcu_get_wifi_connect_status();
    }
    //���������ϱ�
    else if (_Wifi_Online_Status) //�ɹ�������,���100ms����Ƿ����������ϱ�
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
//����        :	wifiģʽ����
//����        :
//��ڲ���    :	��
//���ڲ���    :	��
//��	��: 	GCE ����ƽ
//��	��: 	GCE XXX
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
    case SMART_CONFIG_STATE: // smart config ����״̬ LED ���� ��led ��˸���û����   �����˸ 250ms
    {
        if (_Flash_250ms)
        {
            WiFi_LED_Locate_buf = 1;
        }
        _Wifi_Online_Status = FALSE;
    }
    break;
    //
    case AP_STATE: // AP ����״̬ LED ����  �����˸ 1500ms
    {
        if (_Flash_1500ms)
        {
            WiFi_LED_Locate_buf = 1;
        }
        _Wifi_Online_Status = FALSE;
    }
    break;
    //
    case WIFI_NOT_CONNECTED: // WIFI ������ɣ���������·������LED ����
    {
        wifi_status_FALSE_To_TRUE();
        _Wifi_Online_Status = TRUE; // 2021-07-23
        ;                           // LED����
    }
    break;
    //
    case WIFI_CONNECTED: //·�������ӳɹ� LED ����
    {
        WiFi_LED_Locate_buf = 1;
        wifi_status_FALSE_To_TRUE();
        _Wifi_Online_Status = TRUE;
    }
    break;

    case WIFI_CONN_CLOUD: //�ɹ���������
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
        if(G_Sys_Err.temp_room_err)     //�¶ȸ�Ӧ����·���߿�·  E1
        {
            err_buf|=0x01;
        }
    */
    if (G_Sys_Err.temp_coil_err) //ͭ�ܸ�Ӧ����·���߿�·  E2
    {
        err_buf |= 0x01;
    }

    if (G_Sys_Err.hum_Sensor_err) // E3
    {
        err_buf |= 0x02;
    }

    if (GET_COM_STATUS() == ERROR) //ͨ�Ŵ���  E5
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
