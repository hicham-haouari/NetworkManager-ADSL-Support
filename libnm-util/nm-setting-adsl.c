/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

/*
 * Dan Williams <dcbw@redhat.com>
 * Hicham HAOUARI <hicham.haouari@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 * (C) Copyright 2007 - 2008 Red Hat, Inc.
 */

#include <string.h>
#include "nm-setting-adsl.h"
#include "nm-utils.h"

/**
 * SECTION:nm-setting-adsl
 * @short_description: Describes ADSL-based properties
 * @include: nm-setting-adsl.h
 *
 * The #NMSettingAdsl object is a #NMSetting subclass that describes
 * properties that allow connections to ADSL networks.
 */

/**
 * nm_setting_adsl_error_quark:
 *
 * Registers an error quark for #NMSettingAdsl if necessary.
 *
 * Returns: the error quark used for #NMSettingAdsl errors.
 **/
GQuark
nm_setting_adsl_error_quark (void)
{
	static GQuark quark;

	if (G_UNLIKELY (!quark))
		quark = g_quark_from_static_string ("nm-setting-adsl-error-quark");
	return quark;
}

/* This should really be standard. */
#define ENUM_ENTRY(NAME, DESC) { NAME, "" #NAME "", DESC }

GType
nm_setting_adsl_error_get_type (void)
{
	static GType etype = 0;

	if (etype == 0) {
		static const GEnumValue values[] = {
			/* Unknown error. */
			ENUM_ENTRY (NM_SETTING_ADSL_ERROR_UNKNOWN, "UnknownError"),
			/* The specified property was invalid. */
			ENUM_ENTRY (NM_SETTING_ADSL_ERROR_INVALID_PROPERTY, "InvalidProperty"),
			/* The specified property was missing and is required. */
			ENUM_ENTRY (NM_SETTING_ADSL_ERROR_MISSING_PROPERTY, "MissingProperty"),
			{ 0, 0, 0 }
		};
		etype = g_enum_register_static ("NMSettingAdslError", values);
	}
	return etype;
}

G_DEFINE_TYPE (NMSettingAdsl, nm_setting_adsl, NM_TYPE_SETTING)

#define NM_SETTING_ADSL_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), NM_TYPE_SETTING_ADSL, NMSettingAdslPrivate))

typedef struct {
	char  *username;
	char  *password;
	char  *protocol;
	char  *encapsulation;
	guint vpi;
	guint vci;
} NMSettingAdslPrivate;

enum {
	PROP_0,
	PROP_USERNAME,
	PROP_PASSWORD,
	PROP_PROTOCOL;
	PROP_ENCAPSULATION;
	PROP_VPI;
	PROP_VCI;

	LAST_PROP
};

/**
 * nm_setting_adsl_new:
 *
 * Creates a new #NMSettingAdsl object with default values.
 *
 * Returns: the new empty #NMSettingAdsl object
 **/
NMSetting *
nm_setting_adsl_new (void)
{
	return (NMSetting *) g_object_new (NM_TYPE_SETTING_ADSL, NULL);
}

/**
 * nm_setting_adsl_get_username:
 * @setting: the #NMSettingAdsl
 *
 * Returns: the #NMSettingAdsl:username property of the setting
 **/
const char *
nm_setting_adsl_get_username (NMSettingAdsl *setting)
{
	g_return_val_if_fail (NM_IS_SETTING_ADSL (setting), NULL);

	return NM_SETTING_ADSL_GET_PRIVATE (setting)->username;
}

/**
 * nm_setting_adsl_get_password:
 * @setting: the #NMSettingAdsl
 *
 * Returns: the #NMSettingAdsl:password property of the setting
 **/
const char *
nm_setting_adsl_get_password (NMSettingAdsl *setting)
{
	g_return_val_if_fail (NM_IS_SETTING_ADSL (setting), NULL);

	return NM_SETTING_ADSL_GET_PRIVATE (setting)->password;
}

/**
 * nm_setting_adsl_get_protocol:
 * @setting: the #NMSettingAdsl
 *
 * Returns: the #NMSettingAdsl:protocol property of the setting
 **/
const char *
nm_setting_adsl_get_protocol (NMSettingAdsl *setting)
{
	g_return_val_if_fail (NM_IS_SETTING_ADSL (setting), NULL);

	return NM_SETTING_ADSL_GET_PRIVATE (setting)->protocol;
}

/**
 * nm_setting_adsl_get_encapsulation:
 * @setting: the #NMSettingAdsl
 *
 * Returns: the #NMSettingAdsl:encapsulation property of the setting
 **/
const char *
nm_setting_adsl_get_encapsulation (NMSettingAdsl *setting)
{
	g_return_val_if_fail (NM_IS_SETTING_ADSL (setting), NULL);

	return NM_SETTING_ADSL_GET_PRIVATE (setting)->encapsulation;
}

/**
 * nm_setting_adsl_get_vpi:
 * @setting: the #NMSettingAdsl
 *
 * Returns: the #NMSettingAdsl:vpi property of the setting
 **/
