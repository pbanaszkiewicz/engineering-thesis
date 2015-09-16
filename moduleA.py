import time

import RPi.GPIO as GPIO

# globals, we love them, don't we
RST_SEND = False


def reset_btn(channel):
    print "[PRESSED] reset"
    RST_SEND = True


if __name__ == '__main__':
    GPIO.setmode(GPIO.BCM)  # SoC numbering of pins

    GPIO.setup(17, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    # GPIO.setup(27, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  # 21 in rev1

    GPIO.add_event_detect(17, GPIO.RISING, callback=reset_btn,
                          bouncetime=300)

    try:
        while True:
            time.sleep(1)
            # send PING

            if RST_SEND:
                # send RESET
                RST_SEND = False

    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()
