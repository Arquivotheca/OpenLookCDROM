/*
 * @(#)plat_osf1.c	1.3	02 Jun 1995
 *
 * OSF  drive control routines.
 */
static char *ident = "@(#)plat_osf1.c	1.3 02 Jun 1995";

#ifdef OSF

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ustat.h>
#include <string.h>
/* #include <sys/rzdisk.h>
#include <sys/cdrom.h>  */
#include <io/cam/rzdisk.h>
#include <io/cam/cdrom.h>

#include "struct.h"

/*
 *   This structure will be filled with the TOC header and all entries.
 * Ultrix doesn't seem to allow getting single TOC entries.
 *                              - Chris Ross (cross@eng.umd.edu)
 */
struct cd_toc_header_and_entries {
	struct cd_toc_header	cdth;
	struct cd_toc_entry		cdte[CDROM_MAX_TRACK+1];
};

void *malloc();
char *strchr();

int	min_volume = 128;
int	max_volume = 255;

extern char	*cd_device;

/*
 * find_cdrom
 *
 * Determine the name of the CD-ROM device.
 *
 * Read through the boot records (via a call to uerf) and find the SCSI
 * address of the CD-ROM.
 */
void
find_cdrom()
{
	char	*data, *fgetline();
	FILE	*uerf;
	int	fds[2];
	int	pid;
	extern char *getenv();

	pipe(fds);

	cd_device = getenv("CDROM");
	if (cd_device != NULL)
	    return;
	if ((pid = fork()) == 0) {
		close(fds[0]);
		dup2(fds[1], 1);
		execl("/etc/uerf", "uerf", "-R", "-r", "300", NULL);
		execl("/usr/sbin/uerf", "uerf", "-R", "-r", "300", NULL);
		_exit(1);
	} else if (pid < 0) {
		perror("fork");
		exit(1);
	}

	close(fds[1]);
	uerf = fdopen(fds[0], "r");

	while (data = fgetline(uerf))
		if (strstr(data, "RRD42")) {
			char	*device;

			cd_device = (char *)malloc(sizeof("/dev/rrz##c"));
			strcpy(cd_device, "/dev/r");
			device = strstr(data, "rz");
			device[(int)(strchr(device, ' ') - device)] = '\0';
			strcat(cd_device, device);
			strcat(cd_device, "c");
			break;
		}

	fclose(uerf);

	if (cd_device == NULL) {
		fprintf(stderr,
			"No cdrom (RRD42) is installed on this system\n");
		exit(1);
	}

	kill(pid, 15);
	(void)wait((int *)NULL);
}

/*
 * Initialize the drive.  A no-op for the generic driver.
 */
int
gen_init(d)
	struct wm_drive	*d;
{
	return (0);
}

/*
 * Get the number of tracks on the CD.
 */
int
gen_get_trackcount(d, tracks)
	struct wm_drive	*d;
	int		*tracks;
{
	struct cd_toc_header	hdr;

	if (ioctl(d->fd, CDROM_TOC_HEADER, &hdr))
		return (-1);

	*tracks = hdr.th_ending_track;

	return (0);
}

/*
 * Get the start time and mode (data or audio) of a track.
 *
 * XXX - this should get cached, but that means keeping track of ejects.
 */
int
gen_get_trackinfo(d, track, data, startframe)
	struct wm_drive	*d;
	int		track, *data, *startframe;
{
	struct cd_toc				toc;
	struct cd_toc_header			hdr;
	struct cd_toc_header_and_entries	toc_buffer;

	if (ioctl(d->fd, CDROM_TOC_HEADER, &hdr))
		return (-1);

	bzero((char *)&toc_buffer, sizeof(toc_buffer));
	toc.toc_address_format = CDROM_MSF_FORMAT;
	toc.toc_starting_track = 0;
	toc.toc_alloc_length = (u_short)(((hdr.th_data_len1 << 8) +
					hdr.th_data_len0) & 0xfff) + 2;
	toc.toc_buffer = (caddr_t)&toc_buffer;

	if (ioctl(d->fd, CDROM_TOC_ENTRYS, &toc))
		return (-1);

	if (track == 0)
		track = hdr.th_ending_track + 1;

	*data = (toc_buffer.cdte[track-1].te_control & CDROM_DATA_TRACK) ? 1:0;
	*startframe = toc_buffer.cdte[track - 1].te_absaddr.msf.m_units*60*75 +
		toc_buffer.cdte[track - 1].te_absaddr.msf.s_units * 75 +
		toc_buffer.cdte[track - 1].te_absaddr.msf.f_units;

	return (0);
}

