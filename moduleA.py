import time

import RPi.GPIO as GPIO

# globals, we love them, don't we
STOP_SEND = False


def stop_btn(channel):
    global STOP_SEND
    print "[PRESSED] stop"
    STOP_SEND = True


if __name__ == '__main__':
    GPIO.setmode(GPIO.BCM)  # SoC numbering of pins

    GPIO.setup(17, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    # GPIO.setup(27, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  # 21 in rev1

    GPIO.add_event_detect(17, GPIO.RISING, callback=stop_btn,
                          bouncetime=300)

    try:
        while True:
            time.sleep(1)

            if STOP_SEND:
                # send PING and STOP
                STOP_SEND = False
                print "[SENDING] stop & ping"

            else:
                # send only PING
                print "[SENDING] ping"

    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()
