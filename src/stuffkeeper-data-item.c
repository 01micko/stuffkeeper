/* Generated by GOB (v2.0.14)   (do not edit directly) */

/* End world hunger, donate to the World Food Programme, http://www.wfp.org */

#define GOB_VERSION_MAJOR 2
#define GOB_VERSION_MINOR 0
#define GOB_VERSION_PATCHLEVEL 14

#define selfp (self->_priv)

#include <string.h> /* memset() */

#include "stuffkeeper-data-item.h"

#include "stuffkeeper-data-item-private.h"

#ifdef G_LIKELY
#define ___GOB_LIKELY(expr) G_LIKELY(expr)
#define ___GOB_UNLIKELY(expr) G_UNLIKELY(expr)
#else /* ! G_LIKELY */
#define ___GOB_LIKELY(expr) (expr)
#define ___GOB_UNLIKELY(expr) (expr)
#endif /* G_LIKELY */
/* self casting macros */
#define SELF(x) STUFFKEEPER_DATA_ITEM(x)
#define SELF_CONST(x) STUFFKEEPER_DATA_ITEM_CONST(x)
#define IS_SELF(x) STUFFKEEPER_IS_DATA_ITEM(x)
#define TYPE_SELF STUFFKEEPER_TYPE_DATA_ITEM
#define SELF_CLASS(x) STUFFKEEPER_DATA_ITEM_CLASS(x)

#define SELF_GET_CLASS(x) STUFFKEEPER_DATA_ITEM_GET_CLASS(x)

/* self typedefs */
typedef StuffKeeperDataItem Self;
typedef StuffKeeperDataItemClass SelfClass;

/* here are local prototypes */
static void stuffkeeper_data_item_init (StuffKeeperDataItem * o) G_GNUC_UNUSED;
static void stuffkeeper_data_item_class_init (StuffKeeperDataItemClass * c) G_GNUC_UNUSED;
static void ___real_stuffkeeper_data_item_item_changed (StuffKeeperDataItem * self);
static void ___2_stuffkeeper_data_item_finalize (GObject * obj) G_GNUC_UNUSED;

/*
 * Signal connection wrapper macro shortcuts
 */
#define self_connect__item_changed(object,func,data)	stuffkeeper_data_item_connect__item_changed((object),(func),(data))
#define self_connect_after__item_changed(object,func,data)	stuffkeeper_data_item_connect_after__item_changed((object),(func),(data))
#define self_connect_data__item_changed(object,func,data,destroy_data,flags)	stuffkeeper_data_item_connect_data__item_changed((object),(func),(data),(destroy_data),(flags))

enum {
	ITEM_CHANGED_SIGNAL,
	LAST_SIGNAL
};

static guint object_signals[LAST_SIGNAL] = {0};

/* pointer to the class of our parent */
static GObjectClass *parent_class = NULL;

/* Short form macros */
#define self_item_changed stuffkeeper_data_item_item_changed
#define self_new stuffkeeper_data_item_new
#define self_new_with_id stuffkeeper_data_item_new_with_id
#define self_get_id stuffkeeper_data_item_get_id
#define self_get_title stuffkeeper_data_item_get_title
#define self_set_title stuffkeeper_data_item_set_title
GType
stuffkeeper_data_item_get_type (void)
{
	static GType type = 0;

	if ___GOB_UNLIKELY(type == 0) {
		static const GTypeInfo info = {
			sizeof (StuffKeeperDataItemClass),
			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,
			(GClassInitFunc) stuffkeeper_data_item_class_init,
			(GClassFinalizeFunc) NULL,
			NULL /* class_data */,
			sizeof (StuffKeeperDataItem),
			0 /* n_preallocs */,
			(GInstanceInitFunc) stuffkeeper_data_item_init,
			NULL
		};

		type = g_type_register_static (G_TYPE_OBJECT, "StuffKeeperDataItem", &info, (GTypeFlags)0);
	}

	return type;
}

/* a macro for creating a new object of our type */
#define GET_NEW ((StuffKeeperDataItem *)g_object_new(stuffkeeper_data_item_get_type(), NULL))