/*
 * Get the number of frames on the CD.
 */
int
gen_get_cdlen(d, frames)
	struct wm_drive	*d;
	int		*frames;
{
	int		tmp;

	return (gen_get_trackinfo(d, 0, &tmp, frames));
}

/*
 * Get the current status of the drive: the current play mode, the absolute
 * position from start of disc (in frames), and the current track and index
 * numbers if the CD is playing or paused.
 */
int
gen_get_drive_status(d, oldmode, mode, pos, track, index)
	struct wm_drive	*d;
	enum cd_modes	oldmode, *mode;
	int		*pos, *track, *index;
{
	struct cd_sub_channel		sc;
	struct cd_subc_channel_data	scd;

	/* If we can't get status, the CD is ejected, so default to that. */
	*mode = EJECTED;

	sc.sch_address_format	= CDROM_MSF_FORMAT;
	sc.sch_data_format	= CDROM_CURRENT_POSITION;
	sc.sch_track_number	= 0;
	sc.sch_alloc_length	= sizeof(scd);
	sc.sch_buffer		= (caddr_t)&scd;

	/* Is the device open? */
	if (d->fd < 0)
	{
		switch (wmcd_open(d)) {
		case -1:	/* error */
			return (-1);

		case 1:		/* retry */
			return (0);
		}
	}

	if (ioctl(d->fd, CDROM_READ_SUBCHANNEL, &sc))
		return (0);	/* ejected */

	switch (scd.scd_header.sh_audio_status) {
	case AS_PLAY_IN_PROGRESS:
		*mode = PLAYING;
dopos:
		*pos = scd.scd_position_data.scp_absaddr.msf.m_units * 60 * 75 +
			scd.scd_position_data.scp_absaddr.msf.s_units * 75 +
			scd.scd_position_data.scp_absaddr.msf.f_units;
		*track = scd.scd_position_data.scp_track_number;
		*index = scd.scd_position_data.scp_index_number;
		break;

	case AS_PLAY_PAUSED:
		if (oldmode == PLAYING || oldmode == PAUSED)
		{
			*mode = PAUSED;
			goto dopos;
		}
		else
			*mode = STOPPED;
		break;

	case AS_PLAY_COMPLETED:
		*mode = TRACK_DONE; /* waiting for next track. */
		break;

	case AS_NO_STATUS:
		*mode = STOPPED;
		break;
	default:
	    abort();
	}

	return (0);
}

/*
 * scale_volume(vol, max)
 *
 * Return a volume value suitable for passing to the CD-ROM drive.  "vol"
 * is a volume slider setting; "max" is the slider's maximum value.
 *
 * On Sun and DEC CD-ROM drives, the amount of sound coming out the jack
 * increases much faster toward the top end of the volume scale than it
 * does at the bottom.  To make up for this, we make the volume scale look
 * sort of logarithmic (actually an upside-down inverse square curve) so
 * that the volume value passed to the drive changes less and less as you
 * approach the maximum slider setting.  The actual formula looks like
 *
 *     (max^2 - (max - vol)^2) * (max_volume - min_volume)
 * v = --------------------------------------------------- + min_volume
 *                           max^2
 *
 * If your system's volume settings aren't broken in this way, something
 * like the following should work:
 *
 *	return ((vol * (max_volume - min_volume)) / max + min_volume);
 */
scale_volume(vol, max)
	int	vol, max;
{
	return ((max * max - (max - vol) * (max - vol)) *
		(max_volume - min_volume) / (max * max) + min_volume);
}

/*
 * Set the volume level for the left and right channels.  Their values
 * range from 0 to 100.
 */
