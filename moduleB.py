import time

import RPi.GPIO as GPIO

RESET_PIN = 22
RXTX_PIN = 21  # 27 in rev2
STATUS_PIN = 17


def reset_btn(channel):
    print "[PRESSED] reset"


if __name__ == '__main__':
    GPIO.setmode(GPIO.BCM)  # SoC numbering of pins

    GPIO.setup(RESET_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

    GPIO.setup(STATUS_PIN, GPIO.OUT)
    GPIO.setup(RXTX_PIN, GPIO.OUT)

    GPIO.add_event_detect(RESET_PIN, GPIO.RISING, callback=reset_btn,
                          bouncetime=300)

    try:
        while True:
            time.sleep(1)

            GPIO.output(RXTX_PIN, True)
            GPIO.output(STATUS_PIN, True)
            time.sleep(0.001)
            GPIO.output(RXTX_PIN, False)
            GPIO.output(STATUS_PIN, False)

    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()
