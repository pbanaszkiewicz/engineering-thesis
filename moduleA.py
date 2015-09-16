import RPi.GPIO as GPIO


def reset_btn(channel):
    print "Reset Button PRESSED"


if __name__ == '__main__':
    GPIO.setmode(GPIO.BCM)  # SoC numbering of pins

    GPIO.setup(17, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    # GPIO.setup(27, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  # 21 in rev1

    GPIO.add_event_detect(17, GPIO.RISING, callback=reset_btn,
                          bouncetime=300)

    try:
        while True:
            pass
    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()
