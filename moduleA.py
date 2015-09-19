import time

import RPi.GPIO as GPIO

# globals, we love them, don't we
STOP_SEND = False

STOP_PIN = 17
RXTX_PIN = 27  # 21 in rev1


def stop_btn(channel):
    global STOP_SEND
    print "[PRESSED] stop"
    STOP_SEND = True


if __name__ == '__main__':
    GPIO.setmode(GPIO.BCM)  # SoC numbering of pins

    GPIO.setup(STOP_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    GPIO.setup(RXTX_PIN, GPIO.OUT)

    GPIO.add_event_detect(STOP_PIN, GPIO.RISING, callback=stop_btn,
                          bouncetime=300)

    try:
        while True:
            time.sleep(1)

            GPIO.output(RXTX_PIN, True)
            if STOP_SEND:
                # send PING and STOP
                STOP_SEND = False
                print "[SENDING] stop & ping"

            else:
                # send only PING
                print "[SENDING] ping"

            GPIO.output(RXTX_PIN, False)

    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()
