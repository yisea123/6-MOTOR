
/******************** (C) COPYRIGHT 2019 HOME Team **************************
 * 描述    ：通过单片机CAN总线，发送数据给控制器，实现多个电机的控制      
 * 实验平台：STM32F103最小系统+CAN接收器+TTL转USB+EPOS2 50/5控制器+maxon DC电机
 * 库版本  ：ST3.5.0
*****************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "can.h"
#include "epos.h"
#include "delay.h"
#include "timer.h"

#define PI 3.1415

__IO uint32_t flag = 0xff;          //用于标志是否接收到数据，在中断函数中赋值

CanTxMsg TxMessage;                     //发送缓冲区
CanRxMsg RxMessage;                     //接收缓冲区

Uint32 pos=0;                                       //电机位置
int x=0;                                                //角度自变量
int angle_sensor;

double ang[51]  = {6.464,7.102,8.449,10.158,11.772,12.992,13.599,13.527,12.995,12.22,11.283,10.256,9.193,8.152,7.174,6.271,5.442,4.702,4.079,3.597,3.299,3.222,
    3.418,3.947,4.855,6.2,8.084,10.621,13.945,18.181,23.361,29.341,35.748,42.002,47.489,51.734,54.476,55.651,55.3,53.502,50.384,46.118,40.922,35.037,28.801,22.629,
16.957,12.215,8.741,6.708,6.017};

int angle_1[323] = {10682,10675,10669,10665,10664,10665,10668,10674,10683,10695,10711,10729,10751,10777,10807,10840,10878,10919,10965,11015,11070,11129,11192,11260,
    11333,11411,11493,11580,11672,11769,11870,11976,12086,12201,12321,12445,12573,12706,12843,12983,13128,13276,13428,13583,13741,13902,14066,14233,14401,14573,14745,
    14920,15096,15273,15451,15630,15809,15988,16167,16345,16523,16700,16875,17049,17220,17390,17557,17722,17883,18041,18195,18346,18492,18634,18772,18904,19032,19154,
    19270,19381,19486,19584,19676,19762,19841,19913,19978,20036,20086,20129,20165,20193,20214,20227,20232,20229,20219,20201,20175,20141,20100,20051,19995,19931,19859,
    19781,19695,19602,19503,19396,19283,19164,19039,18907,18770,18627,18479,18326,18167,18005,17838,17667,17492,17314,17132,16948,16760,16571,16380,16186,15992,15796,
    15600,15403,15206,15009,14812,14616,14421,14228,14036,13846,13658,13472,13289,13110,12933,12760,12590,12425,12263,12106,11953,11805,11662,11524,11391,11264,11142,
    11025,10914,10809,10710,10617,10530,10448,10373,10304,10240,10183,10132,10087,10048,10014,9987,9965,9948,9938,9932,9932,9937,9947,9962,9982,10006,10034,10067,10103,
    10143,10187,10234,10284,10337,10393,10451,10511,10573,10637,10703,10770,10837,10906,10975,11045,11114,11184,11253,11322,11389,11456,11522,11586,11649,11710,11769,
    11826,11881,11933,11983,12030,12075,12116,12155,12190,12222,12251,12277,12299,12318,12333,12345,12354,12359,12360,12358,12353,12344,12332,12316,12298,12276,12252,
    12224,12193,12160,12124,12086,12046,12003,11958,11911,11863,11813,11762,11709,11655,11601,11546,11490,11434,11377,11321,11265,11209,11154,11099,11046,10993,10942,
    10892,10844,10797,10752,10709,10669,10630,10594,10560,10529,10500,10475,10452,10432,10415,10401,10390,10382,10377,10375,10377,10381,10389,10400,10414,10431,10450,
    10473,10499,10527,10558,10591,10627,10665,10705,10748,10792,10839,10886,10936,10987,11039,11091,11145,11200,11255,11310,11366,11421,11476,11531,11586,11639,11692};
    
int angle_2[404] = {11744,11794,11843,11891,11936,11980,12022,12062,12099,12134,12166,12196,12223,12247,12268,12287,12302,12314,12323,12328,12331,12330,12326,12318,12307,12294,12276,
    12256,12233,12207,12177,12145,12110,12073,12033,11990,11946,11899,11850,11799,11747,11693,11638,11581,11524,11466,11407,11348,11289,11230,11171,11113,11055,10998,
    10942,10887,10834,10782,10732,10684,10638,10594,10553,10514,10479,10446,10416,10389,10366,10346,10329,10315,10306,10300,10297,10298,10303,10311,10323,10339,10358,
    10380,10406,10435,10467,10502,10540,10581,10624,10670,10718,10768,10819,10872,10926,10981,11037,11093,11150,11206,11262,11317,11371,11423,11474,11523,11569,11613,
    11654,11691,11725,11754,11779,11800,11815,11825,11830,11829,11821,11808,11787,11760,11726,11684,11635,11578,11514,11442,11361,11273,11176,11071,10958,10837,10708,
    10571,10425,10272,10112,9944,9768,9586,9397,9201,8999,8792,8579,8361,8139,7913,7683,7451,7216,6979,6741,6503,6265,6028,5793,5559,5329,5103,4882,4666,4457,4255,4061,
    3876,3702,3537,3385,3246,3120,3008,2912,2832,2770,2726,2701,2695,2711,2749,2809,2892,3000,3133,3292,3477,3690,3930,4200,4498,4827,5186,5576,5997,6450,6936,7454,
    8005,8589,9207,9858,10543,11262,12015,12801,13621,14474,15360,16280,17232,18217,19233,20281,21359,22468,23606,24773,25968,27191,28439,29713,31012,32334,33678,35044,
    36429,37834,39256,40694,42147,43613,45091,46580,48079,49584,51096,52612,54131,55651,57170,58688,60201,61709,63210,64702,66184,67653,69108,70548,71970,73373,74755,
    76115,77451,78761,80044,81299,82523,83715,84874,85999,87087,88138,89151,90123,91055,91944,92790,93591,94347,95057,95720,96335,96901,97418,97884,98301,98666,98980,
    99242,99453,99611,99717,99770,99771,99720,99617,99462,99255,98997,98689,98330,97921,97463,96957,96403,95803,95156,94465,93729,92951,92131,91270,90369,89430,88455,
    87444,86398,85320,84210,83071,81903,80708,79488,78245,76979,75693,74388,73065,71728,70376,69012,67637,66254,64863,63467,62067,60664,59260,57857,56457,55060,53669,
    52284,50908,49541,48185,46842,45512,44198,42899,41617,40354,39111,37888,36686,35507,34351,33219,32112,31030,29975,28947,27946,26972,26027,25111,24224,23366,22537,
    21738,20969,20230,19520,18841,18191,17570,16979,16417,15884,15380,14903,14455,14033,13639,13271,12929,12612,12319,12051,11805,11583,11382,11203,11043,10904,10784,
  10681,10596,10528,10475,10437,10413,10402,10404,10418,10442,10477,10520,10572,10632,10682};
    
Uint8 NODE_ID = 1;                          //EPOS的节点ID
//Uint8 NODE_ID1 = 2;

Epos Controller,Controller1;        //控制器对象

// 描述  : "主机"的主函数 
int main(void){
	Uint32 status, velocity;
	Uint32 speed;

    SysTick_Init();                             //延迟中断定时器
    
    USART1_Config();                            //初始化串口模块
    
    TIME_NVIC_Configuration();
    TIME2_Configuration();
    
    CAN_Config();                               //配置CAN模块   
    
    printf("board init complete!\r\n");
    Epos_Delay(500);    
    
  Epos_Init(&Controller,  NOT_USED, NODE_ID );      //初始化最大加速度，速度，跟踪误差，波特率1M/s，
  //Epos_Init(&Controller1, NOT_USED, NODE_ID1);
    printf("\r\ninitial EPOS done!\r\n\r\n");
    
    Epos_Delay(500);    
    
    //******** 模式设置 *******
    Epos_setMode(&Controller, Profile_Velocity_Mode);
    //Epos_setMode(&Controller1, CSP);
    
    Epos_Delay(500);    
    //SDO_Read(&Controller,OD_STATUS_WORD,0x00);            //Switched On    Status=0x0140 绿灯闪烁
    //printf("\r\n%X\r\n",SDO_Read(&Controller,OD_STATUS_WORD,0x00));
    printf("\r\nstatus\r\n");
    
    //******** 使能EPOS *******
    Epos_OperEn(&Controller);                                               //Switch On Disable to Operation Enable
    //Epos_OperEn(&Controller1);

    printf("\r\nenable EPOS\r\n\r\n");
    
    
    //******** EPOS复零位 *******
    //SDO_Write(&Controller, Target_pos,0x00,0); //home position, 设为0
    //SDO_Write(&Controller1,PM_SET_VALUE,0x00,0x00);
    
    printf("\r\nEPOS set 0 \r\n\r\n");
    //SDO_Write(&Controller, Target_Velocity,0x00,1000); //home position, 设为0
    //SDO_Write(&Controller1,PM_SET_VALUE,0x00,10682);
    
    //Epos_Delay(2000); 
    
    
    //******** Read Angels *******
    //printf("\r\nEPOS control beginning!\r\n\r\n");

    /*NMT_Pre(&Controller, ALL);                        
    SDO_Read(&Controller,OD_STATUS_WORD,0x00);            

    PDO_Config(&Controller);
    SDO_Read(&Controller,0x1400,0x01);
    SDO_Read(&Controller,0x1600,0x00);
    SDO_Read(&Controller,0x1600,0x01);

    NMT_Start(&Controller, ALL);
    SDO_Read(&Controller,OD_STATUS_WORD,0x00);          //第九位为1   Status=0x0337 绿灯常量*/
    

    //SDO_Write(&Controller, 0x206B, 0x00, 500);
    //SDO_Write(&Controller1, 0x206B, 0x00, 1000);
    
    
    /*while(1){
        
        pos = 30000*sin(PI*x/50);                     //从0-30000变化  发送太快，数据间隔太大，电机抖动
        //pos = (x>=323)? angle_2[x-323]:angle_1[x];
        //PDO_Write(201, pos);
        
        angle_sensor = SDO_Read(&Controller,Pos_Actual_Value,0x00);
        
        PM_SetAngle(&Controller, pos);
        //PM_SetAngle(&Controller1,pos);
        
        
        Epos_Delay(7);                                                      //delay 10ms
        printf("%d\t%d\r\n",x,angle_sensor);
        if( ++x==727 ){x = 0;}
    }*/
    
    //TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
			   SDO_Write(&Controller, Motion_Profile_Type,0x00,0);
			   SDO_Write(&Controller, Profile_Acceleration,0x00,1000);
			   SDO_Write(&Controller, Profile_Deceleration,0x00,1000);
				 SDO_Write(&Controller,Target_Velocity,0x00,100);

    
    while(1){

			status = SDO_Read(&Controller, 0x6041,0x00);
			Epos_Delay(2); 
			speed = SDO_Read(&Controller, Velocity_Demand_Value,0x00);		
			Epos_Delay(2); 
			printf("status:0x%X\t%d\r\n",status,speed);
			speed = SDO_Read(&Controller, OP_MODE_Read,0x00);	
			velocity = SDO_Read(&Controller, Target_Velocity,0x00);	
			Epos_Delay(2); 
			printf("mode:%d\t%d\r\n",speed, velocity);
			Epos_Delay(10); 
			/*if(flag == 0){
					flag = 0xff;
					//printf("o\r\n");
			}*/
    }
}
