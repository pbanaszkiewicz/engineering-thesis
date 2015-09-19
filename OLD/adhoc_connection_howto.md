# How-To: WiFi Ad-Hoc connection on RPi

An ad-hoc connection is a peer-to-peer connection, so the configuration should
be the same on all systems.

## No security

Edit `/etc/network/interfaces`:

```
auto wlan0
allow-hotplug wlan0
iface wlan0 inet static
    address STATICIP
    netmask 255.255.255.0
    wireless-channel 1
    wireless-essid MYSSID
    wireless-mode ad-hoc
```

1. You can change your IP address, but I highly suggest using static addresses
   for this kind of connection.  Otherwise you'd have to set up some sort of
   DHCP service like `dhcpcd`.
2. Change SSID.

## WPA

In `/etc/network/interfaces`:

```
auto wlan0
allow-hotplug wlan0
iface wlan0 inet manual
wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
```

Generate an encrypted passphrase:

```
$ wpa_passphrase MYSSID passphrase
```

Edit `/etc/wpa_supplicant/wpa_supplicant.conf/`:

```
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
ap_scan=2
update_config=1

network={
        ssid="MYSSID"
        mode=1
        frequency=2432
        proto=WPA
        key_mgmt=WPA-NONE
        pairwise=NONE
        group=TKIP
        #psk="passphrase"
        psk=encryptedpassphrase
}
```

1. Change your SSID
2. Change your encrypted passphrase to the one that `wpa_passphrase` generated
