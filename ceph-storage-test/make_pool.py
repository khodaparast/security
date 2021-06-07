import rados, sys
  
cluster = rados.Rados(conffile='/etc/ceph/ceph.conf')
print("\nlibrados version: " + str(cluster.version()))
print("Will attempt to connect to: " + str(cluster.conf_get('mon_host')))

cluster.connect()
print("\nCluster ID: " + cluster.get_fsid())



#_______________________________________

try:
    cluster.create_pool('test')
    print("\nPool test is succesfully created!")
except:
    print("\nPool named 'test' exists: " + str(cluster.pool_exists('test')))


