/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* NetworkManager -- Network link manager
 *
 * Dan Williams <dcbw@redhat.com>
 * Hicham HAOUARI <hicham.haouari@gmail.com>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Copyright (C) 2005 - 2010 Red Hat, Inc.
 */

#include <glib.h>
#include <glib/gi18n.h>
#include <netinet/in.h>
#include <string.h>
#include <net/ethernet.h>
#include <stdlib.h>
#include <linux/types.h>
#include <linux/sockios.h>
#include <linux/version.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/if.h>
#include <errno.h>

#define G_UDEV_API_IS_SUBJECT_TO_CHANGE
#include <gudev/gudev.h>

#include <netlink/route/addr.h>

#include "nm-glib-compat.h"
#include "nm-device-adsl.h"
#include "nm-device-interface.h"
#include "nm-device-private.h"
#include "nm-activation-request.h"
#include "NetworkManagerUtils.h"
#include "nm-supplicant-manager.h"
#include "nm-supplicant-interface.h"
#include "nm-supplicant-config.h"
#include "nm-netlink-monitor.h"
#include "nm-system.h"
#include "nm-setting-connection.h"
#include "nm-setting-adsl.h"
#include "nm-setting-pppoe.h"
#include "ppp-manager/nm-ppp-manager.h"
#include "nm-logging.h"
#include "nm-properties-changed-signal.h"
#include "nm-dhcp-manager.h"

G_DEFINE_TYPE (NMDeviceAdsl, nm_device_adsl, NM_TYPE_DEVICE)

#define NM_DEVICE_ADSL_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), NM_TYPE_DEVICE_ADSL, NMDeviceAdslPrivate))

typedef enum
{
	NM_ADSL_ERROR_CONNECTION_INVALID,
	NM_ADSL_ERROR_CONNECTION_INCOMPATIBLE,
} NMAdslError;

#define NM_ADSL_ERROR (nm_adsl_error_quark ())
#define NM_TYPE_ADSL_ERROR (nm_adsl_error_get_type ()) 
