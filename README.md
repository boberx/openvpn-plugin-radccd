# openvpn-plugin-radccd
# Description
TEST  
An OpenVPN plugin  
(tested with Debian 9 on x86)  
# How to build
* Setting up a chroot

```sh
apt-get install coreutils bash debootstrap
```

```sh
ARCH="i386"; SUITE="stretch"; CHROOTDIR="/storage/"${SUITE}"-chroot_"${ARCH}""; LC="ru_RU.UTF-8";
mkdir -p "${CHROOTDIR}" && \
debootstrap --arch="${ARCH}" --variant=minbase --include=locales,apt-utils,dialog,findutils,file,sed,gawk,bzip2 \
"${SUITE}" "${CHROOTDIR}" http://mirror.mephi.ru/debian/ && \
echo "LANG="${LC}"" > "${CHROOTDIR}"/etc/default/locale && \
sed -i 's/# '"${LC}"' UTF-8/'"${LC}"' UTF-8/' "${CHROOTDIR}"/etc/locale.gen && \
chmod 777 "${CHROOTDIR}"/home && \
chroot "${CHROOTDIR}" /bin/bash -c "su - -c \"locale-gen\"";
```

* compile

```sh
make
```

# How to use
Set the plugin in the OpenVPN configfile..

> /etc/openvpn/openvpn.conf
  
```
plugin /usr/lib/openvpn/openvpn-plugin-radccd.so
```
