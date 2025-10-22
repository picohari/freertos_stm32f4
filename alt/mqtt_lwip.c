


void messageArrived(MessageData* md)
{
  MQTTMessage* message = md->message;

  writef("  Topic: %.*s\r\n", md->topicName->lenstring.len, md->topicName->lenstring.data);
  writef("Payload: %.*s\r\n", (int)message->payloadlen, (char*)message->payload);

  if (strncmp((char *) message->payload, "on", (int) message->payloadlen) == 0) {
    writef("ON\r\n");
    //gwinSetText(ghScaleADCvalue, "ON", 1);

  } else if (strncmp((char *) message->payload, "off", (int) message->payloadlen) == 0) {
    writef("OFF\r\n");
    //gwinSetText(ghScaleADCvalue, "OFF", 1);
  }
}






static void MQTT_start (void const * arg)
{

  (void) arg;

  int rc = 0;
  Network n;
  Client c;
  unsigned char sendbuf[100];   /* Sending   buffer */
  unsigned char readbuf[100];   /* Receiving buffer */

  ip4_addr_t broker_ipaddr;
  IP4_ADDR( &broker_ipaddr, 192, 168, 1, 101);  // cumulus.h9l.net:15672


  //err_t err;

  mqtt_lwip_init(&n);


  rc = mqtt_lwip_connect(&n, &broker_ipaddr, 1883);

  if (rc != 0) {
    writef("Failed to connect to MQTT server\r\n");

    goto blink_to_death;
  }

  writef("Initializing MQTT service\r\n");

  MQTTClient(&c, &n, 1000, sendbuf, 100, readbuf, 100);

  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  
  char clientID_buf[256] = {0};

  sprintf(clientID_buf, "AL-MS-CU_%02lx%02lx%02lx%02lx%02lx%02lx", 
    (uint32_t) MAC_ADDR0, 
    (uint32_t) MAC_ADDR1, 
    (uint32_t) MAC_ADDR2, 
    (uint32_t) MAC_ADDR3, 
    (uint32_t) MAC_ADDR4, 
    (uint32_t) MAC_ADDR5);  

  data.clientID.cstring = clientID_buf;
  data.username.cstring = "device";
  data.password.cstring = "device";
  
  data.MQTTVersion = 3;
  data.willFlag = 0;
  data.keepAliveInterval = 5;
  data.cleansession = 1;
  
  rc = MQTTConnect(&c, &data);
  
  if (rc == MQTT_FAILURE) {
    writef("Failed to send connect request\r\n");
    goto blink_to_death;
  }

  writef("Subscribing to its topic\r\n");


  char topic_buf[128] = {0};

  sprintf(topic_buf, "%02lx:%02lx:%02lx:%02lx:%02lx:%02lx/do/#",
    (uint32_t) MAC_ADDR0, 
    (uint32_t) MAC_ADDR1, 
    (uint32_t) MAC_ADDR2, 
    (uint32_t) MAC_ADDR3, 
    (uint32_t) MAC_ADDR4, 
    (uint32_t) MAC_ADDR5); 

  rc = MQTTSubscribe(&c, topic_buf, QOS0, messageArrived);

  //rc = MQTTSubscribe(&c, "hello/world", QOS0, messageArrived);
  
  if (rc == MQTT_FAILURE) {
    writef("Failed to subscribe to its topic\r\n");
    MQTTDisconnect(&c);
    goto blink_to_death;
  }

  writef("Connected to MQTT server...\r\n");



  while(1) {

    MQTTMessage message;
    
    unsigned char sendtemp[] = {'0', '0', '.', '0', '0', 0 };

    message.qos = QOS0;
    message.retained = 0;
    message.payload = sendtemp;

#if 1
    memset(sendtemp, 0, sizeof(sendtemp));
    sprintf((char*)sendtemp, "%2.2f", ds18b20[0].Temperature);

    //message.payloadlen = strlen((char*)sendbuf);
    message.payloadlen = 5;

    char topic_buf_2[128] = {0};

    memset(topic_buf_2, 0, sizeof(topic_buf_2));
    sprintf(topic_buf_2, "%02lx:%02lx:%02lx:%02lx:%02lx:%02lx/basement/temp",
      (uint32_t) MAC_ADDR0, 
      (uint32_t) MAC_ADDR1, 
      (uint32_t) MAC_ADDR2, 
      (uint32_t) MAC_ADDR3, 
      (uint32_t) MAC_ADDR4, 
      (uint32_t) MAC_ADDR5); 

    MQTTPublish(&c, topic_buf_2, &message);

    MQTTYield(&c, 1000);

    osDelay(500);
#endif

#if 1

    MQTTMessage online_status;

    unsigned char sendstr[] = {'o', 'n', 'l', 'i', 'n', 'e'};

    online_status.qos = QOS0;
    online_status.retained = 0;
    online_status.payload = sendstr;

    online_status.payloadlen = 6;

    char topic_online_buf[100] = {0};
    

    memset(topic_online_buf, 0, sizeof(topic_online_buf));
    sprintf(topic_online_buf, "%02lx:%02lx:%02lx:%02lx:%02lx:%02lx/online_status",
      (uint32_t) MAC_ADDR0, 
      (uint32_t) MAC_ADDR1, 
      (uint32_t) MAC_ADDR2, 
      (uint32_t) MAC_ADDR3, 
      (uint32_t) MAC_ADDR4, 
      (uint32_t) MAC_ADDR5); 

    MQTTPublish(&c, topic_online_buf, &online_status);

    MQTTYield(&c, 1000);

    osDelay(500);
#endif

  }

  //return RDY_OK;

  blink_to_death:
  
  while(1) { }

  //return RDY_OK;

}


