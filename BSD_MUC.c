/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */ 

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "string.h"
#include "stdbool.h"
/*-----------------------------------------------GPIO/pin-----------------------------------------------------*/
//3/18/2024
/*------------------------------------------------------------------------------------------------------------*/
#define  R_LS2  AVR32_PIN_PD21
#define  R_LS3  AVR32_PIN_PD22
#define  R_LS4  AVR32_PIN_PD23
#define  R_LS5  AVR32_PIN_PD24
#define  R_LS6  AVR32_PIN_PD27
#define  R_LS7  AVR32_PIN_PD28
#define  R_LS8  AVR32_PIN_PD29
#define  R_LS9  AVR32_PIN_PD30
#define  R_LS10 AVR32_PIN_PD14
#define  R_LS1  AVR32_PIN_PA20
#define  R_Wifi AVR32_PIN_PC19

#define usart0          (&AVR32_USART0)
#define usart_Tx        AVR32_USART0_TXD_1_PIN
#define usart_Tx_fun    AVR32_USART0_TXD_1_FUNCTION
#define usart_Rx        AVR32_USART0_RXD_1_PIN
#define usart_Rx_fun    AVR32_USART0_RXD_1_FUNCTION

#define Freq           4000000
/*------------------------------------------ ADC Single channel------------------------------------------*/
#define ADC_SEQUENCE     2

#define V5_UUT_INP          AVR32_ADCIFA_INP_ADCIN2
#define V5_UUT_INN          AVR32_ADCIFA_INN_GNDANA
#define V5_PIN                AVR32_ADCIN2_PIN
#define V5_FUNCTION            AVR32_ADCIN2_FUNCTION

#define Tp10_INP            AVR32_ADCIFA_INP_ADCIN6
#define Tp10_INN            AVR32_ADCIFA_INN_GNDANA
#define Tp10_PIN            AVR32_ADCIN6_PIN
#define Tp10_FUNCTION        AVR32_ADCIN6_FUNCTION
/*------------------------------------------------- ADC Startups-------------------------------------------*/
adcifa_sequencer_opt_t adcifa_sequence_opt ;
adcifa_sequencer_conversion_opt_t adcifa_sequence_conversion_opt[ADC_SEQUENCE];
adcifa_opt_t adc_config_t ;



/*----global variables*/
int next_state;
char array [6],*string_array;
char *String_value;

void Init_Uart_Mode_Driver(void);
void ADC_Init(void);
float ADC_Calculate(int);
float ADC_Out(int);
void Relay_CallBack(void);
char* UART_Receive(char*);
void ALL_Relay_Callback(void);


