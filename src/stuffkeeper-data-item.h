/* Generated by GOB (v2.0.14)   (do not edit directly) */

#include <glib.h>
#include <glib-object.h>
#ifndef __STUFFKEEPER_DATA_ITEM_H__
#define __STUFFKEEPER_DATA_ITEM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



    #include <stdio.h>


/*
 * Type checking and casting macros
 */
#define STUFFKEEPER_TYPE_DATA_ITEM	(stuffkeeper_data_item_get_type())
#define STUFFKEEPER_DATA_ITEM(obj)	G_TYPE_CHECK_INSTANCE_CAST((obj), stuffkeeper_data_item_get_type(), StuffKeeperDataItem)
#define STUFFKEEPER_DATA_ITEM_CONST(obj)	G_TYPE_CHECK_INSTANCE_CAST((obj), stuffkeeper_data_item_get_type(), StuffKeeperDataItem const)
#define STUFFKEEPER_DATA_ITEM_CLASS(klass)	G_TYPE_CHECK_CLASS_CAST((klass), stuffkeeper_data_item_get_type(), StuffKeeperDataItemClass)
#define STUFFKEEPER_IS_DATA_ITEM(obj)	G_TYPE_CHECK_INSTANCE_TYPE((obj), stuffkeeper_data_item_get_type ())

#define STUFFKEEPER_DATA_ITEM_GET_CLASS(obj)	G_TYPE_INSTANCE_GET_CLASS((obj), stuffkeeper_data_item_get_type(), StuffKeeperDataItemClass)

/* Private structure type */
typedef struct _StuffKeeperDataItemPrivate StuffKeeperDataItemPrivate;

/*
 * Main object structure
 */
#ifndef __TYPEDEF_STUFFKEEPER_DATA_ITEM__
#define __TYPEDEF_STUFFKEEPER_DATA_ITEM__
typedef struct _StuffKeeperDataItem StuffKeeperDataItem;
#endif
struct _StuffKeeperDataItem {
	GObject __parent__;
	/*< private >*/
	StuffKeeperDataItemPrivate *_priv;
};

/*
 * Class definition
 */
typedef struct _StuffKeeperDataItemClass StuffKeeperDataItemClass;
struct _StuffKeeperDataItemClass {
	GObjectClass __parent__;
	/*signal*/void (* item_changed) (StuffKeeperDataItem * self);
};


/*
 * Public methods
 */
GType	stuffkeeper_data_item_get_type	(void);
void 	stuffkeeper_data_item_item_changed	(StuffKeeperDataItem * self);
void 	stuffkeeper_data_item_save_yourself	(StuffKeeperDataItem * self);
void 	stuffkeeper_data_item_delete_yourself	(StuffKeeperDataItem * self);
StuffKeeperDataItem * 	stuffkeeper_data_item_new	(void);
StuffKeeperDataItem * 	stuffkeeper_data_item_new_with_id	(gint id);
gint 	stuffkeeper_data_item_get_id	(StuffKeeperDataItem * self);
const gchar * 	stuffkeeper_data_item_get_title	(StuffKeeperDataItem * self);
const gchar * 	stuffkeeper_data_item_set_title	(StuffKeeperDataItem * self,
					const gchar * title);

/*
 * Signal connection wrapper macros
 */
#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#define stuffkeeper_data_item_connect__item_changed(object,func,data)	g_signal_connect(STUFFKEEPER_DATA_ITEM(__extension__ ({StuffKeeperDataItem *___object = (object); ___object; })),"item_changed",(GCallback) __extension__ ({void (* ___item_changed) (StuffKeeperDataItem * ___fake___self, gpointer ___data ) = (func); ___item_changed; }), (data))
#define stuffkeeper_data_item_connect_after__item_changed(object,func,data)	g_signal_connect_after(STUFFKEEPER_DATA_ITEM(__extension__ ({StuffKeeperDataItem *___object = (object); ___object; })),"item_changed",(GCallback) __extension__ ({void (* ___item_changed) (StuffKeeperDataItem * ___fake___self, gpointer ___data ) = (func); ___item_changed; }), (data))
#define stuffkeeper_data_item_connect_data__item_changed(object,func,data,destroy_data,flags)	g_signal_connect_data(STUFFKEEPER_DATA_ITEM(__extension__ ({StuffKeeperDataItem *___object = (object); ___object; })),"item_changed",(GCallback) __extension__ ({void (* ___item_changed) (StuffKeeperDataItem * ___fake___self, gpointer ___data ) = (func); ___item_changed; }), (data), (destroy_data), (GConnectFlags)(flags))
#else /* __GNUC__ && !__STRICT_ANSI__ */
#define stuffkeeper_data_item_connect__item_changed(object,func,data)	g_signal_connect(STUFFKEEPER_DATA_ITEM(object),"item_changed",(GCallback)(func),(data))
#define stuffkeeper_data_item_connect_after__item_changed(object,func,data)	g_signal_connect_after(STUFFKEEPER_DATA_ITEM(object),"item_changed",(GCallback)(func),(data))
#define stuffkeeper_data_item_connect_data__item_changed(object,func,data,destroy_data,flags)	g_signal_connect_data(STUFFKEEPER_DATA_ITEM(object),"item_changed",(GCallback)(func),(data),(destroy_data),(GConnectFlags)(flags))
#endif /* __GNUC__ && !__STRICT_ANSI__ */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
