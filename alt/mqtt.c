


mqtt_client_t mqtt_client;


static int inpub_id;



static void mqtt_incoming_publish_cb(void * arg,
    const char * topic, u32_t tot_len) {
    writef("Incoming publish at topic %s with total length %u\r\n", topic, (unsigned int) tot_len);
    /* Decode topic string into a user defined reference */
    if (strcmp(topic, "print_payload") == 0) {
        inpub_id = 0;
    } else if (topic[0] == 'A') {
        /* All topics starting with 'A' might be handled at the same way */
        inpub_id = 1;
    } else {
        /* For all other topics */
        inpub_id = 2;
    }
}



static void mqtt_incoming_data_cb(void * arg,
    const u8_t * data, u16_t len, u8_t flags) {
    writef("Incoming publish payload with length %d, flags %u\r\n", len, (unsigned int) flags);
    if (flags & MQTT_DATA_FLAG_LAST) {
        /* Last fragment of payload received (or whole part if payload fits receive buffer
        See MQTT_VAR_HEADER_BUFFER_LEN) */
        /* Call function or do action depending on reference, in this case inpub_id */
        if (inpub_id == 0) {
            /* Don't trust the publisher, check zero termination */
            if (data[len - 1] == 0) {
                writef("mqtt_incoming_data_cb: %s\r\n", (const char * ) data);
            }
        } else if (inpub_id == 1) {
            /* Call an 'A' function... */
        } else {
            writef("mqtt_incoming_data_cb: Ignoring payload...\r\n");
        }
    } else {
        /* Handle fragmented payload, store in buffer, write to file or whatever */
    }
}



static void mqtt_sub_request_cb(void * arg, err_t result) {
    /* Just print the result code here for simplicity,
    normal behaviour would be to take some action if subscribe fails like
    notifying user, retry subscribe or disconnect from server */
    writef("Subscribe result: %d\r\n", result);
}



/* Called when publish is complete either with success or failure */
static void mqtt_pub_request_cb(void * arg, err_t result) {
    if (result != ERR_OK) {
        writef("Publish result: %d\r\n", result);
    }
}



static void mqtt_connection_cb(mqtt_client_t * client, void * arg, mqtt_connection_status_t status) {
    err_t err;
    if (status == MQTT_CONNECT_ACCEPTED) {
        writef("mqtt_connection_cb: Successfully connected\r\n");
        /* Setup callback for incoming publish requests */
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

        /* Subscribe to a topic named "subtopic" with QoS level 1, call mqtt_sub_request_cb with result */
        err = mqtt_subscribe(client, "hello/world", 1, mqtt_sub_request_cb, arg);

        if (err != ERR_OK) {
            writef("mqtt_subscribe return: %d\r\n", err);
        }

    } else {
        writef("mqtt_connection_cb: Disconnected, reason: %d\r\n", status);
        
        /* Its more nice to be connected, so try to reconnect */
        //MQTT_connect(client);
    }
}

static void MQTT_connect(mqtt_client_t *client)
{

  ip4_addr_t broker_ipaddr;
  struct mqtt_connect_client_info_t ci;
  err_t err;

  IP4_ADDR( &broker_ipaddr, 192, 168, 1, 74);

  /* Setup an empty client info structure */
  memset(&ci, 0, sizeof(ci));

  /* Minimal amount of information required is client identifier, so set it here */
  ci.client_id   = "stm32";
  ci.client_user = "guest";
  ci.client_pass = "guest";

  /* Initiate client and connect to server, if this fails immediately an error code is returned
  otherwise mqtt_connection_cb will be called with connection result after attempting
  to establish a connection with the server.
  For now MQTT version 3.1.1 is always used */
  err = mqtt_client_connect(client, &broker_ipaddr, MQTT_PORT, mqtt_connection_cb, 0, &ci);

  /* For now just print the result code if something goes wrong */
  if (err != ERR_OK) {
    writef("mqtt_connect return %d\r\n", err);
  }


}




static void MQTT_start (void const * arg)
{

  (void) arg;

#if 0
  mqtt_client_t *mqtt_client = mqtt_client_new();
  if(mqtt_client != NULL) {
    MQTT_connect(mqtt_client);
  }
#endif

  MQTT_connect(&mqtt_client);


#if 1
    const char * pub_payload = "abcd";
    err_t err;
    u8_t qos = 2; /* 0 1 or 2, see MQTT specification */
    u8_t retain = 0; /* No don't retain such crappy payload... */

    err = mqtt_publish(&mqtt_client, "hello/world", pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, 0);
    
    if (err != ERR_OK) {
        writef("Publish err: %d\r\n", err);
    }
#endif


  while(1) {
    
    osDelay(2000);

  }

}



