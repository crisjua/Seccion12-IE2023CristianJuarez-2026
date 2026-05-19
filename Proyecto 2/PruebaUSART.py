# ================= LIBRERIAS =================
import sys
import time
import serial
from Adafruit_IO import MQTTClient

# ================= CONFIG =================
USERNAME = "jua22478"
KEY = "aio_IsZj643eugKDMsJiUmaJSeUtZcHW"

# ================= FEEDS =================
FEED_MODO     = "modo"
FEED_S1       = "servo1"
FEED_S2       = "servo2"
FEED_S3       = "servo3"
FEED_VEL      = "velocidad"
FEED_SAVE     = "guardar"
FEED_POS      = "posicion"

# ================= SERIAL =================
# CAMBIA COM3 SI TU ARDUINO ESTA EN OTRO PUERTO
ser = serial.Serial('COM3', 9600, timeout=1)

time.sleep(2)

modo = 1

# ================= FUNCIONES =================

def connected(client):

    print("Conectado a Adafruit IO")

    client.subscribe(FEED_MODO)
    client.subscribe(FEED_S1)
    client.subscribe(FEED_S2)
    client.subscribe(FEED_S3)
    client.subscribe(FEED_VEL)
    client.subscribe(FEED_SAVE)
    client.subscribe(FEED_POS)


def disconnected(client):

    print("Desconectado")

    sys.exit(1)


def enviar_comando(comando):

    print(f"Enviado -> {comando}")

    ser.write(f"{comando}\n".encode())

    time.sleep(0.1)


def message(client, feed_id, payload):

    global modo

    print(f"{feed_id} -> {payload}")

    # ==================================================
    # CAMBIO DE MODO
    # ==================================================
    if feed_id == FEED_MODO:

        modo = int(payload)

        print(f"Modo actual: {modo}")

        enviar_comando(str(modo))

        return

    # ==================================================
    # MODO MANUAL / UART
    # ==================================================
    if modo == 1 or modo == 3:

        # ==============================================
        # SERVO1
        # ==============================================
        if feed_id == FEED_S1:

            ang = int(payload)

            pwm = int((ang / 360) * 255)

            enviar_comando(f"A{pwm}")

        # ==============================================
        # SERVO2
        # ==============================================
        elif feed_id == FEED_S2:

            ang = int(payload)

            pwm = int((ang / 360) * 255)

            enviar_comando(f"B{pwm}")

        # ==============================================
        # SERVO3
        # ==============================================
        elif feed_id == FEED_S3:

            ang = int(payload)

            pwm = int((ang / 360) * 255)

            enviar_comando(f"C{pwm}")

        # ==============================================
        # VELOCIDAD MOTOR
        # ==============================================
        elif feed_id == FEED_VEL:

            vel = int(payload)

            enviar_comando(f"D{vel}")

        # ==============================================
        # GUARDAR EEPROM
        # ==============================================
        elif feed_id == FEED_SAVE:

            if payload == "1":

                enviar_comando("S")

        # ==============================================
        # RECUPERAR EEPROM
        # ==============================================
        elif feed_id == FEED_POS:

            if payload == "1":

                enviar_comando("R")


# ================= MQTT =================

client = MQTTClient(USERNAME, KEY)

client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message

client.connect()

client.loop_background()

# ================= LOOP PRINCIPAL =================

while True:

    print("Sistema activo...")

    time.sleep(5)