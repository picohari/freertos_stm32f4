  
FATFS   USBDISKFatFs;           /* File system object for USB disk logical drive */
char    USBDISKPath[4];         /* USB Host logical drive path */

FIL     MyFile;                 /* File object */

USBH_HandleTypeDef hUSB_Host;   /* USB Host handle */

typedef enum {
  DISCONNECTION_EVENT = 1,  
  CONNECTION_EVENT,    
}MSC_ApplicationTypeDef;

typedef enum {
  DISK_READY_EVENT = 1,  
  DISK_REMOVE_EVENT,    
}FatFs_DiskTypeDef;


osMessageQId AppliEvent;
osMessageQId DiskEvent;






/**
  * @brief  Start task
  * @param  pvParameters not used
  * @retval None
  */
  static void USB_thread(void const * arg)
  {
  
    (void) arg;
  
    osEvent event;
    
    /* Link the USB Host disk I/O driver */
    if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
      LOG_DEBUG("Driver linked");   // TODO: Check include log.h
  
      /* Init Host Library */
      USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
      
      /* Add Supported Class */
      USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
      
      /* Start Host Process */
      USBH_Start(&hUSB_Host);
      
      for( ;; )
      {
        event = osMessageGet(AppliEvent, osWaitForever);
        
        if(event.status == osEventMessage)
        {
          switch(event.value.v)
          {
          case CONNECTION_EVENT:
            MSC_Application();
            break;
            
          case DISCONNECTION_EVENT:
            f_mount(NULL, (TCHAR const*)"", 0);
            break;
            
          default:
            break;
          }
        }
      }
    }
  }
  
  
  /**
    * @brief  Main routine for Mass Storage Class
    * @param  None
    * @retval None
    */
  static void MSC_Application(void)
  {
    
    //FRESULT res;                                          /* FatFs function common result code */
    //uint32_t byteswritten, bytesread;                     /* File write/read counts */
    //uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
    //Suint8_t rtext[100];                                   /* File read buffer */
    
    GFILE* imgFile;
    static gdispImage myImage;
  
  
    /* Register the file system object to the FatFs module */
    if(f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
  
  
  #if 1
    else {
  
      /* Allow Second task to have access to FatFs */
      osMessagePut(DiskEvent, DISK_READY_EVENT, 0);
  
    }
  #endif
  
  
  
    //else
    //{
  
  
  #if 0
      /* Create and Open a new text file object with write access */
      if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
      {
        Error_Handler();
      }
  
      else {
  
        f_close(&MyFile);
        /*End test for file existence*/
  
        imgFile = gfileOpen("tenor.gif", "rb");
  
        gdispImageOpenGFile(&myImage, imgFile);
  
        //taskENTER_CRITICAL();
        //gdispImageDraw(&myImage, 0, 0, 320, 240, 0, 0);
        //taskEXIT_CRITICAL();
        
        delaytime_t delay;
  
        while (1) {
          gdispImageDraw(&myImage, 0, 0, 320, 240, 0, 0);
  
          delay = gdispImageNext(&myImage);
  
          gfxSleepMilliseconds(100);
        }
  
        gdispImageClose(&myImage);
  
        gfileClose(imgFile);
      }
  #endif
  
  
  
  
  
  #if 0
  
      else
      {
        /* Write data to the text file */
        res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
        
        if((byteswritten == 0) || (res != FR_OK))
        {
          /* 'STM32.TXT' file Write or EOF Error */
          Error_Handler();
        }
        else
        {
          /* Close the open text file */
          f_close(&MyFile);
          
          /* Open the text file object with read access */
          if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
          {
            /* 'STM32.TXT' file Open for read Error */
            Error_Handler();
          }
          else
          {
            /* Read data from the text file */
            res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);
            
            if((bytesread == 0) || (res != FR_OK))
            {
              /* 'STM32.TXT' file Read or EOF Error */
              Error_Handler();
            }
            else
            {
              /* Close the open text file */
              f_close(&MyFile);
              
              /* Compare read data with the expected data */
              if((bytesread != byteswritten))
              {                
                /* Read data is different from the expected data */
                Error_Handler();
              }
              else
              {
                /* Success of the demo: no error occurrence */
                BSP_LED_On(LED4);
              }
            }
          }
        }
      }
  #endif
  
    //}
    
    /* Unlink the USB disk I/O driver */
    //FATFS_UnLinkDriver(USBDISKPath);
  
  }
  
  
  /**
    * @brief  User Process
    * @param  phost: Host handle
    * @param  id: Host Library user message ID
    * @retval None
    */
  static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
  {  
    switch(id)
    { 
    case HOST_USER_SELECT_CONFIGURATION:
      break;
      
    case HOST_USER_DISCONNECTION:
      BSP_LED_Off(LED4);
      osMessagePut(AppliEvent, DISCONNECTION_EVENT, 0);
      break;
      
    case HOST_USER_CLASS_ACTIVE:
      osMessagePut(AppliEvent, CONNECTION_EVENT, 0);
      break;
      
    default:
      break; 
    }
  }
  
  
  
  #include "settings.h"
  
  
  static void DISK_thread(void const * arg)
  {
    
    (void) arg;
  
    osEvent event;
    
    //FRESULT res;                                          /* FatFs function common result code */
    //uint16_t byteswritten;                                /* File write count */
    //uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
    
    for( ;; )
    {
      event = osMessageGet(DiskEvent, osWaitForever);
      
      if(event.status == osEventMessage)
      {
        switch(event.value.v)
        {
        case DISK_READY_EVENT:
          writef("USBDisk mounted.\r\n");
          
          osDelay(500);
          
          ini_create_file();
          ini_parse_file("SETTINGS.water");
          
          break;
          
        case DISK_REMOVE_EVENT:
          /* Unlink the USB disk I/O driver */
          FATFS_UnLinkDriver(USBDISKPath);
          break;
          
        default:
          break; 
        }
      }
    }
  }
  