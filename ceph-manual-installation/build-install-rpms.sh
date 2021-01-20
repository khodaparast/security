#!/bin/bash -e

./make-dist

CEPH_TAR=ceph*.tar.bz2
cp $CEPH_TAR ~/rpmbuild/SOURCES/
tar --strip-components=1 -C ~/rpmbuild/SPECS/ --no-anchored -xvjf ~/rpmbuild/SOURCES/${CEPH_TAR} "ceph.spec"



rpmbuild -ba ~/rpmbuild/SPECS/ceph.spec
cd ~/rpmbuild/RPMS/

yum --nogpgcheck localinstall *.rpm

