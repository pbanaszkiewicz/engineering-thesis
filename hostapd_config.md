# WLAN Access Point on Raspberry Pi

Install `hostapd` software: `sudo aptitude install hostapd`.

Configure `hostapd` (`/etc/hostapd/hostapd.conf` may not exists, create if needed):

```
interface=wlan0
driver=nl80211
ssid=My_AP
hw_mode=g
channel=6
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=My_Passphrase
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
```

Configure `hostapd` to load that config on start (`/etc/default/hostapd`), add line:

```
DAEMON_CONF="/etc/hostapd/hostapd.conf"
```

Then, to run `hostapd`: `sudo service hostapd start`.

To get `hostapd` running upon boot: `sudo update-rc.d hostapd enable`.