int
gen_set_volume(d, left, right)
	struct wm_drive	*d;
	int		left, right;
{
	struct cd_playback		pb;
	struct cd_playback_status	ps;
	struct cd_playback_control	pc;

	left = scale_volume(left, 100);
	right = scale_volume(right, 100);

	bzero((char *)&pb, sizeof(pb));
	bzero((char *)&ps, sizeof(ps));
	bzero((char *)&pc, sizeof(pc));

	pb.pb_alloc_length = sizeof(ps);
	pb.pb_buffer = (caddr_t)&ps;

	if (ioctl(d->fd, CDROM_PLAYBACK_STATUS, &pb))
		return (-1);

	pc.pc_chan0_select = ps.ps_chan0_select;
	pc.pc_chan0_volume = (left < CDROM_MIN_VOLUME) ?
		CDROM_MIN_VOLUME : (left > CDROM_MAX_VOLUME) ?
		CDROM_MAX_VOLUME : left;
	pc.pc_chan1_select = ps.ps_chan1_select;
	pc.pc_chan1_volume = (right < CDROM_MIN_VOLUME) ?
		CDROM_MIN_VOLUME : (right > CDROM_MAX_VOLUME) ?
		CDROM_MAX_VOLUME : right;

	pb.pb_alloc_length = sizeof(pc);
	pb.pb_buffer = (caddr_t)&pc;

	if (ioctl(d->fd, CDROM_PLAYBACK_CONTROL, &pb))
		return (-1);

	return (0);
}

/*
 * Pause the CD.
 */
int
gen_pause(d)
	struct wm_drive	*d;
{
	return (ioctl(d->fd, CDROM_PAUSE_PLAY, 0));
}

/*
 * Resume playing the CD (assuming it was paused.)
 */
int
gen_resume(d)
	struct wm_drive	*d;
{
	return (ioctl(d->fd, CDROM_RESUME_PLAY, 0));
}

/*
 * Stop the CD.
 */
int
gen_stop(d)
	struct wm_drive *d;
{
	return (ioctl(d->fd, SCSI_STOP_UNIT, 0));
}

/*
 * Play the CD from one position to another (both in frames.)
 */
int
gen_play(d, start, end)
	struct wm_drive	*d;
	int		start, end;
{
	struct cd_play_audio_msf	msf;

	msf.msf_starting_M_unit	= start / (60*75);
	msf.msf_starting_S_unit	= (start % (60*75)) / 75;
	msf.msf_starting_F_unit	= start % 75;
	msf.msf_ending_M_unit	= end / (60*75);
	msf.msf_ending_S_unit	= (end % (60*75)) / 75;
	msf.msf_ending_F_unit	= end % 75;

	if (ioctl(d->fd, SCSI_START_UNIT))
		return (-1);
	if (ioctl(d->fd, CDROM_PLAY_AUDIO_MSF, &msf))
		return (-2);

	return (0);
}

/*
 * Eject the current CD, if there is one.
 */
int
gen_eject(d)
	struct wm_drive	*d;
{
	/* On some systems, we can check to see if the CD is mounted. */
	struct stat	stbuf;
	struct ustat	ust;

	if (fstat(d->fd, &stbuf) != 0)
		return (-2);

	/* Is this a mounted filesystem? */
	if (ustat(stbuf.st_rdev, &ust) == 0)
		return (-3);

	return (ioctl(d->fd, CDROM_EJECT_CADDY, 0));
}

/*
 * unscale_volume(cd_vol, max)
 *
 * Given a value between min_volume and max_volume, return the volume slider
 * value needed to achieve that value.
 *
 * Rather than perform floating-point calculations to reverse the above
 * formula, we simply do a binary search of scale_volume()'s return values.
 */
static int
unscale_volume(cd_vol, max)
	int	cd_vol, max;
{
	int	vol = 0, top = max, bot = 0, scaled;

	while (bot <= top)
	{
		vol = (top + bot) / 2;
		scaled = scale_volume(vol, max);
		if (cd_vol == scaled)
			break;
		if (cd_vol < scaled)
			top = vol - 1;
		else
			bot = vol + 1;
	}
	
	if (vol < 0)
		vol = 0;
	else if (vol > max)
		vol = max;

	return (vol);
}

