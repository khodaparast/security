#!/bin/bash -e

# Primitive setup

fsid=$(uuidgen) 
host=$(hostname) 
ip=$(hostname -I | awk '{print $1}')


vim /etc/ceph/ceph.conf 

sudo ceph-authtool --create-keyring /tmp/ceph.mon.keyring --gen-key -n mon. --cap mon 'allow *' 
 
sudo ceph-authtool --create-keyring /etc/ceph/ceph.client.admin.keyring --gen-key -n client.admin --cap mon 'allow *' --cap osd 'allow *' --cap mds 'allow *' --cap mgr 'allow *' 
 
sudo ceph-authtool --create-keyring /var/lib/ceph/bootstrap-osd/ceph.keyring --gen-key -n client.bootstrap-osd --cap mon 'profile bootstrap-osd' --cap mgr 'allow r' 
 
sudo ceph-authtool /tmp/ceph.mon.keyring --import-keyring /etc/ceph/ceph.client.admin.keyring 
 
sudo ceph-authtool /tmp/ceph.mon.keyring --import-keyring /var/lib/ceph/bootstrap-osd/ceph.keyring 
 
sudo chown ceph:ceph /tmp/ceph.mon.keyring 
 
 
 monmaptool --create --add $host $ip --fsid $fsid /tmp/monmap 
 
 sudo -u ceph mkdir /var/lib/ceph/mon/ceph-$host
 sudo -u ceph ceph-mon --mkfs -i $host --monmap /tmp/monmap --keyring /tmp/ceph.mon.keyring
 sudo systemctl start ceph-mon@$host
 
  
 ceph -s
  
  
# Add OSDs   

hards=( sdc sdd sde sdf sdg sdh sdi sdg sdk sdl sdm sdn sdo sdp sdq )
for i in "${hards[@]}"
do
  ceph-volume lvm create --data /dev/$i 
done
  
 
 systemctl start ceph-osd\*.service ceph-osd.target 
 
 
 # Add MDS 
 
 name=mds01
 mkdir -p /var/lib/ceph/mds/ceph-$name 

ceph-authtool --create-keyring /var/lib/ceph/mds/ceph-$name/keyring --gen-key -n mds.$name 

ceph auth add mds.$name osd "allow rwx" mds "allow *" mon "allow profile mds" -i /var/lib/ceph/mds/ceph-$name/keyring 
 

 # Add mgr
 
 ceph auth get-or-create mgr.$name mon 'allow profile mgr' osd 'allow *' mds 'allow *' 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
  
