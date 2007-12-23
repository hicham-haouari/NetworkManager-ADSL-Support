/* -*- Mode: C; tab-width: 5; indent-tabs-mode: t; c-basic-offset: 5 -*- */

#include "nm-gsm-device.h"

G_DEFINE_TYPE (NMGsmDevice, nm_gsm_device, NM_TYPE_DEVICE)

#define NM_GSM_DEVICE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), NM_TYPE_GSM_DEVICE, NMGsmDevicePrivate))

typedef struct {
	DBusGProxy *gsm_proxy;

	gboolean disposed;
} NMGsmDevicePrivate;

static void
nm_gsm_device_init (NMGsmDevice *device)
{
}

static GObject*
constructor (GType type,
		   guint n_construct_params,
		   GObjectConstructParam *construct_params)
{
	GObject *object;
	NMGsmDevicePrivate *priv;

	object = G_OBJECT_CLASS (nm_gsm_device_parent_class)->constructor (type,
														  n_construct_params,
														  construct_params);
	if (!object)
		return NULL;

	priv = NM_GSM_DEVICE_GET_PRIVATE (object);

	priv->gsm_proxy = dbus_g_proxy_new_for_name (nm_object_get_connection (NM_OBJECT (object)),
										NM_DBUS_SERVICE,
										nm_object_get_path (NM_OBJECT (object)),
										NM_DBUS_INTERFACE_GSM_DEVICE);
	return object;
}

static void
dispose (GObject *object)
{
	NMGsmDevicePrivate *priv = NM_GSM_DEVICE_GET_PRIVATE (object);

	if (priv->disposed) {
		G_OBJECT_CLASS (nm_gsm_device_parent_class)->dispose (object);
		return;
	}

	priv->disposed = TRUE;

	g_object_unref (priv->gsm_proxy);

	G_OBJECT_CLASS (nm_gsm_device_parent_class)->dispose (object);
}

static void
nm_gsm_device_class_init (NMGsmDeviceClass *device_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (device_class);

	g_type_class_add_private (device_class, sizeof (NMGsmDevicePrivate));

	/* virtual methods */
	object_class->constructor = constructor;
	object_class->dispose = dispose;
}

NMGsmDevice *
nm_gsm_device_new (DBusGConnection *connection, const char *path)
{
	g_return_val_if_fail (connection != NULL, NULL);
	g_return_val_if_fail (path != NULL, NULL);

	return (NMGsmDevice *) g_object_new (NM_TYPE_GSM_DEVICE,
								  NM_OBJECT_CONNECTION, connection,
								  NM_OBJECT_PATH, path,
								  NULL);
}