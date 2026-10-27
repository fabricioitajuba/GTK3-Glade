#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura para passar os campos de texto para a função do botão
typedef struct {
    GtkWidget *entry_nome;
    GtkWidget *entry_idade;
} AppWidgets;

// Função para descobrir o próximo ID livre com base nas linhas do arquivo
int obter_proximo_id() {

    FILE *arquivo = fopen("usuarios.txt", "r");
    if (!arquivo) return 1; // Se o arquivo não existir, este é o ID 1

    int id_atual = 0;
    char linha[256];
    
    // Lê o arquivo linha por linha até o final para contar os registros
    while (fgets(linha, sizeof(linha), arquivo)) {
        id_atual++;
    }
    
    fclose(arquivo);
    return id_atual + 1;
}

// Função que encerra o programa quando o botão Sair for clicado
void on_btn_sair_clicked(GtkButton *button, gpointer user_data) {
    g_print("Fechando o programa...\n");
    gtk_main_quit(); // Esta função finaliza o GTK com segurança
}


// Função chamada quando o botão é clicado
void on_btn_salvar_clicked(GtkButton *button, gpointer user_data) {

    AppWidgets *widgets = (AppWidgets *)user_data;

    // 1. Captura o texto digitado nos campos
    const char *nome = gtk_entry_get_text(GTK_ENTRY(widgets->entry_nome));
    const char *idade = gtk_entry_get_text(GTK_ENTRY(widgets->entry_idade));

    // Validação simples para não salvar campos vazios
    if (g_strcmp0(nome, "") == 0 || g_strcmp0(idade, "") == 0) {
        g_print("Erro: Preencha todos os campos!\n");
        return;
    }

    // 2. Calcula o ID automático
    int proximo_id = obter_proximo_id();

    // 3. Abre o arquivo em modo "append" (a) para adicionar no final sem apagar o que já existe
    FILE *arquivo = fopen("usuarios.txt", "a");
    if (arquivo == NULL) {
        g_print("Erro ao abrir o arquivo!\n");
        return;
    }

    // 4. Salva os dados formatados (ID | Nome | Idade)
    fprintf(arquivo, "ID: %d | Nome: %s | Idade: %s\n", proximo_id, nome, idade);
    fclose(arquivo);

    g_print("Usuário ID %d inserido com sucesso!\n", proximo_id);

    // 5. Limpa os campos da tela para a próxima digitação
    gtk_entry_set_text(GTK_ENTRY(widgets->entry_nome), "");
    gtk_entry_set_text(GTK_ENTRY(widgets->entry_idade), "");
}

int main(int argc, char *argv[]) {

    GtkBuilder *builder;
    GtkWidget *window;
    AppWidgets widgets; // Instancia nossa estrutura

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("interface.glade");

    // Procura e vincula os componentes do Glade às variáveis em C
    widgets.entry_nome = GTK_WIDGET(gtk_builder_get_object(builder, "entry_nome"));
    widgets.entry_idade = GTK_WIDGET(gtk_builder_get_object(builder, "entry_idade"));
    window = GTK_WIDGET(gtk_builder_get_object(builder, "minha_janela"));

    //ADICIONE ESTA LINHA AQUI ABAIXO se estiver no windows<<<
    // Ela diz manualmente ao Windows onde encontrar a função que você colocou no Glade
    gtk_builder_add_callback_symbol(builder, "on_btn_salvar_clicked", G_CALLBACK(on_btn_salvar_clicked));
    gtk_builder_add_callback_symbol(builder, "on_btn_sair_clicked", G_CALLBACK(on_btn_sair_clicked)); // Nova linha!


    // Conecta o sinal do botão passando a estrutura 'widgets' como argumento (user_data)
    gtk_builder_connect_signals(builder, &widgets);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
