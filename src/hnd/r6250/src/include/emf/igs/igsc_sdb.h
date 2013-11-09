/*
 * Copyright (C) 2011, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: igsc_sdb.h 241182 2011-02-17 21:50:03Z $
 */

#ifndef _IGSC_SDB_H_
#define _IGSC_SDB_H_

#define IGSDB_MGRP_HASH(m)     ((((m) >> 24) + ((m) >> 16) + \
				 ((m) >> 8) + ((m) & 0xff)) & 7)

/*
 * Group entry of IGSDB
 */
typedef struct igsc_mgrp
{
	clist_head_t   mgrp_hlist;   /* Multicast Groups hash list */
	uint32         mgrp_ip;      /* Multicast Group IP Address */
	clist_head_t   mh_head;      /* List head of group members */
	clist_head_t   mi_head;      /* List head of interfaces */
	igsc_info_t    *igsc_info;   /* IGSC instance data */
} igsc_mgrp_t;

/*
 * Interface entry of IGSDB
 */
typedef struct igsc_mi
{
	clist_head_t   mi_list;      /* Multicast i/f list prev and next */
	void           *mi_ifp;      /* Interface pointer */
	int32          mi_ref;       /* Ref count of hosts on the i/f */
	#if defined(EMF_MCAST_IN_UCAST)
	uint32         mi_mcast_mode;/* EMF_MCAST_NORMAL or EMF_MCAST_TO_UCAST */
	#endif
} igsc_mi_t;

/*
 * Host entry of IGSDB
 */
typedef struct igsc_mh
{
	clist_head_t   mh_list;      /* Group members list prev and next */
	uint32         mh_ip;        /* Unicast IP address of host */
	igsc_mgrp_t    *mh_mgrp;     /* Multicast forwarding entry for the
	                              * group
	                              */
	osl_timer_t    *mgrp_timer;  /* Group Membership Interval timer */
	igsc_mi_t      *mh_mi;       /* Interface connected to host */
#if defined(EMF_MCAST_IN_UCAST)
	uint16         polls;        /* number of times being polled */
	uint8          mh_mac[6];    /* MAC address of the host */
#endif /* EMF_MCAST_IN_UCAST */
} igsc_mh_t;

/*
 * Prototypes
 */
/* modify start by Hank 08/10/2012 */
/*change function definition when __CONFIG_IGMP_SNOOPING__ enable*/
#ifdef __CONFIG_IGMP_SNOOPING__
int32 igsc_sdb_member_add(igsc_info_t *igsc_info, void *ifp, uint32 mgrp_ip,
                          uint32 mh_ip, uint8 *mh_mac,int port_id);
int32 igsc_sdb_member_del(igsc_info_t *igsc_info, void *ifp, uint32 mgrp_ip,
                          uint32 mh_ip,int portID);
#else
int32 igsc_sdb_member_add(igsc_info_t *igsc_info, void *ifp, uint32 mgrp_ip,
                          uint32 mh_ip);
int32 igsc_sdb_member_del(igsc_info_t *igsc_info, void *ifp, uint32 mgrp_ip,
                          uint32 mh_ip);
#endif
/* modify end by Hank 08/10/2012 */
void igsc_sdb_init(igsc_info_t *igsc_info);

#if defined(EMF_MCAST_IN_UCAST)
int32 igsc_sdb_member_get_maclist(igsc_info_t *igsc_info, void *ifp, uint32 mgrp_ip, 
	                     uint32 hmaclist_size, uint8 *hmaclist);
#endif /* EMF_MCAST_IN_UCAST */

#endif /* _IGSC_SDB_H_ */