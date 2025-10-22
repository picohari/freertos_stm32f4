


#if 1

static void ADC_thread (void const * arg)
{

  (void) arg;

  char adc_string[16];
  char adc_temp[16];


#if 0
  /*##-3- Start the conversion process and enable interrupt ##################*/  
  if(HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
#endif

  while (1) {

#if 0
    // Polling mode

    /*##-3- Start the conversion process #######################################*/  
    if(HAL_ADC_Start(&AdcHandle) != HAL_OK)
    {
      /* Start Conversation Error */
      Error_Handler();
    }

    /*##-4- Wait for the end of conversion #####################################*/  
     /*  Before starting a new conversion, you need to check the current state of 
          the peripheral; if its busy you need to wait for the end of current
          conversion before starting a new one.
          For simplicity reasons, this example is just waiting till the end of the 
          conversion, but application may perform other tasks while conversion 
          operation is ongoing. */
    HAL_ADC_PollForConversion(&AdcHandle, 10);

    /* Check if the continuous conversion of regular channel is finished */
    if((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
    {
      /*##-5- Get the converted value of regular channel #######################*/
      uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);

      float volt;

      volt = (((float)uhADCxConvertedValue * 3300) / 4096) / 1000;

      sprintf(adc_string, "%.3f", volt);

      if(uhADCxConvertedValue < 2048) {
        gwinHide(ghLabelADCindicOK);
        gwinShow(ghLabelADCindicFAIL);
      } else {
        gwinShow(ghLabelADCindicOK);
        gwinHide(ghLabelADCindicFAIL);
      }

      gwinSetText(ghLabelADCvalue, adc_string, 1);
      //gwinRedraw(ghLabelADCval);

      gwinSetDefaultFont(fixed_7x14);
      gwinSetDefaultColor(White);
      gwinFillString(ghLabelADCindicOK, 2, 1, "OK");
      gwinFillString(ghLabelADCindicFAIL, 2, 1, "ERR");

    }

#endif

#if 0
    /*##-3- Start the conversion process and enable interrupt ##################*/  
    if(HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
    {
      /* Start Conversation Error */
      Error_Handler();
    }
#endif


#if 0
  /*##-3- Start the conversion process #######################################*/  
  if(HAL_ADC_Start(&AdcTempHandle) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
  
  /*##-4- Wait for the end of conversion #####################################*/  
   /*  Before starting a new conversion, you need to check the current state of 
        the peripheral; if its busy you need to wait for the end of current
        conversion before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        conversion, but application may perform other tasks while conversion 
        operation is ongoing. */
  HAL_ADC_PollForConversion(&AdcTempHandle, 10);
  
  /* Check if the continuous conversion of regular channel is finished */
  if((HAL_ADC_GetState(&AdcTempHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
  {

    HAL_ADC_Stop(&AdcTempHandle);

    /*##-5- Get the converted value of regular channel #######################*/
    adc_temp_value = (float)HAL_ADC_GetValue(&AdcTempHandle);
  }

  temperature = 25.0f+(3.3f*(float)adc_temp_value/(float)4096-0.76f)/0.0025f;

  sprintf(adc_temp, "%.3f%cC", temperature, 176);

  gwinSetText(ghScaleADCvalue, adc_temp, 1);
#endif



#if 0
    //float volt;
    //volt = (((float)uhADCxConvertedValue * 3300) / 4096) / 1000;
    //sprintf(adc_string, "%.3f", volt);
    //sprintf(adc_string, "%d", gwinSliderGetPosition(ghSliderADCvalue));

/*
    if(uhADCxConvertedValue < 2048) {
      gwinHide(ghLabelADCindicOK);
      gwinShow(ghLabelADCindicFAIL);
    } else {
      gwinShow(ghLabelADCindicOK);
      gwinHide(ghLabelADCindicFAIL);
    }
*/
    //gwinSetText(ghLabelADCvalue, adc_string, 1);
    //gwinRedraw(ghLabelADCval);

    gwinSetDefaultFont(fixed_7x14);
    gwinSetDefaultColor(White);
    gwinFillString(ghLabelADCindicOK, 2, 1, "OK");
    gwinFillString(ghLabelADCindicFAIL, 2, 1, "ERR");
#endif

    osDelay(250);
  }

}



/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and 
  *         you can add your own implementation.    
  * @retval None
  */
#if 0
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{

  /* Get the converted value of regular channel */
  uhADCxConvertedValue = HAL_ADC_GetValue(AdcHandle);

  /* Stop the ISR, otherwise a loop would permanently call this !!! */
  HAL_ADC_Stop_IT(AdcHandle);

#if 0
  // Does not work here...     
  char adc_string[16];

  float volt;

  volt = (((float)uhADCxConvertedValue * 3300) / 4096) / 1000;

  sprintf(adc_string, "%.3f", volt);

  if(uhADCxConvertedValue < 2048) {
    gwinHide(ghLabelADCindicOK);
    gwinShow(ghLabelADCindicFAIL);
  } else {
    gwinShow(ghLabelADCindicOK);
    gwinHide(ghLabelADCindicFAIL);
  }

  gwinSetText(ghLabelADCvalue, adc_string, 1);
  //gwinRedraw(ghLabelADCval);

  gwinSetDefaultFont(fixed_7x14);
  gwinSetDefaultColor(White);
  gwinFillString(ghLabelADCindicOK, 2, 1, "OK");
  gwinFillString(ghLabelADCindicFAIL, 2, 1, "ERR");
#endif


}
#endif

#endif


