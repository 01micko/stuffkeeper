#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <locale.h> /* For setlocale */
#include <config.h>
#include <gio/gio.h>

#include "debug.h"
#include "revision.h"

/* Include the database */
#include "stuffkeeper-data-backend.h"
#include "stuffkeeper-interface.h"
#include "stuffkeeper-plugin-manager.h"
#include "bacon-message-connection.h"


/* List of active interface objects */
GList *interface_list = NULL;

/**
 * Config system
 */
GKeyFile *config_file = NULL;

/**
 * Handle ipc messages
 */
static void bacon_on_message_received(const char *message, gpointer data)
{
    debug_printf("IPC: got '%s'\n", (message)?message:"(null)");
    if(message)
    {
        if(strcmp(message, "New Window") == 0)
        {
            /* Get open window */
            GList *node = g_list_first(interface_list);
            if(node)
            {
                StuffkeeperInterface *ski=  node->data;
                /* tell the open window to create new one */
                stuffkeeper_interface_new_window(ski);
                debug_printf("IPC: Requested new window\n");
            }
        }
    }
}

/**
 * Function destroy objects
 */
void interface_clear(GObject *interface, gpointer data)
{
    g_object_unref(interface);
}

/**
 * The main programs
 */

int main ( int argc, char **argv )
{
    GList                       *node;
    GList                       *iter;
    GError                      *error                  = NULL;
    gchar                       *path                   = NULL;
    gchar                       *config_path            = NULL;
    /**
     * Interprocess communication. Used to make sure
     * only one instance is running
     */
    BaconMessageConnection      *bacon_connection       = NULL;

    /* pointer holding the backend */
    StuffkeeperDataBackend      *skdb                   = NULL;
    StuffkeeperInterface        *ski                    = NULL;
    StuffkeeperPluginManager    *spm                    = NULL;
    /**
     * Command line parsing stuff 
     */
    GOptionContext              *context;
    gchar                       *db_path                = NULL;
    gboolean                    version                 = FALSE;
    gboolean                    first_run               = FALSE;

    GOptionEntry entries[] = 
    {
        { "db-path", 'd', 0, G_OPTION_ARG_STRING, &db_path,     _("Path to the database"),                                 "Path" },
        { "version", 'v', 0, G_OPTION_ARG_NONE,   &version,     _("Version"),                                              NULL},
        { "firstrun",'f', 0, G_OPTION_ARG_NONE,   &first_run,   _("Act like it is the first time stuffkeeper is ran"),     NULL},
        { NULL }
    };

    /* setup tic-tac system */
    INIT_TIC_TAC();

    /* set application name */
    g_set_prgname("stuffkeeper");
    g_set_application_name("stuffkeeper");
    gtk_window_set_default_icon_name("stuffkeeper");

    /* Set  up locales */
#ifdef ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
    setlocale(LC_ALL, "");
#endif
    gtk_set_locale();

    /* Parse command line options */
    context = g_option_context_new ("- Stuffkeeper");
    g_option_context_add_main_entries (context, entries, "stuffkeeper");
    g_option_context_add_group (context, gtk_get_option_group (TRUE));
    g_option_context_parse (context, &argc, &argv, &error);
    g_option_context_free(context);

    g_thread_init(NULL);
    /* Initialize gtk */
    if(!gtk_init_check(&argc, &argv))
    {
        /* If we failed to initialize gtk+*/
        /* Tell the user */
        debug_printf("Failed to initialize gtk+\n");
        /* return a failure */
        return EXIT_FAILURE;
    }

    /**
     * Printout the version
     */
    if(version)
    {
        printf("%s: %s\n", _("Version"), PROGRAM_VERSION);
        if(strlen(revision)>0)
        {
            printf("%s: %s\n", _("Revision"), revision);
        }
        return EXIT_SUCCESS;
    }

    /**
     * Add icon theme directory
     */
    gtk_icon_theme_append_search_path(gtk_icon_theme_get_default (),
                                   PACKAGE_DATADIR G_DIR_SEPARATOR_S "icons");

    /* Create the plugin manager */
    spm = stuffkeeper_plugin_manager_new();
    /* load the plugins */
    stuffkeeper_plugin_manager_load_plugin(spm);
    /* Initialize the backend */
    skdb = stuffkeeper_data_backend_new();
    /* Check if creating the backend worked */
    if(!skdb)
    {
        debug_printf("Failed to create a backend\n");
        return EXIT_FAILURE;
    }


    /* Setup the interprocess communication. this does not work in win32! */ 
    bacon_connection = bacon_message_connection_new("stuffkeeper");
    if(bacon_connection) {
        /* Check if we are the only instance, this can be checked by looking if we are the server */
        if (!bacon_message_connection_get_is_server (bacon_connection)) 
        {
            /* There is an instant allready running */
            /* message the running instance, to show a new window */
            bacon_message_connection_send(bacon_connection, "New Window");

            /* Close the connection */
            bacon_message_connection_free (bacon_connection);
            /* Returning */
            debug_printf(_("There is allready an instance running. quitting."));
            g_object_unref(skdb); 
            exit(EXIT_SUCCESS);
        }
        /* setup signal handler */
        /* Listen for incoming messages */
        bacon_message_connection_set_callback (bacon_connection,
                bacon_on_message_received,
                skdb);
    } else {
        /* print an error and quit */
        debug_printf("Failed to setup IPC, quitting\n");
        g_object_unref(skdb); 
        /* Return error code */
        exit(EXIT_FAILURE);
    }


    /**
     * Do filesystem checking 
     */

    /* build the directory where data is stored */
    if(db_path != NULL) {
        path = g_build_path(G_DIR_SEPARATOR_S, db_path, NULL);
    } else {
        path = g_build_path(G_DIR_SEPARATOR_S, g_get_home_dir(), ".stuffkeeper", NULL);
    }


    /* Test if the directory exists */
    if(!g_file_test(path, G_FILE_TEST_IS_DIR))
    {
        /* Create the directory if it does not exists */
        if(g_mkdir(path, 0755))
        {
            g_error("Failed to create: %s\n", path);
            return EXIT_FAILURE;
        }
        first_run = TRUE;
    }


    /* open config file */
    debug_printf("Opening config file\n"); 
    config_file = g_key_file_new();

    config_path = g_build_path(G_DIR_SEPARATOR_S, path, "config.cfg", NULL);
    if(g_file_test(path, G_FILE_TEST_EXISTS))
    {
        GError *error = NULL;
        debug_printf("Loading config file\n");
        if(!g_key_file_load_from_file(config_file, config_path, G_KEY_FILE_NONE, &error))
        {
            debug_printf("Failed to load config file\n");
            if(error)
            {
                debug_printf("Reported error: %s\n", error->message);
                g_error_free(error); 
            }
        }
        
    }

    /* restore locked state */
    if(g_key_file_get_boolean(config_file, "BACKEND", "locked", NULL))
    {
        stuffkeeper_data_backend_set_locked(skdb, TRUE);
    }

    /* Create a main interface */
    ski= stuffkeeper_interface_new(config_file);
    interface_list = g_list_append(interface_list, ski);

    TAC("Time elapsed until creating gui");

    /* This tells the backend to populate itself */
    stuffkeeper_data_backend_load(skdb,path);

    TAC("time elapsed until backend load");
    
    stuffkeeper_interface_initialize_interface(ski,skdb,G_OBJECT(spm));

    /* path */
    g_free(path);


    TAC("time elapsed until gtk_main()");


    if(first_run)
    {
        printf("Initial run of stuffkeeper\n");
        stuffkeeper_interface_first_run(ski);
    }

    /* initialize plugin */
    node = stuffkeeper_plugin_manager_get_loaded_plugins(spm); 
    if(node)
    {
        printf("Loading plugins list\n");
        for(iter = g_list_first(node);iter;iter = g_list_next(iter))
        {
            StuffkeeperPlugin *plugin = iter->data;
            /* Background plugins are run now, they are not allowed to block */
            if(stuffkeeper_plugin_get_plugin_type(plugin)&PLUGIN_BACKGROUND)
            {
                stuffkeeper_plugin_run_background(STUFFKEEPER_PLUGIN(plugin), skdb);
            }
        }
        g_list_free(node);
    }

    /* Start the main loop */
    gtk_main();

    /* close open interfaces */
    g_list_foreach(interface_list, (GFunc)interface_clear, NULL);
    g_list_free(interface_list);

    /* save locked state */
    g_key_file_set_boolean(config_file, "BACKEND", "locked", stuffkeeper_data_backend_get_locked(skdb));
    /* cleanup  */
    g_object_unref(skdb);

    /* clean spm */
    g_object_unref(spm);

    /* Close the IPC bus */
    if(bacon_connection)
    {
        bacon_message_connection_free (bacon_connection);
    }

    /* Savin config */
    if(config_path)
    {
        gchar *content = NULL;
        gsize length = 0;
        GError *error = NULL;

        content = g_key_file_to_data(config_file, &length, &error);
        if(content) {
            GError *error2 = NULL;
            /* Create file */
            GFile *file = g_file_new_for_path(config_path);
            /* replace file and make backup */
            if(!g_file_replace_contents(file, content, length, NULL, TRUE, G_FILE_COPY_OVERWRITE,NULL,NULL, &error2))
            {
                debug_printf("Failed to save file '%s': %s\n", config_path, error2->message);
                g_error_free(error2);
            }

            g_object_unref(file);
            g_free(content);
        } else {
            if(error)
            {
                debug_printf("Failed to serialize config file: %s\n", error->message);
                g_error_free(error);
            }
        }

        /* Free config path */
        g_free(config_path);
    }

    /* exit */
    return EXIT_SUCCESS;
}
