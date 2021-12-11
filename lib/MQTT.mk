
LIB_MQTT = ./lib/mqtt

# FatFS basis files
CSRC += $(LIB_MQTT)/MQTTClient.c \
		$(LIB_MQTT)/MQTTConnectClient.c \
		$(LIB_MQTT)/MQTTConnectServer.c \
		$(LIB_MQTT)/MQTTDeserializePublish.c \
		$(LIB_MQTT)/MQTTSerializePublish.c \
		$(LIB_MQTT)/MQTTFormat.c \
		$(LIB_MQTT)/MQTTPacket.c \
		$(LIB_MQTT)/MQTTSubscribeClient.c \
		$(LIB_MQTT)/MQTTSubscribeServer.c \
		$(LIB_MQTT)/MQTTUnsubscribeClient.c \
		$(LIB_MQTT)/MQTTUnsubscribeServer.c \
		$(LIB_MQTT)/MQTTlwip.c \
		#$(LIB_MQTT)/mqtt.c \


# FatFS Includes
INCDIR += $(LIB_MQTT)/ \




