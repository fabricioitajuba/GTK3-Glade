#include <gtk/gtk.h>

// Estrutura para passar os componentes necessários para o callback
typedef struct {
    GtkWidget *label;
} AppWidgets;

// Função callback que será executada quando o botão for clicado
void on_botao_clique_clicked(GtkButton *button, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    
    // Altera o texto do Label para "Hello World!"
    gtk_label_set_text(GTK_LABEL(widgets->label), "Hello World!");
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;
    AppWidgets *widgets = g_slice_new(AppWidgets);

    // Inicializa o GTK
    gtk_init(&argc, &argv);

    // 1. Carrega o arquivo XML criado no Glade
    builder = gtk_builder_new_from_file("hello.glade");

    // 2. Captura os componentes pelos IDs definidos no Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "janela_principal"));
    widgets->label = GTK_WIDGET(gtk_builder_get_object(builder, "texto_label"));
    button = GTK_WIDGET(gtk_builder_get_object(builder, "botao_clique"));

    // 3. Conecta os eventos (Sinais)
    // Fecha o programa corretamente ao fechar a janela
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Conecta o clique do botão à nossa função, passando a estrutura com o label
    g_signal_connect(button, "clicked", G_CALLBACK(on_botao_clique_clicked), widgets);

    // Libera a memória do builder que não será mais necessário
    g_object_unref(builder);

    // 4. Exibe a janela e inicia o loop principal do GTK
    gtk_widget_show_all(window);
    gtk_main();

    // Libera a memória da estrutura ao fechar o programa
    g_slice_free(AppWidgets, widgets);

    return 0;
}