int main (void)
{
    /* Insert system clock initialization code here (sysclk_init()). */

    board_init();
    sysclk_init();
    Init_Uart_Mode_Driver();
    ADC_Init();

    float ADC_Result;

    ALL_Relay_Callback();

        while(1){

            String_value = UART_Receive(array);                     //Receive Command

            if(strcmp("BSD01",String_value)==0){
                delay_ms(550);
                gpio_set_gpio_pin(R_LS1);
                ADC_Result = ADC_Out(0);
                ADC_Result= 2*ADC_Result;
                delay_ms(1475);
                sprintf(string_array,"TEST_JIG_VOLTAGE: %.3f V\n\0",ADC_Result);//LS1
                usart_write_line(usart0,string_array);
                sprintf(string_array,"Max:5.25 V  Min:4.75 V\n\0");usart_write_line(usart0,string_array); // change if required
                if(ADC_Result >= 4.75 && ADC_Result <= 5.25){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                ADC_Result=0;
            }

            if(strcmp("BSD02",String_value)==0){                   //Command Interception

                next_state=1;


                    while(next_state<8){                              //sub polling

                            switch(next_state){                    //state switch and read ADC values
                                case 1: {
                                            Relay_CallBack();
                                            delay_ms(550);
                                            gpio_set_gpio_pin(R_LS2);
                                            ADC_Result = ADC_Out(1);
                                            ADC_Result= 2*ADC_Result;
                                            delay_ms(1200);
                                            sprintf(string_array,"TP42: %.3f V\n\0",ADC_Result);//LS2
                                            usart_write_line(usart0,string_array);
                                            sprintf(string_array,"Max:5.25V  Min:4.75 V\n\0");usart_write_line(usart0,string_array); // change if required
                                            if(ADC_Result >= 4.75 && ADC_Result <= 5.25){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                                            else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                                            ADC_Result=0;
                                            next_state=2;
                                            break;
                                        }
                                case 2: {
                                            Relay_CallBack();//gpio_clr_gpio_pin(R_LS2);
                                            delay_ms(550);
                                            gpio_set_gpio_pin(R_LS3);
                                            ADC_Result = ADC_Out(1);
                                            ADC_Result= 2*ADC_Result;
                                            sprintf(string_array,"TP33: %.3f V\n\0",ADC_Result);//LS3
                                            usart_write_line(usart0,string_array);
                                            sprintf(string_array,"Max:4.3V  Min:3.3 V\n\0");usart_write_line(usart0,string_array); // change if required
                                            if(ADC_Result >= 3.3 && ADC_Result <= 4.3 ){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                                            else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                                            delay_ms(1200);
                                            ADC_Result=0;
                                            next_state=3;
                                            break;
                                        }
                                case 3: {
                                            Relay_CallBack();//gpio_clr_gpio_pin(R_LS3);
                                            delay_ms(550);
                                            gpio_set_gpio_pin(R_LS4);
                                            ADC_Result = ADC_Out(1);
                                            ADC_Result= 2*ADC_Result;
                                            delay_ms(1200);
                                            sprintf(string_array,"TP32: %.3f V\n\0",ADC_Result);//LS4
                                            usart_write_line(usart0,string_array);
                                            sprintf(string_array,"Max:4.3 V  Min:3.3 V\n\0");usart_write_line(usart0,string_array); // change if required
                                            if(ADC_Result >= 3.3 && ADC_Result <= 4.3){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                                            else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                                            ADC_Result=0;
                                            next_state=4;
                                            break;
                                        }
                                case 4: {
                                            Relay_CallBack();//gpio_clr_gpio_pin(R_LS4);
                                            delay_ms(550);
                                            gpio_set_gpio_pin(R_LS5);
                                            ADC_Result = ADC_Out(1);
                                            ADC_Result= 2*ADC_Result;
                                            delay_ms(1200);
                                            sprintf(string_array,"TP39: %.3f V\n\0",ADC_Result);//LS5
                                            usart_write_line(usart0,string_array);
                                            sprintf(string_array,"Max:4.3 V  Min:3.3 V\n\0");usart_write_line(usart0,string_array); // change if required
                                            if(ADC_Result >= 3.3 && ADC_Result <= 4.3){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                                            else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                                            ADC_Result=0;
                                            next_state=5;
                                            break;
                                        }

                                case 5: {
                                            Relay_CallBack();//gpio_clr_gpio_pin(R_LS5);
                                            delay_ms(550);
                                            gpio_set_gpio_pin(R_LS6);
                                            ADC_Result = ADC_Out(1);
                                            ADC_Result= 2*ADC_Result;
                                            delay_ms(1200);
                                            sprintf(string_array,"TP38: %.3f V\n\0",ADC_Result);//LS6
                                            usart_write_line(usart0,string_array);
                                            sprintf(string_array,"Max:4.3 V  Min:3.3 V\n\0");usart_write_line(usart0,string_array); // change if required
                                            if(ADC_Result >= 3.3 && ADC_Result <= 4.3){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                                            else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                                            ADC_Result=0;
                                            next_state=6;
                                            break;
                                        }
                                case 6: {
                                            Relay_CallBack();//gpio_clr_gpio_pin(R_LS6);
                                            delay_ms(550);
                                            gpio_set_gpio_pin(R_LS7);
                                            ADC_Result = ADC_Out(1);
                                            ADC_Result= 2*ADC_Result;
                                            delay_ms(1200);
                                            sprintf(string_array,"TP1: %.3f V\n\0",ADC_Result);//LS7
                                            usart_write_line(usart0,string_array);
                                            sprintf(string_array,"Max:3.0 V  Min:2.6 V\n\0");usart_write_line(usart0,string_array); // change if required
                                            if(ADC_Result >= 2.6 && ADC_Result <= 3.0){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                                            else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                                            gpio_clr_gpio_pin(R_Wifi);
                                            ADC_Result=0;
                                            next_state=7;
                                            delay_ms(1800);

                                            sprintf(string_array,"Flash Wifi\n\0");
                                            usart_write_line(usart0,string_array);
											
											delay_ms(1200);

                                            String_value = UART_Receive(array);

                                            if(strcmp(String_value,"FWIFI")==0){
                                                sprintf(string_array,"Flash did\n\0");
                                                usart_write_line(usart0,string_array);
                                                gpio_set_gpio_pin(R_Wifi);
                                            }

                                            break;
                                        }

                                case 7:{
                                            Relay_CallBack();
                                            delay_ms(1200);
                                            sprintf(string_array,"COM-END\n\0",ADC_Result);
                                            usart_write_line(usart0,string_array);
                                            ADC_Result=0;
                                            next_state=8;
                                            break;
                                        }

                            }                                               // end of switch

                   }                                                          // sub polling


        }
                                                                // end of if
        if(strcmp("BSD03",String_value)==0){                   //Command Interception

            next_state=1;


            while(next_state<5){                              //sub polling

                switch(next_state){                    //state switch and read ADC values
                    case 1: {
                        Relay_CallBack();//gpio_clr_gpio_pin(R_LS7);
                        delay_ms(550);
                        gpio_set_gpio_pin(R_LS8);  //R_LS8
                        ADC_Result = ADC_Out(1);
                        ADC_Result= 2*ADC_Result;
                        delay_ms(1200);
                        sprintf(string_array,"TP43: %.3f V\n\0",ADC_Result);//LS8
                        usart_write_line(usart0,string_array);
                        sprintf(string_array,"Max:4.3 V  Min:3.3 V\n\0");usart_write_line(usart0,string_array); // change if required
                        if(ADC_Result >= 3.3 && ADC_Result <= 4.3){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                        else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                        ADC_Result=0;
                        next_state=2;
                        break;
                    }
                    case 2: {
                        Relay_CallBack();//gpio_clr_gpio_pin(R_LS8);
                        delay_ms(550);
                        gpio_set_gpio_pin(R_LS10); //R_LS9
                        ADC_Result = ADC_Out(1);
                        ADC_Result= 2*ADC_Result;
						delay_ms(1200);
                        sprintf(string_array,"TP27: %.3f V\n\0",ADC_Result);//LS9
                        usart_write_line(usart0,string_array);
                        sprintf(string_array,"Max:1.9 V  Min:1.7 V\n\0");usart_write_line(usart0,string_array); // change if required
                        if(ADC_Result >= 1.7 && ADC_Result <= 1.9){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                        else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                        ADC_Result=0;
                        next_state=3;
                        break;
                    }
                    case 3: {
                        Relay_CallBack(); //gpio_clr_gpio_pin(R_LS8);
                        delay_ms(550);
                        gpio_set_gpio_pin(R_LS9); //R_Ls10
                        ADC_Result = ADC_Out(1);
                        ADC_Result= 2*ADC_Result;
						delay_ms(1200);
                        sprintf(string_array,"TP61: %.3f V\n\0",ADC_Result);//LS10
                        usart_write_line(usart0,string_array);
                        sprintf(string_array,"Max:1.9 V  Min:1.7 V\n\0");usart_write_line(usart0,string_array); // change if required
                        if(ADC_Result >= 1.7 && ADC_Result <= 1.9){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                        else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
                        ADC_Result=0;
                        next_state=4;
                        break;
                    }
                    case 4:{
                        Relay_CallBack();
                        delay_ms(1200);
                        sprintf(string_array,"COM-END\n\0",ADC_Result);
                        usart_write_line(usart0,string_array);
                        ADC_Result=0;
                        next_state=5;
                        break;
                    }

                }                                               // end of switch

            }                                                          // sub polling
        }

        if(strcmp("BSD00",String_value)==0){
            ALL_Relay_Callback();
        }

        if(strcmp("TSD01",String_value)==0){
        gpio_set_gpio_pin(R_LS1);
        }

        if(strcmp("TSDF1",String_value)==0){
            gpio_set_gpio_pin(R_LS10);
        }

        if(strcmp("TSD02",String_value)==0){                   //Command Interception

            next_state=1;
            while(next_state<7){                              //sub polling

                switch(next_state){                    //state switch and read ADC values
                    case 1: {
                        delay_ms(800);
                        gpio_set_gpio_pin(R_LS2);
                        ADC_Result = ADC_Out(1);
                        ADC_Result= 2*ADC_Result;
						
                        sprintf(string_array,"TP14: %.3f V\n\0",ADC_Result);//LS2
                        usart_write_line(usart0,string_array);
                        sprintf(string_array,"Max:3.3V  Min:2.50 V\n\0");usart_write_line(usart0,string_array); // change if required
                        if(ADC_Result >= 2.50 && ADC_Result <= 3.3){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                        else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
						delay_ms(800);
                        ADC_Result=0;
                        next_state=2;
                        break;
                    }
                    case 2: {
                        Relay_CallBack();//gpio_clr_gpio_pin(R_LS4);
                        delay_ms(800);
                        gpio_set_gpio_pin(R_LS5);
                        ADC_Result = ADC_Out(1);
                        ADC_Result= 2*ADC_Result;
             
                        sprintf(string_array,"TP17: %.3f V\n\0",ADC_Result);//LS5
                        usart_write_line(usart0,string_array);
                        sprintf(string_array,"Max:3.3 V  Min:2.50 V\n\0");usart_write_line(usart0,string_array); // change if required
                        if(ADC_Result >= 2.50 && ADC_Result <= 3.3){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                        else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
						delay_ms(800);
                        ADC_Result=0;
                        next_state=3;
                        break;
                    }
                    case 3: {
                        Relay_CallBack();//gpio_clr_gpio_pin(R_LS3);
                        delay_ms(800);
                        gpio_set_gpio_pin(R_LS4);
                        ADC_Result = ADC_Out(1);
                        ADC_Result= 2*ADC_Result;
						
                        sprintf(string_array,"TP18: %.3f V\n\0",ADC_Result);//LS4
                        usart_write_line(usart0,string_array);
                        sprintf(string_array,"Max:3.3 V  Min:2.50 V\n\0");usart_write_line(usart0,string_array); // change if required
                        if(ADC_Result >= 2.50 && ADC_Result <= 3.3){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                        else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
						delay_ms(800);
                        ADC_Result=0;
                        next_state=4;
                        break;
                    }
                    case 4: {
                        Relay_CallBack();//gpio_clr_gpio_pin(R_LS2);
                        delay_ms(800);
                        gpio_set_gpio_pin(R_LS3);
                        ADC_Result = ADC_Out(1);
                        ADC_Result= 2*ADC_Result;
                        
                        sprintf(string_array,"TP19: %.3f V\n\0",ADC_Result);//LS3
                        usart_write_line(usart0,string_array);
                        sprintf(string_array,"Max:3.3 V  Min:2.50 V\n\0");usart_write_line(usart0,string_array); // change if required
                        if(ADC_Result >= 2.50 && ADC_Result <=3.3 ){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                        else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
						delay_ms(800);
                        ADC_Result=0;
                        next_state=5;
                        break;
                    }

                    case 5: {
                        Relay_CallBack();//gpio_clr_gpio_pin(R_LS5);
                        delay_ms(800);
                        gpio_set_gpio_pin(R_LS6);
                        ADC_Result = ADC_Out(1);
                        ADC_Result= 2*ADC_Result;
                       
                        sprintf(string_array,"TP2: %.3f V\n\0",ADC_Result);//LS6
                        usart_write_line(usart0,string_array);
                        sprintf(string_array,"Max:1.90 V  Min:1.50 V\n\0");usart_write_line(usart0,string_array); // change if required
                        if(ADC_Result >= 1.50 && ADC_Result <= 1.90){sprintf(string_array,"PASS\n\0");usart_write_line(usart0,string_array); }
                        else{sprintf(string_array,"FAIL\n\0");usart_write_line(usart0,string_array); }
						delay_ms(800);
                        ADC_Result=0;
                        next_state=6;
                        break;
                    }
                    case 6: {
                        Relay_CallBack();//gpio_clr_gpio_pin(R_LS7);
                        delay_ms(1200);
                        sprintf(string_array,"COM-END\n\0",ADC_Result);
                        usart_write_line(usart0,string_array);
                        ADC_Result=0;
                        next_state=7;
                        break;
                    }
                }                                               // end of switch
            }                                                          // sub polling
        }
        if(strcmp("TSD00",String_value)==0){
            gpio_clr_gpio_pin(R_LS1);
        }

        if(strcmp("TSDF0",String_value)==0){
            gpio_clr_gpio_pin(R_LS10);
        }

        if(strcmp("BSBF1",String_value)==0){
            gpio_clr_gpio_pin(R_Wifi); // box flash ble
        }
        if(strcmp("BSBF0",String_value)==0){
            gpio_set_gpio_pin(R_Wifi); // box flash ble
        }


    }                                                                  // end of main while

    /*while(i<50){

        ADC_Result = ADC_Out();
        sprintf(array,"%.3f\n",ADC_Result);
        usart_write_line(usart0,array);
        i++;
        delay_ms(25);

    }*/
    /* Insert application code here, after the board has been initialized. */
}



float ADC_Calculate(int k){
    float ADC_Return=0.0f;
    int16_t adc_values[ADC_SEQUENCE];

    adcifa_start_sequencer(&AVR32_ADCIFA, 0);
    if (adcifa_get_values_from_sequencer(&AVR32_ADCIFA, 0,
    &adcifa_sequence_opt, adc_values) == ADCIFA_STATUS_COMPLETED) {
        ADC_Return = 3*((float)adc_values[k]/2043);
        ADCIFA_clear_eos_sequencer_0();
        //adcifa_start_sequencer(&AVR32_ADCIFA, 0);
        //delay_ms(10);
    }
    adcifa_start_sequencer(&AVR32_ADCIFA, 0);

    return ADC_Return;
}



float ADC_Out(int i){

    float ADC_Sum = 0,ADC_Avg=0;
    int iter;

    for(iter=0;iter<100;iter++){
        ADC_Sum +=ADC_Calculate(i);
        delay_ms(5);
    }
    delay_ms(50);
    ADC_Avg = ADC_Sum/100;
    //sprintf(string_array,"avg: %.3f\n",ADC_Avg);
    //usart_write_line(usart0,string_array);
    return ADC_Avg;
}


void ADC_Init(){
    /*gpio mapping*/
    const gpio_map_t ADCIFA_GPIO_MAP = {
        {AVR32_ADCREF0_PIN, AVR32_ADCREF0_FUNCTION},
        {AVR32_ADCREFP_PIN, AVR32_ADCREFP_FUNCTION},
        {AVR32_ADCREFN_PIN, AVR32_ADCREFN_FUNCTION},
        //{V5_PIN,V5_FUNCTION},
        {Tp10_PIN,Tp10_FUNCTION},
        {V5_PIN,V5_FUNCTION}
    };
	
    gpio_enable_module(ADCIFA_GPIO_MAP, sizeof(ADCIFA_GPIO_MAP) /sizeof(ADCIFA_GPIO_MAP[0]));

    adc_config_t.frequency                = 1000000;
    adc_config_t.reference_source         = ADCIFA_REF06VDD;
    adc_config_t.sample_and_hold_disable  = false;
    adc_config_t.single_sequencer_mode    = true;
    adc_config_t.free_running_mode_enable = false;
    adc_config_t.sleep_mode_enable        = false;
    adc_config_t.mux_settle_more_time     = false;

        adcifa_get_calibration_data(&AVR32_ADCIFA, &adc_config_t);
        adcifa_calibrate_offset(&AVR32_ADCIFA, &adc_config_t, sysclk_get_cpu_hz());
        adcifa_configure(&AVR32_ADCIFA, &adc_config_t, sysclk_get_cpu_hz());

            adcifa_sequence_opt.convnb              = ADC_SEQUENCE;
            adcifa_sequence_opt.resolution          = ADCIFA_SRES_12B;
            adcifa_sequence_opt.trigger_selection   = ADCIFA_TRGSEL_SOFT;
            adcifa_sequence_opt.start_of_conversion = ADCIFA_SOCB_SINGLECONV;
            adcifa_sequence_opt.sh_mode             = ADCIFA_SH_MODE_OVERSAMP;
            adcifa_sequence_opt.half_word_adjustment= ADCIFA_HWLA_NOADJ;
            adcifa_sequence_opt.software_acknowledge= ADCIFA_SA_NO_EOS_SOFTACK;

            adcifa_sequence_conversion_opt[0].channel_p = V5_UUT_INP;
            adcifa_sequence_conversion_opt[0].channel_n = V5_UUT_INN;
            adcifa_sequence_conversion_opt[0].gain      = ADCIFA_SHG_1;

            adcifa_sequence_conversion_opt[1].channel_p = Tp10_INP;
            adcifa_sequence_conversion_opt[1].channel_n = Tp10_INN;
            adcifa_sequence_conversion_opt[1].gain      = ADCIFA_SHG_1;

            adcifa_configure_sequencer(&AVR32_ADCIFA, 0, &adcifa_sequence_opt,adcifa_sequence_conversion_opt);
           //adcifa_start_itimer(&AVR32_ADCIFA,15);
}


void Init_Uart_Mode_Driver(){

    static const gpio_map_t USART_GPIO_MAP = {
        {AVR32_USART0_RXD_1_PIN, AVR32_USART0_RXD_1_FUNCTION},
        {AVR32_USART0_TXD_1_PIN, AVR32_USART0_TXD_1_FUNCTION}};
        static const usart_options_t USART_OPTIONS =
        {
            .baudrate     = 115200,
            .charlength   = 8,
            .paritytype   = USART_NO_PARITY,
            .stopbits     = USART_1_STOPBIT,
            .channelmode  = USART_NORMAL_CHMODE
        };
        gpio_enable_module(USART_GPIO_MAP,sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));    // Assign GPIO to USART.
        usart_init_rs232(usart0, &USART_OPTIONS,Freq);
        stdio_serial_init(usart0,  &USART_OPTIONS);

    }

void Relay_CallBack(){

        gpio_clr_gpio_pin(R_LS2);
        gpio_clr_gpio_pin(R_LS3);
        gpio_clr_gpio_pin(R_LS4);
        gpio_clr_gpio_pin(R_LS5);
        gpio_clr_gpio_pin(R_LS6);
        gpio_clr_gpio_pin(R_LS7);
        gpio_clr_gpio_pin(R_LS8);
        gpio_clr_gpio_pin(R_LS9);
        gpio_clr_gpio_pin(R_LS10);
}

void ALL_Relay_Callback(){

    gpio_clr_gpio_pin(R_LS2);
    gpio_clr_gpio_pin(R_LS3);
    gpio_clr_gpio_pin(R_LS4);
    gpio_clr_gpio_pin(R_LS5);
    gpio_clr_gpio_pin(R_LS6);
    gpio_clr_gpio_pin(R_LS7);
    gpio_clr_gpio_pin(R_LS8);
    gpio_clr_gpio_pin(R_LS9);
    gpio_clr_gpio_pin(R_LS10);
    gpio_clr_gpio_pin(R_LS1);
	gpio_clr_gpio_pin(R_Wifi);
}


char* UART_Receive(char *array){

    int i=0;
        for(i=0;i<5;i++){
            array[i]=usart_getchar(usart0);
        }
        array[5]='\0';
        return array;
}