/*
 * Read the initial volume from the drive, if available.  Each channel
 * ranges from 0 to 100, with -1 indicating data not available.
 */
int
gen_get_volume(d, left, right)
	struct wm_drive	*d;
	int		*left, *right;
{
	struct cd_playback		pb;
	struct cd_playback_status	ps;

	bzero((char *)&pb, sizeof(pb));
	bzero((char *)&ps, sizeof(ps));

	pb.pb_alloc_length = sizeof(ps);
	pb.pb_buffer = (caddr_t)&ps;

	if (d->fd >= 0)
	{
		if (ioctl(d->fd, CDROM_PLAYBACK_STATUS, &pb))
			*left = *right = -1;
		else
		{
			*left = unscale_volume(ps.ps_chan0_volume, 100);
			*right = unscale_volume(ps.ps_chan1_volume, 100);
		}
	}
	else
		*left = *right = -1;

	return (0);
}

/*
 * Send an arbitrary SCSI command to a device.
 */
int
wm_scsi(d, cdb, cdblen, retbuf, retbuflen, getreply)
	struct wm_drive	*d;
	unsigned char	*cdb;
	int		cdblen;
	void		*retbuf;
	int		retbuflen;
	int		getreply;
{
	/* ULTRIX doesn't have a SCSI passthrough interface, does it? */
	return (-1);
}

/*
 * fgetline()
 *
 *   Simulate fgets, but joining continued lines in the output of uerf.
 */

#define BUF_SIZE        85              /* Max length of a (real) line */

char *
fgetline(fp)
FILE    *fp;
{
        static char     *retval = NULL;
        static char     holdbuf[BUF_SIZE + 1];
        char            tmp[BUF_SIZE + 1];
        char            *stmp;

        if (!retval) {
                retval = malloc(BUF_SIZE * 3);  /* 3 lines can be joined */
                if (!retval)
                        return(NULL);
                else
                        *retval = '\0';
        }

        if (*holdbuf) {
                strcpy(retval, holdbuf);
                retval[strlen(retval)-1] = '\0';
                memset(holdbuf, 0, BUF_SIZE+1);
        }
        while (fgets(tmp, BUF_SIZE, fp)) {
                stmp = tmp + strspn(tmp, " \t");
                if (*stmp == '_') {                     /* Continuation line */
			retval[strlen(retval)-1] = '\0';   /* Trim off C/R */
                        strcat(retval, stmp+1);
                } else {
                        if (*retval) {
                                strcpy(holdbuf, tmp);
                                holdbuf[strlen(holdbuf)-1] = -1;
                                return retval;
                        } else {             /* First line read, keep reading */
                                strcat(retval, stmp);
                                retval[strlen(retval)-1] = '\0';
                        }
                }
        }
        return NULL;
}

/*
 * Open the CD device and figure out what kind of drive is attached.
 */
int
wmcd_open(d)
	struct wm_drive	*d;
{
	int		fd;
	static int	warned = 0;

	if (d->fd >= 0)		/* Device already open? */
		return (0);

	if (cd_device == NULL)
		find_cdrom();

	d->fd = open(cd_device, O_RDWR);
	if (d->fd < 0)
	{
		if (errno == EACCES)
		{
			if (!warned)
			{
				fprintf(stderr,
		"As root, please run\n\nchmod 666 %s\n\n%s\n", cd_device,
		"to give yourself permission to access the CD-ROM device.");
				warned++;
			}
		}
		else if (errno != EINTR)
		{
			perror(cd_device);
			exit(1);
		}

		/* No CD in drive. */
		return (1);
	}

	if (warned)
	{
		warned = 0;
		fprintf(stderr, "Thank you.\n");
	}

	/* Now fill in the relevant parts of the wm_drive structure. */
	fd = d->fd;
	*d = *(find_drive_struct("", "", ""));
	d->fd = fd;

	(d->init)(d);

	return (0);
}

void
keep_cd_open() { }

#endif