guint
nm_setting_adsl_get_vpi (NMSettingAdsl *setting)
{
	g_return_val_if_fail (NM_IS_SETTING_ADSL (setting), NULL);

	return NM_SETTING_ADSL_GET_PRIVATE (setting)->vpi;
}

/**
 * nm_setting_adsl_get_vci:
 * @setting: the #NMSettingAdsl
 *
 * Returns: the #NMSettingAdsl:vci property of the setting
 **/
guint
nm_setting_adsl_get_vci (NMSettingAdsl *setting)
{
	g_return_val_if_fail (NM_IS_SETTING_ADSL (setting), NULL);

	return NM_SETTING_ADSL_GET_PRIVATE (setting)->vci;
}

static gint
find_setting_by_name (gconstpointer a, gconstpointer b)
{
	NMSetting *setting = NM_SETTING (a);
	const char *str = (const char *) b;

	return strcmp (nm_setting_get_name (setting), str);
}

static gboolean
verify (NMSetting *setting, GSList *all_settings, GError **error)
{
	NMSettingAdslPrivate *priv = NM_SETTING_ADSL_GET_PRIVATE (setting);

	if (priv->username && !strlen (priv->username)) {
		g_set_error (error,
		             NM_SETTING_ADSL_ERROR,
		             NM_SETTING_ADSL_ERROR_INVALID_PROPERTY,
		             NM_SETTING_ADSL_USERNAME);
		return FALSE;
	}

	if (priv->password && !strlen (priv->password)) {
		g_set_error (error,
		             NM_SETTING_ADSL_ERROR,
		             NM_SETTING_ADSL_ERROR_INVALID_PROPERTY,
		             NM_SETTING_ADSL_PASSWORD);
		return FALSE;
	}

	if (!g_strcasecmp(priv->protocol, NM_SETTING_ADSL_PROTOCOL_PPPOA ) &&
	    !g_strcasecmp(priv->protocol, NM_SETTING_ADSL_PROTOCOL_PPPOE ) &&
	    !g_strcasecmp(priv->protocol, NM_SETTING_ADSL_PROTOCOL_IPOATM) ) {
		g_set_error (error,
		             NM_SETTING_ADSL_ERROR,
		             NM_SETTING_ADSL_ERROR_INVALID_PROPERTY,
		             NM_SETTING_ADSL_PROTOCOL);
		return FALSE;
	}

	if (!g_strcasecmp(priv->encapsulation, NM_SETTING_ADSL_ENCAPSULATION_VCMUX) &&
	    !g_strcasecmp(priv->encapsulation, NM_SETTING_ADSL_ENCAPSULATION_LLC) ) {
		g_set_error (error,
		             NM_SETTING_ADSL_ERROR,
		             NM_SETTING_ADSL_ERROR_INVALID_PROPERTY,
		             NM_SETTING_ADSL_ENCAPSULATION);
		return FALSE;
	}

	if (!priv->vpi) {
		g_set_error (error,
		             NM_SETTING_ADSL_ERROR,
		             NM_SETTING_ADSL_ERROR_MISSING_PROPERTY,
		             NM_SETTING_ADSL_VPI);
		return FALSE;
	}

	if (!priv->vci) {
		g_set_error (error,
		             NM_SETTING_ADSL_ERROR,
		             NM_SETTING_ADSL_ERROR_INVALID_PROPERTY,
		             NM_SETTING_ADSL_VCI);
		return FALSE;
	}

	return TRUE;
}

static GPtrArray *
need_secrets (NMSetting *setting)
{
	NMSettingAdslPrivate *priv = NM_SETTING_ADSL_GET_PRIVATE (setting);
	GPtrArray *secrets = NULL;

	if (priv->password)
		return NULL;

	if (priv->username) {
		secrets = g_ptr_array_sized_new (1);
		g_ptr_array_add (secrets, NM_SETTING_ADSL_PASSWORD);
	}

	return secrets;
}

static void
nm_setting_adsl_init (NMSettingAdsl *setting)
{
	g_object_set (setting, NM_SETTING_NAME, NM_SETTING_ADSL_SETTING_NAME, NULL);
}

static void
finalize (GObject *object)
{
	NMSettingAdslPrivate *priv = NM_SETTING_ADSL_GET_PRIVATE (object);

	g_free (priv->username);
	g_free (priv->password);
	g_free (priv->protocol);
	g_free (priv->encapsulation);

	G_OBJECT_CLASS (nm_setting_adsl_parent_class)->finalize (object);
}

