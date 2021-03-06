# OLCD Makefile
# $Id: Makefile,v 1.19 2004/09/24 01:28:21 ian Exp $

# How we made your version of "The OPEN LOOK, XView and NeWS Archive CD-ROM".

SHELL=			/bin/ksh

VERSION=		OpenLook-XView-1.2a		# CHANGE THIS EVERY TIME!!
# Used as the Primary Volume Identifier. AVOID volume names with 
# imbedded slashes as some UNIX versions will be unable to auto-mount them!

# Where to put the generated image
DISK_IMAGE=		$$HOME/olcd_image.iso

# The cdrecord description of the CD burner
CD_BURNER=		/dev/rcd1c:1,5,0
CD_MOUNT=		/dev/cd1c
SPEED=			2

LOGFILE=		$$HOME/proj/olcd/log.$(VERSION)

all:			master check

cleanlog:
	rm -f $(LOGFILE)

master:
	@if [ -f $(LOGFILE) ]; then \
		echo "$$0: $(LOGFILE) exists: remove it or change version number"; \
		exit 1; \
	fi

	echo "Start premastering at `date`"

	mkisofs \
		-P 'Darwin Open Systems, ADDRESS REDACTED' \
		-A "The OPEN LOOK and XView CD-ROM" \
		-p 'Ian F. Darwin, EMAIL REDACTED' 	\
		-r 			\
		-V $(VERSION)		\
		-o $(DISK_IMAGE) \
		.  | \
	tee $(LOGFILE)

# Now tell UNIX that we have a CD-ROM image ready, ls it.
check:
	# volcheck $(DISK_IMAGE)		# Doesn't work on Solaris 2.4
	# Mount using vnd...
	vnconfig -c -v svnd0 $(DISK_IMAGE)
	mount -t cd9660 /dev/svnd0c /mnt
	ls -l /mnt
	umount /dev/svnd0c
	vnconfig -u -v svnd0

burn:
	cdrecord -v speed=$(SPEED) dev=${CD_BURNER} -isosize ${DISK_IMAGE}
	mount -t cd9660 $(CD_MOUNT) /mnt
	ls . /mnt
	umount $(CD_MOUNT)
	eject  $(CD_MOUNT)