/* a function for creating a new object of our type */
#include <stdarg.h>
static StuffKeeperDataItem * GET_NEW_VARG (const char *first, ...) G_GNUC_UNUSED;
static StuffKeeperDataItem *
GET_NEW_VARG (const char *first, ...)
{
	StuffKeeperDataItem *ret;
	va_list ap;
	va_start (ap, first);
	ret = (StuffKeeperDataItem *)g_object_new_valist (stuffkeeper_data_item_get_type (), first, ap);
	va_end (ap);
	return ret;
}


static void
___finalize(GObject *obj_self)
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::finalize"
	StuffKeeperDataItem *self G_GNUC_UNUSED = STUFFKEEPER_DATA_ITEM (obj_self);
	gpointer priv G_GNUC_UNUSED = self->_priv;
#line 26 "stuffkeeper-data-item.gob"
	___2_stuffkeeper_data_item_finalize(obj_self);
#line 119 "stuffkeeper-data-item.c"
#line 16 "stuffkeeper-data-item.gob"
	if(self->_priv->title) { g_free ((gpointer) self->_priv->title); self->_priv->title = NULL; }
#line 122 "stuffkeeper-data-item.c"
}
#undef __GOB_FUNCTION__

static void 
stuffkeeper_data_item_init (StuffKeeperDataItem * o G_GNUC_UNUSED)
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::init"
	o->_priv = G_TYPE_INSTANCE_GET_PRIVATE(o,STUFFKEEPER_TYPE_DATA_ITEM,StuffKeeperDataItemPrivate);
#line 15 "stuffkeeper-data-item.gob"
	o->_priv->id = 0;
#line 133 "stuffkeeper-data-item.c"
#line 16 "stuffkeeper-data-item.gob"
	o->_priv->title = g_strdup("N/A");
#line 136 "stuffkeeper-data-item.c"
}
#undef __GOB_FUNCTION__
static void 
stuffkeeper_data_item_class_init (StuffKeeperDataItemClass * c G_GNUC_UNUSED)
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::class_init"
	GObjectClass *g_object_class G_GNUC_UNUSED = (GObjectClass*) c;

	g_type_class_add_private(c,sizeof(StuffKeeperDataItemPrivate));

	parent_class = g_type_class_ref (G_TYPE_OBJECT);

	object_signals[ITEM_CHANGED_SIGNAL] =
		g_signal_new ("item_changed",
			G_TYPE_FROM_CLASS (g_object_class),
			(GSignalFlags)(G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION),
			G_STRUCT_OFFSET (StuffKeeperDataItemClass, item_changed),
			NULL, NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE, 0);

#line 18 "stuffkeeper-data-item.gob"
	c->item_changed = ___real_stuffkeeper_data_item_item_changed;
#line 26 "stuffkeeper-data-item.gob"
	g_object_class->finalize = ___finalize;
#line 162 "stuffkeeper-data-item.c"
}
#undef __GOB_FUNCTION__



#line 18 "stuffkeeper-data-item.gob"
void 
stuffkeeper_data_item_item_changed (StuffKeeperDataItem * self)
#line 171 "stuffkeeper-data-item.c"
{
	GValue ___param_values[1];
	GValue ___return_val;

memset (&___return_val, 0, sizeof (___return_val));
memset (&___param_values, 0, sizeof (___param_values));

#line 18 "stuffkeeper-data-item.gob"
	g_return_if_fail (self != NULL);
#line 18 "stuffkeeper-data-item.gob"
	g_return_if_fail (STUFFKEEPER_IS_DATA_ITEM (self));
#line 183 "stuffkeeper-data-item.c"

	___param_values[0].g_type = 0;
	g_value_init (&___param_values[0], G_TYPE_FROM_INSTANCE (self));
	g_value_set_instance (&___param_values[0], (gpointer) self);

	g_signal_emitv (___param_values,
		object_signals[ITEM_CHANGED_SIGNAL],
		0 /* detail */,
		&___return_val);

	g_value_unset (&___param_values[0]);
}
#line 18 "stuffkeeper-data-item.gob"
static void 
___real_stuffkeeper_data_item_item_changed (StuffKeeperDataItem * self G_GNUC_UNUSED)
#line 199 "stuffkeeper-data-item.c"
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::item_changed"
{
#line 21 "stuffkeeper-data-item.gob"
	
        printf("Signal my change: %i\n", self->_priv->id);
    }}
#line 207 "stuffkeeper-data-item.c"
#undef __GOB_FUNCTION__