static void
set_property (GObject *object, guint prop_id,
		    const GValue *value, GParamSpec *pspec)
{
	NMSettingAdslPrivate *priv = NM_SETTING_ADSL_GET_PRIVATE (object);

	switch (prop_id) {
	case PROP_USERNAME:
		g_free (priv->username);
		priv->username = g_value_dup_string (value);
		break;
	case PROP_PASSWORD:
		g_free (priv->password);
		priv->password = g_value_dup_string (value);
		break;
	case PROP_PROTOCOL:
		g_free (priv->protocol);
		priv->protocol = g_value_dup_string (value);
		break;
	case PROP_ENCAPSULATION:
		g_free (priv->encapsulation);
		priv->encapsulation = g_value_dup_string (value);
		break;
	case PROP_VPI:
		priv->vpi = g_value_dup_string (value);
		break;
	case PROP_VCI:
		priv->vci = g_value_dup_string (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
get_property (GObject *object, guint prop_id,
		    GValue *value, GParamSpec *pspec)
{
	NMSettingAdsl *setting = NM_SETTING_ADSL (object);

	switch (prop_id) {
	case PROP_USERNAME:
		g_value_set_string (value, nm_setting_adsl_get_username (setting));
		break;
	case PROP_PASSWORD:
		g_value_set_string (value, nm_setting_adsl_get_password (setting));
		break;
	case PROP_PROTOCOL:
		g_value_set_string (value, nm_setting_adsl_get_protocol (setting));
		break;
	case PROP_ENCAPSULATION:
		g_value_set_string (value, nm_setting_adsl_get_encapsulation (setting));
		break;
	case PROP_VPI:
		g_value_set_string (value, nm_setting_adsl_get_vpi (setting));
		break;
	case PROP_VCI:
		g_value_set_string (value, nm_setting_adsl_get_vci (setting));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
nm_setting_adsl_class_init (NMSettingAdslClass *setting_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (setting_class);
	NMSettingClass *parent_class = NM_SETTING_CLASS (setting_class);

	g_type_class_add_private (setting_class, sizeof (NMSettingAdslPrivate));

	/* virtual methods */
	object_class->set_property = set_property;
	object_class->get_property = get_property;
	object_class->finalize     = finalize;
	parent_class->verify       = verify;
	parent_class->need_secrets = need_secrets;

	/* Properties */

	/**
	 * NMSettingAdsl:username:
	 *
	 * The username used to authenticate with the network, if required.  Many
	 * providers do not require a username, or accept any username.  But if a
	 * username is required, it is specified here.
	 **/
	g_object_class_install_property
		(object_class, PROP_USERNAME,
		 g_param_spec_string (NM_SETTING_ADSL_USERNAME,
						  "Username",
						  "Username used to authenticate with the network, if "
						  "required.  Note that many providers do not require "
						  "a username or accept any username.",
						  NULL,
						  G_PARAM_READWRITE | NM_SETTING_PARAM_SERIALIZE));

	/**
	 * NMSettingAdsl:password:
	 *
	 * The password used to authenticate with the network, if required.  Many
	 * providers do not require a password, or accept any password.  But if a
	 * password is required, it is specified here.
	 **/
	g_object_class_install_property
		(object_class, PROP_PASSWORD,
		 g_param_spec_string (NM_SETTING_ADSL_PASSWORD,
						  "Password",
						  "Password used to authenticate with the network, if "
						  "required.  Note that many providers do not require "
						  "a password or accept any password.",
						  NULL,
						  G_PARAM_READWRITE | NM_SETTING_PARAM_SERIALIZE | NM_SETTING_PARAM_SECRET));

	/**
	 * NMSettingAdsl:protocol:
	 *
	 * ADSL connection protocol, can be PPPoA, PPPoE, or IPoATM.
	 **/
	g_object_class_install_property
		(object_class, PROP_PROTOCOL,
		 g_param_spec_string (NM_SETTING_ADSL_PROTOCOL,
						  "Protocol",
						  "ADSL connection protocol.",
						  NULL,
						  G_PARAM_READWRITE | NM_SETTING_PARAM_SERIALIZE));

	/**
	 * NMSettingAdsl:encapsulation:
	 *
	 * ADSL connection encapsulation, can be VCMux or LLC.
	 **/
	g_object_class_install_property
		(object_class, PROP_ENCAPSULATION,
		 g_param_spec_string (NM_SETTING_ADSL_ENCAPSULATION,
						  "Encapsulation",
						  "Encapsulation of ADSL connection",
						  NULL,
						  G_PARAM_READWRITE | NM_SETTING_PARAM_SERIALIZE));

	/**
	 * NMSettingAdsl:vpi:
	 *
	 * ADSL connection vpi.
	 **/
	g_object_class_install_property
		(object_class, PROP_VPI,
		 g_param_spec_string (NM_SETTING_ADSL_VPI,
						  "VPI",
						  "VPI of ADSL connection",
						  NULL,
						  G_PARAM_READWRITE | NM_SETTING_PARAM_SERIALIZE));

	/**
	 * NMSettingAdsl:vci:
	 *
	 * ADSL connection vci.
	 **/
	g_object_class_install_property
		(object_class, PROP_VCI,
		 g_param_spec_string (NM_SETTING_ADSL_VCI,
						  "VCI",
						  "VCI of ADSL connection",
						  NULL,
						  G_PARAM_READWRITE | NM_SETTING_PARAM_SERIALIZE));
}
