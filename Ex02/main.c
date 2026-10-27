#include <gtk/gtk.h>
#include <stdlib.h> // Necessário para a função atof (converte texto para número)

// Estrutura atualizada para passar os componentes para o callback do botão
typedef struct {
    GtkWidget *entry_num1;
    GtkWidget *entry_num2;
    GtkWidget *label_resultado;
} AppWidgets;

// Função executada ao clicar no botão "Somar"
void on_botao_somar_clicked(GtkButton *button, gpointer user_data) {
    
    AppWidgets *widgets = (AppWidgets *)user_data;
    
    // 1. Pega o texto de dentro das duas caixas de entrada
    const gchar *texto1 = gtk_entry_get_text(GTK_ENTRY(widgets->entry_num1));
    const gchar *texto2 = gtk_entry_get_text(GTK_ENTRY(widgets->entry_num2));
    
    // 2. Converte o texto obtido para números decimais (double)
    double num1 = atof(texto1);
    double num2 = atof(texto2);
    
    // 3. Faz a soma
    double resultado = num1 + num2;
    
    // 4. Cria uma string formatada para exibir o resultado com 2 casas decimais
    // g_strdup_printf funciona como um sprintf, mas aloca a memória sozinho
    gchar *mensagem = g_strdup_printf("Resultado: %.2f", resultado);
    
    // 5. Altera o texto do Label
    gtk_label_set_text(GTK_LABEL(widgets->label_resultado), mensagem);
    
    // Libera a memória da string que foi criada temporariamente
    g_free(mensagem);
}

int main(int argc, char *argv[]) {

    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *button;
    AppWidgets *widgets = g_slice_new(AppWidgets);

    gtk_init(&argc, &argv);

    // 1. Carrega o arquivo XML do Glade
    builder = gtk_builder_new_from_file("soma.glade");
    if (builder == NULL) {
        g_print("ERRO CRÍTICO: Arquivo 'soma.glade' não encontrado!\n");
        return 1;
    }

    // 2. Captura os componentes pelos IDs definidos no Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "janela_principal"));
    widgets->entry_num1 = GTK_WIDGET(gtk_builder_get_object(builder, "campo_num1"));
    widgets->entry_num2 = GTK_WIDGET(gtk_builder_get_object(builder, "campo_num2"));
    widgets->label_resultado = GTK_WIDGET(gtk_builder_get_object(builder, "texto_resultado"));
    button = GTK_WIDGET(gtk_builder_get_object(builder, "botao_somar"));

    // Validação de segurança para garantir que todos os componentes existem
    if (!window || !widgets->entry_num1 || !widgets->entry_num2 || !widgets->label_resultado || !button) {
        g_print("ERRO: Um ou mais IDs de componentes estão errados no Glade!\n");
        return 1;
    }

    // 3. Conecta os sinais de fechar a janela e clicar no botão
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(button, "clicked", G_CALLBACK(on_botao_somar_clicked), widgets);

    g_object_unref(builder);

    // 4. Exibe a interface
    gtk_widget_show_all(window);
    gtk_main();

    g_slice_free(AppWidgets, widgets);
    return 0;
}