#line 26 "stuffkeeper-data-item.gob"
static void 
___2_stuffkeeper_data_item_finalize (GObject * obj G_GNUC_UNUSED)
#line 213 "stuffkeeper-data-item.c"
#define PARENT_HANDLER(___obj) \
	{ if(G_OBJECT_CLASS(parent_class)->finalize) \
		(* G_OBJECT_CLASS(parent_class)->finalize)(___obj); }
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::finalize"
{
#line 29 "stuffkeeper-data-item.gob"
	
        printf("destroying item\n");
        PARENT_HANDLER(obj);
    }}
#line 225 "stuffkeeper-data-item.c"
#undef __GOB_FUNCTION__
#undef PARENT_HANDLER

#line 35 "stuffkeeper-data-item.gob"
StuffKeeperDataItem * 
stuffkeeper_data_item_new (void)
#line 232 "stuffkeeper-data-item.c"
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::new"
{
#line 38 "stuffkeeper-data-item.gob"
	
            
            Self *obj = GET_NEW;
            obj->_priv->id = g_random_int();
            printf("Creating item: %i\n", obj->_priv->id);
            return obj;
        }}
#line 244 "stuffkeeper-data-item.c"
#undef __GOB_FUNCTION__

#line 46 "stuffkeeper-data-item.gob"
StuffKeeperDataItem * 
stuffkeeper_data_item_new_with_id (gint id)
#line 250 "stuffkeeper-data-item.c"
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::new_with_id"
{
#line 49 "stuffkeeper-data-item.gob"
	
        Self *obj = GET_NEW;
        obj->_priv->id = id;
        printf("Creating item: %i\n", obj->_priv->id);
        return obj;
    }}
#line 261 "stuffkeeper-data-item.c"
#undef __GOB_FUNCTION__

#line 57 "stuffkeeper-data-item.gob"
gint 
stuffkeeper_data_item_get_id (StuffKeeperDataItem * self)
#line 267 "stuffkeeper-data-item.c"
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::get_id"
#line 57 "stuffkeeper-data-item.gob"
	g_return_val_if_fail (self != NULL, (gint )0);
#line 57 "stuffkeeper-data-item.gob"
	g_return_val_if_fail (STUFFKEEPER_IS_DATA_ITEM (self), (gint )0);
#line 274 "stuffkeeper-data-item.c"
{
#line 60 "stuffkeeper-data-item.gob"
	
        return self->_priv->id;
    }}
#line 280 "stuffkeeper-data-item.c"
#undef __GOB_FUNCTION__

#line 64 "stuffkeeper-data-item.gob"
const gchar * 
stuffkeeper_data_item_get_title (StuffKeeperDataItem * self)
#line 286 "stuffkeeper-data-item.c"
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::get_title"
#line 64 "stuffkeeper-data-item.gob"
	g_return_val_if_fail (self != NULL, (const gchar * )0);
#line 64 "stuffkeeper-data-item.gob"
	g_return_val_if_fail (STUFFKEEPER_IS_DATA_ITEM (self), (const gchar * )0);
#line 293 "stuffkeeper-data-item.c"
{
#line 67 "stuffkeeper-data-item.gob"
	
        return self->_priv->title;
    }}
#line 299 "stuffkeeper-data-item.c"
#undef __GOB_FUNCTION__

#line 70 "stuffkeeper-data-item.gob"
const gchar * 
stuffkeeper_data_item_set_title (StuffKeeperDataItem * self, const gchar * title)
#line 305 "stuffkeeper-data-item.c"
{
#define __GOB_FUNCTION__ "StuffKeeper:Data:Item::set_title"
#line 70 "stuffkeeper-data-item.gob"
	g_return_val_if_fail (self != NULL, (const gchar * )0);
#line 70 "stuffkeeper-data-item.gob"
	g_return_val_if_fail (STUFFKEEPER_IS_DATA_ITEM (self), (const gchar * )0);
#line 312 "stuffkeeper-data-item.c"
{
#line 73 "stuffkeeper-data-item.gob"
	
        if(self->_priv->title)
        {
            g_free(self->_priv->title);
            self->_priv->title = NULL;
        }
        if(title)
        {
            self->_priv->title = g_strdup(title);
        }
        printf("new title: %s\n", title);
        self_item_changed(self);
    }}
#line 328 "stuffkeeper-data-item.c"
#undef __GOB_FUNCTION__
