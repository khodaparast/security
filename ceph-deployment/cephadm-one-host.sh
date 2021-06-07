
# 1. Check no other keyring exist from previous deploymen: find / -name "*ceph*keyring*"  and remove it. 

 

# 2.  Stop Ceph service if excist: systemctl stop ceph-\*.service 


# 3. dnf update podman 

# 4.  dnf update docker 
   
# 5. Deploy cluster with cephadm   
    ip=$(ip route get 1 | head -1 | awk '{print $7}') 


    cephadm bootstrap --mon-ip $ip --mgr-id $(hostname -s) --mon-id $(hostname -s) --allow-fqdn-hostname |& tee ~/cephadm.log 

# 6. Change crush rule from "host" to "osd" 

# 7. Add OSDs

    ceph orch apply osd --all-available-devices 
    
    
# 8. Start services

    systemctl start ceph-\*.service ceph-*.target 

   

     
     
