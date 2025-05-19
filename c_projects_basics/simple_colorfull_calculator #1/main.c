/* main.c: Add keyboard support for digits, operators, Enter, Backspace, C */

#include <gtk/gtk.h>
#include "calculator.h"

// Global entry widget for display
static GtkWidget *entry;

// Show divide-by-zero error dialog
static void
show_div_by_zero_dialog(GtkWindow *parent)
{
    GtkWidget *dlg = gtk_message_dialog_new(
        parent,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_CLOSE,
        "Do not divide by 0!");
    gtk_dialog_run(GTK_DIALOG(dlg));
    gtk_widget_destroy(dlg);
}

// Compute expression in entry
static void
compute_expression(GtkWindow *parent)
{
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    double a, b, result;
    char op;

    if (sscanf(text, "%lf%c%lf", &a, &op, &b) == 3 && op == '/' && b == 0.0) {
        show_div_by_zero_dialog(parent);
        return;
    }

    if (calc_compute(text, &result)) {
        gchar buf[64];
        g_snprintf(buf, sizeof(buf), "%g", result);
        gtk_entry_set_text(GTK_ENTRY(entry), buf);
    } else {
        gtk_entry_set_text(GTK_ENTRY(entry), "Error");
    }
}

// Handle button clicks
static void
on_button_clicked(GtkButton *button, gpointer user_data)
{
    const gchar *lbl = gtk_button_get_label(button);
    GtkWindow *parent = GTK_WINDOW(user_data);

    if (g_strcmp0(lbl, "=") == 0) {
        compute_expression(parent);
    } else if (g_strcmp0(lbl, "C") == 0) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    } else {
        const gchar *cur = gtk_entry_get_text(GTK_ENTRY(entry));
        gchar *new = g_strconcat(cur, lbl, NULL);
        gtk_entry_set_text(GTK_ENTRY(entry), new);
        g_free(new);
    }
}

// Handle keyboard events
static gboolean
on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    GtkWindow *parent = GTK_WINDOW(user_data);
        /* Get pressed keyval directly from event struct */
    gunichar key = event->keyval;

    if (g_unichar_isdigit(key) || key == '+' || key == '-' || key == '*' || key == '/' || key == '.') {
        // Append char
        gchar str[2] = { key, '\0' };
        const gchar *cur = gtk_entry_get_text(GTK_ENTRY(entry));
        gchar *new = g_strconcat(cur, str, NULL);
        gtk_entry_set_text(GTK_ENTRY(entry), new);
        g_free(new);
        return TRUE;
    }
    switch (key) {
        case GDK_KEY_Return:
        case GDK_KEY_KP_Enter:
            compute_expression(parent);
            return TRUE;
        case GDK_KEY_BackSpace: {
            gchar *cur = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
            int len = strlen(cur);
            if (len > 0) {
                cur[len-1] = '\0';
                gtk_entry_set_text(GTK_ENTRY(entry), cur);
            }
            g_free(cur);
            return TRUE;
        }
        case GDK_KEY_c:
        case GDK_KEY_C:
            gtk_entry_set_text(GTK_ENTRY(entry), "");
            return TRUE;
        default:
            break;
    }
    return FALSE; // propagate other keys
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    // Load CSS
    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);

    // Window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Calculator");
    gtk_container_set_border_width(GTK_CONTAINER(window), 12);
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 400);

    // Grid
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Entry: editable
    entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1.0);
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_editable_set_editable(GTK_EDITABLE(entry), TRUE);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    // Buttons
    const char *buttons[] = {"7","8","9","/","4","5","6","*","1","2","3","-","0",".","=","+","C"};
    int pos = 0;
    for (int row = 1; row <= 5; row++) {
        for (int col = 0; col < 4; col++) {
            if (pos >= G_N_ELEMENTS(buttons)) break;
            const char *lbl = buttons[pos++];
            GtkWidget *btn = gtk_button_new_with_label(lbl);
            gtk_widget_set_hexpand(btn, TRUE);
            gtk_widget_set_vexpand(btn, TRUE);
            const gchar *cls;
            if (g_strcmp0(lbl, "=") == 0 || strchr("+-*/", lbl[0])) cls = "operator";
            else if (g_strcmp0(lbl, "C") == 0) cls = "control";
            else cls = "number";
            gtk_style_context_add_class(gtk_widget_get_style_context(btn), cls);
            g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked), window);
            gtk_grid_attach(GTK_GRID(grid), btn, col, row, 1, 1);
        }
    }

    // Keyboard signal
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
